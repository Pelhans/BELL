#pragma once

#include "base_signal_output.h"
#include "bell_types.h"
#include <vector>
#include <string>
#include <memory>

using std::string;

namespace bell {
namespace event {

class SegResOutput {
public:
    SegResOutput() {}
    virtual ~SegResOutput() {}

    std::vector<bell::types::NerPath> seg_tag_list;
};

}
}

using SegResOutputPtr = std::shared_ptr<bell::event::SegResOutput>;