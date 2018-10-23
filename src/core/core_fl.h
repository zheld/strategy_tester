#pragma once

#include <sys/stat.h>
#include <iostream>
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"

void FL_CREATE_DIRECTORY(const std::string& path);

std::vector<std::string> FL_FILE_LIST(const std::string& path);

std::vector<std::string> FL_DIRECTORY_LIST(const std::string& path);

bool FL_IS_DIRECTORY(const std::string& path);

bool FL_EXISTS(const std::string& path);

std::string FL_READ(const std::string& path);