#pragma once

#include <string>
#include <vector>

#include "muduo/base/Logging.h"
#include "util/json_wrap.h"

using std::string;

namespace bell {

class Record {
public:
    Record() : m_flag(-1), m_invalid(false) {}
    virtual ~Record() {}

    virtual int recordParse(const string& line, string& key) { return -1; }

    virtual int recordParse(const string& line) { return -1; }

    virtual int permuteRecordParse(const string& line,
                                   std::vector<string>& key) {
        return -1;
    }

    virtual int recordParseIncrement(const string& line, string& key) {
        parseIsInvalid(line);
        return recordParse(line, key);
    }

    int parseIsInvalid(const string& line) {
        bell::JsonWrap js;
        bool ret = js.json_parser(line);
        if (!ret) {
            LOG_ERROR << "file line: " << line.c_str()
                      << " json_parser is failed!!!";
        }
        int temp = 0;
        js.get_int("invalid", temp);
        m_invalid = (temp == 1);
        return 0;
    }

public:
    int32_t m_flag;
    bool m_invalid;
};

} // namespace bell