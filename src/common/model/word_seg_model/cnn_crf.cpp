#include "cnn_crf.h"
#include "muduo/base/Logging.h"
#include "util/xml_helper.h"
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

using namespace bell;

std::vector<ResultTag> CNNCRF::forward(std::vector<std::string> &input) {
  std::vector<ResultTag> result_tags;
  int max_len = m_max_len;
  int seq_len = input.size() < max_len ? input.size() : max_len;
  if (seq_len <= 1) {
    return result_tags;
  }

  EigenOp::padding(input, max_len, "[PAD]");
  MatrixXf emb_input =
      EigenOp::Embedding(input, m_word2id, m_embedding, m_emb_dim);

  MatrixXf conv1d_layer_1_out = EigenOp::multi_kernel_conv1d(
      emb_input, m_conv_layer_1_1, m_conv_layer_1_3, m_conv_layer_1_5, "relu");
  MatrixXf conv1d_layer_2_out =
      EigenOp::multi_kernel_conv1d(conv1d_layer_1_out, m_conv_layer_2_3,
                                   m_conv_layer_2_5, m_conv_layer_2_7, "relu");
  MatrixXf conv1d_layer_3_out =
      EigenOp::multi_kernel_conv1d(conv1d_layer_2_out, m_conv_layer_3_1,
                                   m_conv_layer_3_3, m_conv_layer_3_5, "relu");

  MatrixXf linear_out_1 =
      EigenOp::Linear(conv1d_layer_3_out, m_fc_0_layer, "tanh");
  MatrixXf logits = EigenOp::Linear(linear_out_1, m_fc_1_layer, "");

  if (m_decode_type == "crf") {
    if (m_nbest == 2) {
      result_tags = EigenOp::viterbi_decode_nbest(logits, seq_len, m_tag_size,
                                                  m_crf_transistion_layer,
                                                  m_nbest, label2tag);
    } else {
      result_tags = EigenOp::viterbi_decode(logits, seq_len, m_tag_size,
                                            m_crf_transistion_layer, label2tag);
    }
  } else {
    result_tags = EigenOp::greed_decode(logits, seq_len, m_tag_size, label2tag);
  }

  return result_tags;
}

