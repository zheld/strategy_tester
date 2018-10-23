#include "history.h"
#include "../core/core_db.h"
#include "../core/core_str.h"
#include "../core/core.h"
#include <boost/format.hpp>
#include <mutex>

using boost::format;

std::mutex m;
std::string history::sol_id = "";
std::string history::core_id = "";
int history::thread_nums = 0;

const std::string obj = "history";


PGresult*
mutexSqlQueryResult(std::string& sql) {
    m.lock();
    auto res = SQL_QUERY_RESULT(sql);
    m.unlock();
    return res;
}


void
mutexSqlQuery(std::string& sql) {
    m.lock();
    SQL_QUERY(sql);
    m.unlock();
}


std::vector<std::string>
history::InitSessions(int threads_numbs) {
    LOG(obj, "in Init");

    if (history::is_first_session()) {
        INFO("Its first start the solution");
        std::string sql = "";
        auto fmt = format("INSERT INTO session(solution, thread) VALUES(" + sol_id + ",%s);");
        for (int i = 0; i < threads_numbs; ++i) {
            sql += (fmt % i).str();
        }
        mutexSqlQuery(sql);
        return std::vector<std::string>();

    } else {
        std::string sql_sess_last = (boost::format(
                "SELECT thread, params "
                "FROM session "
                "WHERE solution = %s ORDER BY thread") %
                                     sol_id).str();
        auto sess_res = mutexSqlQueryResult(sql_sess_last);

        // result
        int rows_count = PQntuples(sess_res);
        auto res = std::vector<std::string>(rows_count);

        for (int row = 0; row < rows_count; ++row) {
            std::string line = PQgetvalue(sess_res, row, 1);
            res[row] = line;
        }
        SQL_RESULT_CLEAR(sess_res);

        return res;
    }
}

bool
history::is_first_session() {
    LOG(obj, "in is_first_session");

    auto sql = "SELECT count(0) FROM session WHERE solution = " + sol_id;

    auto res = mutexSqlQueryResult(sql);
    std::string count_sess = PQgetvalue(res, 0, 0);
    SQL_RESULT_CLEAR(res);
    return count_sess == "0";
}

void
history::InsertProfit(bool is_long, std::string& prname, Report& report, std::string& params) {
    LOG(obj, "in InsertProfit");

    // session
    std::string profit_str = std::to_string(report.profit);
    std::string sql;
    if (is_long) {
        sql = (boost::format(
                "INSERT INTO profit(solution, is_long, pair, profit, best_params, report)"
                "SELECT %2%,%3%,'%5%',%4%,'%1%','%6%' FROM solution "
                "WHERE id = %2% AND profit_long < %4%;"

                "UPDATE solution "
                "SET profit_long = %4% "
                "WHERE id = %2% AND profit_long < %4%;"

        ) % params % sol_id % "TRUE" % profit_str % prname % report.String()).str();
    } else {
        sql = (boost::format(
                "INSERT INTO profit(solution, is_long, pair, profit, best_params, report)"
                "SELECT %2%,%3%,'%5%',%4%,'%1%','%6%' FROM solution "
                "WHERE id = %2% AND profit_short < %4%;"

                "UPDATE solution "
                "SET profit_short = %4% "
                "WHERE id = %2% AND profit_short < %4%;"

        ) % params % sol_id % "FALSE" % profit_str % prname % report.String()).str();
    }

//    INFO(sql);

    mutexSqlQuery(sql);
}

void history::UpdateCurrentParams(std::string& prname, std::string& params, int thread_nm) {
    LOG(obj, "in UpdateCurrentParams");

    // session
    std::string thread_str = std::to_string(thread_nm);

    std::string sql = (boost::format(
            "UPDATE session "
            "SET params = '%1%' "
            "WHERE solution = %2% AND thread = %3%; ") % params % sol_id % thread_str).str();
    mutexSqlQuery(sql);
//    INFO(sql);
}

bool
history::InitSolution(std::string& solution_name, std::string& prname, std::string& solution_description) {
    LOG(obj, "in init_solution");

    // core
    auto sql_core_id =
            "SELECT id FROM core WHERE name = '" + solution_name + "';";
    auto res_core = mutexSqlQueryResult(sql_core_id);

    if (res_core != nullptr) {
        std::string c_id = PQgetvalue(res_core, 0, 0);
        SQL_RESULT_CLEAR(res_core);
        core_id = c_id;
    } else {
        LOG(obj, "the solution never started, we create core for it: " + solution_name);
        auto sql =
                "INSERT INTO core(name, description) "
                "VALUES('" + solution_name + "','" + solution_description + "') RETURNING id;";
        auto res_s = mutexSqlQueryResult(sql);
        auto c_id = PQgetvalue(res_s, 0, 0);
        SQL_RESULT_CLEAR(res_s);
        core_id = c_id;
    }


    // solution
    auto sql_sol_id =
            "SELECT id, complete FROM solution WHERE core = " + core_id + " AND pair = '" + prname + "';";
    auto res = mutexSqlQueryResult(sql_sol_id);

    if (res != nullptr) {
        std::string solution_id = PQgetvalue(res, 0, 0);
        std::string solution_complete = PQgetvalue(res, 0, 1);
        SQL_RESULT_CLEAR(res);
        if (solution_complete == "t") {
            return false;
        }
        sol_id = solution_id;


    } else {
        auto sql =
                "INSERT INTO solution(core, pair, profit_long, profit_short, complete) "
                "VALUES(" + core_id + ",'" + prname + "', 0.0, 0.0, FALSE ) RETURNING id;";
        auto res_s = mutexSqlQueryResult(sql);
        auto solution_id = PQgetvalue(res_s, 0, 0);
        SQL_RESULT_CLEAR(res_s);
        sol_id = solution_id;
        return true;
    }

}

void history::SolutionCompleted() {
    LOG(obj, "in ExplorerCompleted");

    std::string sql = (boost::format(
            "UPDATE solution "
            "SET complete = TRUE "
            "WHERE id = %1% ;"
    ) % sol_id).str();
    mutexSqlQuery(sql);
//    INFO(sql);
}





