#pragma once

#include "eigen/Dense"
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <math.h>

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

class EigenOp{
public:
    EigenOp() {};
    ~EigenOp() {};

    // NN module
    static MatrixXf relu(MatrixXf& output);

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