bool CNNCRF::load(std::string &cnn_config) {
  if (access(cnn_config.c_str(), R_OK) == -1) {
    LOG_ERROR << "config file doesn't exist: " << cnn_config.c_str();
    return false;
  }
  XmlDoc parser;
  XmlNode root = parser.parser(cnn_config.c_str());

  if (!root.isValid()) {
    LOG_INFO << "Faild to get root node, conf file:" << cnn_config.c_str();
    return false;
  }
  LOG_INFO << "开始加载模型文件 ";
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
    bell::GET_XML_CONF_INT(*iter, "vocab_size", 22200, &m_vocab_size);
    bell::GET_XML_CONF_INT(*iter, "emb_dim", 60, &m_emb_dim);
    bell::GET_XML_CONF_INT(*iter, "tag_size", 4, &m_tag_size);
    bell::GET_XML_CONF_INT(*iter, "max_len", 64, &m_max_len);

    // word2id
    bell::GET_XML_CONF_STRING(*iter, "word2id_path", "", &word2id_path);
    if (word2id_path.empty()) {
      LOG_INFO << "word seg cnn_crf model word2id_path empty";
      return false;
    }

    // 加载词汇表
    bell::GET_XML_CONF_STRING(*iter, "word_embedding_path", "",
                              &word_embedding_path);
    if (word_embedding_path.empty()) {
      LOG_INFO << "word seg cnn_crf model word_embedding_path empty";
      return false;
    }

    // 加载卷积层 layer 1
    bell::GET_XML_CONF_STRING(*iter, "conv_layer_11_path_prefix", "",
                              &convs_layer_1_1_path);
    if (convs_layer_1_1_path.empty()) {
      LOG_INFO << "word seg cnn_crf model conv_layer_11_path_prefix empty";
      return false;
    }
    bell::GET_XML_CONF_STRING(*iter, "conv_layer_13_path_prefix", "",
                              &convs_layer_1_3_path);
    if (convs_layer_1_3_path.empty()) {
      LOG_INFO << "word seg cnn_crf model conv_layer_13_path_prefix empty";
      return false;
    }
    bell::GET_XML_CONF_STRING(*iter, "conv_layer_15_path_prefix", "",
                              &convs_layer_1_5_path);
    if (convs_layer_1_5_path.empty()) {
      LOG_INFO << "word seg cnn_crf model conv_layer_15_path_prefix empty";
      return false;
    }

    // 加载卷积层 layer 2
    bell::GET_XML_CONF_STRING(*iter, "conv_layer_23_path_prefix", "",
                              &convs_layer_2_3_path);
    if (convs_layer_2_3_path.empty()) {
      LOG_INFO << "word seg cnn_crf model conv_layer_23_path_prefix empty";
      return false;
    }
    bell::GET_XML_CONF_STRING(*iter, "conv_layer_25_path_prefix", "",
                              &convs_layer_2_5_path);
    if (convs_layer_2_5_path.empty()) {
      LOG_INFO << "word seg cnn_crf model conv_layer_25_path_prefix empty";
      return false;
    }
    bell::GET_XML_CONF_STRING(*iter, "conv_layer_27_path_prefix", "",
                              &convs_layer_2_7_path);
    if (convs_layer_2_7_path.empty()) {
      LOG_INFO << "word seg cnn_crf model conv_layer_27_path_prefix empty";
      return false;
    }

    // 加载卷积层 layer 3
    bell::GET_XML_CONF_STRING(*iter, "conv_layer_31_path_prefix", "",
                              &convs_layer_3_1_path);
    if (convs_layer_3_1_path.empty()) {
      LOG_INFO << "word seg cnn_crf model conv_layer_31_path_prefix empty";
      return false;
    }
    bell::GET_XML_CONF_STRING(*iter, "conv_layer_33_path_prefix", "",
                              &convs_layer_3_3_path);
    if (convs_layer_3_3_path.empty()) {
      LOG_INFO << "word seg cnn_crf model conv_layer_33_path_prefix empty";
      return false;
    }
    bell::GET_XML_CONF_STRING(*iter, "conv_layer_35_path_prefix", "",
                              &convs_layer_3_5_path);
    if (convs_layer_3_5_path.empty()) {
      LOG_INFO << "word seg cnn_crf model conv_layer_35_path_prefix empty";
      return false;
    }

    // 加载全连接层 1
    bell::GET_XML_CONF_STRING(*iter, "fc_layer_0_prefix", "",
                              &fc_layer_0_prefix);
    if (fc_layer_0_prefix.empty()) {
      LOG_INFO << "word seg cnn_crf model fc_layer_0_prefix empty";
      return false;
    }
    // 加载全连接层 2
    bell::GET_XML_CONF_STRING(*iter, "fc_layer_1_prefix", "",
                              &fc_layer_1_prefix);
    if (fc_layer_1_prefix.empty()) {
      LOG_INFO << "word seg cnn_crf model fc_layer_1_prefix empty";
      return false;
    }

    // 转移矩阵层
    bell::GET_XML_CONF_STRING(*iter, "crf_trans_prefix", "", &crf_path);
    if (crf_path.empty()) {
      LOG_INFO << "word seg cnn_crf model crf_trans_prefix empty";
      return false;
    }
  }

  // word2id
  bool is_get_word2id = EigenOp::genWord2id(word2id_path, m_word2id);
  LOG_INFO << "word2id_path: " << word2id_path.c_str();
  if (!is_get_word2id) {
    LOG_INFO << "word seg cnn_crf model load Word2id failed!!";
  }

  // embedding
  m_embedding =
      EigenOp::loadEmbeddingParam(word_embedding_path, m_vocab_size, m_emb_dim);
  LOG_INFO << "word seg cnn_crf m_embedding row "
           << std::to_string(m_embedding.rows()).c_str()
           << " , cols: " << std::to_string(m_embedding.cols()).c_str();

  // CNN
  LOG_INFO << "开始加载 CNN 部分";
  m_conv_layer_1_1 =
      EigenOp::loadConv1dParam(convs_layer_1_1_path, m_emb_dim, 20, 1);
  LOG_INFO << "加载....";
  m_conv_layer_1_3 =
      EigenOp::loadConv1dParam(convs_layer_1_3_path, m_emb_dim, 20, 3);
  LOG_INFO << "加载....";
  m_conv_layer_1_5 =
      EigenOp::loadConv1dParam(convs_layer_1_5_path, m_emb_dim, 20, 5);
  
  LOG_INFO << "加载....";
  m_conv_layer_2_3 =
      EigenOp::loadConv1dParam(convs_layer_2_3_path, m_emb_dim, 20, 3);
  LOG_INFO << "加载....";
  m_conv_layer_2_5 =
      EigenOp::loadConv1dParam(convs_layer_2_5_path, m_emb_dim, 20, 5);
  LOG_INFO << "加载....";
  m_conv_layer_2_7 =
      EigenOp::loadConv1dParam(convs_layer_2_7_path, m_emb_dim, 20, 7);
  LOG_INFO << "加载....";

  m_conv_layer_3_1 =
      EigenOp::loadConv1dParam(convs_layer_3_1_path, m_emb_dim, 20, 1);
  LOG_INFO << "加载....";
  m_conv_layer_3_3 =
      EigenOp::loadConv1dParam(convs_layer_3_3_path, m_emb_dim, 20, 3);
  LOG_INFO << "加载....";
  m_conv_layer_3_5 =
      EigenOp::loadConv1dParam(convs_layer_3_5_path, m_emb_dim, 20, 5);
  LOG_INFO << "加载....";

  // fc
  m_fc_0_layer = EigenOp::loadFullConnectParam(fc_layer_0_prefix, 60, 60);
  LOG_INFO << "加载....";
  m_fc_1_layer = EigenOp::loadFullConnectParam(fc_layer_1_prefix, 60, 4);
  LOG_INFO << "加载....";

  // crf
  if (access(crf_path.c_str(), R_OK) == -1) {
    LOG_ERROR << "crf_path file doesn't exist: " << crf_path.c_str();
    return false;
  }
  LOG_INFO << "m_tag_size: " << m_tag_size;
  m_crf_transistion_layer = EigenOp::crfTransistionParam(crf_path, m_tag_size);
  LOG_INFO << "加载....";

  return true;
}