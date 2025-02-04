/*------------------------------------------------------------------------------
* Copyright (C) 2003-2006 Ben van Klinken and the CLucene Team
*
* Distributable under the terms of either the Apache License (Version 2.0) or
* the GNU Lesser General Public License, as specified in the COPYING file.
------------------------------------------------------------------------------*/
#ifndef _lucene_queryParser_QueryParser_
#define _lucene_queryParser_QueryParser_

#include "CLucene/util/Array.h"
#include "QueryParserTokenManager.h"
#include "CLucene/document/DateTools.h"
#include "CLucene/util/VoidMap.h"
#include "CLucene/util/VoidList.h"

CL_CLASS_DEF(index,Term)
CL_CLASS_DEF(analysis,Analyzer)
CL_CLASS_DEF(search,Query)
CL_CLASS_DEF(search,BooleanClause)

CL_NS_DEF(queryParser)

class QueryParserConstants;

/**
 * This class is generated by JavaCC.  The most important method is
 * {@link #parse(String)}.
 *
 * The syntax for query strings is as follows:
 * A Query is a series of clauses.
 * A clause may be prefixed by:
 * <ul>
 * <li> a plus (<code>+</code>) or a minus (<code>-</code>) sign, indicating
 * that the clause is required or prohibited respectively; or
 * <li> a term followed by a colon, indicating the field to be searched.
 * This enables one to construct queries which search multiple fields.
 * </ul>
 *
 * A clause may be either:
 * <ul>
 * <li> a term, indicating all the documents that contain this term; or
 * <li> a nested query, enclosed in parentheses.  Note that this may be used
 * with a <code>+</code>/<code>-</code> prefix to require any of a set of
 * terms.
 * </ul>
 *
 * Thus, in BNF, the query grammar is:
 * <pre>
 *   Query  ::= ( Clause )*
 *   Clause ::= ["+", "-"] [&lt;TERM&gt; ":"] ( &lt;TERM&gt; | "(" Query ")" )
 * </pre>
 *
 * <p>
 * Examples of appropriately formatted queries can be found in the <a
 * href="http://lucene.apache.org/java/docs/queryparsersyntax.html">query syntax
 * documentation</a>.
 * </p>
 *
 * <p>
 * In {@link RangeQuery}s, QueryParser tries to detect date values, e.g.
 * <tt>date:[6/1/2005 TO 6/4/2005]</tt> produces a range query that searches
 * for "date" fields between 2005-06-01 and 2005-06-04. Note that the format
 * of the accepted input depends on {@link #setLocale(Locale) the locale}.
 * By default a date is converted into a search term using the deprecated
 * {@link DateField} for compatibility reasons.
 * To use the new {@link DateTools} to convert dates, a
 * {@link org.apache.lucene.document.DateTools.Resolution} has to be set.
 * </p>
 * <p>
 * The date resolution that shall be used for RangeQueries can be set
 * using {@link #setDateResolution(DateTools.Resolution)}
 * or {@link #setDateResolution(String, DateTools.Resolution)}. The former
 * sets the default date resolution for all fields, whereas the latter can
 * be used to set field specific date resolutions. Field specific date
 * resolutions take, if set, precedence over the default date resolution.
 * </p>
 * <p>
 * If you use neither {@link DateField} nor {@link DateTools} in your
 * index, you can create your own
 * query parser that inherits QueryParser and overwrites
 * {@link #getRangeQuery(String, String, String, boolean)} to
 * use a different method for date conversion.
 * </p>
 *
 * <p>Note that QueryParser is <em>not</em> thread-safe.</p>
 *
 * @author Brian Goetz
 * @author Peter Halacsy
 * @author Tatu Saloranta
 */
class CLUCENE_EXPORT QueryParser : public virtual QueryParserConstants
{
private:
  LUCENE_STATIC_CONSTANT(int32_t,  CONJ_NONE=0);
  LUCENE_STATIC_CONSTANT(int32_t,  CONJ_AND=1);
  LUCENE_STATIC_CONSTANT(int32_t,  CONJ_OR=2);

  LUCENE_STATIC_CONSTANT(int32_t,  MOD_NONE=0);
  LUCENE_STATIC_CONSTANT(int32_t,  MOD_NOT=10);
  LUCENE_STATIC_CONSTANT(int32_t,  MOD_REQ=11);

public:
  /** The default operator for parsing queries.
  * Use {@link QueryParser#setDefaultOperator} to change it.
  */
  enum Operator {
    OR_OPERATOR,
    AND_OPERATOR
  };

private:
  /** The actual operator that parser uses to combine query terms */
  Operator _operator;

  bool lowercaseExpandedTerms;
  bool useOldRangeQuery;
  bool allowLeadingWildcard;
  bool enablePositionIncrements;

