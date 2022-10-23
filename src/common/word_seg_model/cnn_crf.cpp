#include <stdio.h>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include "util/xml_helper.h"
#include "muduo/base/Logging.h"
#include "cnn_crf.h"


using namespace bell;

MatrixXf CNN_CRF::Embedding(std::vector<std::string>& input) {
    int row = input.size();
    MatrixXf emb_input(row, m_emb_dim);
    int id = 1;

    for (size_t i = 0; i<input.size(); ++i) {
        if (m_word2id.find(input[i]) == m_word2id.end()) {
            id = 1;
        } else {
            id = m_word2id[input[i]];
        }
        emb_input.row(i) = m_embedding.row(id);
    }
    return emb_input;
}

Matrix CNN_CRF::Conv1d(MatrixXf& inbput, conv1DLayer& conv, std::string& active) {
    MatrixXf input_2_col = EigenOp::im2col(input, conv.kernel_size);
    MatrixXf output = input_2_col * conv.weight;
    output.rowwise() += conv.bias.transpose();

    if (active == "relu") {
        output = EigenOp::relu(output);
    } else if (active == "tanh") {
        output = EigenOp::ctanh(output);
    } else {
        LOG_ERROR << "word seg cnn_crf model conv1d active func not exist: " << active.c_str();
    }
    return output;
}

Matrix CNN_CRF::multi_kernel_conv1d(MatrixXf& emb_input, conv1DLayer& layer1, conv1DLayer& layer1, conv1DLayer& layer1, std::string& active) {
    MatrixXf conv11_input = Conv1d(emb_input, layer1, "relu");
    MatrixXf conv13_input = Conv1d(emb_input, layer1, "relu");
    MatrixXf conv15_input = Conv1d(emb_input, layer1, "relu");

    MatrixXf out_input(conv11_input.rows(), conv11_input.cols() + conv13_input.cols() + conv16_input.cols());
    out_input << conv11_input, conv13_input, conv15_input;
    return out_input;
}

std::vector<ResultTag> CNN_CRF::forward(std::vector<std::string>& input) {
    std::vector<ResultTag> result_tag;
    int seq_len = input.size();
    if (seq_len <= 1) {
        return result_tag;
    }

    MatrixXf emb_input = Embedding(input);
    
    Matrix conv1d_layer_1_out = multi_kernel_conv1d(emb_input, m_conv_layer_1_1, m_conv_layer_1_3, m_conv_layer_1_5, "relu")
}

