#include "seg_handler.h"
#include "common/model/model_util.h"
#include "common/util/base_define.h"
#include "common/util/str_util.h"
#include "muduo/base/Logging.h"
#include <iostream>
#include <string>

using std::string;

int WordSeg::work(std::shared_ptr<bell::Event> bevent) {
  const string& origin_query = bevent->req.origin_query;
  if (origin_query.size() <= 1) {
    return BELL_SUCESS;
  }
  //req = "今天的烤鱼还是很好吃的。";
  LOG_INFO << "输入的文本为: " << origin_query.c_str();
  vector<ResultTag> tag_res;
  std::vector<std::string> input_str_vec;
  spaceString::string2vector(origin_query, input_str_vec);
  bell_model::ModelUtil::cnncrf_predict("word_seg_cnn_crf", input_str_vec,
  tag_res);
  for (int i=0; i<tag_res.size(); ++i) {
    LOG_INFO << "tag_res[i]: " << "对应的tag大小: " << tag_res[i].tags.size();
    for (int j=0; j<input_str_vec.size(); ++j) {
      LOG_INFO << "input_str_vec[i]: " << input_str_vec[j] << " , 对应的tag大小: " << tag_res[i].tags[j+1].c_str();
    }
  }  
  return BELL_SUCESS;
}

// int WordSeg::decode(std::vector<std::string> input_str_vec, vector<ResultTag> tag_res) {
//   if (input_str_vec.size() <= 2) {
//     return BELL_FAIL;
//   }

//   for (auto seg : )
// }