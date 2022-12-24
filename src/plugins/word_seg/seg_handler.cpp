#include "seg_handler.h"
#include "common/model/model_util.h"
#include "common/util/str_util.h"
#include "muduo/base/Logging.h"
#include <iostream>
#include <string>

int WordSeg::work(std::string &req) {
  if (req.size() <= 1) {
    return 0;
  }
  //req = "今天的烤鱼还是很好吃的。";
  LOG_INFO << "输入的文本为: " << req.c_str();
  vector<ResultTag> tag_res;
  std::vector<std::string> input_str_vec;
  spaceString::string2vector(req, input_str_vec);
  bell_model::ModelUtil::cnncrf_predict("word_seg_cnn_crf", input_str_vec,
  tag_res);
  for (int i=0; i<tag_res.size(); ++i) {
    LOG_INFO << "tag_res[i]: " << "对应的tag大小: " << tag_res[i].tags.size();
    for (int j=0; j<input_str_vec.size(); ++j) {
      LOG_INFO << "input_str_vec[i]: " << input_str_vec[j] << " , 对应的tag大小: " << tag_res[i].tags[j+1].c_str();
    }
  }

  return 0;
}