#include "api.h"
#include "../core/core_str.h"
#include "../core/json.hpp"
#include "../analyzer/explorer_face.h"

using json = nlohmann::json;

const std::string obj = "api";


std::string
get_report() {
    std::string report;

    report = "it is a report... it work!";

    return report;
}

std::string
Execute(char* msg) {
    auto query = std::string(msg);
//    LOG(obj, "execute::query:" + query);

    std::string result = "";

    json conf;
    conf = json::parse(query);

    std::string method = conf["method"];
    json payload = conf["payload"];

    if (method == "echo") {
        result = payload;
    } else if ( method == "analyst"){
        result = FaceAnalyze(payload);
    } else if ( method == "analyst_strong"){
        result = FaceAnalyzeStrong(payload);
    } else if (method == "an_long") {
        result = FaceAnalyzeLong(payload);
    } else if (method == "an_short") {
        result = FaceAnalyzeShort(payload);
    }

//    LOG(obj, "execute::method:" + method + ", response:" + result);
//    std::cout << result << std::endl;
    return result;
}


