#pragma once

#include <string>
#include <vector>
#include <string.h>
#include <sstream>
#include <map>

namespace spaceString {

const std::vector<std::string> punctuations = {"~", "!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "_", "+"
,"=", "-", "`", "{", "}", "[", "]", "|", "\"", ";", "'", "<", ">", "?", "/", ".", "`", "！", "￥", "……", "（", "）", "—", "、",
"【", "】", "；", "‘", "：", "“", "《", "》", "，", "。", "？", "·",};

inline int string2vector(const std::string& text, std::vector<std::string>& v) {
    for (size_t i=0; i<text.length();) {
        size_t cplen = 1;
        if ((text[i] & 0xf8) == 0xf0)
            cplen = 4;
        else if ((text[i] & 0xf0) == 0xe0)
            cplen = 3;
        else if ((text[i] & 0xe0) == 0xc0)
            cplen = 2;
        if ((i+cplen) > text.length()) cplen = 1;

        v.push_back(text.substr(i, cplen));
        i += cplen;
    }
    return 0;
}

inline int vector2map(std::map<std::string, int>& m) {
    for (int i=0; i< punctuations.size(); i++) {
        std::string p = punctuations[i];
        m[p] = 1;
    }
    return 0;
}

inline std::string query_norm(std::string query) {
    if (query.size() < 2) {
        return query;
    }
    std::vector<std::string> vec;
    string2vector(query, vec);
    std::string query_norm = "";
    if (vec.size() > 0) {
        std::map<std::string, int> puncs;
        vector2map(puncs);
        for (int i=0; i<vec.size(); ++i) {
            if (puncs.find(vec[i]) != puncs.end()) {
                continue;
            } else {
                query_norm += vec[i];
            }
        }
        return query_norm;
    } else {
        return query;
    }
}

};