#pragma once

#include <string>
#include "clucene_core/analysis/standard/StandardAnalyzer.h"

#include <stdio.h>
#include <iostream>
#include <string.h>

using namespace std;

namespace bell {
class LuceneWraper {
public:
    static int IndexFiles(const char* path, const char* target,
                          const bool clearIndex);
};

} // namespace bell