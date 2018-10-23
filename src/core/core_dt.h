#pragma once

#include <boost/date_time/posix_time/posix_time.hpp>

using namespace boost::posix_time;


ptime NOW();

std::string TIME_TO_STRING(const ptime& dt);

ptime STR_TO_TIME(const std::string& str);

long TIME_TO_UNIX(const ptime& dt);

ptime UTC_TO_TIME(long utc);
