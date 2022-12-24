#pragma once

#include "model_base.h"
#include "word_seg_model/cnn_crf.h"

namespace bell_model {

class ModelSegCNNCRFInput : public Input {
public:
    ModelSegCNNCRFInput(vector<string>& data) : data(data) {}

public:
    vector<string> data;
};

class ModelSegCNNCRFOutput : public Output {
public:
    ModelSegCNNCRFOutput() = default;

public:
    vector<ResultTag> res;
};

class ModelSegCNNCRF : public Model {
public:
    ModelSegCNNCRF(const string& name, const string& model_file)
        : Model(name, type(), model_file) {
        model = std::make_shared<CNNCRF>();
    }

    virtual string type() { return zoo::TYPE_CNNCRF; }

    virtual bool load() {
        if (model_file_.empty()) {
            return false;
        }
        return model->load(model_file_);
    }

private:
    virtual bool do_predict(shared_ptr<Input>& input,
                            shared_ptr<Output>& output) {
        shared_ptr<ModelSegCNNCRFInput> in =
            std::dynamic_pointer_cast<ModelSegCNNCRFInput>(input);
        shared_ptr<ModelSegCNNCRFOutput> out =
            std::dynamic_pointer_cast<ModelSegCNNCRFOutput>(output);
        if (in && out) {
            out->res = model->forward(in->data);
            return true;
        } else {
            return false;
        }
    }

private:
    shared_ptr<CNNCRF> model;
};

} // namespace bell_model