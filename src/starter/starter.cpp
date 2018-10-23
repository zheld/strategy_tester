//
// Created by caesaradmin on 17.04.18.
//

#include "starter.h"
#include <fstream>
#include "../core/json.hpp"
#include "../history/history.h"
#include "../core/core.h"
#include "../core/core_str.h"
#include "../alla/alla.h"
#include "../params/params.h"
#include "../prm.h"
#include <thread>
#include <unistd.h>
#include "../core/core_fl.h"

using json = nlohmann::json;
const std::string obj = "starter";

json get_config() {
    json js;
    std::ifstream stream("config.json", std::ifstream::in);
    stream >> js;
    stream.close();
    INFO("Config file is getting");
    return js;
}

void START(std::string& prname) {
    LOG(obj, "in start");

    std::vector<std::thread> threads_ls;
    // update config object
    auto config = get_config();

    // solution name
    std::string solution_name = config["solution"];
    INFO("Solution name: " + solution_name);

    // description description
    std::string solution_desc = config.dump();

    // initial history
    auto is_not_complete = history::InitSolution(solution_name, prname, solution_desc);
    if (!is_not_complete) {
        INFO("solution: " + solution_name + " is already completed.");
        return;
    }

    // trade list
    auto trade_ls = trade_list(prname);

    // threads count
    int thread_count = config["thread_count"];
    if (thread_count == 1) {
        RUN(trade_ls, config, 0);
        return;
    }

    auto params_ls = history::InitSessions(thread_count);

    if (params_ls.size() > 0) {
        for (int thread_nm = 0; thread_nm < params_ls.size(); ++thread_nm) {
            std::string thread_params = params_ls[thread_nm];
            INFO(thread_params);

            INFO("initial thread number: " + std::to_string(thread_nm));

            auto prm = params(thread_params);

            // start thread
//            RUN(thread_config, thread_nm);
            threads_ls.emplace_back(RUN, trade_ls, prm, thread_nm);
            uint microseconds = 4000000;
            usleep(microseconds);

        }
    } else {
        for (int thread_nm = 0; thread_nm < thread_count; ++thread_nm) {

            std::string cnf_str = config.dump();
            auto prm = params(cnf_str);
            prm.ThreadDiffParams(thread_count, thread_nm);

            std::string param_str = prm.GetData().dump();
            history::UpdateCurrentParams(prname, param_str, thread_nm);

            threads_ls.emplace_back(RUN, trade_ls, prm, thread_nm);
            uint microseconds = 4000000;
            usleep(microseconds);

        }
    }

    // join threads
    for (auto& tr : threads_ls) {
        tr.join();
    }

    // history close
    history::SolutionCompleted();

}

void START_ALL() {
    std::string path = conf::DIRECTORY + "/";

    INFO("START ALL, path: " + path);
    auto file_ls = FL_DIRECTORY_LIST(path);
    assert(file_ls.size() != 0);
    INFO("push..");
    for (auto& prname : file_ls) {
        INFO("Start analysis pair: " + prname);
        START(prname);
    }


}
