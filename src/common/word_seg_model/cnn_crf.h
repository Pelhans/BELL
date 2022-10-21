#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <map>

#include "eigen/Dense"

const std::map<int, std::string> label2tag = { {0, "S"},
{1, "B"},
{2, "I"},
{3, "E"}
};

class CNN_CRF {
public:
    CNN_CRF();
    ~CNN_CRF();

    bool load(std::string& cnn_config);


};