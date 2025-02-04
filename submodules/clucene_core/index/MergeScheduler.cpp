/*------------------------------------------------------------------------------
* Copyright (C) 2003-2006 Ben van Klinken and the CLucene Team
*
* Distributable under the terms of either the Apache License (Version 2.0) or
* the GNU Lesser General Public License, as specified in the COPYING file.
------------------------------------------------------------------------------*/
#include "../_ApiHeader.h"
#include "MergeScheduler.h"
#include "IndexWriter.h"

CL_NS_DEF(index)

const char* SerialMergeScheduler::getObjectName() const {
    return getClassName();
}
const char* SerialMergeScheduler::getClassName() {
    return "SerialMergeScheduler";
}

void SerialMergeScheduler::merge(IndexWriter* writer) {
    SCOPED_LOCK_MUTEX(THIS_LOCK)
    while (true) {
        MergePolicy::OneMerge* merge = writer->getNextMerge();
        if (merge == NULL) break;
        writer->merge(merge);
    }
}

void SerialMergeScheduler::close() {}

CL_NS_END
