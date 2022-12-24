#pragma once

#include <string>
#include <memory>
#include "word_seg_output.h"
#include "ner_output.h"

namespace bell {

struct ReqInput {
    string origin_query;
    string norm_query;
};

class Event {
public:
    ReqInput req;

    bell::event::SegResOutput seg_output;
};

} // namespace bell
