#pragma once

#include "model_base.h"
#include "muduo/base/Logging.h"
#include <atomic>
#include <memory>
#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;

namespace bell_model {

class ModelMgr {
public:
  ModelMgr() : reg_done(false){};

  shared_ptr<Model> get_model(const string &name) {
    if (reg_done && model_map.find(name) != model_map.end()) {
      return model_map[name];
    }
    return nullptr;
  }

  bool reg_model(shared_ptr<Model> model) {
    if (!model) {
      return false;
    }
    string name = model->name();
    LOG_INFO << "reg_model..., model name: " << name.c_str();
    if (reg_done || model_map.find(name) != model_map.end()) {
      return false;
    }
    model_map[name] = model;
    LOG_INFO << "reg_model done., model_map.size() = " << model_map.size();
    return true;
  }

  void load_all() {
    LOG_INFO << "load_all...";
    LOG_INFO << "model_map.size() : " << model_map.size();
    for (auto it : model_map) {
      LOG_INFO << "开始 Load 模型...";
      if (!it.second->load()) {
        LOG_ERROR << "ModelMgr load model failed, model name = "
                  << it.second->name().c_str();
      } else {
        LOG_INFO << "ModelMgr load model  cur_name = "
                 << it.second->name().c_str();
        it.second->set_init(true);
      }
    }
  }

  void done_reg() { reg_done = true; }

private:
  unordered_map<string, shared_ptr<Model>> model_map;
  bool reg_done = false;
};

} // namespace bell_model