#include "core_dt.h"

ptime genesis(boost::gregorian::date(1970, boost::gregorian::Jan, 1));

ptime NOW() {
    return microsec_clock::local_time();
}

std::string TIME_TO_STRING(const ptime& dt) {
    return to_simple_string(dt);
}

ptime STR_TO_TIME(const std::string& str) {
    return time_from_string(str);
}

long TIME_TO_UNIX(const ptime& dt) {
    return (dt - genesis).total_seconds();// - 10800;
}

ptime UTC_TO_TIME(long utc) {
    return genesis + seconds(utc);
}

