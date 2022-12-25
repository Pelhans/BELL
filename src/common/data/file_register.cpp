#include "file_registerh"
#include "record_factory.h"
#include "file_template_map.h"

#include "record/key_value_record.h"

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

} // namespace bell