#pragma once

#include "record.h"
#include "buffer_loader.h"
#include <map>
#include <memory>
#include <string>
#include <functional>

using std::string;

namespace bell {
class DictRecordFactory {
public:
    static DictRecordFactory* Inst() {
        static DictRecordFactory inst;
        return &inst;
    }

    template <typename T>
    static BufferLoader* CreateTBufferLoader() {
        return new T;
    }

    template <typename T>
    static std::shared_ptr<Record> CreateTRecord() {
        return std::make_shared<T>();
    }

    std::shared_ptr<Record> CreateRecord(const string& filename) {
        auto it = name_record_creator_map_.find(filename);
        if (it != name_record_creator_map_.end()) {
            return it->second();
        }
        return nullptr;
    }

    std::shared_ptr<Record> CreateRecordByFilename(const string& filename) {
        auto it = file_record_creator_map_.find(filename);
        if (it != file_record_creator_map_.end()) {
            return it->second();
        }
        return nullptr;
    }

    std::function<std::shared_ptr<Record>()> CreateRecordFunc(
        const string& filename) {
        auto it = file_record_creator_map_.find(filename);
        if (it != file_record_creator_map_.end()) {
            return it->second;
        }
        return nullptr;
    }

    template <typename T>
    void RegisterBufferLoader(const string& filename) {
        file_bufferloader_creator_map_[filename] = &CreateTBufferLoader<T>;
    }

    template <typename T>
    void RegisterFileRecord(const string& filename) {
        file_record_creator_map_[filename] = &CreateTRecord<T>;
    }

    template <typename T>
    void RegisterRecord(const string& recordname) {
        name_record_creator_map_[recordname] = &CreateTRecord<T>;
    }

    std::map<string, std::function<BufferLoader*()>>& GetBufferLoaderMap() {
        return file_bufferloader_creator_map_;
    }

private:
    std::map<string, std::function<BufferLoader*()>>
        file_bufferloader_creator_map_;
    std::map<string, std::function<std::shared_ptr<Record>()>>
        file_record_creator_map_;
    std::map<string, std::function<std::shared_ptr<Record>()>>
        name_record_creator_map_;
};

} // namespace bell
