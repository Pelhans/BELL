#pragma once

#include "base_record.h"

namespace bell {
class KeyValueRecord : public BaseRecord {
public:
    virtual int recordParse(const std::string& line, std::string& key);
    void printSelf() { LOG_INFO << "KeyValueRecord parse value" << m_value; };

public:
    float m_value;
};
} // namespace bell