#include "file_register.h"
#include "record_factory.h"
#include "dict_manager.h"
#include "file_template_map.h"

#include "record/key_value_record.h"
#include <boost/serialization/singleton.hpp>
#include <iostream>

namespace bell {

#define REGISTER_FILE(filename, TMAP, RECORD)                        \
    DictRecordFactory::Inst()->RegisterBufferLoader<TMAP>(filename); \
    DictRecordFactory::Inst()->RegisterFileRecord<RECORD>(filename); \
    DictRecordFactory::Inst()->RegisterRecord<RECORD>(#RECORD);

bool FileRegister() {
    REGISTER_FILE(WORD_SEG_FINE, FileTemplateMap, KeyValueRecord);

    return true;
}

bool DictRegister() {
    auto m = DictRecordFactory::Inst()->GetBufferLoaderMap();
    auto dict_manager =
        boost::serialization::singleton<DictManager>::get_const_instance();
    LOG_INFO << "file_bufferloader_creator_map_.size: " << m.size();
    for (auto it = m.begin(); it != m.end(); ++it) {
        auto ptr = it->second();
        if (!ptr) {
            LOG_INFO << "dict register load fail: " << it->first;
            return false;
        }
        ptr->m_str_buffer_name = it->first;
        ptr->m_file_name = it->first;
        LOG_INFO << "dict register load sucess: " << it->first;
        dict_manager.register_loader(ptr);
    }
    return true;
}

} // namespace bell
