#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <map>

#include "eigen/Dense"
#include "eigen_utils.h"

const std::map<int, std::string> label2tag = { {0, "S"},
{1, "B"},
{2, "I"},
{3, "E"}
};

struct ResultTag {
    double prob;
    std::vector<std::string> tags;
};

class CNN_CRF {
public:
    CNN_CRF();
    ~CNN_CRF();

    bool load(std::string& cnn_config);

private:
    int m_vocab_size;
    int m_emb_dim;
    int m_tag_size;
    int m_max_len;

    Eigen::MatrixXf m_embedding;

    conv1DLayer m_conv_layer_1_1;
    conv1DLayer m_conv_layer_1_3;
    conv1DLayer m_conv_layer_1_5;

    conv1DLayer m_conv_layer_2_3;
    conv1DLayer m_conv_layer_2_5;
    conv1DLayer m_conv_layer_2_7;

    conv1DLayer m_conv_layer_3_1;
    conv1DLayer m_conv_layer_3_3;
    conv1DLayer m_conv_layer_3_5;

    fnnConnectLayer m_fc_0_layer;
    fnnConnectLayer m_fc_1_layer;
    Eigen::MatrixXf m_crf_transistion_layer;

    std::map<std::string, int> m_word2id;

};