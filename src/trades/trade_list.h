#pragma once

#include "second.h"
#include "../../src/core/core.h"


class candle {
public:
    candle() {}

    candle(double pmin, double pmax, double plast) : MinPrice(pmin), MaxPrice(pmax), LastPrice(plast) {}

    double MinPrice;
    double MaxPrice;
    double LastPrice;
};

class signal_block {
public:
    std::vector<double>* price_ls;
    std::vector<double>* signal_ls;

    size_t Len() { return price_ls->size(); }
};


class trade_list {
public:
    trade_list() {}

    trade_list(const std::string& prname);

    second& Second(int idx) { return list[idx]; }

    second& First() { return list[0]; }

    second& Last() { return list[list.size() - 1]; }

    size_t Len() { return list.size(); }

    double FirstPrice() { return this->First().LastPrice; }

    double LastPrice() { return this->Last().LastPrice; }

    int Count();

    std::string GetPairName() { return pair_name; }

    double Volume();

//    double VolumeSell();
//
//    double VolumeBuy();
//
    double MaxPrice();

    double MinPrice();

//    double AveragePrice();
    std::vector<double> GetSequenceLastPrice(int end, int step, int period);

    void GetSequenceMaxPrice(int end, int step, int period, double* res);

    void GetSequenceMinPrice(int end, int step, int period, double* res);

    std::vector<candle> GetCandleList(int step);

    // ANALYST

    signal_block GetEmptySignalBlock();


private:
    std::string pair_name;
    std::vector<second> list;
    std::vector<double> price_list;
    std::vector<double> sig_list;

    void load_file(const std::string& path);

    void normalize();

};




