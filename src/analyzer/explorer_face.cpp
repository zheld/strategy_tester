#include "explorer_face.h"
#include <unordered_map>
#include "./analyzer.h"
#include <mutex>

std::string prname = "";
trade_list trade_ls;
std::unordered_map<int, std::vector<candle>> cls_storage;
std::mutex mtx;

void
init(std::string& pr) {
    if (pr != prname) {
        prname = pr;
        cls_storage.clear();
        trade_ls = trade_list(prname);
    }
}


std::vector<candle>&
get_candle_list(int step) {
    mtx.lock();
    auto elem = cls_storage.find(step);
    if (elem != cls_storage.end()) {
        std::vector<candle>& ref = cls_storage[step];
        mtx.unlock();
        return ref;
    } else {
        cls_storage[step] = trade_ls.GetCandleList(step);
        std::vector<candle>& ref = cls_storage[step];
        mtx.unlock();
        return ref;
    }
}

int
get_step(json& conf) {
    auto params = conf["params"];
    for (auto& pr : params) {
        if (pr["name"] == "step") {
//            std::cout << pr["current"] << std::endl;
            return pr["current"];
        }
    }
    return 0;
}

double best_profit = -1000000.0;


std::string
FaceAnalyzeLong(json& conf) {
    json result;

    try {

        std::string pr = conf["pair_name"];

        // init
        init(pr);

        int step = get_step(conf);
        std::vector<candle>& cls = get_candle_list(step);

        auto long_report = std::move(Long_AnalyzeAndReport(conf, cls));

        // result
        auto re = long_report.profit;

        result["result"] = re;
        if (re > best_profit) {
            best_profit = re;
            std::cout << "[long]  profit: " << long_report.profit << "  count trades: " << long_report.trade_count
                      << "\nparams: " << conf.dump() << std::endl;
        }
    } catch (std::exception e) {
        std::cout << e.what() << std::endl;
        result["error"] = std::string(e.what());
    }

    auto res_str = result.dump();

    return res_str;
}


std::string
FaceAnalyzeLongStrong(json& conf) {

    json result;

    try {

        std::string pr = conf["pair_name"];

        // init
        init(pr);

        std::vector<candle> cls = std::vector<candle>();

        auto long_report = std::move(Long_AnalyzeAndReport(conf, cls));

        // result
        auto re = long_report.profit;

        result["result"] = re;
        if (re > best_profit) {
            best_profit = re;
            std::cout << "[long strong]  profit: " << long_report.profit << "  count trades: " << long_report.trade_count
                      << "\nparams: " << conf.dump() << std::endl;
        }
    } catch (std::exception e) {
        std::cout << e.what() << std::endl;
        result["error"] = std::string(e.what());
    }

    auto res_str = result.dump();

    return res_str;
}


std::string
FaceAnalyzeShort(json& conf) {

    std::string pr = conf["pair_name"];
    json result;


    // init
    init(pr);


    try {
        int step = get_step(conf);
        std::vector<candle>& cls = get_candle_list(step);
        auto short_report = std::move(Short_AnalyzeAndReport(conf, cls));
        // result
        result["result"] = short_report.profit;
        if (short_report.profit > best_profit) {
            best_profit = short_report.profit;
            std::cout << "[short]  profit: " << short_report.profit << "  count trades: " << short_report.trade_count
                      << "\nparams: " << conf.dump() << std::endl;
        }
    } catch (std::exception e) {
        std::cout << "error: " << std::string(e.what()) << std::endl;
        result["error"] = std::string(e.what());
    }


    auto res_str = result.dump();

    return res_str;
}

std::string
FaceAnalyze(json& params) {
    std::string ttype = params["trading_type"];
    if (ttype == "long") {
        return FaceAnalyzeLong(params);
    } else if (ttype == "short") {
        return FaceAnalyzeShort(params);
    }
    return "";
}

trade_list*
GetTradeList() {
    return &trade_ls;
}

std::string FaceAnalyzeStrong(json& params) {
    std::string ttype = params["trading_type"];
    if (ttype == "long") {
        return FaceAnalyzeLongStrong(params);
    } else if (ttype == "short") {
        return FaceAnalyzeShort(params);
    }
    return "";
}

