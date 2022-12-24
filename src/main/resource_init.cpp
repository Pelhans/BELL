#include "resource_init.h"
#include "common/model/model_manager.h"
#include "common/model/model_seg_cnncrf.h"
#include "muduo/base/Logging.h"
#include <mutex>

#include <boost/serialization/singleton.hpp>

static std::once_flag flag1;
using namespace std;
using namespace bell;

bool ResourceInit::init(const BellConfig &config) {
    bool ret = init_func(config);
    return ret;
}

bool ResourceInit::init_func(const bell::BellConfig &config) {
    LOG_INFO << "ResourceInit::init begin...";
    if (config.m_word_seg_model_config.empty()) {
        LOG_ERROR << "fail to init word seg cnn crf";
        return false;
    }
    std::call_once(flag1, initWordSeg, "word_seg_cnn_crf",
                   config.m_word_seg_model_config);
    LOG_INFO << "Resource::init_func end.";
    return true;
}

bool ResourceInit::initWordSeg(const std::string &model_name,
                               const std::string &cnn_crf_config) {
    auto model = std::make_shared<bell_model::ModelSegCNNCRF>(model_name,
                                                              cnn_crf_config);
    return boost::serialization::singleton<
               bell_model::ModelMgr>::get_const_instance()
        .reg_model(model);
}
