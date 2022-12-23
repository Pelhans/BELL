#include "eigen_utils.h"
#include "muduo/base/Logging.h"
#include <map>

using namespace Eigen;

bool EigenOp::genWord2id(std::string &path,
                         std::map<std::string, int> &word2id) {
  FILE *fp;
  char str[100];
  int index = 0;
  fp = fopen(path.c_str(), "r");
  if (fp == NULL) {
    return false;
  }
  while (fscanf(fp, "%s %d", str, &index) != EOF) {
    word2id[str] = index;
  }
  fclose(fp);
  return true;
}

MatrixXf EigenOp::loadEmbeddingParam(std::string &path, int vocab_size,
                                     int embedding_dim) {
  MatrixXf embedding(vocab_size, embedding_dim);
  bool is_get_data = ReadData(path, vocab_size, embedding_dim, embedding);
  return embedding;
}

conv1DLayer EigenOp::loadConv1dParam(std::string &path, int in_channels,
                                     int out_channels, int kernel_size) {
  conv1DLayer conv; // weight: (kernel_size * in_channels, out_channels)
  MatrixXf cweight(out_channels, in_channels * kernel_size);
  VectorXf cbias(out_channels);
  bool is_get_weight = ReadData(path + ".weight", out_channels,
                                in_channels * kernel_size, cweight);
  conv.weight = cweight.transpose();
  bool is_get_bias = ReadDataForVector(path + ".bias", out_channels, cbias);
  conv.bias = cbias;
  conv.kernel_size = kernel_size;
  if (!is_get_weight || !is_get_bias) {
    LOG_ERROR << "load word seg cnn_crf conv1d param failed";
  }
  return conv;
}

fnnConnectLayer EigenOp::loadFullConnectParam(std::string &path,
                                              int in_channels,
                                              int out_channels) {
  fnnConnectLayer fc;
  // shape: (in_channels, out_channels)
  MatrixXf fweight(out_channels, in_channels);
  VectorXf fbias(out_channels);
  bool is_get_weight =
      ReadData(path + ".weight", out_channels, in_channels, fweight);
  fc.weight = fweight.transpose();
  bool is_get_bias = ReadDataForVector(path + ".bias", out_channels, fbias);
  fc.bias = fbias;
  if (!is_get_weight || !is_get_bias) {
    LOG_ERROR << "load word seg cnn_crf fnn param failed";
  }
  return fc;
}

Eigen::MatrixXf EigenOp::crfTransistionParam(std::string &path, int tag_size) {
  MatrixXf crf_transistion_layer(tag_size, tag_size);
  bool is_get_weight =
      ReadData(path, tag_size, tag_size, crf_transistion_layer);
  if (!is_get_weight) {
    LOG_ERROR << "load word seg cnn_crf crf param failed";
  }
  return crf_transistion_layer;
}

bool EigenOp::ReadData(std::string path, int a, int b, MatrixXf &matrix) {
  // load matrix from .txt file
  std::ifstream data(path.c_str(), std::ios::in);
  if (!data.is_open()) {
    return false;
  }

  VectorXf col(b);
  for (int j = 0; j < a; ++j) {
    for (int i = 0; i < b; ++i) {
      data >> col(i);
    }
    matrix.row(j) = col;
  }
  return true;
}

bool EigenOp::ReadDataForVector(std::string path, int a, VectorXf &vectorxf) {
  // load matrix from .txt file
  std::ifstream data(path.c_str(), std::ios::in);
  if (!data.is_open()) {
    return false;
  }

  for (int j = 0; j < a; ++j) {
    data >> vectorxf(j);
  }
  return true;
}

MatrixXf EigenOp::im2col(MatrixXf &input, int kernel_size) {
  int length = input.rows();
  int dim = input.cols();
  MatrixXf output = MatrixXf::Zero(length, kernel_size * dim);

  for (int i = 0; i < length; ++i) {
    int start = i - kernel_size / 2;
    for (int j = 0; j < kernel_size; ++j, ++start) {
      if (start >= 0 && start < length) {
        for (int d = 0; d < dim; ++d) {
          output(i, j * dim + d) = input(start, d);
        }
      }
    }
  }
  return output;
}

