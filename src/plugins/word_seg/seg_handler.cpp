#include <iostream>
#include <string>
#include "seg_handler.h"
#include "common/util/str_util.h"

int WordSeg::work(std::string& req) {
    req += "##after seg ";
    // CNN_CRF cnn_crf_model;
    // std::string config_path = "/home/pelhans/project/BELL/deploy/conf/word_seg_v1.xml";
    // cnn_crf_model.load(config_path);
    // std::vector<std::string> input_str_vec;
    // spaceString::string2vector(req, input_str_vec);
    // //cnn_crf_model.forward(input_str_vec);
    return 0;
}