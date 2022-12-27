#include "dict_manager.h"
#include "muduo/base/Logging.h"

using namespace bell;

void DictManager::register_loader(BufferLoader* loader) {
    const string& buffer_name = loader->m_file_name;
    auto iter = m_buffer_map.find(buffer_name);
    if (iter != m_buffer_map.end()) {
        LOG_WARN
            << "buffer_name: " << buffer_name.c_str()
            << " has been loaded before, please check your file register!!";
        return -1;
    }
    m_buffer_map[buffer_name] = loader;
    LOG_INFO << "开始加载 dict: " << buffer_name.c_str();
    loader->load(buffer_name);
    LOG_INFO << "当前字典加载完毕: " << buffer_name.c_str();
    return;
}

BufferLoader* DictManager::get_buffer(const string& buffer_name) {
    auto iter = m_buffer_map.find(buffer_name);
    if (iter != m_buffer_map.end()) {
        return iter->second;
    }
    return nullptr;
}

void DictManager::load_all() {
    for (auto it = m_buffer_map.begin(); it != m_buffer_map.end(); it++) {
        const string& buffer_name = it->first;
        BufferLoader* buffer_loader = it->second;
        it->second->load(it->first);
    }
}