void EigenOp::padding(std::vector<std::string> &input, int max_len,
                      std::string pad_str) {
  int size_input = input.size();
  if (size_input < max_len) {
    for (size_t i = 0; i < max_len - size_input; ++i) {
      input.push_back(pad_str);
    }
    return;
  } else if (size_input > max_len) {
    while (input.size() > max_len) {
      input.pop_back();
    }
    return;
  }
}

MatrixXf EigenOp::Embedding(std::vector<std::string> &input,
                            std::map<std::string, int> &word2id,
                            Eigen::MatrixXf &embedding, int emb_dim) {
  int row = input.size();
  MatrixXf emb_input(row, emb_dim);
  int id = 1;

  for (size_t i = 0; i < input.size(); ++i) {
    if (word2id.find(input[i]) == word2id.end()) {
      id = 1;
    } else {
      id = word2id[input[i]];
    }
    emb_input.row(i) = embedding.row(id);
  }
  return emb_input;
}

MatrixXf EigenOp::Linear(MatrixXf &input, fnnConnectLayer &fc,
                         std::string active) {
  MatrixXf output = input * fc.weight;
  output.rowwise() += fc.bias.transpose();
  if (active == "relu") {
    output = relu(output);
  } else if (active == "tanh") {
    output = ctanh(output);
  }
  return output;
}

MatrixXf EigenOp::Conv1d(MatrixXf &input, conv1DLayer &conv,
                         std::string active) {
  MatrixXf input_2_col = EigenOp::im2col(input, conv.kernel_size);
  LOG_INFO << "im2col done., rows()" << input_2_col.rows() << " cols: " << input_2_col.cols();
  LOG_INFO << "conv.weight., rows()" << conv.weight.rows() << " cols: " << conv.weight.cols();
  MatrixXf output = input_2_col * conv.weight;
  LOG_INFO << "output., rows()" << output.rows() << " cols: " << output.cols();
  LOG_INFO << "conv.bias., rows()" << conv.bias.rows() << " cols: " << conv.bias.cols() ;
  output.rowwise() += conv.bias.transpose();

  if (active == "relu") {
    output = relu(output);
  } else if (active == "tanh") {
    output = ctanh(output);
  } else {
    LOG_ERROR << "word seg cnn_crf model conv1d active func not exist: "
              << active.c_str();
  }
  return output;
}

MatrixXf EigenOp::multi_kernel_conv1d(MatrixXf &emb_input, conv1DLayer &layer1,
                                      conv1DLayer &layer2, conv1DLayer &layer3,
                                      const std::string active) {
  LOG_INFO << "multi_kernel_conv1d layer1 row "
           << std::to_string(layer1.weight.rows()).c_str()
           << " , cols: " << std::to_string(layer1.weight.cols()).c_str();
  LOG_INFO << "multi_kernel_conv1d layer2 row "
           << std::to_string(layer2.weight.rows()).c_str()
           << " , cols: " << std::to_string(layer2.weight.cols()).c_str();
  LOG_INFO << "multi_kernel_conv1d layer3 row "
           << std::to_string(layer3.weight.rows()).c_str()
           << " , cols: " << std::to_string(layer3.weight.cols()).c_str();
  MatrixXf conv11_input = Conv1d(emb_input, layer1, "relu");
  LOG_INFO << "蛤蛤";
  MatrixXf conv13_input = Conv1d(emb_input, layer2, "relu");
  MatrixXf conv15_input = Conv1d(emb_input, layer3, "relu");

  MatrixXf out_input(conv11_input.rows(), conv11_input.cols() +
                                              conv13_input.cols() +
                                              conv15_input.cols());
  out_input << conv11_input, conv13_input, conv15_input;
  return out_input;
}

