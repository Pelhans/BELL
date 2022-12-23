#pragma once

#include "eigen/Dense"
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <math.h>
#include <map>

using namespace Eigen;
    
struct conv1DLayer {
    Eigen::MatrixXf weight;
    Eigen::VectorXf bias;
    int kernel_size;
};

struct fnnConnectLayer {
    Eigen::MatrixXf weight;
    Eigen::VectorXf bias;
};

struct ResultTag {
    double prob;
    std::vector<std::string> tags;
};

class EigenOp{
public:
    EigenOp() {};
    ~EigenOp() {};

    // NN module
    static void relu(MatrixXf& output);
    static void ctanh(MatrixXf& output);
    static bool softmax(Eigen::VectorXf& input_vector);
    static bool softmax(MatrixXf& matrix, MatrixXf& out, int dim);

    static MatrixXf Linear(MatrixXf& input, fnnConnectLayer& fc, std::string active);
    static MatrixXf Conv1d(MatrixXf& input, conv1DLayer& conv, std::string active);
    static MatrixXf multi_kernel_conv1d(MatrixXf& emb_input, conv1DLayer& layer1, conv1DLayer& layer2, conv1DLayer& layer3, const std::string active);

    // decode
    static std::vector<ResultTag> viterbi_decode_nbest(Eigen::MatrixXf& input, int seq_len, int tag_size,
                                                Eigen::MatrixXf& crf_transistion, int nbest, const std::map<int, std::string>& label2tag);
    static std::vector<ResultTag> viterbi_decode(Eigen::MatrixXf& input, int seq_len, int tag_size,
                                                Eigen::MatrixXf& crf_transistion, const std::map<int, std::string>& label2tag);
    static  std::vector<ResultTag> greed_decode(Eigen::MatrixXf& input, int seq_len, int tag_size, const std::map<int, std::string>& label2tag);
    static void get_top2_values(Eigen::MatrixXf& input, Eigen::MatrixXf& partition, Eigen::MatrixXi& indexs);
    static std::vector<ResultTag> mapping_id(Eigen::MatrixXi& decode_id, Eigen::VectorXf& scores, const std::map<int, std::string>& label2tag);
    static std::vector<ResultTag> mapping_id(Eigen::VectorXi& decode_id, const std::map<int, std::string>& label2tag);
    static void get_max_and_index(Eigen::MatrixXf& input, Eigen::VectorXf& partition, Eigen::VectorXi& indexs);
    static void get_max_index(Eigen::MatrixXf& input, Eigen::VectorXi& indexs, int axis=0);

    // matrix op
    static MatrixXf im2col(MatrixXf& input, int kernel_size);
    static void padding(std::vector<std::string>& input, int max_len, std::string pad_str);
    static MatrixXf Embedding(std::vector<std::string>& input, std::map<std::string, int>& word2id, Eigen::MatrixXf& embedding, int emb_dim);

    // basic func
    static bool ReadData(std::string path, int a, int b, MatrixXf& matrix);
    static bool ReadDataForVector(std::string path, int a, VectorXf& vectorxf);

    // load
    static bool genWord2id(std::string& path, std::map<std::string, int>& word2id);
    static MatrixXf loadEmbeddingParam(std::string& path, int vocab_size, int embedding_dim);
    static conv1DLayer loadConv1dParam(std::string& path, int in_channels, int out_channels, int kernel_size);
    static fnnConnectLayer loadFullConnectParam(std::string& path, int in_channels, int out_channels);
    static Eigen::MatrixXf crfTransistionParam(std::string& path, int tag_size);
    
};
