#pragma once

#include <memory>
#include <vector>
#include "word_seg_model/eigen_utils.h"

using std::string;
using std::vector;
using std::shared_ptr;

namespace bell_model {

class ModelUtil {
public:
    static bool cnncrf_predict(const string& model_name, vector<string>& query, vector<ResultTag>& res);

};

}