MatrixXf EigenOp::ctanh(MatrixXf &output) {
  for (int i = 0; i < output.rows(); ++i) {
    for (int j = 0; j < output.cols(); ++j) {
      output(i, j) = std::tanh(output(i, j));
    }
  }
  return output;
}

MatrixXf EigenOp::relu(MatrixXf &output) {
  for (int i = 0; i < output.rows(); ++i) {
    for (int j = 0; j < output.cols(); ++j) {
      if (output(i, j) < 0) {
        output(i, j) = 0;
      }
    }
  }
}

bool EigenOp::softmax(MatrixXf &matrix, MatrixXf &out, int dim) {
  // 输入 (rows, cols), 返回每个 row 内的 softmax 操作
  out = matrix.array().exp();
  int rows = out.rows();
  int cols = out.cols();
  if (dim == 1) {
    for (int r = 0; r < rows; ++r) {
      out.row(r) = out.row(r) / (out.row(r).sum() + 1e-25);
    }
  } else if (dim == 0) {
    for (int c = 0; c < cols; ++c) {
      out.col(c) = out.col(c) / (out.col(c).sum() + 1e-25);
    }
  }
  out = out.array().isNaN().select(1, out);
  return true;
}

bool EigenOp::softmax(Eigen::VectorXf &input_vector) {
  float sum = 0;
  for (int i = 0; i < input_vector.rows(); ++i) {
    float exp_value = exp(input_vector(i));
    sum += exp_value;
    input_vector(i) = exp_value;
  }
  input_vector /= sum;

  return true;
}

// decode
std::vector<ResultTag>
EigenOp::viterbi_decode_nbest(Eigen::MatrixXf &input, int seq_len, int tag_size,
                              Eigen::MatrixXf &crf_transistion, int nbest,
                              const std::map<int, std::string> &label2tag) {
  std::vector<Eigen::MatrixXf> scores(seq_len);
  Eigen::MatrixXf x = Eigen::MatrixXf::Zero(tag_size, tag_size);
  for (int i = 0; i < seq_len; ++i) {
    x.rowwise() = input.row(i);
    scores[i] = x + crf_transistion;
  }

  std::vector<Eigen::MatrixXf> partition_history;
  std::vector<Eigen::MatrixXi> back_points;
  Eigen::VectorXf partition_v = scores[0].row(tag_size - 2);
  Eigen::MatrixXf partition_m = scores[0].row(tag_size - 2);
  partition_history.push_back(partition_m);

  Eigen::MatrixXf partition_v2 = Eigen::MatrixXf::Zero(2, tag_size);
  Eigen::MatrixXi indexs = Eigen::MatrixXi::Zero(2, tag_size);

  for (int i = 1; i < seq_len; ++i) { // tag * tag
    if (i == 1) {
      Eigen::MatrixXf cur_values = scores[i].colwise() + partition_v;
      get_top2_values(cur_values, partition_v2, indexs);
    } else {
      Eigen::MatrixXf cur_values =
          Eigen::MatrixXf::Zero(nbest * tag_size, tag_size);
      partition_v = partition_v2.row(0);
      Eigen::MatrixXf m_score_v1 = scores[i].colwise() + partition_v;
      partition_v = partition_v2.row(1);
      Eigen::MatrixXf m_score_v2 = scores[i].colwise() + partition_v;

      int index_1 = 0, index_2 = 0;
      for (int r = 0; r < nbest + tag_size; ++r) {
        if (r & 1) {
          cur_values.row(r) = m_score_v2.row(index_2++);
        } else {
          cur_values.row(r) = m_score_v1.row(index_1++);
        }
      }
      get_top2_values(cur_values, partition_v2, indexs);
    }
    if (i == 1) {
      indexs = indexs * 2;
    }
    partition_history.push_back(partition_v2);
    back_points.push_back(indexs);
  }

  Eigen::MatrixXf last_partition = partition_history.back();
  Eigen::MatrixXf last_values =
      Eigen::MatrixXf::Zero(nbest * tag_size, tag_size);
  partition_v = last_partition.row(0);
  Eigen::MatrixXf m_score_v1 = crf_transistion.colwise() + partition_v;
  partition_v = last_partition.row(1);
  Eigen::MatrixXf m_score_v2 = crf_transistion.colwise() + partition_v;

  int index_1 = 0, index_2 = 0;
  for (int r = 0; r < nbest * tag_size; ++r) {
    if (r & 1) {
      last_values.row(r) = m_score_v2.row(index_2++);
    } else {
      last_values.row(r) = m_score_v1.row(index_1++);
    }
  }
  get_top2_values(last_values, partition_v2, indexs);
  Eigen::VectorXf final_scores = partition_v2.col(tag_size - 1);
  Eigen::VectorXi pointer = indexs.col(tag_size - 1);
  Eigen::MatrixXi decode_id = Eigen::MatrixXi::Zero(seq_len, nbest);
  decode_id.row(seq_len - 1) = pointer / nbest;
  for (int i = seq_len - 2; i >= 0; i--) {
    for (int j = 0; j < nbest; j++) {
      pointer(j) = back_points[i](pointer(j) % nbest, pointer(j) / nbest);
    }
    decode_id.row(i) = pointer / nbest;
  }
  float max_score = final_scores.maxCoeff();
  for (int i = 0; i < final_scores.rows(); i++) {
    final_scores(i) -= max_score;
  }
  softmax(final_scores);
  decode_id.transposeInPlace();
  std::vector<ResultTag> result_tag =
      mapping_id(decode_id, final_scores, label2tag);

  return result_tag;
}

