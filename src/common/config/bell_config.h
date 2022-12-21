#pragma once

#include "util/xml_helper.h"
#include <string>

namespace bell {

class BellConfig {
public:
    BellConfig() {};
    virtual ~BellConfig() {};
    bool init(const std::string& config_file);

private:
    void init_global(const XmlNode& golbal_node);

public:
    std::string m_word_seg_model_config;
};


}