  CL_NS(analysis)::Analyzer* analyzer;
  TCHAR* field;
  int32_t phraseSlop;
  float_t fuzzyMinSim;
  int32_t fuzzyPrefixLength;
  //TODO: Locale locale = Locale.getDefault();

  // the default date resolution
  CL_NS(document)::DateTools::Resolution dateResolution;
  // maps field names to date resolutions
  typedef CL_NS(util)::CLHashMap<const TCHAR*,
              CL_NS(document)::DateTools::Resolution,
              CL_NS(util)::Compare::TChar,
              CL_NS(util)::Equals::TChar,
              CL_NS(util)::Deletor::Dummy,
              CL_NS(util)::Deletor::DummyInt32
              > FieldToDateResolutionType;
  FieldToDateResolutionType* fieldToDateResolution;

public:
  /** Constructs a query parser.
  *  @param f  the default field for query terms.
  *  @param a   used to find terms in the query text.
  */
  QueryParser(const TCHAR* f, CL_NS(analysis)::Analyzer* a);
  virtual ~QueryParser();
  void _deleteTokens();

  /** For backward compatibility */
  static CL_NS(search)::Query* parse(const TCHAR* q, const TCHAR* f, CL_NS(analysis)::Analyzer* a);

  /** Parses a query string, returning a {@link org.apache.lucene.search.Query}.
  *  @param query  the query string to be parsed.
  *  @throws ParseException if the parsing fails
  */
  CL_NS(search)::Query* parse(const TCHAR* _query);

  /**
  * @return Returns the analyzer.
  */
  CL_NS(analysis)::Analyzer* getAnalyzer() const;

  /**
  * @return Returns the field.
  */
  const TCHAR* getField() const;

  /**
  * Get the minimal similarity for fuzzy queries.
  */
  float_t getFuzzyMinSim() const;

  /**
  * Set the minimum similarity for fuzzy queries.
  * Default is 0.5f.
  */
  void setFuzzyMinSim(const float_t _fuzzyMinSim);

  /**
  * Get the prefix length for fuzzy queries.
  * @return Returns the fuzzyPrefixLength.
  */
  int32_t getFuzzyPrefixLength() const;

  /**
  * Set the prefix length for fuzzy queries. Default is 0.
  * @param fuzzyPrefixLength The fuzzyPrefixLength to set.
  */
  void setFuzzyPrefixLength(const int32_t _fuzzyPrefixLength);

  /**
  * Sets the default slop for phrases.  If zero, then exact phrase matches
  * are required.  Default value is zero.
  */
  void setPhraseSlop(const int32_t _phraseSlop);

  /**
  * Gets the default slop for phrases.
  */
  int32_t getPhraseSlop() const;

  /**
  * Set to <code>true</code> to allow leading wildcard characters.
  * <p>
  * When set, <code>*</code> or <code>?</code> are allowed as
  * the first character of a PrefixQuery and WildcardQuery.
  * Note that this can produce very slow
  * queries on big indexes.
  * <p>
  * Default: false.
  */
  void setAllowLeadingWildcard(const bool _allowLeadingWildcard);

  /**
  * @see #setAllowLeadingWildcard(boolean)
  */
  bool getAllowLeadingWildcard() const;

  /**
  * Set to <code>true</code> to enable position increments in result query.
  * <p>
  * When set, result phrase and multi-phrase queries will
  * be aware of position increments.
  * Useful when e.g. a StopFilter increases the position increment of
  * the token that follows an omitted token.
  * <p>
  * Default: false.
  */
  void setEnablePositionIncrements(const bool _enable);

  /**
  * @see #setEnablePositionIncrements(boolean)
  */
  bool getEnablePositionIncrements() const;

  /**
  * Sets the boolean operator of the QueryParser.
  * In default mode (<code>OR_OPERATOR</code>) terms without any modifiers
  * are considered optional: for example <code>capital of Hungary</code> is equal to
  * <code>capital OR of OR Hungary</code>.<br/>
  * In <code>AND_OPERATOR</code> mode terms are considered to be in conjuction: the
  * above mentioned query is parsed as <code>capital AND of AND Hungary</code>
  */
  void setDefaultOperator(Operator _op);

  /**
  * Gets implicit operator setting, which will be either AND_OPERATOR
  * or OR_OPERATOR.
  */
  Operator getDefaultOperator() const;

  /**
  * Whether terms of wildcard, prefix, fuzzy and range queries are to be automatically
  * lower-cased or not.  Default is <code>true</code>.
  */
  void setLowercaseExpandedTerms(const bool _lowercaseExpandedTerms);

  /**
  * @see #setLowercaseExpandedTerms(boolean)
  */
  bool getLowercaseExpandedTerms() const;

