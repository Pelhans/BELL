#pragma once

#include <string>
#include "util/md5.h"
#include "file_template_map.h"

using std::string;

template <typename TRecord>
static bool match_key_word(const string& file_name, const string& key_word, Trecord& record) {
    auto template_map = GetTemplateMapDict(file_name);
    if (!template_map) {
        LOG_ERROR << "dict match fail to load file: " << file_name.c_str();
    }

    string intKey = bell::md5::md5_hash_string(key);
    uint64_t uintKey = spaceString::stringtolong<uint64_t>(intKey);
    if (template_map->Find(md5, record)) {
        return true;
    }
    return false;
}
