project(clucene_core)

SET(CLUCENE_VERSION_MAJOR "0")
SET(CLUCENE_VERSION_MINOR "9")
SET(CLUCENE_VERSION_REVISION "23")
SET(CLUCENE_VERSION_PATCH "0")
SET(CLUCENE_INT_VERSION 92300)

SET(CLUCENE_VERSION "${CLUCENE_VERSION_MAJOR}.${CLUCENE_VERSION_MINOR}.${CLUCENE_VERSION_REVISION}.${CLUCENE_VERSION_PATCH}")
SET(CLUCENE_SOVERSION "${CLUCENE_VERSION_MAJOR}.${CLUCENE_VERSION_MINOR}.${CLUCENE_VERSION_REVISION}")

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

Include (CLuceneDocs)
Include (FindThreads)
INCLUDE (DefineOptions)

DEFINE_OPTIONS(EXTRA_OPTIONS EXTRA_LIBS)
ADD_DEFINITIONS(${EXTRA_OPTIONS} -DMAKE_CLUCENE_CORE_LIB)

DEFINE_OPTIONS(EXTRA_OPTIONS EXTRA_LIBS)
ADD_DEFINITIONS(${EXTRA_OPTIONS} -DMAKE_CLUCENE_SHARED_LIB)


IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ELSE(NOT CMAKE_BUILD_TYPE)
    MESSAGE( "Compiling as ${CMAKE_BUILD_TYPE}" )
ENDIF(NOT CMAKE_BUILD_TYPE)

OPTION(ENABLE_DEBUG
  "enable debug support"
  OFF)
OPTION(ENABLE_MMAP
  "enable mmap support (experimental)"
  OFF)
OPTION(DISABLE_MULTITHREADING
  "disable multithreading - remove all locking code"
  OFF)
OPTION(ENABLE_DMALLOC
  "enable dmalloc memory leak checker"
  OFF)
OPTION(ENABLE_ASCII_MODE
  "enable ascii support"
  OFF)
OPTION(LUCENE_USE_INTERNAL_CHAR_FUNCTIONS
  "use internal character functions. required to run tests correctly"
  ON)
OPTION(ENABLE_PACKAGING
  "create build scripts for creating clucene packages"
  OFF)
OPTION(BUILD_STATIC_LIBRARIES
  "create targets for building static libraries"
  OFF)
OPTION(BUILD_CONTRIBS
  "create targets for building the clucene-contribs"
  OFF)
OPTION(BUILD_CONTRIBS_LIB
  "create targets for building the clucene-contribs-lib"
  OFF)
SET(LUCENE_SYS_INCLUDES "" CACHE PATH
      "location for non-system independent files. defaults to CMAKE_INSTALL_PREFIX. see INSTALL documentation for further information."
      )

SET ( ENABLE_COMPILE_TESTS_VALUE ON )
IF ( MSVC_IDE )
	#this is annoying...
	SET ( ENABLE_COMPILE_TESTS_VALUE OFF )
ENDIF( MSVC_IDE )

OPTION(ENABLE_COMPILE_TESTS
  "enable various projects that test alternative build switches"
  ${ENABLE_COMPILE_TESTS_VALUE})

#check flags...
INCLUDE (TestCXXAcceptsFlag)
IF ( CMAKE_COMPILER_IS_GNUCC )
CHECK_CXX_ACCEPTS_FLAG(-pg GccFlagPg)
IF ( GccFlagPg )
	OPTION(ENABLE_GPROF
		"turn on gprof profiling support"
		OFF)

	IF ( ENABLE_GPROF )
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
		SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
		SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")
		SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -pg")
	ENDIF ( ENABLE_GPROF )
ENDIF ( GccFlagPg )

IF("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC"  )
ENDIF("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64")

ENDIF(CMAKE_COMPILER_IS_GNUCC) 


INCLUDE (CheckIncludeFiles)
INCLUDE (CheckIncludeFileCXX)
INCLUDE (CheckCXXSourceCompiles)
INCLUDE (CheckCXXSourceRuns)
INCLUDE (CheckFunctionExists)

#local macros
INCLUDE (MacroMustDefine)
INCLUDE (MacroChooseType)
INCLUDE (MacroChooseMisc)
INCLUDE (MacroChooseFunction)
INCLUDE (MacroChooseSymbol)
INCLUDE (MacroCheckGccVisibility)

