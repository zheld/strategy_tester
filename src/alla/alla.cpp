#include "alla.h"
#include "../history/history.h"
#include "../analyzer/analyzer.h"
#include "../trades/trade_list.h"



void save_profit(bool is_long, json& params, Report& report, std::string& prname) {
    INFO("save report with profit: " + std::to_string(report.profit));
    std::string str = params.dump();
    // insert history
    history::InsertProfit(is_long, prname, report, str);
}


void update_params(json& params, std::string& prname, int number) {
    std::string str = params.dump();
    // insert history
    history::UpdateCurrentParams(prname, str, number);
}


void
WALK(params& prm_ls, int state, double& max_profit_long, double& max_profit_short, int number, std::vector<candle>& cls,
     std::string& prname) {
//    INFO("run walk, state: " + std::to_string(state));
    json& prm = prm_ls.GetIndicatorParam(state);

//    std::cout << "param name: " << prm["name"] << std::endl;

    json& conf = prm_ls.GetData();
    double current = prm["current"];
    double start = prm["start"];
    double end = prm["end"];
    double step = prm["step"];

    Report report;
    int count_for_save = 0;

    state++;
    for (; current <= end; current += step) {
        prm["current"] = current;
        if (state == prm_ls.VarParamLen()) {

            // ANALYSIS LONG
            report = std::move(Long_AnalyzeAndReport(conf, cls));
            if (report.profit > max_profit_long) {
                max_profit_long = report.profit;
                save_profit(true, conf, report, prname);
            }

            // ANALYSIS SHORT
            report = std::move(Short_AnalyzeAndReport(conf, cls));
            if (report.profit > max_profit_short) {
                max_profit_short = report.profit;
                save_profit(false, conf, report, prname);
            }

        } else {
            WALK(prm_ls, state, max_profit_long, max_profit_short, number, cls, prname);
        }

        if (state == prm_ls.VarParamLen() - 2) {
            count_for_save++;
            if (count_for_save == 30) {
                std::cout << "NO:" << number << "   MAX_LONG: " << max_profit_long << "   MAX_SHORT: "
                          << max_profit_short << std::endl;
                update_params(conf, prname, number);
                INFO(" NO: " + std::to_string(number) + "   " + prm_ls.String());
                count_for_save = 0;
            }
        }

    }

    prm["current"] = start;

}


void
RUN(trade_list trade_ls, params prm, int number) {
    INFO("run analyst thread number: " + std::to_string(number));

    std::cout << "param ls size: " << prm.VarParamLen() << std::endl;
    json& conf = prm.GetData();
    json& core_step_prm = conf["params"][0];

//    int thread_count = prm.GetParamByName("", "thread_count");
    double max_profit_long = 0;
    double max_profit_short = 0;


    int start_current = core_step_prm["current"];
    int start_end = core_step_prm["end"];
    int start_step = core_step_prm["step"];


    for (int i = start_current; i < start_end; i += start_step) {
        std::vector<candle> cls = trade_ls.GetCandleList(i);
        conf["params"][0]["current"] = i;
        std::string prname = trade_ls.GetPairName();
        WALK(prm, 1, max_profit_long, max_profit_short, number, cls, prname);

    }
    std::cout << "###" << number << "  REPORT: MAX_LONG: " << max_profit_long << "   MAX_SHORT: "
              << max_profit_short << std::endl;

}


