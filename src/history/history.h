#ifndef C_ALLA_EXPLORER_HISTORY_H
#define C_ALLA_EXPLORER_HISTORY_H


#include <boost/algorithm/string.hpp>
#include "../analyzer/analyzer.h"

class history {
public:
    static std::vector<std::string>
    InitSessions(int threads_numbs);

    static void
    InsertProfit(bool is_long, std::string& prname, Report& report, std::string& params);

    static void
    UpdateCurrentParams(std::string& prname, std::string& params, int thread_nm);

    static void
    SolutionCompleted();

    static bool
    InitSolution(std::string& solution_name, std::string& prname, std::string& solution_description);

private:
    static int thread_nums;
    static std::string sol_id;

    static std::string core_id;

    static bool is_first_session();
};


#endif //C_ALLA_EXPLORER_HISTORY_H