INCLUDE (DefineFloat)
INCLUDE (DefineDword)
INCLUDE (DefineLongLongSyntax)
INCLUDE (DefineStaticSyntax)
INCLUDE (DefineMAXPATHValue)

INCLUDE (CheckFloatByte)
INCLUDE (CheckErrorHandling)
INCLUDE (CheckHashmaps)
INCLUDE (CheckNamespace)
INCLUDE (CheckSnprintf)
INCLUDE (CheckStdCallFunctionExists)

find_package(Threads REQUIRED)
INCLUDE (CheckPthread)
INCLUDE (CheckAtomicFunctions)

find_package(ZLIB)
IF ( ZLIB_FOUND )
SET ( EXTRA_LIBS ${EXTRA_LIBS} ${ZLIB_LIBRARY} )
INCLUDE_DIRECTORIES( ${ZLIB_INCLUDE_DIR} )
ELSE ( ZLIB_FOUND )
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/zlib )
ENDIF ( ZLIB_FOUND )

MACRO_CHECK_GCC_VISIBILITY(_CL_HAVE_GCCVISIBILITYPATCH)
	
#check flags...
INCLUDE (TestCXXAcceptsFlag)
IF ( CMAKE_COMPILER_IS_GNUCC )
    CHECK_CXX_ACCEPTS_FLAG(-pg GccFlagPg)
    IF ( GccFlagPg )
        OPTION(ENABLE_GPROF
          "turn on gprof profiling support"
          OFF)

        IF ( ENABLE_GPROF )
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
            SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
            SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")
            SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -pg")
        ENDIF ( ENABLE_GPROF )
    ENDIF ( GccFlagPg )
    
    IF("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC"  )
    ENDIF("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64")

ENDIF(CMAKE_COMPILER_IS_GNUCC) 


########################################################################
# test for headers
########################################################################

CHECK_INCLUDE_FILES ("sys/time.h;time.h" _CL_TIME_WITH_SYS_TIME)

CHECK_REQUIRED_HEADERS ( stdlib.h stdarg.h stdio.h stddef.h ctype.h algorithm  
                        functional map vector list set math.h fcntl.h limits.h)
                        
CHECK_OPTIONAL_HEADERS ( string.h sys/time.h memory.h sys/types.h 
                        stdint.h unistd.h io.h direct.h sys/dir.h sys/ndir.h dirent.h wctype.h fcntl.h
                        stat.h sys/stat.h stdexcept errno.h fcntl.h windef.h windows.h wchar.h 
                        hash_map hash_set ext/hash_map ext/hash_map tr1/unordered_set tr1/unordered_map
                        sys/timeb.h tchar.h strings.h stdexcept sys/mman.h winerror.h )


########################################################################
# test for types
########################################################################
#find int_t types
CHOOSE_TYPE(int8_t      1 signed   "int8_t;char")
CHOOSE_TYPE(uint8_t     1 unsigned "uint8_t;char")
CHOOSE_TYPE(int16_t     2 signed   "int16_t;short")
CHOOSE_TYPE(uint16_t    2 unsigned "uint16_t;short")
CHOOSE_TYPE(int32_t     4 signed   "int32_t;int;long")
CHOOSE_TYPE(uint32_t    4 unsigned "uint32_t;int;long")
CHOOSE_TYPE(int64_t     8 signed   "int64_t;long long;__int64")
CHOOSE_TYPE(uint64_t    8 unsigned "uint64_t;long long;__int64")

CHOOSE_TYPE(size_t      -1 unsigned "size_t;int" )
CHOOSE_MISC(TYPE__TIMEB
            HAVE_TYPE__TIMEB
            "struct _timeb x\;" "/* #undef _timeb */" "_timeb"
            "struct timeb x\;" "#define _timeb timeb" "timeb"
            )

#run macro for checking float. (and _FLT_EVAL_METHOD)
DEFINE_FLOAT()
DEFINE_DWORD()

########################################################################
# Test for functions
########################################################################


CHECK_REQUIRED_FUNCTIONS(  printf strftime wcscpy wcsncpy wcscat wcschr wcsstr wcslen 
                            wcscmp wcsncmp wcscspn )

#todo: wcstoq is bsd equiv of wcstoll, we can use that...
CHECK_OPTIONAL_FUNCTIONS( wcsupr wcscasecmp wcsicmp wcstoll wprintf lltow 
    wcstod wcsdup strupr strlwr lltoa strtoll gettimeofday _vsnwprintf mmap "MapViewOfFile(0,0,0,0,0)"
)

