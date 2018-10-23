#include "analyzer.h"
#include "../core/core_str.h"
#include <memory>

int
get_step_from_config(json& conf) {
    auto params = conf["params"];
    for (auto& pr : params) {
        if (pr["name"] == "step") {
            return pr["current"];
        }
    }
    return 0;
}

Report
long_AnalyzeAndReportStrong(json& conf) {
    double price = 0;
    double sig = 0;

    int stop_loss = -1;
    int try_exit = 1;

    json& jindi = conf["indicators"][0];

    // signal block
    signal_block sb = CalcSignalsStrong(jindi);

    // position
    Engine engine = Engine(true, 1000, stop_loss, get_step_from_config(conf), try_exit);

    for (int x = 14400; x < sb.Len(); x++) {
        price = (*sb.price_ls)[x];
        sig = (*sb.signal_ls)[x];

        if (engine.IsOpen()) {
            if (sig < 0) {
                engine.ClosePosition(price, x);
            }
        } else {
            if (sig > 0) {
                engine.OpenPosition(price, x);
            }
        }
    }

    auto report = engine.GetReport();

    return report;
}

Report
Long_AnalyzeAndReport(json& conf, std::vector<candle>& cls) {
    // indicator list
    int ind_period = 0;
    int max_period = 0;
    int c = 0;

    std::vector<std::vector<double>> signal_ls = std::vector<std::vector<double>>();

    for (json& jindi : conf["indicators"]) {
        if (ind_period > max_period) {
            max_period = ind_period;
        }
        // indicator list
        signal_ls.emplace_back(cls.size());
        CalcSignals(jindi, cls, signal_ls[c]);
        c++;
    }

    // operation price
    double price = 0;
    double sig = 0;

    int stop_loss = -1;

    // try_exit
    int try_exit = 1;

    // position
    Engine engine = Engine(true, 1000, stop_loss, get_step_from_config(conf), try_exit);

    for (int x = max_period; x < cls.size(); x++) {
        price = cls[x].LastPrice;
        sig = signal_ls[0][x];
//        sig_ema = signal_ls[1][x];

        if (engine.IsOpen()) {
            if (sig < 0) {
                engine.ClosePosition(price, x);
            }
//            engine.NextPrice(price, x);
        } else {
            if (sig > 0) {
                engine.OpenPosition(price, x);
            }
        }
    }
    return engine.GetReport();
}

Report
Short_AnalyzeAndReport(json& conf, std::vector<candle>& cls) {
    // indicator list
    int ind_period = 0;
    int max_period = 0;
    int c = 0;

    std::vector<std::vector<double>> signal_ls = std::vector<std::vector<double>>();

    for (json& jindi : conf["indicators"]) {
        if (ind_period > max_period) {
            max_period = ind_period;
        }
        // indicator list
        signal_ls.emplace_back(cls.size());
        CalcSignals(jindi, cls, signal_ls[c]);
        c++;
    }


    // operation price
    double price = 0;
    double sig = 0;

    // stop loss
    int stop_loss = -1;
//    for (auto& prm : conf["params"]) {
//        if (prm["name"] == "stop_loss") {
//            stop_loss = prm["current"];
//        }
//    }

    // try_exit
    int try_exit = 1;
//    for (auto& prm : conf["params"]) {
//        if (prm["name"] == "try_exit_count") {
//            try_exit = prm["current"];
//        }
//    }

    // position
    Engine engine = Engine(false, 1000, stop_loss, get_step_from_config(conf), try_exit);

    for (int x = max_period; x < cls.size(); x++) {
        price = cls[x].LastPrice;
        sig = signal_ls[0][x];

        if (engine.IsOpen()) {
            if (sig > 0) {
                engine.ClosePosition(price, x);
            }
//            engine.NextPrice(price, x);
        } else {
            if (sig < 0) {
                engine.OpenPosition(price, x);
            }
        }
    }
    return engine.GetReport();
}


