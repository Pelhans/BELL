/*------------------------------------------------------------------------------
* Copyright (C) 2003-2006 Ben van Klinken and the CLucene Team
*
* Distributable under the terms of either the Apache License (Version 2.0) or
* the GNU Lesser General Public License, as specified in the COPYING file.
------------------------------------------------------------------------------*/
/*
 * this is a monolithic file that can be used to compile clucene using one
 * source file. it simplifies some build processes by avoiding static & dynamic
 * compalation pitfalls.
 *
 * note: when creating a project add either this file, or all the other .cpp
 * files, not both!
 */
#include "SharedHeader.cpp"
#include "config/gunichartables.cpp"
#include "config/repl_tcscasecmp.cpp"
#include "config/repl_tcslwr.cpp"
#include "config/repl_tcstod.cpp"
#include "config/repl_lltot.cpp"
#include "config/repl_tcstoll.cpp"
#include "config/repl_tprintf.cpp"
#include "config/threads.cpp"
#include "config/utf8.cpp"
#include "shared/debug/condition.cpp"
#include "shared/util/Misc.cpp"
#include "shared/util/dirent.cpp"
#include "shared/util/StringBuffer.cpp"
