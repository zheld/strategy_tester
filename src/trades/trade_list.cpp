#include <fstream>
#include <msgpack.hpp>
#include "trade_list.h"
#include "../../src/prm.h"
#include "../../src/core/core_fl.h"
#include "../../src/core/core_dt.h"

const std::string lobj = "trade_list";

class raw_trade {
public:
    long dt;
    double price;
    double amount;
    MSGPACK_DEFINE (dt, price, amount);
};

trade_list::trade_list(const std::string& prname) {
    LOG(lobj, "in trade_list constructor");

    pair_name = prname;
    std::string path = conf::DIRECTORY + "/" + prname + "/trades";
    // filename list
    auto file_ls = FL_FILE_LIST(path);

    LOG(lobj, "file_list len: " + std::to_string(file_ls.size()));

//    size_t len = 142 * 86400;
    size_t len = file_ls.size() * 86400;
    // list
    list = std::vector<second>(len);

    // sig list
    price_list = std::vector<double>(len);
    sig_list = std::vector<double>(len);

    // initialize list
    for (auto&& i : file_ls) {
        load_file(path + "/" + i);
    }

    // normalize
    normalize();
}

void trade_list::load_file(const std::string& path) {
    LOG(lobj, "in load_file: " + path);


    std::streampos bsize;
    char* memblock;

    std::ifstream file(path, std::ios::in | std::ios::binary | std::ios::ate);

    if (file.is_open()) {
        bsize = file.tellg();
        memblock = new char[bsize];
        file.seekg(0, std::ios::beg);
        file.read(memblock, bsize);
        file.close();

        msgpack::object_handle oh = msgpack::unpack(memblock, bsize);
        msgpack::object obj = oh.get();
        std::vector<raw_trade> ls;
        obj.convert(ls);

        // insert into list
        long bias;
        LOG(lobj, "size new struct: " + std::to_string(ls.size()));
        auto start_unix = TIME_TO_UNIX(conf::FIRST_TRADE);
        for (auto&& tr : ls) {
//            std::cout << "unix: " << tr.dt<< ", start unix: " << start_unix
//            << ", sub unix: " << tr.dt - start_unix << ", len list: " << trade_list::list.size() << std::endl;
//            std::cout << "start_unix:  " << UTC_TO_TIME(start_unix) << std::endl;
//            std::cout << "datetime:  " << UTC_TO_TIME(tr.dt) << std::endl;
            bias = tr.dt - start_unix;
            if (trade_list::list.size() < bias || bias < 0) {
                continue;
            }
            trade_list::list.at(bias).Add(tr.price, tr.amount);
        }

        delete[] memblock;
        INFO("file: " + path + " is loaded");
        return;
    }
    ERROR("Unable to open file: " + path);
}


void trade_list::normalize() {
    INFO("normalize trade list...");
    double last_price;
    for (auto&& i : trade_list::list) {
        if (!i.IsEmpty()) {
            last_price = i.LastPrice;
            break;
        }
    }
    //
    size_t idx = 0;
    for (auto&& sec : trade_list::list) {
        if (sec.IsEmpty()) {
            sec.InsertDummy(last_price);
            price_list[idx] = last_price;
        } else {
            last_price = sec.LastPrice;
            price_list[idx] = sec.LastPrice;
        }

        idx++;
    }
    INFO("..done");
}

int trade_list::Count() {
    int count = 0;
    for (auto& sec : list) {
        count += sec.Count;
    }
    return count;
}

double trade_list::Volume() {
    double vol = 0;
    for (auto& sec : list) {
        vol += sec.Volume;
    }
    return vol;
}

//double trade_list::VolumeSell() {
//    double vol = 0;
//    for (int i = begin; i < begin + length; ++i) {
//        vol += list[i].VolumeSell();
//    }
//    return vol;
//}
//
//double trade_list::VolumeBuy() {
//    double vol = 0;
//    for (int i = begin; i < begin + length; ++i) {
//        vol += list[i].VolumeBuy();
//    }
//    return vol;
//}
//
double trade_list::MaxPrice() {
    double max = 0;
    for (auto& sec : list) {
        if (max < sec.MaxPrice) {
            max = sec.MaxPrice;
        }
    }
    return max;
}

//double trade_list::AveragePrice() {
//    double sum = 0;
//    for (int i = begin; i < begin + length; ++i) {
//        sum += list[i].AveragePrice();
//    }
//    return sum / double(length);