std::vector<ResultTag>
EigenOp::viterbi_decode(Eigen::MatrixXf &input, int seq_len, int tag_size,
                        Eigen::MatrixXf &crf_transistion,
                        const std::map<int, std::string> &label2tag) {
  std::vector<Eigen::MatrixXf> scores(seq_len);
  Eigen::MatrixXf x = Eigen::MatrixXf::Zero(tag_size, tag_size);
  for (int i = 0; i < seq_len; ++i) {
    x.rowwise() = input.row(i);
    scores[i] = x + crf_transistion;
  }

  std::vector<Eigen::VectorXf> partition_history;
  Eigen::MatrixXi back_points = Eigen::VectorXi::Zero(seq_len, tag_size);
  Eigen::VectorXi indexs = Eigen::VectorXi::Zero(tag_size);
  Eigen::VectorXf partition = scores[0].row(tag_size - 2);
  partition_history.push_back(partition);

  for (int i = 1; i < seq_len; ++i) {
    Eigen::MatrixXf cur_values = scores[i].colwise() + partition;
    get_max_and_index(cur_values, partition, indexs);
    partition_history.push_back(partition);
    back_points.row(i - 1) = indexs;
  }

  Eigen::VectorXf last_partition = partition_history.back();
  Eigen::MatrixXf last_value = crf_transistion.colwise() + last_partition;
  Eigen::VectorXi last_bp = Eigen::VectorXi::Zero(tag_size);
  get_max_index(last_value, last_bp);

  int pointer = last_bp(tag_size - 1);
  for (int i = 0; i < tag_size; ++i) {
    back_points(seq_len - 1, i) = pointer;
  }

  Eigen::VectorXi decode_id = Eigen::VectorXi::Zero(seq_len);
  decode_id(seq_len - 1) = pointer;
  for (int i = seq_len - 2; i >= 0; i--) {
    decode_id(i) = back_points(i, pointer);
  }
  std::vector<ResultTag> result_tag = mapping_id(decode_id, label2tag);

  return result_tag;
}

std::vector<ResultTag>
EigenOp::greed_decode(Eigen::MatrixXf &input, int seq_len, int tag_size,
                      const std::map<int, std::string> &label2tag) {
  Eigen::VectorXi decode_id = Eigen::VectorXi::Zero(seq_len);
  get_max_index(input, decode_id, 1);
  std::vector<ResultTag> result_tags = mapping_id(decode_id, label2tag);
  return result_tags;
}

