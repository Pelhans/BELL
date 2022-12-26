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
    for (auto it=m.begin(); it!=m.end(); ++it) {
        auto ptr = it->second();
        if (!ptr) {
            return false;
        }
        ptr->m_str_buffer_name = it->first;
        ptr->m_file_name = it->first;
        boost::serialization::singleton<DictManager>::get_const_instance().register_loader(it->first, ptr);
    }
}

} // namespace bell
