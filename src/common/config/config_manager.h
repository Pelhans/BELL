#pragma once

#include "bell_config.h"
#include <string>
#include <map>

namespace bell {

class ConfigManager {
public:
    ConfigManager() : m_has_init(false) {}
    bool init(const std::string& config_file, bool m_single_config);
    const BellConfig& getDefaultConfig() const;
    const BellConfig& getConfig(const std::string& config_name) const;

public:
    std::string m_config_file;
    bool m_use_single_config;
    bool m_has_init;

private:
    bool load_config_mapping(const std::string& mapping_file);

private:
    BellConfig m_base_config;
    std::map<std::string, BellConfig> m_config_map;
};

};