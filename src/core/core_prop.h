#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace std;


namespace pt = boost::property_tree;

class property {
public:
    property(std::string& filename);

    template<typename K, typename T>
    T get(const K& key) { return tree.get<T>(key); }

    template<typename T, typename V>
    void set(const T& key, const V& val) { tree.put(key, val); }

    void save() { pt::write_json(filename, tree); }

private:
    std::string filename;
    pt::ptree tree;
};



