#include "core_fl.h"


void FL_CREATE_DIRECTORY(const std::string& path) {
    boost::filesystem::path full_path(path);
    if (!boost::filesystem::exists(full_path)) {
        mkdir(path.c_str(), 0777);
    }
}

bool FL_IS_DIRECTORY(const std::string& path) {
    boost::filesystem::path full_path(path);
    return boost::filesystem::is_directory(full_path);
}

std::vector<std::string> FL_FILE_LIST(const std::string& path) {
    auto res = std::vector<std::string>();
    boost::filesystem::path full_path(path);
    boost::filesystem::directory_iterator end_iter;
    for (boost::filesystem::directory_iterator dir_itr(full_path); dir_itr != end_iter; ++dir_itr) {
        if (boost::filesystem::is_regular_file(dir_itr->status())) {
            res.push_back(dir_itr->path().filename().string());
        }
    }
    return res;

}

std::vector<std::string> FL_DIRECTORY_LIST(const std::string& path) {
    auto res = std::vector<std::string>();
    boost::filesystem::path full_path(path);
    boost::filesystem::directory_iterator end_iter;
    for (boost::filesystem::directory_iterator dir_itr(full_path); dir_itr != end_iter; ++dir_itr) {
        if (boost::filesystem::is_directory(dir_itr->status())) {
            res.push_back(dir_itr->path().filename().string());
        }
    }
    return res;

}

bool FL_EXISTS(const std::string& path) {
    boost::filesystem::path full_path(path);
    return boost::filesystem::exists(full_path);
}

std::string FL_READ(const std::string& path) {
    return std::string();
}
