#pragma once

#include <string>

namespace bell {
namespace event {

class BaseSignalOutput{
public:
    BaseSignalOutput() {}
    virtual ~BaseSignalOutput() {}

    const std::string& GetName() const { return m_name; }
    void SetName(const std::string& name) { m_name = name; }

private:
    std::string m_name;
};

}
}