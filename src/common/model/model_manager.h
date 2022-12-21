#pragma once

#include "model_base.h"
#include <atomic>
#include <string>
#include <memory>
#include <unordered_map>
#include "muduo/base/Logging.h"

using std::string;
using std::unordered_map;

namespace bell_model {

class ModelMgr {
public:
    ModelMgr() : reg_done(false) {};

    shared_ptr<Model> get_model(const string& name) {
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
        if (reg_done || model_map.find(name) != model_map.end()) {
            return false;
        }
        model_map[name] = model;
        return true;
    }

    void load_all() {
        for (auto it : model_map) {
            if (!it.second->load()) {
                LOG_ERROR << "ModelMgr load model failed, model name = " << it.second->name();
            } else {
                it.second->set_init(true);
            }
        }
    }

    void done_reg() { reg_done = true; }

private:
    unordered_map<string, shared_ptr<Model>> model_map;
    bool reg_done = false;
};

}