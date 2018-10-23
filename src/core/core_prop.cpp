#include "core_prop.h"

property::property(std::string& fl) : filename(fl) {
    pt::read_json(fl, tree);
}
