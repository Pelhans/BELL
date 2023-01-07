/*------------------------------------------------------------------------------
* Copyright (C) 2003-2006 Ben van Klinken and the CLucene Team
*
* Distributable under the terms of either the Apache License (Version 2.0) or
* the GNU Lesser General Public License, as specified in the COPYING file.
------------------------------------------------------------------------------*/
// Includes some standard headers for searching and indexing.
#ifndef _lucene_CLucene_
#define _lucene_CLucene_

#include "StdHeader.h"
#include "index/IndexReader.h"
#include "index/IndexWriter.h"
#include "index/MultiReader.h"
#include "index/Term.h"
#include "search/IndexSearcher.h"
#include "search/MultiSearcher.h"
#include "search/DateFilter.h"
#include "search/WildcardQuery.h"
#include "search/FuzzyQuery.h"
#include "search/PhraseQuery.h"
#include "search/PrefixQuery.h"
#include "search/RangeQuery.h"
#include "search/BooleanQuery.h"
#include "search/TermQuery.h"
#include "search/SearchHeader.h"
#include "search/Similarity.h"
#include "search/Sort.h"
#include "search/Hits.h"
#include "search/Explanation.h"
#include "document/Document.h"
#include "document/Field.h"
#include "document/DateField.h"
#include "document/DateTools.h"
#include "document/NumberTools.h"
#include "store/Directory.h"
#include "store/FSDirectory.h"
#include "store/RAMDirectory.h"
#include "queryParser/QueryParser.h"
#include "analysis/standard/StandardAnalyzer.h"
#include "analysis/Analyzers.h"
#include "util/BitSet.h"
#include "util/CLStreams.h"
#include "util/PriorityQueue.h"

#endif
