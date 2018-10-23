#ifndef C_ALLA_EXPLORER_SIGNALS_H
#define C_ALLA_EXPLORER_SIGNALS_H


#include "../core/json.hpp"
#include "../core/core.h"
#include "../trades/trade_list.h"


using json = nlohmann::json;


void
CalcSignals(json& conf, std::vector<candle>& cls, std::vector<double>& signals);

signal_block
CalcSignalsStrong(json& conf);


#endif //C_ALLA_EXPLORER_SIGNALS_H
