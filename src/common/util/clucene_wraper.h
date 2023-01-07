#pragma once

#include "CLucene.h"
#include <string.h>

using std::string;

namespace bell {
class CluceneWrap {
public:
    void IndexFiles(const char* path, const char* target,
                    const bool clearIndex);
};

} // namespace bell