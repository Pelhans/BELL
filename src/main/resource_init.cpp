#include "resource_init.h"
#include "common/model/model_manager.h"
#include "common/model/model_seg_cnncrf.h"
#include "muduo/base/Logging.h"
#include <mutex>

#include <boost/serialization/singleton.hpp>

static std::once_flag flag1;
using namespace std;
using namespace bell;

// std::shared_ptr<std::thread> ResourceInit::thd = nullptr;
//
// void ResourceInit::async_init_wait() {
//   if (thd) {
//     thd->join();
//     thd.reset();
//   }
//   LOG_INFO << "ResourceInit::async_init_wait end.";
// }
//
// void ResourceInit::async_init(const BellConfig &config) {
//     init_func(config);
//     LOG_INFO << "活者到这了？";
// }

bool ResourceInit::init(const BellConfig &config) {
  LOG_INFO << "开始初始化 resourceInit";
  bool ret = init_func(config);
  LOG_INFO << "初始化完成？？";
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
  LOG_INFO << "model_name: " << model_name.c_str();
  LOG_INFO << "cnn_crf_config: " << cnn_crf_config.c_str();
  auto model =
      std::make_shared<bell_model::ModelSegCNNCRF>(model_name, cnn_crf_config);
  LOG_INFO << "模型指针创建完成";
  return boost::serialization::singleton<
             bell_model::ModelMgr>::get_const_instance()
      .reg_model(model);
}
