#ifndef C_ALLA_EXPLORER_EXPLORER_FACE_H
#define C_ALLA_EXPLORER_EXPLORER_FACE_H


#include <vector>
#include "../trades/trade_list.h"
#include "./engine.h"
#include "../core/json.hpp"

using json = nlohmann::json;

std::string
FaceAnalyzeLong(json& params);

std::string
FaceAnalyzeShort(json& params);

std::string
FaceAnalyze(json& params);

std::string
FaceAnalyzeStrong(json& params);

trade_list*
GetTradeList();




#endif //C_ALLA_EXPLORER_EXPLORER_FACE_H
