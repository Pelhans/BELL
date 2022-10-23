#include <map>
#include "muduo/base/Logging.h"
#include "eigen_utils.h"

using namespace Eigen;

bool EigenOp::genWord2id(std::string& path, std::map<std::string, int>& word2id) {
    FILE* fp;
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

MatrixXf EigenOp::loadEmbeddingParam(std::string& path, int vocab_size, int embedding_dim) {
    MatrixXf embedding(vocab_size, embedding_dim);
    bool is_get_data = ReadData(path, vocab_size, embedding_dim, embedding);
    return embedding;
}

conv1DLayer EigenOp::loadConv1dParam(std::string& path, int in_channels, int out_channels, int kernel_size) {
    conv1DLayer conv;  // weight: (kernel_size * in_channels, out_channels)
    MatrixXf cweight(out_channels, in_channels * kernel_size);
    VectorXf cbias(out_channels);
    bool is_get_weight = ReadData(path + ".weight", out_channels, in_channels*kernel_size, cweight);
    conv.weight = cweight.transpose();
    bool is_get_bias = ReadDataForVector(path + ".bias", out_channels, cbias);
    conv.bias = cbias;
    conv.kernel_size = kernel_size;
    if (!is_get_weight || !is_get_bias) {
        LOG_ERROR << "load word seg cnn_crf conv1d param failed";
    }
    return conv;
}

fnnConnectLayer EigenOp::loadFullConnectParam(std::string& path, int in_channels, int out_channels) {
    fnnConnectLayer fc;
    // shape: (in_channels, out_channels)
    MatrixXf fweight(out_channels, in_channels);
    VectorXf fbias(out_channels);
    bool is_get_weight = ReadData(path + ".weight", out_channels, in_channels, fweight);
    fc.weight = fweight.transpose();
    bool is_get_bias = ReadDataForVector(path + ".bias", out_channels, fbias);
    fc.bias = fbias;
    if (!is_get_weight || !is_get_bias) {
        LOG_ERROR << "load word seg cnn_crf fnn param failed";
    }
    return fc;
}

Eigen::MatrixXf EigenOp::crfTransistionParam(std::string& path, int tag_size) {
    MatrixXf crf_transistion_layer;
    bool is_get_weight = ReadData(path, tag_size, tag_size, crf_transistion_layer);
    if (!is_get_weight) {
        LOG_ERROR << "load word seg cnn_crf crf param failed";
    }
    return crf_transistion_layer;
}

bool EigenOp::ReadData(std::string path, int a, int b, MatrixXf& matrix) {
    // load matrix from .txt file
    std::ifstream data(path.c_str(), std::ios::in);
    if (!data.is_open()) {
        return false;
    }

    VectorXf col(b);
    for (int j=0; j<a; ++j) {
        for (int i=0; i<b; ++i) {
            data >> col(i);
        }
        matrix.row(j) = col;
    }
    return true;
}

bool EigenOp::ReadDataForVector(std::string path, int a, VectorXf& vectorxf) {
    // load matrix from .txt file
    std::ifstream data(path.c_str(), std::ios::in);
    if (!data.is_open()) {
        return false;
    }

    for (int j=0; j<a; ++j) {
        data >> vectorxf(j);
    }
    return true;
}

MatrixXf EigenOp::relu(MatrixXf& output) {
    for (int i=0; i<output.rows(); ++i) {
        for (int j=0; j<output.cols(); ++j) {
            if (output(i, j) < 0) {
                output(i, j) = 0;
            }
        }
    }
}