#pragma once


#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>


template<typename T, typename S>
std::vector<std::string> STR_SPLIT(const T& str, const S& sp) {
    std::vector<std::string> res;
    boost::algorithm::split(res, str, boost::algorithm::is_any_of(sp));
    return res;
}

template<typename S>
std::string STR_JOIN(const std::vector<std::string>& list, const S& sp) {
    return boost::algorithm::join(list, sp);
}

template<typename T>
int STR_TO_INT(const T& str) {
    return std::stoi(str);
}

template<typename T>
long STR_TO_LONG(const T& str) {
    return std::stol(str);
}

template<typename T>
float STR_TO_FLOAT(const T& str) {
    return std::stof(str);
}

template<typename T>
double STR_TO_DOUBLE(const T& str) {
    return std::stod(str);
}



