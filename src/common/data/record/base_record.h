#pragma once

#include "common/data/record.h"
#include "json_cpp/json.h"
#include <string>

namespace bell {

class BaseRecord : public Record {
public:
    BaseRecord() {}
    virtual ~BaseRecord() {}

    virtual int recordParse(const std::string& line, std::string& key) = 0;
    virtual void printSelf() = 0;
};

} // namespace bell