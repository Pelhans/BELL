#include "seg_handler.h"
#include "common/model/model_util.h"
#include "common/util/str_util.h"
#include "muduo/base/Logging.h"
#include <iostream>
#include <string>

int WordSeg::work(std::string &req) {
  req += "##after seg ";
  vector<ResultTag> tag_res;
  std::vector<std::string> input_str_vec;
  spaceString::string2vector(req, input_str_vec);
  LOG_INFO << "开始跑模型了";
  bell_model::ModelUtil::cnncrf_predict("word_seg_cnn_crf", input_str_vec,
  tag_res);
  return 0;
}