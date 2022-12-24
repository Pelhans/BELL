#include "config_manager.h"
#include "muduo/base/Logging.h"
#include "util/xml_helper.h"
#include <vector>
#include <unistd.h>

using namespace bell;
using namespace std;

static const std::string DEFAULT_CONFIG_NAME = "default";

bool ConfigManager::init(const std::string& config_file,
                         bool use_singel_config) {
    m_config_file = config_file;
    m_use_single_config = false;
    m_has_init = true;
    if (use_singel_config) {
        m_config_map[DEFAULT_CONFIG_NAME] = m_base_config;
        return m_base_config.init(config_file);
    } else {
        return load_config_mapping(config_file);
    }
}

const BellConfig& ConfigManager::getDefaultConfig() const {
    return m_base_config;
}

const BellConfig& ConfigManager::getConfig(
    const std::string& config_name) const {
    auto iter = m_config_map.find(config_name);
    if (iter == m_config_map.end()) {
        LOG_ERROR << "unknown config name " << config_name.c_str();
        return m_base_config;
    }
    return iter->second;
}

bool ConfigManager::load_config_mapping(const std::string& mapping_file) {
    if (access(mapping_file.c_str(), R_OK) == -1) {
        LOG_ERROR << "config file doesn't exist: " << mapping_file.c_str();
        return false;
    }

    XmlDoc parser;
    auto root = parser.parser(mapping_file.c_str());
    // 读取默认的配置文件
    std::string default_config_file;
    GET_XML_CONF_STRING(root, "default_config", "", &default_config_file);
    BellConfig config;
    if (!config.init(default_config_file)) {
        LOG_ERROR << "fail to init default config file"
                  << default_config_file.c_str();
        return false;
    }
    m_base_config = config;

    // 把 base 加入 map, name 为 default
    m_config_map[DEFAULT_CONFIG_NAME] = m_base_config;
    return true;
}