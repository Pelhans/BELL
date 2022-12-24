#pragma once

#include "base_signal_output.h"
#include "bell_types.h"
#include <vector>
#include <string>
#include <map>
#include <memory>

using std::string;

namespace bell {
namespace event {

class NerResOutput {
public:
    NerResOutput() {}
    virtual ~NerResOutput() {}
};

} // namespace event
} // namespace bell

using NerResOutputPtr = std::shared_ptr<bell::event::NerResOutput>;