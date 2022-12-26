#pragma once

#include <string>
#include <map>
#include <unordered_map>

#include "buffer_loader.h"

using std::string;

namespace bell {
class DictManager {
public:
    int register_loader(const string& buffer_name, BufferLoader* loader);
    BufferLoader* get_buffer(const string& buffer_name);

    void load_all();
public:
    std::map<string, BufferLoader*> m_buffer_map;
};
}
