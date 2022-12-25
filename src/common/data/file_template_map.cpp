#include "file_template_map.h"
#include "util/md5.h"
#include "util/str_util.h"
#include <sys/time.h>
#include <fstream>
#include <functional>

using std::shared_ptr;
using std::string;
using std::vector;

namespace bell {

bool FileTemplateMap::load(const string& fname) {
    LOG_INFO << "start to load data from file: " << fname.c_str();
    std::ifstream fin(fname);
    if (!fin) {
        LOG_WARN << "load file: " << fname.c_str() << " failed";
        return false;
    }
    struct timeval start, end;
    gettimeofday(&start, NULL);
    string line;
    getline(fin, line);
    int record_size = atoi(line.c_str());
    if (record_size < 0) {
        fin.close();
        LOG_WARN << "Invalid record size: " << record_size << " for file"
                 << fname.c_str();
        return false;
    }
    LOG_INFO << "record size = " << record_size
             << " for file: " << fname.c_str();
    auto tmp_table = std::make_shared<KeyRecordMap>();
    tmp_table->reserve(record_size);

    // TODO: 考虑并行加载
    int load_size = 0;
    std::function<std::shared_ptr<Record>()> factory =
        DictRecordFactory::Inst()->CreateRecordFunc(m_file_name);
    while (getline(fin, line)) {
        string key;
        auto record = factory();
        if (!record) {
            LOG_ERROR << "record parse failed: " << fname.c_str();
            return false;
        }
        try {
            int ret;
            ret = record->recordParse(line, key);
            if (ret == 0) {
                string intKey = bell::md5::md5_hash_string(key);
                uint64_t uintKey = spaceString::stringtolong<uint64_t>(intKey);
                tmp_table->emplace(uintKey, record);
                ++load_size;
            }
        } catch (...) {
            LOG_WARN << "parse record catched exception for line: " << line
                     << ", file: " << fname;
            continue;
        }
    }
    fin.close();

    gettimeofday(&end, NULL);
    double cost =
        end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec) / 1000000.0;
    LOG_INFO << "FileTemplateMap::load record_size=" << record_size
             << ", load_size=" << load_size << " for line: " << fname
             << ", cost=" << cost;
    return true;
}

} // namespace bell