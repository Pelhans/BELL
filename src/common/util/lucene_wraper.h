#pragma once

#include <string>
//#include "clucene_core/CLucene.h"

#include <stdio.h>
#include <iostream>
#include <string.h>

using namespace std;

namespace bell {
class LuceneWraper {
public:
    static void IndexFiles(const char* path, const char* target, const bool clearIndex); 
};

}