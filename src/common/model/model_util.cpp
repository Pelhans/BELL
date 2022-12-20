#include "model_util.h"
#include "model_manager.h"
#include "model_seg_cnncrf.h"
#include "muduo/base/Logging.h"

using namespace bell_model;

bool ModelUtil::cnncrf_predict(const string& model_name, vector<string>& query, vector<ResultTag>& res) {
    ModelMgr* mgr = singleton<ModelMgr>::getInstance();
    string name = Model::gen_name(zoo::TYPE_CNNCRF, model_name);
    shared_ptr<ModelSegCNNCRF> model = std::dynamic_pointer_cast<ModelSegCNNCRF>(mgr->get_model(name));
    if (!model) {
        LOG_ERROR << "ModelUtil::cnncrf_predict model not found, model_name: " << name.c_str();
        return false;
    }

    shared_ptr<ModelSegCNNCRFInput> in = std::make_shared<ModelSegCNNCRFInput>(query);
    shared_ptr<ModelSegCNNCRFOutput> out = std::make_shared<ModelSegCNNCRFOutput>();
    if (model->predict(in, out)) {
        res = out->res;
        return true;
    }
    return false;
}