void EigenOp::get_max_and_index(Eigen::MatrixXf &input,
                                Eigen::VectorXf &partition,
                                Eigen::VectorXi &indexs) {
  int rows = input.rows(), cols = input.cols();
  for (int col = 0; col < cols; ++col) {
    float max_elem = INT_MIN;
    int index = 0;
    for (int row = 0; row < rows; row++) {
      if (max_elem <= input(row, col)) {
        max_elem = input(row, col);
        index = row;
      }
    }
    partition(col) = max_elem;
    indexs[col] = index;
  }
}

void EigenOp::get_max_index(Eigen::MatrixXf &input, Eigen::VectorXi &indexs,
                            int axis = 0) {
  // axis = 0，默认沿行，找每列的最大值，axis = 1 ，每行的最大值
  int rows = input.rows(), cols = input.cols();
  if (axis == 0) {
    for (int col = 0; col < cols; ++col) {
      float max_elem = INT_MIN;
      int index = 0;
      for (int row = 0; row < rows; ++row) {
        if (max_elem <= input(row, col)) {
          max_elem = input(row, col);
          index = row;
        }
      }
      indexs[col] = index;
    }
  } else {
    for (int row = 0; row < rows; ++row) {
      float max_elem = INT_MIN;
      int index = 0;
      for (int col = 0; col < cols; ++col) {
        if (max_elem <= input(row, col)) {
          max_elem = input(row, col);
          index = col;
        }
      }
      indexs[row] = index;
    }
  }
}

std::vector<ResultTag>
EigenOp::mapping_id(Eigen::VectorXi &decode_id,
                    const std::map<int, std::string> &label2tag) {
  int size = decode_id.size();
  std::vector<ResultTag> result_tags;
  ResultTag result_tag;
  std::vector<std::string> tags(size);
  for (int i = 0; i < size; ++i) {
    if (label2tag.find(decode_id(i)) != label2tag.end()) {
      tags[i] = label2tag.at(decode_id(i));
    } else {
      return std::vector<ResultTag>();
    }
  }
  result_tag.prob = -1;
  result_tag.tags = tags;
  result_tags.push_back(result_tag);
  return result_tags;
}

std::vector<ResultTag>
EigenOp::mapping_id(Eigen::MatrixXi &decode_id, Eigen::VectorXf &scores,
                    const std::map<int, std::string> &label2tag) {
  std::vector<ResultTag> result_tags;
  int rows = decode_id.rows(), cols = decode_id.cols();

  if (scores.size() != decode_id.rows()) {
    return result_tags;
  }

  for (int r = 0; r < rows; ++r) {
    ResultTag result_tag;
    std::vector<std::string> tags(cols);
    for (int c = 0; c < cols; ++c) {
      int id = decode_id(r, c);
      if (label2tag.find(id) != label2tag.end()) {
        tags[c] = label2tag.at(id);
      } else {
        return std::vector<ResultTag>();
      }
    }
    result_tag.prob = scores[r];
    result_tag.tags = tags;
    result_tags.push_back(result_tag);
  }
  return result_tags;
}

void EigenOp::get_top2_values(Eigen::MatrixXf &input,
                              Eigen::MatrixXf &partition,
                              Eigen::MatrixXi &indexs) {
  int rows = input.rows(), cols = input.cols();
  for (int col = 0; col < cols; ++col) {
    float max_elem = INT_MIN, second_max_elem = INT_MIN;
    int max_index = 0, second_max_index = 0;
    for (int row = 0; row < rows; ++row) {
      if (max_elem <= input(row, col)) {
        second_max_elem = max_elem;
        max_elem = input(row, col);
        second_max_index = max_index;
        max_index = row;
      } else {
        if (second_max_elem <= input(row, col)) {
          second_max_elem = input(row, col);
          second_max_index = row;
        }
      }
    }
    partition(0, col) = max_elem;
    partition(1, col) = second_max_elem;
    indexs(0, col) = max_index;
    indexs(1, col) = second_max_index;
  }
}