bool CNN_CRF::load(std::string& cnn_config) {
    XmlDoc parser;
    XmlNode root = parser.parser(cnn_config.c_str());

    if (!root.isValid()) {
        LOG_INFO << "Faild to get root node, conf file:" << cnn_config.c_str();
        return false;
    }

    auto node_list = bell::GET_XML_CONF_LIST(root, "node");

    std::string model_dir, word2id_path, word_embedding_path;
    std::string convs_layer_1_1_path, convs_layer_1_3_path, convs_layer_1_5_path;
    std::string convs_layer_2_3_path, convs_layer_2_5_path, convs_layer_2_7_path;
    std::string convs_layer_3_1_path, convs_layer_3_3_path, convs_layer_3_5_path;
    std::string fc_layer_0_prefix, fc_layer_1_prefix;
    std::string crf_path;

    for (auto iter = node_list.begin(); iter != node_list.end(); ++iter) {
        // 加载文件路径
        bell::GET_XML_CONF_STRING(*iter, "model_dir", "", &model_dir);
        if (model_dir.empty()) {
            LOG_INFO << "word seg cnn_crf model model_dir empty";
            return false;
        }
        LOG_INFO << "模型目录为: " << model_dir.c_str();

        // 参数
        bell::GET_XML_CONF_INT(*iter, "vocab_size", 22110, &m_vocab_size);
        bell::GET_XML_CONF_INT(*iter, "emb_dim", 60, &m_emb_dim);
        bell::GET_XML_CONF_INT(*iter, "tag_size", 4, &m_tag_size);
        bell::GET_XML_CONF_INT(*iter, "max_len", 64, &m_max_len);

        // 加载词汇表
        bell::GET_XML_CONF_STRING(*iter, "word_embedding_path", "", &word_embedding_path);
        if (word_embedding_path.empty()) {
            LOG_INFO << "word seg cnn_crf model word_embedding_path empty";
            return false;
        }

        // 加载卷积层 layer 1
        bell::GET_XML_CONF_STRING(*iter, "conv_layer_11_path_prefix", "", &convs_layer_1_1_path);
        if (convs_layer_1_1_path.empty()) {
            LOG_INFO << "word seg cnn_crf model conv_layer_11_path_prefix empty";
            return false;
        }
        bell::GET_XML_CONF_STRING(*iter, "conv_layer_13_path_prefix", "", &convs_layer_1_3_path);
        if (convs_layer_1_3_path.empty()) {
            LOG_INFO << "word seg cnn_crf model conv_layer_13_path_prefix empty";
            return false;
        }
        bell::GET_XML_CONF_STRING(*iter, "conv_layer_15_path_prefix", "", &convs_layer_1_5_path);
        if (convs_layer_1_5_path.empty()) {
            LOG_INFO << "word seg cnn_crf model conv_layer_15_path_prefix empty";
            return false;
        }

        // 加载卷积层 layer 2
        bell::GET_XML_CONF_STRING(*iter, "conv_layer_23_path_prefix", "", &convs_layer_2_3_path);
        if (convs_layer_2_3_path.empty()) {
            LOG_INFO << "word seg cnn_crf model conv_layer_23_path_prefix empty";
            return false;
        }
        bell::GET_XML_CONF_STRING(*iter, "conv_layer_25_path_prefix", "", &convs_layer_2_5_path);
        if (convs_layer_2_5_path.empty()) {
            LOG_INFO << "word seg cnn_crf model conv_layer_25_path_prefix empty";
            return false;
        }
        bell::GET_XML_CONF_STRING(*iter, "conv_layer_27_path_prefix", "", &convs_layer_2_7_path);
        if (convs_layer_2_7_path.empty()) {
            LOG_INFO << "word seg cnn_crf model conv_layer_27_path_prefix empty";
            return false;
        }

        // 加载卷积层 layer 3
        bell::GET_XML_CONF_STRING(*iter, "conv_layer_31_path_prefix", "", &convs_layer_3_1_path);
        if (convs_layer_3_1_path.empty()) {
            LOG_INFO << "word seg cnn_crf model conv_layer_31_path_prefix empty";
            return false;
        }
        bell::GET_XML_CONF_STRING(*iter, "conv_layer_33_path_prefix", "", &convs_layer_3_3_path);
        if (convs_layer_3_3_path.empty()) {
            LOG_INFO << "word seg cnn_crf model conv_layer_33_path_prefix empty";
            return false;
        }
        bell::GET_XML_CONF_STRING(*iter, "conv_layer_35_path_prefix", "", &convs_layer_3_5_path);
        if (convs_layer_3_5_path.empty()) {
            LOG_INFO << "word seg cnn_crf model conv_layer_35_path_prefix empty";
            return false;
        }

        // 加载全连接层 1
        bell::GET_XML_CONF_STRING(*iter, "fc_layer_0_prefix", "", &fc_layer_0_prefix);
        if (fc_layer_0_prefix.empty()) {
            LOG_INFO << "word seg cnn_crf model fc_layer_0_prefix empty";
            return false;
        }
        // 加载全连接层 2
        bell::GET_XML_CONF_STRING(*iter, "fc_layer_1_prefix", "", &fc_layer_1_prefix);
        if (fc_layer_1_prefix.empty()) {
            LOG_INFO << "word seg cnn_crf model fc_layer_1_prefix empty";
            return false;
        }
    }

    // word2id
    bool is_get_word2id = EigenOp::genWord2id(word2id_path, m_word2id);
    if (!is_get_word2id) {
        LOG_INFO << "word seg cnn_crf model load Word2id failed!!";
    }

    // embedding
    m_embedding = EigenOp::loadEmbeddingParam(word_embedding_path, m_vocab_size, m_emb_dim);
    LOG_INFO << "word seg cnn_crf m_embedding row " << std::to_string(m_embedding.rows()).c_str() << " , cols: " <<  std::to_string(m_embedding.cols()).c_str();

    // CNN
    m_conv_layer_1_1 = EigenOp::loadConv1dParam(convs_layer_1_1_path, m_emb_dim, 20, 1);
    m_conv_layer_1_3 = EigenOp::loadConv1dParam(convs_layer_1_3_path, m_emb_dim, 20, 1);
    m_conv_layer_1_5 = EigenOp::loadConv1dParam(convs_layer_1_5_path, m_emb_dim, 20, 1);

    m_conv_layer_2_3 = EigenOp::loadConv1dParam(convs_layer_2_3_path, m_emb_dim, 20, 1);
    m_conv_layer_2_5 = EigenOp::loadConv1dParam(convs_layer_2_5_path, m_emb_dim, 20, 1);
    m_conv_layer_2_7 = EigenOp::loadConv1dParam(convs_layer_2_7_path, m_emb_dim, 20, 1);
    
    m_conv_layer_3_1 = EigenOp::loadConv1dParam(convs_layer_3_1_path, m_emb_dim, 20, 1);
    m_conv_layer_3_3 = EigenOp::loadConv1dParam(convs_layer_3_3_path, m_emb_dim, 20, 1);
    m_conv_layer_3_5 = EigenOp::loadConv1dParam(convs_layer_3_5_path, m_emb_dim, 20, 1);

    // fc
    m_fc_0_layer = EigenOp::loadFullConnectParam(fc_layer_0_prefix, 60, 60);
    m_fc_1_layer = EigenOp::loadFullConnectParam(fc_layer_1_prefix, 60, 60);

    // crf
    m_crf_transistion_layer = EigenOp::crfTransistionParam(crf_path, m_tag_size);

    return true;
}