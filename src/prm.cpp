//
// Created by caesaradmin on 17.03.18.
//
#include "prm.h"
#include "core/core.h"
#include <thread>
#include <unistd.h>
#include <fstream>


json get_conf() {
    json js;
    std::ifstream stream("manifest.json", std::ifstream::in);
    stream >> js;
    stream.close();
    INFO("Manifest file is getting");
    return js;
}

json conf::data = get_conf();
std::string conf::DIRECTORY = conf::data["directory"];


boost::posix_time::ptime conf::FIRST_TRADE = boost::posix_time::ptime(
        boost::gregorian::date(2018, boost::gregorian::Jun, 1));

std::string conf::DB_CONN = conf::data["db_conn"];

