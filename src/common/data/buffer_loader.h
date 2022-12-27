#pragma once

#include <string>

using std::string;

class BufferLoader {
public:
    string m_str_buffer_name;
    string m_file_name;

public:
    BufferLoader() {}
    virtual int load(const string& fileName) = 0;

    virtual void* getBufferPtr() = 0;
    virtual ~BufferLoader(){};
};