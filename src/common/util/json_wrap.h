#pragma once

#include "json_cpp/reader.h"
#include "json_cpp/json.h"

#include <string>
#include <vector>
#include <map>
#include <set>

using std::string;
using std::vector;

namespace bell {

class JsonWrap {
public:
    bool json_parser(const string& src);

    unsigned int get_uint(const char* item_name) const;
    bool get_int(const char* item_name, int& res) const;
    bool get_float(const char* item_name, float& res) const;
    bool get_string(const char* item_name, string& res) const;

public:
    Json::Value m_value;
};

} // namespace bell