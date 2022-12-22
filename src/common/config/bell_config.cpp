#include "bell_config.h"
#include <unistd.h>
#include "muduo/base/Logging.h"

using namespace std;
using namespace bell;

bool BellConfig::init(const std::string& config_file) {
    if (access(config_file.c_str(), R_OK) == -1) {
        LOG_ERROR << "config file doesn't exist" << config_file.c_str();
        return false;
    }

    XmlDoc parser;
    auto root = parser.parser(config_file.c_str());

    if (!root.isValid()) {
        LOG_ERROR << "fail to get node";
        return false;
    }

    auto node = root["global_config"];
    if (node.isValid()) {
        LOG_ERROR << "global_config section init";
        init_global(node);
    } else {
        LOG_ERROR << "global_config  not found";
        return false;
    }

    return true;
}

void BellConfig::init_global(const XmlNode& global_node) {
    auto node_list = GET_XML_CONF_LIST(global_node, "entry");
    LOG_INFO << "*********** global config init begin *******************";
    for (auto iter = node_list.begin(); iter!= node_list.end(); ++iter) {
        std::string name;
        GET_XML_CONF_STRING(*iter, "key", "", &name);
        if (name.empty()) {
            LOG_ERROR << "global_config has an empty key";
            continue;
        }

        if (name == "WordSeg") {
            GET_XML_CONF_STRING(*iter, "word_seg_cnncrf_conf", "", &m_word_seg_model_config);
            LOG_INFO << "word_seg_cnncrf_conf " << m_word_seg_model_config.c_str();
        } 
    }

    LOG_INFO << "*********** global config init done *******************";
}