  /**
  * By default QueryParser uses new ConstantScoreRangeQuery in preference to RangeQuery
  * for range queries. This implementation is generally preferable because it
  * a) Runs faster b) Does not have the scarcity of range terms unduly influence score
  * c) avoids any "TooManyBooleanClauses" exception.
  * However, if your application really needs to use the old-fashioned RangeQuery and the above
  * points are not required then set this option to <code>true</code>
  * Default is <code>false</code>.
  */
  void setUseOldRangeQuery(const bool _useOldRangeQuery);

  /**
  * @see #setUseOldRangeQuery(boolean)
  */
  bool getUseOldRangeQuery() const;

  /**
  * Set locale used by date range parsing.
  *
  void setLocale(const Locale _locale) {
    locale = _locale;
  }

  
  * Returns current locale, allowing access by subclasses.
  *
  Locale getLocale() const {
    return locale;
  }
  */
 
  /** 
  * Sets the default date resolution used by RangeQueries for fields for which no
  * specific date resolutions has been set. Field specific resolutions can be set
  * with {@link #setDateResolution(String, DateTools.Resolution)}.
  *
  * @param dateResolution the default date resolution to set
  */
  void setDateResolution(const CL_NS(document)::DateTools::Resolution _dateResolution);

  /**
  * Sets the date resolution used by RangeQueries for a specific field.
  *
  * @param fieldName field for which the date resolution is to be set
  * @param dateResolution date resolution to set
  */
  void setDateResolution(const TCHAR* fieldName, const CL_NS(document)::DateTools::Resolution _dateResolution);

  /**
  * Returns the date resolution that is used by RangeQueries for the given field.
  * Returns null (NO_RESOLUTION), if no default or field specific date resolution has been set
  * for the given field.
  *
  */
  CL_NS(document)::DateTools::Resolution getDateResolution(const TCHAR* fieldName) const;

protected:
  void addClause(std::vector<CL_NS(search)::BooleanClause*>& clauses, int32_t conj, int32_t mods, CL_NS(search)::Query* q);

  /**
  * @exception ParseException throw in overridden method to disallow
  */
  virtual CL_NS(search)::Query* getFieldQuery(const TCHAR* _field, TCHAR* queryText);

  /**
  * Base implementation delegates to {@link #getFieldQuery(String,String)}.
  * This method may be overridden, for example, to return
  * a SpanNearQuery instead of a PhraseQuery.
  *
  * @exception ParseException throw in overridden method to disallow
  */
  virtual CL_NS(search)::Query* getFieldQuery(const TCHAR* _field, TCHAR* queryText, const int32_t slop);

  /**
  * @exception ParseException throw in overridden method to disallow
  */
  virtual CL_NS(search)::Query* getRangeQuery(const TCHAR* field, TCHAR* part1, TCHAR* part2, const bool inclusive);

  /**
  * Factory method for generating query, given a set of clauses.
  * By default creates a boolean query composed of clauses passed in.
  *
  * Can be overridden by extending classes, to modify query being
  * returned.
  *
  * @param clauses Vector that contains {@link BooleanClause} instances
  *    to join.
  * @param disableCoord true if coord scoring should be disabled.
  *
  * @return Resulting {@link Query} object.
  * @exception ParseException throw in overridden method to disallow
  */
  CL_NS(search)::Query* getBooleanQuery(std::vector<CL_NS(search)::BooleanClause*>& clauses, bool disableCoord = false);

  /**
  * Factory method for generating a query. Called when parser
  * parses an input term token that contains one or more wildcard
  * characters (? and *), but is not a prefix term token (one
  * that has just a single * character at the end)
  *<p>
  * Depending on settings, prefix term may be lower-cased
  * automatically. It will not go through the default Analyzer,
  * however, since normal Analyzers are unlikely to work properly
  * with wildcard templates.
  *<p>
  * Can be overridden by extending classes, to provide custom handling for
  * wildcard queries, which may be necessary due to missing analyzer calls.
  *
  * @param field Name of the field query will use.
  * @param termStr Term token that contains one or more wild card
  *   characters (? or *), but is not simple prefix term
  *
  * @return Resulting {@link Query} built for the term
  * @exception ParseException throw in overridden method to disallow
  */
  virtual CL_NS(search)::Query* getWildcardQuery(const TCHAR* _field, TCHAR* termStr);

