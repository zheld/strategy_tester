#include "src/starter/starter.h"
#include "src/api/rpc_server.h"
#include "src/core/core_db.h"

int main() {
    // database initialization
    DB::Init();


    StartRPC();

    START_ALL();
    return 0;
}