#ifndef C_ALLA_EXPLORER_INDICATOR_H
#define C_ALLA_EXPLORER_INDICATOR_H

#include "../core/json.hpp"
#include "../core/core.h"
#include "../trades/trade_list.h"


using json = nlohmann::json;

struct Signal {
    double body;
};


class Indicator {
public:
    Indicator() {}

    Indicator(json& conf);

    Signal GetSignal(int end) const { return Signal();}

    std::string String() { return name; }

protected:
    std::string name;
};

class IndiEMASimpleCrossWithPrice : public Indicator {
public:
    IndiEMASimpleCrossWithPrice(json& conf, std::vector<candle>& cls);

    Signal GetSignal(int end) const;

private:
    std::vector<double> signals;
};

class IndiSarParabolic : public Indicator {
public:
    IndiSarParabolic(json& conf, std::vector<candle>& cls);

    Signal GetSignal(int end) const;

private:
    std::vector<double> signals;
};


#endif //C_ALLA_EXPLORER_INDICATOR_H
