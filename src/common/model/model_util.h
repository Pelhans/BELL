#pragma once

#include <memory>
#include <vector>

using std::string;
using std::vector;
using std::shared_ptr;

namespace bell_model {

class ModelUtil {
public:
    static bool cnncrf_predict(const string& model_name, vector<string>& query, vector<ResultTag>& res);

};

};