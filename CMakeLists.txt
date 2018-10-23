cmake_minimum_required(VERSION 3.9)
project(strategy_tester)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -lpq  -lboost_system -lboost_date_time -lboost_filesystem ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread -march=native -O3 -Wno-error")
find_package(Boost COMPONENTS system thread REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_executable(
        strategy_tester
        main.cpp

        src/prm.h
        src/prm.cpp

        src/trades/second.cpp
        src/trades/second.h
        src/trades/trade_list.cpp
        src/trades/trade_list.h

        src/core/core.h
        src/core/core.cpp
        src/core/core_fl.cpp
        src/core/core_fl.h
        src/core/core_dt.cpp
        src/core/core_dt.h
        src/core/core_str.cpp
        src/core/core_str.h
        src/core/core_prop.cpp
        src/core/core_prop.h
        src/core/core_db.cpp
        src/core/core_db.h

        src/analyzer/analyzer.cpp
        src/analyzer/analyzer.h

        src/indicators/indicator.cpp
        src/indicators/indicator.h
        src/indicators/movings.cpp
        src/indicators/movings.h

        src/history/history.cpp
        src/history/history.h

        src/starter/starter.cpp
        src/starter/starter.h

        src/alla/alla.cpp
        src/alla/alla.h

        src/params/params.cpp
        src/params/params.h

        src/indicators/signals.cpp
        src/indicators/signals.h

        src/api/rpc_server.cpp
        src/api/rpc_server.h

        src/api/api.cpp
        src/api/api.h

        src/analyzer/engine.cpp
        src/analyzer/engine.h
        src/analyzer/explorer_face.cpp src/analyzer/explorer_face.h src/api/mult_connect.cpp)
