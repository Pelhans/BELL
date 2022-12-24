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

}
}

using NerResOutputPtr = std::shared_ptr<bell::event::NerResOutput>;