#make decisions about which functions to use...
CHOOSE_FUNCTION(fileHandleStat "fstati64;_fstati64;fstat64;fstat;_fstat")
IF ( _CL_HAVE_FUNCTION_FSTATI64 OR _CL_HAVE_FUNCTION__FSTATI64 OR _CL_HAVE_FUNCTION_FSTAT64 )
    SET ( USE_STAT64 1 )
ENDIF ( _CL_HAVE_FUNCTION_FSTATI64 OR _CL_HAVE_FUNCTION__FSTATI64 OR _CL_HAVE_FUNCTION_FSTAT64 )

IF ( USE_STAT64 )
    CHOOSE_FUNCTION(fileStat "stati64;_stati64;stat64;stat;_stat")
    CHOOSE_FUNCTION(fileSize "filelengthi64;_filelengthi64;filelength;_filelength" "#define fileSize CL_NS(util)::Misc::filelength")
    CHOOSE_FUNCTION(fileSeek "lseeki64;_lseeki64;lseek64;lseek;_lseek")
    CHOOSE_MISC(TYPE_CL_STAT_T
                HAVE_TYPE_CL_STAT_T
                "struct stati64 x\;" "#define cl_stat_t stati64" "stati64"
                "struct _stati64 x\;" "#define cl_stat_t _stati64" "_stati64"
                "struct stat64 x\;" "#define cl_stat_t stat64" "stat64"
                "struct stat x\;" "#define cl_stat_t stat" "stat"
                "struct _stat x\;" "#define cl_stat_t _stat" "_stat"
                )
ELSE( USE_STAT64 )
    #borland doesn't have a fstat64, so we have to fallback to non 64 bit everything...
    CHOOSE_FUNCTION(fileStat "stat;_stat")
    CHOOSE_FUNCTION(fileSize "filelength;_filelength" "#define fileSize CL_NS(util)::Misc::filelength")
    CHOOSE_FUNCTION(fileSeek "lseek;_lseek")
    CHOOSE_MISC(TYPE_CL_STAT_T
                HAVE_TYPE_CL_STAT_T
                "struct stat x\;" "#define cl_stat_t stat" "stat"
                "struct _stat x\;" "#define cl_stat_t _stat" "_stat"
                )
ENDIF( USE_STAT64 )

#ftell (and probably soon ftell64) are POSIX standard functions, but tell and
#tell64 are not, so we define fileTell in terms of fileSeek.
CHOOSE_FUNCTION(fileTell "telli64;_telli64;tell64;tell;_tell" "#define fileTell(fhandle) fileSeek(fhandle, 0, SEEK_CUR)")

CHOOSE_FUNCTION(_realpath "realpath" "#define _realpath(rel,abs) ::_fullpath(abs,rel,CL_MAX_PATH)")
CHOOSE_FUNCTION(_rename "rename")
CHOOSE_FUNCTION(_close "_close((int)0);close")
CHOOSE_FUNCTION(_read "_read((int)0, (void*)0, (unsigned int)0);read")
CHOOSE_FUNCTION(_cl_open "_open(0,0,0);open")
CHOOSE_FUNCTION(_write "_write((int)0, (const void*)0, (unsigned int)0);write")
CHOOSE_FUNCTION(_unlink "_unlink((const char*)0);unlink")
CHOOSE_FUNCTION(_ftime "_ftime(0);ftime")
CHOOSE_FUNCTION(_mkdir "_mkdir((const char*)0)" "#define _mkdir(x) mkdir(x,0777)")
CHOOSE_FUNCTION(SLEEPFUNCTION "usleep;Sleep(0);_sleep")

CHOOSE_FUNCTION(_snprintf "snprintf;_snprintf")
CHOOSE_FUNCTION(_snwprintf "snwprintf;_snwprintf")

########################################################################
# test for symbols
########################################################################
CHOOSE_SYMBOL (_O_RANDOM "_O_RANDOM;O_RANDOM")
CHOOSE_SYMBOL (_O_BINARY "_O_BINARY;O_BINARY")
CHOOSE_SYMBOL (_S_IREAD  "_S_IREAD;S_IREAD")
CHOOSE_SYMBOL (_S_IWRITE "_S_IWRITE;S_IWRITE")


