#include "core_db.h"
#include "../prm.h"
#include "core.h"

// initialize connect to main data base
PGconn* DB::conn = nullptr;


PGconn* DB::Connect_To_DB() {
    const char* connect_c = conf::DB_CONN.c_str();

    PGconn* conn = PQconnectdb(connect_c);

    if (PQstatus(conn) == CONNECTION_BAD) {
        ERROR("Connect to database fail, conn: " + conf::DB_CONN);
    } else {
        INFO("Connect to database success. connect string: " + conf::DB_CONN);
    }
    return conn;
}

void DB::SqlQuery(const std::string& query) {
    try {
        PGresult* result = PQexec(DB::conn, query.c_str());
        PQclear(result);
    }
    catch (std::exception& e) {
        std::string error(e.what());
        ERROR("in to DB fail: " + error);
    }
}

PGresult* DB::SqlQueryResult(const std::string& query) {
    try {
        PGresult* result = PQexec(DB::conn, query.c_str());

        if (PQresultStatus(result) == PGRES_TUPLES_OK && PQntuples(result) > 0) {
            return result;
        }
        PQclear(result);
        return nullptr;
    }
    catch (std::exception& e) {
        std::string error(e.what());
        ERROR("in to DB fail: " + error);
        return nullptr;
    }
}

void DB::Init() {
// initialize connect to main data base
    DB::conn = DB::Connect_To_DB();
}

