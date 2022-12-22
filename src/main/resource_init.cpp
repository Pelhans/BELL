#include "resource_init.h"
#include "common/model/model_manager.h"
#include "common/model/model_seg_cnncrf.h"
#include "muduo/base/Logging.h"

#include <boost/serialization/singleton.hpp>

using namespace std;
using namespace bell;

std::shared_ptr<std::thread> ResourceInit::thd = nullptr;

void ResourceInit::async_init_wait() {
    if (thd) {
        thd->join();
        thd.reset();
    }
    LOG_INFO << "ResourceInit::async_init_wait end.";
}

void ResourceInit::async_init(const BellConfig& config) {
    init_func(config);
}

bool ResourceInit::init(const BellConfig& config) {
    bool ret = init_func(config);
    return ret;
}

bool ResourceInit::init_func(const bell::BellConfig& config) {
    LOG_INFO << "ResourceInit::init begin...";

    if (!config.m_word_seg_model_config.empty() && !initWordSeg("word_seg_cnn_crf", config.m_word_seg_model_config)) {
        LOG_ERROR << "fail to init word seg cnn crf";
        return false;
    }

    LOG_INFO << "Resource::init_func end.";
}

bool ResourceInit::initWordSeg(const std::string& model_name, const std::string& cnn_crf_config) {
    auto model = std::make_shared<bell_model::ModelSegCNNCRF>(model_name, cnn_crf_config);
    //return singleton<bell_mode::ModelMgr>::getInstance()->reg_model(model)
    auto model_mgr = boost::serialization::singleton<bell_model::ModelMgr>::get_const_instance();
    return model_mgr.reg_model(model);
}
