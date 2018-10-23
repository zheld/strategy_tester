//
// Created by caesaradmin on 21.04.18.
//

#ifndef C_ALLA_EXPLORER_PARAMS_H
#define C_ALLA_EXPLORER_PARAMS_H

#include "../core/json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;


class params {
public:
    params(const json& js) : data(js) { init_field(data); }

    params(const std::string& conf_str) : data(json::parse(conf_str)) { init_field(data); }

    params(char* file_name) {
        std::ifstream stream("config.json", std::ifstream::in);
        stream >> data;
        stream.close();
        init_field(data);
    }

    json& GetParamByName(const std::string& indi, const std::string& name) {
        if (indi == "") {
            return data[name];
        } else {
            for (auto& indi_param : data["indicators"]) {
                if (indi == indi_param["name"]) {
                    for (auto& param : indi_param["params"]) {
                        if (name == param["name"]) {
                            return param;
                        }
                    }
                }
            }
        }

    }

    json& GetIndicatorParam(int idx) {
        assert(idx < param_len);
        int count = 0;

        for (auto& core_param : data["params"]) {
            if (count == idx) {
                return core_param;
            }
            count++;

        }

        for (auto& indi_param : data["indicators"]) {
            for (auto& param : indi_param["params"]) {
                if (count == idx) {
                    return param;
                }
                count++;
            }
        }
    }

    int ParamLen() { return param_len; }

    int IndiLen() { return indi_len; }

    int VarParamLen() { return var_param_len; }

    json& GetData() { return data; }

    void ThreadDiffParams(int thread_count, int thread_nm) {
        // insert different param
        int is_diff;
        for (auto& jindi : data["indicators"]) {
            for (auto& pr : jindi["params"]) {
                is_diff = pr["diff"];
                if (is_diff == 1) {
                    int start = pr["start"];
                    int end = pr["end"];
                    int len_tr = int((end - start) / thread_count);

                    int st = start + thread_nm * len_tr;
                    pr["start"] = st;

                    int en = (st + len_tr) - 1;
                    pr["end"] = en;
                    pr["current"] = st;
                    std::cout << "THREAD NO: " << thread_nm << ", START: " << st << ", END: " << en << ", start:"
                              << start << ", end:" << end << std::endl;

                }
            }
        }
    }

    std::string String() {
        std::string res = "";

        for (auto& param : data["params"]) {
            std::string n = param["name"];
            double c = param["current"];
            res += ("   " + n + " = " + std::to_string(c));
        }

        for (auto& indi_param : data["indicators"]) {
            for (auto& param : indi_param["params"]) {
                std::string n = param["name"];
                double c = param["current"];
                res += ("   " + n + " = " + std::to_string(c));
            }
        }
        return res;
    }


private:
    void init_field(const json& js) {
        param_len = 0;
        indi_len = 0;
        var_param_len = 0;
        for (auto& prm : js["params"]) {
            param_len++;
            if (prm["var"] == true) {
                var_param_len++;
            }
        }
        for (auto& indi : js["indicators"]) {
            indi_len++;
            for (auto& prm : indi["params"]) {
                param_len++;
                if (prm["var"] == true) {
                    var_param_len++;
                }
            }
        }
    }

    int var_param_len;
    int param_len;
    int indi_len;
    json data;
};


#endif //C_ALLA_EXPLORER_PARAMS_H
