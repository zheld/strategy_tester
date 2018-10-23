#ifndef ANALYTIC_ALICE_DB_H
#define ANALYTIC_ALICE_DB_H

#include <postgresql/libpq-fe.h>
#include <string>

#define  SQL_QUERY_RESULT(query) ( DB::SqlQueryResult( query ) )
#define  SQL_QUERY(query) ( DB::SqlQuery( query ) )
#define  SQL_RESULT_CLEAR(res) ( DB::ResultClear( res ) )


class DB {
public:
    //
    static void Init();

    static PGconn* Connect_To_DB();

    static PGresult* SqlQueryResult(const std::string& query);

    static void SqlQuery(const std::string& query);

    static void ResultClear(PGresult* res) {PQclear(res);}

private:
    static PGconn* conn;

};


#endif //ANALYTIC_ALICE_DB_H
