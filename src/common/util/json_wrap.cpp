#include "json_wrap.h"
#include "muduo/base/Logging.h"
#include <stdexcept>
#include <sstream>

using namespace std;
using namespace Json;
using namespace bell;

bool JsonWrap::json_parser(const string& src) {
    bool sucess;
    try {
        Json::Reader reader;
        sucess = reader.parse(src, m_value);
    } catch (runtime_error& err) {
        LOG_ERROR << "json parser file error " << err.what();
        return false;
    }
    if (!sucess) {
        LOG_ERROR << "json data maybe wrong, input: " << src.c_str();
        return false;
    }
    return true;
}

unsigned int JsonWrap::get_uint(const char* item_name) const {
    if (m_value.isMember(item_name)) {
        try {
            return m_value[item_name].asUInt();

        } catch (runtime_error& err) {
            LOG_ERROR << "json parser fail: " << err.what();
        }
    }
    return -1;
}

bool JsonWrap::get_int(const char* item_name, int& res) const {
    if (m_value.isMember(item_name)) {
        try {
            res = m_value[item_name].asInt();
            return true;

        } catch (runtime_error& err) {
            LOG_ERROR << "json parser fail: " << err.what();
        }
    }
    return false;
}

bool JsonWrap::get_float(const char* item_name, float& res) const {
    if (m_value.isMember(item_name)) {
        try {
            res = m_value[item_name].asDouble();
            return true;

        } catch (runtime_error& err) {
            LOG_ERROR << "json parser fail: " << err.what();
        }
    }
    return false;
}

bool JsonWrap::get_string(const char* item_name, string& res) const {
    if (m_value.isMember(item_name)) {
        try {
            res = m_value[item_name].asString();
            return true;

        } catch (runtime_error& err) {
            LOG_ERROR << "json parser fail: " << err.what();
        }
    }
    return false;
}