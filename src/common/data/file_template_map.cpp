#include "file_template_map.h"
#include "dict_manager.h"
#include "util/md5.h"
#include "util/str_util.h"
#include <sys/time.h>
#include <fstream>
#include <functional>
#include <boost/serialization/singleton.hpp>

using std::shared_ptr;
using std::string;
using std::vector;

namespace bell {

FileTemplateMap* GetTemplateMapDict(const string& buffer_name) {
    if (buffer_name.empty()) {
        LOG_ERROR
            << "Get buffer from dict framework failed for empty buffer_name";
        return nullptr;
    }
    auto buffer_loader =
        boost::serialization::singleton<DictManager>::get_const_instance()
            .get_buffer(buffer_name);
    if (buffer_loader == nullptr) {
        LOG_ERROR << "Get buffer from dict framework failed for "
                  << buffer_name;
        return nullptr;
    }
    auto template_map = dynamic_cast<FileTemplateMap*>(buffer_loader);
    if (template_map == nullptr) {
        LOG_ERROR << "Cast bufloader to templatemap failed for " <<
        buffer_name; return nullptr;
    }
    LOG_INFO << "get templatemap dict succeed for " << buffer_name;
    return template_map;
}

int FileTemplateMap::load(const string& fname) {
    string path = "./deploy/data/data_file/";
    string path2fname = path + fname;
    LOG_INFO << "start to load data from file: " << path2fname.c_str();
    std::ifstream fin(path2fname);
    if (!fin) {
        LOG_WARN << "load file: " << fname.c_str() << " failed";
        return -1;
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
        return -1;
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
            return -1;
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
    return 0;
}

} // namespace bell