#define defaults
IF ( NOT HAVE_SYMBOL__O_RANDOM )
    SET (SYMBOL__O_RANDOM "#define _O_RANDOM 0")
ENDIF ( NOT HAVE_SYMBOL__O_RANDOM )

IF ( NOT HAVE_SYMBOL__O_BINARY )
    SET (SYMBOL__O_BINARY "#define _O_BINARY 0")
ENDIF ( NOT HAVE_SYMBOL__O_BINARY)

IF ( NOT HAVE_SYMBOL__S_IREAD )
    SET (SYMBOL__S_IREAD "#define _S_IREAD 0333")
ENDIF ( NOT HAVE_SYMBOL__S_IREAD )

IF ( NOT HAVE_SYMBOL__S_IWRITE )
    SET (SYMBOL__S_IWRITE "#define _S_IWRITE 0333")
ENDIF ( NOT HAVE_SYMBOL__S_IWRITE )

#try and figure out the actual value of what _CL_MAX_PATH is
DEFINE_MAXPATH_VALUE (SYMBOL_CL_MAX_PATH)


########################################################################
# test for tchar replacments
########################################################################
IF ( ENABLE_ASCII_MODE )
    CHOOSE_TYPE(TCHAR   1 "" "TCHAR;char" SYMBOL_TCHAR )
ELSE ( ENABLE_ASCII_MODE )
    CHOOSE_TYPE(TCHAR   -1 "" "TCHAR;wchar_t;unsigned short" SYMBOL_TCHAR)
ENDIF ( ENABLE_ASCII_MODE )
IF ( NOT SYMBOL_TCHAR STREQUAL "TCHAR" )
    SET( SYMBOL_TCHAR "#define TCHAR ${SYMBOL_TCHAR}" )
ELSE ( NOT SYMBOL_TCHAR STREQUAL "TCHAR" )
    SET( SYMBOL_TCHAR "/* #undef TCHAR */" )
ENDIF ( NOT SYMBOL_TCHAR STREQUAL "TCHAR" )

CHOOSE_SYMBOL (_T "_T" SYMBOL__T)
IF ( NOT HAVE_SYMBOL__T )
IF ( ENABLE_ASCII_MODE )
    SET (SYMBOL__T "#define _T(x) x")
ELSE ( ENABLE_ASCII_MODE )
    SET (SYMBOL__T "#define _T(x) L ## x")
ENDIF ( ENABLE_ASCII_MODE )
ELSE ( NOT HAVE_SYMBOL__T )
    SET( SYMBOL__T "/* #undef _T */" )
ENDIF ( NOT HAVE_SYMBOL__T )


########################################################################
# test for compiler capabilities.
########################################################################

#check for pthreads
IF ( CMAKE_USE_WIN32_THREADS_INIT )
    SET ( _CL_HAVE_WIN32_THREADS 1 )
ENDIF ( CMAKE_USE_WIN32_THREADS_INIT )
IF ( CMAKE_USE_PTHREADS_INIT )
    SET ( _CL_HAVE_PTHREAD 1 )
ENDIF (CMAKE_USE_PTHREADS_INIT)
IF ( CMAKE_USE_SPROC_INIT OR CMAKE_HP_PTHREADS_INIT )
    MESSAGE(FATAL_ERROR "Threads library not implemented")
ENDIF( CMAKE_USE_SPROC_INIT OR CMAKE_HP_PTHREADS_INIT )

#define if we have pthreads with recursive capabilities
CHECK_PTHREAD_RECURSIVE(_CL_HAVE_PTHREAD _CL_HAVE_PTHREAD_MUTEX_RECURSIVE)

CHECK_HAVE_GCC_ATOMIC_FUNCTIONS(_CL_HAVE_GCC_ATOMIC_FUNCTIONS)

#see if we can hide all symbols by default...
MACRO_CHECK_GCC_VISIBILITY(_CL_HAVE_GCCVISIBILITYPATCH)

#Check that we can handle try/catch
CHECK_HAVE_FUNCTION_TRY_BLOCKS (_CL_HAVE_TRY_BLOCKS)

#check that we support new float byte<->float conversions
CHECK_FLOAT_BYTE_WORKS(_CL_HAVE_NO_FLOAT_BYTE, 1)

#check how to use hashmaps
CHECK_HASH_MAPS (CL_NS_HASHING_VALUE LUCENE_DISABLE_HASHING)

#check that we have namespace support
CHECK_NAMESPACE (_CL_HAVE_NAMESPACES)

