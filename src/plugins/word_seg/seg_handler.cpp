#include "seg_handler.h"

#include <iostream>
#include <string>

#include "common/model/model_util.h"
#include "common/util/base_define.h"
#include "common/util/str_util.h"
#include "muduo/base/Logging.h"

using std::string;

int WordSeg::work(std::shared_ptr<bell::Event> bevent) {
    const string &origin_query = bevent->req.origin_query;
    if (origin_query.size() <= 1) {
        return BELL_SUCESS;
    }
    // req = "今天的烤鱼还是很好吃的。";
    bell::event::SegResOutput &seg_output = bevent->seg_output;
    LOG_INFO << "输入的文本为: " << origin_query.c_str();
    vector<ResultTag> tag_res;
    std::vector<std::string> input_str_vec;
    spaceString::string2vector(origin_query, input_str_vec);
    bell_model::ModelUtil::cnncrf_predict("word_seg_cnn_crf", input_str_vec,
                                          tag_res);
    decode(seg_output, input_str_vec, tag_res);

    return BELL_SUCESS;
}

int WordSeg::decode(bell::event::SegResOutput &seg_output,
                    std::vector<std::string> &input_str_vec,
                    std::vector<ResultTag> &tag_res) {
    if (tag_res.size() < 1) {
        return BELL_FAIL;
    }

    LOG_INFO << "tag_res[0].tags.size(): " << tag_res[0].tags.size();
    LOG_INFO << "input_str_vec.size(): " << input_str_vec.size();

    if (tag_res[0].tags.size() != (input_str_vec.size() + 2)) {
        LOG_ERROR << "word seg tag length != input str length";
        return BELL_FAIL;
    }

    bell::types::NerPath ner_path;
    ner_path.score = tag_res[0].prob;
    std::vector<bell::types::Entity> &ner_list = ner_path.ner_list;

    string pre_text;
    int pre_term_begin = 0;
    for (int i = 0; i < input_str_vec.size(); ++i) {
        string &cur_tag_str = tag_res[0].tags[i + 1];
        string &cur_str = input_str_vec[i];
        if (cur_tag_str == "S" || cur_tag_str == "B") {
            if (!pre_text.empty()) {
                bell::types::Entity entity;
                entity.text = pre_text;
                entity.term_begin = pre_term_begin;
                entity.term_end = i - 1;
                pre_text = "";
                ner_list.push_back(entity);
            }
            pre_term_begin = i;
            pre_text += input_str_vec[i];
        } else if (cur_tag_str == "E" || cur_tag_str == "I") {
            pre_text += input_str_vec[i];
        }
    }
    if (!pre_text.empty()) {
        bell::types::Entity entity;
        entity.text = pre_text;
        entity.term_begin = pre_term_begin;
        entity.term_end = input_str_vec.size() - 1;
        pre_text = "";
        ner_list.push_back(entity);
    }

    seg_output.seg_tag_list.push_back(ner_path);
    return BELL_SUCESS;
}