#include "key_value_record.h"
#include "muduo/base/Logging.h"

using std::string;
using namespace bell;

int KeyValueRecord::recordParse(const string& line, string& key) {
    bell::JsonWrap js;
    bool ret = js.json_parser(line);

    if (!ret) {
        LOG_ERROR << "line: " << line.c_str() << " json_parser is failed!!";
        return ret;
    }

    if (js.m_value.isMember("key_word")) {
        ret = js.get_string("key_word", key);
        if (!ret) {
            LOG_ERROR << "line: " << line.c_str()
                      << " get_string key_word is failed!!";
            return ret;
        }
    } else {
        LOG_ERROR << "line: " << line.c_str() << " don't has key_word!!";
        return ret;
    }

    if (js.m_value.isMember("value")) {
        ret = js.get_float("value", m_value);
        if (!ret) {
            LOG_ERROR << "line: " << line.c_str()
                      << " get_string value is failed!!";
            return ret;
        }
    }

    return 0;
}