#check if snprintf functions are buggy
CHECK_SNPRINTF()

#define how we are going to define 64bit numbers
DEFINE_LONGLONG_SYNTAX()

#define how to define a static const (or fallback to enum)
DEFINE_STATIC_SYNTAX()

#test for ansi for scope (needed for msvc6)
INCLUDE(TestForANSIForScope)
IF ( CMAKE_ANSI_FOR_SCOPE )
    SET ( CMAKE_ANSI_FOR_SCOPE 1 )
ELSE ( CMAKE_ANSI_FOR_SCOPE )
    SET ( CMAKE_ANSI_FOR_SCOPE 0 )
ENDIF ( CMAKE_ANSI_FOR_SCOPE )


#check that all these definitions are set, or fail...
MUSTDEFINE_VAR("HAVE_TYPE_INT8_T;HAVE_TYPE_UINT8_T;HAVE_TYPE_INT16_T;HAVE_TYPE_UINT16_T;HAVE_TYPE_UINT16_T;" )
MUSTDEFINE_VAR("HAVE_TYPE_INT32_T;HAVE_TYPE_UINT32_T;HAVE_TYPE_INT64_T;HAVE_TYPE_UINT64_T;HAVE_TYPE_SIZE_T;HAVE_TYPE_CL_STAT_T")
MUSTDEFINE_VAR("_CL_HAVE_TRY_BLOCKS")

#must have timeb OR GETTIMEOFDAY
IF ( NOT HAVE_TYPE__TIMEB AND NOT _CL_HAVE_FUNCTION_GETTIMEOFDAY )
        MESSAGE( FATAL_ERROR "timeb or gettimeofday must be available." )
ENDIF ( NOT HAVE_TYPE__TIMEB AND NOT _CL_HAVE_FUNCTION_GETTIMEOFDAY )

#now write out our configuration....
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/shared/clucene-config.h.cmake ${clucene_core_BINARY_DIR}/shared/clucene-config.h)
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/shared/_clucene-config.h.cmake ${clucene_core_BINARY_DIR}/shared/_clucene-config.h)
      
