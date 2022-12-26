#include "dict_manager.h"
#include "muduo/base/Logging.h"

using namespace bell;

int DictManager::register_loader(const string& buffer_name, BufferLoader* loader){
    auto iter = m_buffer_map.find(buffer_name);
    if (iter != m_buffer_map.end()) {
        LOG_WARN << "buffer_name: " << buffer_name.c_str() << " has been loaded before, please check your file register!!";
        return -1;
    }
    m_buffer_map[buffer_name] = loader;
    LOG_INFO << "开始加载 dict: " << buffer_name.c_str();
    loader->load(buffer_name);
    return 0;
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
