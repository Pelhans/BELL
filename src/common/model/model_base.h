#pragma once

#include <memory>
#include <vector>
#include <string>
#include<sys/time.h>
#include "muduo/base/Logging.h"

using std::shared_ptr;
using std::vector;
using std::string;

namespace bell_model {

namespace zoo {
    const static string TYPE_CNNCRF = "CNNCRF";
};

class Input {
public:
    virtual ~Input() = default;
};

class Output {
public:
    virtual ~Output() = default;
    int32_t ucost() { return this->cost_; }

public:
    int32_t cost_ = 0; // us
};

class FVInput : public Input {
public:
    FVInput() = default;
    FVInput(const vector<double>& feature) : data(feature) {}

public:
    vector<double> data;
};

class SVInput : public Input {
public:
    SVInput() = default;
    SVInput(const vector<string>& feature) : data(feature) {}

public:
    vector<string> data;
};

class FOutput : public Input {
public:
    FOutput() = default;

public:
    double res;
};

class FVOutput : public Input {
public:
    FVOutput() = default;

public:
    vector<double> res;
};

class Timer {
public:
    void start() { gettimeofday(&start_, NULL); }
    void end() {
        gettimeofday(&end_, NULL);
        cost_ = (int32_t) ((end_.tv_sec - start_.tv_sec) * 1000000 + end_.tv_usec - start_.tv_usec);
    }
    int32_t ucost() { return cost_; };

private:
    int32_t cost_;
    struct timeval start_;
    struct timeval end_;
};

class Model {
public:
    Model(const string& name, const string& model_type, const string& model_file) {
        this->name_ = gen_name(model_type, name);
        this->model_file_ = model_file;
    }

    virtual ~Model() = default;

    virtual string type() = 0;

    virtual bool load() = 0;

    static string gen_name(const string& model_type, const string& name) { return model_type + "::" + name; }

    void set_init(bool init) { this->is_init_ = init; }

    string name() { return name_; }

    bool predict(shared_ptr<Input> input, shared_ptr<Output> output) {
        if (!is_init_) {
            LOG_ERROR << "Local Model predict() model not init, model name " << name_.c_str();
            return false;
        }

        Timer timer;
        timer.start();

        bool ret = do_predict(input, output);
        if (!ret) {
            LOG_WARN << "Local Model predict failed, model name " << name_.c_str();
        }

        timer.end();
        output->cost_ = timer.ucost();
        return true;
    }

private:
    virtual bool do_predict(shared_ptr<Input>& input, shared_ptr<Output>& output) = 0;

protected:
    string name_;
    string model_file_;
    bool is_init_ = false;
};

}
