#pragma once

#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "core/json.hpp"

using json = nlohmann::json;

class conf {
public:
    // conf
    static json data;

    // directory
    static std::string DIRECTORY;
    // start date
    static boost::posix_time::ptime FIRST_TRADE;
    // DB connection string
    static std::string DB_CONN;
};