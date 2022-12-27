#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <mutex>
#include <utility>
#include <malloc.h>

#include "record.h"
#include "record_factory.h"

#include "muduo/base/Logging.h"

using std::string;
using std::unordered_map;

namespace bell {

class FileTemplateMap : public BufferLoader {
public:
    typedef unordered_map<uint64_t, std::shared_ptr<Record>> KeyRecordMap;
    FileTemplateMap(const string& file = "") {
        m_str_buffer_name = file;
        m_file_name = file;
        base_key_record_map_ = std::make_shared<KeyRecordMap>();
        inc_key_record_map_ = std::make_shared<KeyRecordMap>();
    }

    virtual ~FileTemplateMap() {}

    bool isEmpry() {
        return (base_key_record_map_->empty() && inc_key_record_map_->empty());
    }

    virtual void* getBufferPtr() override { return this; }

    virtual int load(const string& fname) override;

private:
    std::shared_ptr<KeyRecordMap> base_key_record_map_;
    std::shared_ptr<KeyRecordMap> inc_key_record_map_;
};

FileTemplateMap* GetTemplateMapDict(const string& buffer_name);

} // namespace bell