#ifndef C_ALLA_EXPLORER_ALLA_H
#define C_ALLA_EXPLORER_ALLA_H

#include "../core/json.hpp"
#include "../params/params.h"
#include "../trades/trade_list.h"

using json = nlohmann::json;


void RUN(trade_list trade_ls, params prm, int number);


#endif //C_ALLA_EXPLORER_ALLA_H
