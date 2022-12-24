#pragma once

#include <iostream>
#include <string>
#include "common/event/word_seg_output.h"
#include "common/event/bell_handler_event.h"
#include "common/model/word_seg_model/eigen_utils.h"
#include "common/event/bell_types.h"

class WordSeg {
public:
    int work(std::shared_ptr<bell::Event> bevent);
    int decode(bell::event::SegResOutput& seg_output,
               std::vector<std::string>& input_str_vec,
               std::vector<ResultTag>& tag_res);
};