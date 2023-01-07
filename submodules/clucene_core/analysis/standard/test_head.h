#pragma once

#include <string>

class TestHead {
public:
    std::string m_test_str;
    static void add_str(std::string& str);
};