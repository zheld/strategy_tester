#pragma once

#include "../core/core.h"
#include "../core/json.hpp"
#include "../trades/trade_list.h"
#include "../indicators/signals.h"
#include "./engine.h"
using json = nlohmann::json;


Report Long_AnalyzeAndReport(json& conf, std::vector<candle>& cls);

Report Short_AnalyzeAndReport(json& conf, std::vector<candle>& cls);