double trade_list::MinPrice() {
    double min = 1000000;
    for (auto& sec : list) {
        if (min > sec.MinPrice) {
            min = sec.MinPrice;
        }
    }
    return min;
}

//}

std::vector<double>
trade_list::GetSequenceLastPrice(int end, int step, int period) {
    auto res = std::vector<double>(period);
    int len = step * period;
    int pci = end - len;

    for (int i = 0; i < period; ++i) {
        res[i] = list[pci + (i + 1) * step].LastPrice;
    }

    return res;
}

void
trade_list::GetSequenceMaxPrice(int end, int step, int period, double* res) {
    assert(step != 0);
    assert(period != 0);

    int len = step * period;
    int start = end - len;

    int bias;
    double max_price = 0;
    double loc_mprice = 0;
    for (int i = 0; i < period; i++) {
        bias = start + i * step;
        for (int j = bias; j < bias + step; ++j) {
            loc_mprice = list[j].MaxPrice;
            if (loc_mprice > max_price) max_price = loc_mprice;
        }
        res[i] = max_price;
        max_price = 0;
    }
}

void
trade_list::GetSequenceMinPrice(int end, int step, int period, double* res) {
    int len = step * period;
    int start = end - len;

    int bias;
    double mix_price = 1000000;
    double loc_mprice = 0;
    for (int i = 0; i < period; i++) {
        bias = start + i * step;
        for (int j = bias; j < bias + step; ++j) {
            loc_mprice = list[j].MinPrice;
            if (loc_mprice < mix_price) mix_price = loc_mprice;
        }
        res[i] = mix_price;
        mix_price = 1000000;
    }
}

std::vector<candle>
trade_list::GetCandleList(int step) {
    if (step == 1) {
        auto cls = std::vector<candle>(list.size());
        double price = 0;

        for (int i = 0; i < list.size(); ++i) {
            price = list[i].LastPrice;
            cls[i] = candle(price, price, price);
        }

        return cls;

    } else {
        int len = list.size() / step;
        auto cls = std::vector<candle>(len);

        int bias;
        double max_price = 0;
        double min_price = 0;
        double last_price = 0;

        double loc_min_price = 0;
        double loc_max_price = 0;

        for (int i = 0; i < len; ++i) {
            bias = i * step;
            max_price = 0;
            min_price = 100000;

            for (int j = bias; j < bias + step; ++j) {
                loc_min_price = list[j].MinPrice;
                if (loc_min_price < min_price) min_price = loc_min_price;

                loc_max_price = list[j].MaxPrice;
                if (loc_max_price > max_price) max_price = loc_max_price;

                last_price = list[j].LastPrice;
            }

            cls[i] = candle(min_price, max_price, last_price);

        }
//    last_candle_list.swap(tmp);

        return cls;
    }
}

signal_block trade_list::GetEmptySignalBlock() {
    auto sb = signal_block();

    sb.price_ls = &price_list;
    sb.signal_ls = &sig_list;

    return sb;
}






//func LoadTrades() *TradeList {
//    // Storage
//    st := NewTradeStorage()
//    it := st.Iterator()
//
//    // Trades list
//    file_ls := st.GetFileList()
//    n := len(file_ls) * 86400
//    ls := NewTradeList(n)
//
//    wg := sync.WaitGroup{}
//    tstart := time.Now()
//
//    var unix int64
//    var rate float64
//    var amount float64
//    var volume float64
//    for it.Next() {
//        wg.Add(1)
//        go func(fname string, w *sync.WaitGroup) {
//            data := this.Storage.LoadFile(fname)
//            fmt.Println(fname, " >> ", len(data))
//
//            for _, itrade := range data {
//                // data
//                unix = int64(itrade[0].(uint64))
//                rate = itrade[1].(float64)
//                amount = itrade[2].(float64)
//                volume = rate * amount
//                if volume < 0 {
//                    volume *= -1
//                }
//                // insert
//                this.Trades.Insert(unix, rate, amount, volume)
//            }
//
//            w.Done()
//        }(it.CurrentFileName(), &wg)
//    }
//    tend := time.Now()
//    wg.Wait()
//    fmt.Println("time: ", tend.Sub(tstart))
//    fmt.Println("...done")
//    // normalize trade_list
//    this.Trades.Normalize()
//}