#add the files to our groups and core
SOURCE_GROUP("analysis" ./analysis/*)
#seems to be a bug in cmake, can't put these in analysis\\standard folder :(
SOURCE_GROUP("analysis-standard" ./analysis/standard/*)
SOURCE_GROUP("document" ./document/*)
SOURCE_GROUP("debug" ./debug/*)
SOURCE_GROUP("index" ./index/*)
SOURCE_GROUP("queryParser" ./queryParser/*)
SOURCE_GROUP("queryParser-legacy" ./queryParser/legacy/*)
SOURCE_GROUP("search" ./search/*)
SOURCE_GROUP("search-spans" ./search/spans/*)
SOURCE_GROUP("store" ./store/*)
SOURCE_GROUP("util" ./util/*)

SOURCE_GROUP("shared" ./shared/*)
SOURCE_GROUP("config" ./shared/config/*)
SOURCE_GROUP("debug" ./shared/debug/*)
SOURCE_GROUP("util" ./shared/util/*)
SOURCE_GROUP("zlib" ./zlib/*)

SET(clucene_core_Files
	./StdHeader.cpp
	./debug/error.cpp
	./util/ThreadLocal.cpp
	./util/Reader.cpp
	./util/Equators.cpp
	./util/FastCharStream.cpp
	./util/MD5Digester.cpp
	./util/StringIntern.cpp
	./util/BitSet.cpp
	./queryParser/FastCharStream.cpp
	./queryParser/MultiFieldQueryParser.cpp
	./queryParser/QueryParser.cpp
	./queryParser/QueryParserTokenManager.cpp
	./queryParser/QueryToken.cpp
	./queryParser/legacy/Lexer.cpp
	./queryParser/legacy/MultiFieldQueryParser.cpp
	./queryParser/legacy/QueryParser.cpp
	./queryParser/legacy/QueryParserBase.cpp
	./queryParser/legacy/QueryToken.cpp
	./queryParser/legacy/TokenList.cpp
	./analysis/standard/StandardAnalyzer.cpp
	./analysis/standard/StandardFilter.cpp
	./analysis/standard/StandardTokenizer.cpp
	./analysis/standard/test_head.cpp
	./analysis/Analyzers.cpp
	./analysis/AnalysisHeader.cpp
	./store/MMapInput.cpp
	./store/IndexInput.cpp
	./store/Lock.cpp
	./store/LockFactory.cpp
	./store/IndexOutput.cpp
	./store/Directory.cpp
	./store/FSDirectory.cpp
	./store/RAMDirectory.cpp
	./document/Document.cpp
	./document/DateField.cpp
	./document/DateTools.cpp
	./document/Field.cpp
	./document/FieldSelector.cpp
	./document/NumberTools.cpp
	./index/IndexFileNames.cpp
	./index/IndexFileNameFilter.cpp
	./index/IndexDeletionPolicy.cpp
	./index/SegmentMergeInfo.cpp
	./index/SegmentInfos.cpp
	./index/MergeScheduler.cpp
	./index/SegmentTermDocs.cpp
	./index/FieldsWriter.cpp
	./index/TermInfosWriter.cpp
	./index/Term.cpp
	./index/Terms.cpp
	./index/MergePolicy.cpp
	./index/DocumentsWriter.cpp
	./index/DocumentsWriterThreadState.cpp
	./index/SegmentTermVector.cpp
	./index/TermVectorReader.cpp
	./index/FieldInfos.cpp
	./index/CompoundFile.cpp
	./index/SkipListReader.cpp
	./index/SkipListWriter.cpp
	./index/IndexFileDeleter.cpp
	./index/SegmentReader.cpp
	./index/DirectoryIndexReader.cpp
	./index/TermVectorWriter.cpp
	./index/IndexReader.cpp
	./index/SegmentTermPositions.cpp
	./index/SegmentMerger.cpp
	./index/IndexWriter.cpp
	./index/MultiReader.cpp
	./index/MultiSegmentReader.cpp
	./index/Payload.cpp
	./index/SegmentTermEnum.cpp
	./index/TermInfo.cpp
	./index/IndexModifier.cpp
	./index/SegmentMergeQueue.cpp
	./index/FieldsReader.cpp
	./index/TermInfosReader.cpp
	./index/MultipleTermPositions.cpp
	./search/Compare.cpp
	./search/Scorer.cpp
	./search/ScorerDocQueue.cpp
	./search/PhraseScorer.cpp
	./search/SloppyPhraseScorer.cpp
	./search/DisjunctionSumScorer.cpp
	./search/ConjunctionScorer.cpp
	./search/PhraseQuery.cpp
	./search/PrefixQuery.cpp
	./search/ExactPhraseScorer.cpp
	./search/TermScorer.cpp
	./search/Similarity.cpp
	./search/BooleanScorer.cpp
	./search/BooleanScorer2.cpp
	./search/HitQueue.cpp
	./search/FieldCacheImpl.cpp
	./search/ChainedFilter.cpp
	./search/RangeFilter.cpp
	./search/CachingWrapperFilter.cpp
	./search/QueryFilter.cpp
	./search/TermQuery.cpp
	./search/FuzzyQuery.cpp
	./search/SearchHeader.cpp
	./search/RangeQuery.cpp
	./search/IndexSearcher.cpp
	./search/Sort.cpp
	./search/PhrasePositions.cpp
	./search/FieldDocSortedHitQueue.cpp
	./search/WildcardTermEnum.cpp
	./search/MultiSearcher.cpp
	./search/Hits.cpp
	./search/MultiTermQuery.cpp
	./search/FilteredTermEnum.cpp
	./search/FieldSortedHitQueue.cpp
	./search/WildcardQuery.cpp
	./search/Explanation.cpp
	./search/BooleanQuery.cpp
	./search/FieldCache.cpp
	./search/DateFilter.cpp
	./search/MatchAllDocsQuery.cpp
	./search/MultiPhraseQuery.cpp
	./search/ConstantScoreQuery.cpp
	./search/CachingSpanFilter.cpp
	./search/CachingSpanFilter.h
	./search/SpanFilter.h
	./search/SpanFilterResult.h
	./search/SpanQueryFilter.cpp
	./search/SpanQueryFilter.h	
	./search/spans/_EmptySpans.h
	./search/spans/_NearSpansOrdered.h
	./search/spans/_NearSpansUnordered.h
	./search/spans/_TermSpans.h
	./search/spans/NearSpansOrdered.cpp
	./search/spans/NearSpansUnordered.cpp
	./search/spans/SpanFirstQuery.cpp
	./search/spans/SpanFirstQuery.h
	./search/spans/SpanNearQuery.cpp
	./search/spans/SpanNearQuery.h
	./search/spans/SpanNotQuery.cpp
	./search/spans/SpanNotQuery.h
	./search/spans/SpanOrQuery.cpp
	./search/spans/SpanOrQuery.h
	./search/spans/SpanQuery.h
	./search/spans/Spans.h
	./search/spans/SpanScorer.cpp
	./search/spans/SpanScorer.h
	./search/spans/SpanTermQuery.cpp
	./search/spans/SpanTermQuery.h
	./search/spans/SpanWeight.cpp
	./search/spans/SpanWeight.h
	./search/spans/TermSpans.cpp
    ./shared/SharedHeader.cpp
	./shared/SharedHeader.h
	./shared/LuceneThreads.h
	./shared/CLSharedMonolithic.cpp
    ./shared/config/gunichartables.cpp
    ./shared/config/repl_tcslwr.cpp
    ./shared/config/repl_tcstoll.cpp
    ./shared/config/repl_tcscasecmp.cpp
    ./shared/config/repl_tprintf.cpp
    ./shared/config/repl_lltot.cpp
    ./shared/config/repl_tcstod.cpp
    ./shared/config/utf8.cpp
    ./shared/config/threads.cpp
    ./shared/debug/condition.cpp
    ./shared/util/StringBuffer.cpp
    ./shared/util/Misc.cpp
    ./shared/util/dirent.cpp
	./shared/config/test.h
    ./zlib/adler32.c
    ./zlib/compress.c
    ./zlib/crc32.c
    ./zlib/deflate.c
    ./zlib/gzio.c
    ./zlib/inffast.c
    ./zlib/inflate.c
    ./zlib/inftrees.c
    ./zlib/trees.c
    ./zlib/zutil.c
)

#find our headers
file(GLOB HEADERS ${clucene_core_SOURCE_DIR}/*.h ${clucene_core_SOURCE_DIR}/shared/*.h)

#create the libraries
INCLUDE_DIRECTORIES( ${clucene_core_SOURCE_DIR}/src/core )

add_library(clucene_core
	${clucene_core_Files} ${HEADERS}
)

#set properties on the libraries
SET_TARGET_PROPERTIES(clucene_core PROPERTIES
    COMPILE_DEFINITIONS_DEBUG _DEBUG
)

#install public headers.
FOREACH(file ${HEADERS})
	get_filename_component(apath ${file} PATH)
	get_filename_component(aname ${file} NAME)
	file(RELATIVE_PATH relpath ${clucene_core_SOURCE_DIR}/ ${apath})
	IF ( NOT aname MATCHES "^_.*" )
		install(FILES ${file} 
		        DESTINATION include/${relpath})
	ENDIF ( NOT aname MATCHES "^_.*" )
ENDFOREACH(file)

TARGET_LINK_LIBRARIES(clucene_core  ${EXTRA_LIBS} ${CMAKE_THREAD_LIBS_INIT})

# code for installing an script to help cmake applications determine
# the CLucene version number
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/CLuceneConfig.cmake" "
    set(CLUCENE_VERSION_MAJOR ${CLUCENE_VERSION_MAJOR})
    set(CLUCENE_VERSION_MINOR ${CLUCENE_VERSION_MINOR})
    set(CLUCENE_VERSION_REVISION ${CLUCENE_VERSION_REVISION})
    set(CLUCENE_VERSION_PATCH ${CLUCENE_VERSION_PATCH})
    
    set(CLUCENE_VERSION ${CLUCENE_VERSION})
    set(CLUCENE_SOVERSION ${CLUCENE_SOVERSION})
")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/CLuceneConfig.cmake"
        DESTINATION ${LIB_DESTINATION}/CLuceneConfig.cmake)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/libclucene.pc.cmake ${CMAKE_CURRENT_BINARY_DIR}/libclucene.pc)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libclucene.pc
        DESTINATION ${LIB_DESTINATION}/pkgconfig )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/shared/clucene-config.h 
        DESTINATION include/CLucene)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/CLuceneConfig.cmake 
        DESTINATION include/CLucene)

install(FILES ${HEADERS} DESTINATION include/clucene_core)

#and install library
install(TARGETS clucene_core 
        DESTINATION lib )

#this must go last...
IF (ENABLE_PACKAGING)
  INCLUDE(CreateClucenePackages)
ENDIF ( ENABLE_PACKAGING)