  /**
  * Factory method for generating a query (similar to
  * {@link #getWildcardQuery}). Called when parser parses an input term
  * token that uses prefix notation; that is, contains a single '*' wildcard
  * character as its last character. Since this is a special case
  * of generic wildcard term, and such a query can be optimized easily,
  * this usually results in a different query object.
  *<p>
  * Depending on settings, a prefix term may be lower-cased
  * automatically. It will not go through the default Analyzer,
  * however, since normal Analyzers are unlikely to work properly
  * with wildcard templates.
  *<p>
  * Can be overridden by extending classes, to provide custom handling for
  * wild card queries, which may be necessary due to missing analyzer calls.
  *
  * @param field Name of the field query will use.
  * @param termStr Term token to use for building term for the query
  *    (<b>without</b> trailing '*' character!)
  *
  * @return Resulting {@link Query} built for the term
  * @exception ParseException throw in overridden method to disallow
  */
  virtual CL_NS(search)::Query* getPrefixQuery(const TCHAR* _field, TCHAR* _termStr);

  /**
  * Factory method for generating a query (similar to
  * {@link #getWildcardQuery}). Called when parser parses
  * an input term token that has the fuzzy suffix (~) appended.
  *
  * @param field Name of the field query will use.
  * @param termStr Term token to use for building term for the query
  *
  * @return Resulting {@link Query} built for the term
  * @exception ParseException throw in overridden method to disallow
  */
  virtual CL_NS(search)::Query* getFuzzyQuery(const TCHAR* _field, TCHAR* termStr, const float_t minSimilarity);

private:
  /**
  * Returns a String where the escape char has been
  * removed, or kept only once if there was a double escape.
  *
  * Supports escaped unicode characters, e. g. translates
  * <code>A</code> to <code>A</code>.
  *
  * @memory caller is responsible to free the returned string
  *
  */
  TCHAR* discardEscapeChar(TCHAR* input, TCHAR* output=NULL);

  /** Returns the numeric value of the hexadecimal character */
  static int32_t hexToInt(TCHAR c);

  struct JJCalls;

public:
  /**
  * Returns a String where those characters that QueryParser
  * expects to be escaped are escaped by a preceding <code>\</code>.
  *
  * @memory caller is responsible to free the returned string
  */
  static TCHAR* escape(const TCHAR* s);

  // *   Query  ::= ( Clause )*
  // *   Clause ::= ["+", "-"] [<TERM> ":"] ( <TERM> | "(" Query ")" )
  int32_t Conjunction();

  int32_t Modifiers();

  // This makes sure that there is no garbage after the query string
  CL_NS(search)::Query* TopLevelQuery(TCHAR* _field);

  CL_NS(search)::Query* fQuery(TCHAR* _field);

  CL_NS(search)::Query* fClause(TCHAR* _field);

public:
  CL_NS(search)::Query* fTerm(const TCHAR* _field);

private:
  bool jj_2_1(const int32_t xla);
  bool jj_3R_2();
  bool jj_3_1();
  bool jj_3R_3();

public:
  QueryParserTokenManager* token_source;
  QueryToken *token, *jj_nt;
private:
  QueryToken *_firstToken;
  int32_t jj_ntk;
  QueryToken *jj_scanpos, *jj_lastpos;
  int32_t jj_la;
public:
  bool lookingAhead;
private:
  bool jj_semLA;
  int32_t jj_gen;
  int32_t jj_la1[23];
  static const uint32_t jj_la1_0[];
  static const uint32_t jj_la1_1[];
  JJCalls* jj_2_rtns;
  bool jj_rescan;
  int32_t jj_gc;

public:
  QueryParser(CharStream* stream);
  void ReInit(CharStream* stream);
  QueryParser(QueryParserTokenManager* tm);
  void ReInit(QueryParserTokenManager* tm);

private:
  void _init(CharStream* stream);
  QueryToken* jj_consume_token(const int32_t kind);
  bool jj_scan_token(const int32_t kind);

public:
  QueryToken* getNextToken();
  QueryToken* getToken(int32_t index);

private:
  int32_t f_jj_ntk();

  CL_NS(util)::CLVector<  CL_NS(util)::ValueArray<int32_t>*,
              CL_NS(util)::Deletor::Object< CL_NS(util)::ValueArray<int32_t> >
             >* jj_expentries;
  CL_NS(util)::ValueArray<int32_t>* jj_expentry;
  int32_t jj_kind;
  int32_t jj_lasttokens[100];
  int32_t jj_endpos;

  void jj_add_error_token(const int32_t kind, int32_t pos);

public:
  void generateParseException();

  //void enable_tracing() {}
  //void disable_tracing() {}

private:
  void jj_rescan_token();
  void jj_save(const int32_t index, int32_t xla);

  TCHAR* getParseExceptionMessage(QueryToken* currentToken,
    CL_NS(util)::CLVector<  CL_NS(util)::ValueArray<int32_t>*,
    CL_NS(util)::Deletor::Object< CL_NS(util)::ValueArray<int32_t> > >* expectedTokenSequences,
    const TCHAR* tokenImage[]);
};
CL_NS_END
#endif
