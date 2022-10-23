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

MatrixXf EigenOp::im2col(MatrixXf& input, int kernel_size) {
    int length = input.rows();
    int dim = input.cols();
    MatrixXf output = MatrixXf::Zero(length, kernel_size*dim);

    for (int i = 0; i<length; ++i) {
        int start = i - kernel_size/2;
        for (int j=0; j<kernel_size; ++j, ++start) {
            if (start >= 0 && start<length) {
                for (int d=0; d<dim; ++d) {
                    output(i, j*dim+d) = input(start, d);
                }
            }
        }
    }
    return output;
}

void EigenOp::padding(std::vector<std::string>& input, int max_len, std::string pad_str) {
    int size_input = input.size();
    if (size_input < max_len) {
        for (size_t i=0; i<max_len-size_input; ++i) {
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

MatrixXf EigenOp::Embedding(std::vector<std::string>& input, std::map<std::string, int>& word2id, Eigen::MatrixXf& embedding, int emb_dim) {
    int row = input.size();
    MatrixXf emb_input(row, emb_dim);
    int id = 1;

    for (size_t i = 0; i<input.size(); ++i) {
        if (word2id.find(input[i]) == word2id.end()) {
            id = 1;
        } else {
            id = word2id[input[i]];
        }
        emb_input.row(i) = embedding.row(id);
    }
    return emb_input;
}

MatrixXf EigenOp::Linear(MatrixXf& input, fnnConnectLayer& fc, std::string active) {
    MatrixXf output = input*fc.weight;
    output.rowwise() += fc.bias.transpose();
    if (active == "relu") {
        output = relu(output);
    } else if (active == "tanh") {
        output = ctanh(output);
    }
    return output;
}

MatrixXf EigenOp::Conv1d(MatrixXf& input, conv1DLayer& conv, std::string active) {
    MatrixXf input_2_col = EigenOp::im2col(input, conv.kernel_size);
    MatrixXf output = input_2_col * conv.weight;
    output.rowwise() += conv.bias.transpose();

    if (active == "relu") {
        output = relu(output);
    } else if (active == "tanh") {
        output = ctanh(output);
    } else {
        LOG_ERROR << "word seg cnn_crf model conv1d active func not exist: " << active.c_str();
    }
    return output;
}

MatrixXf EigenOp::multi_kernel_conv1d(MatrixXf& emb_input, conv1DLayer& layer1, conv1DLayer& layer2, conv1DLayer& layer3, const std::string active) {
    MatrixXf conv11_input = Conv1d(emb_input, layer1, "relu");
    MatrixXf conv13_input = Conv1d(emb_input, layer1, "relu");
    MatrixXf conv15_input = Conv1d(emb_input, layer1, "relu");

    MatrixXf out_input(conv11_input.rows(), conv11_input.cols() + conv13_input.cols() + conv15_input.cols());
    out_input << conv11_input, conv13_input, conv15_input;
    return out_input;
}

MatrixXf EigenOp::ctanh(MatrixXf& output) {
    for (int i=0; i<output.rows(); ++i) {
        for (int j=0; j<output.cols(); ++j) {
            output(i, j) = std::tanh(output(i, j));
        }
    }
    return output;
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