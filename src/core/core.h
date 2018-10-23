#pragma once

#include <string>
#include <iostream>


template<typename T>
void message(const std::string& prefix, const T& msg) {
    std::cout << prefix << "   " << msg << std::endl;
}

template<typename T>
void INFO(const T& msg) {
    message("INFO ", msg);
}

template<typename T>
void LOG(const std::string& object, const T& msg) {
    std::cout << "LOG  => [" << object << "]   " << msg << std::endl;
}

template<typename T>
void ERROR(const T& msg) {
    message("ERROR", msg);
}



