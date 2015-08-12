/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * $Id: MatchXPath.hpp 932887 2010-04-11 13:04:59Z borisk $
 */

#if !defined(MATCHC_INCLUDE_GUARD_MATCHXPATH_HPP)
#define MATCHC_INCLUDE_GUARD_MATCHXPATH_HPP


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <match/util/QName.hpp>
#include <match/util/RefVectorOf.hpp>
#include <match/util/ValueVectorOf.hpp>
#include <match/validators/schema/NamespaceScope.hpp>
#include <match/internal/XSerializable.hpp>

MATCH_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Forward Declarations
// ---------------------------------------------------------------------------
class XMLStringPool;

class VALIDATORS_EXPORT MatchNodeTest : public XSerializable, public XMemory
{
public:
    // -----------------------------------------------------------------------
    //  Constants
    // -----------------------------------------------------------------------
    enum NodeType {
        NodeType_QNAME = 1,
        NodeType_WILDCARD = 2,
        NodeType_NODE = 3,
        NodeType_NAMESPACE= 4,
        NodeType_UNKNOWN
    };

    // -----------------------------------------------------------------------
    //  Constructors/Destructor
    // -----------------------------------------------------------------------
    MatchNodeTest(const short type,
                   MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
    MatchNodeTest(const QName* const qName);
    MatchNodeTest(const XMLCh* const prefix, const unsigned int uriId,
                   MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
    MatchNodeTest(const MatchNodeTest& other);
    ~MatchNodeTest() { delete fName; }

    // -----------------------------------------------------------------------
    //  Operators
    // -----------------------------------------------------------------------
    MatchNodeTest& operator= (const MatchNodeTest& other);
    bool operator== (const MatchNodeTest& other) const;
    bool operator!= (const MatchNodeTest& other) const;

	// -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    short getType() const { return fType; }
    QName* getName() const { return fName; }

    /***
     * Support for Serialization/De-serialization
     ***/
    DECL_XSERIALIZABLE(MatchNodeTest)

    MatchNodeTest(MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

private:
    // -----------------------------------------------------------------------
    //  Data members
    // -----------------------------------------------------------------------
    short  fType;
    QName* fName;
};


/**
  * A location path step comprised of an axis and node test.
  */
class VALIDATORS_EXPORT MatchStep : public XSerializable, public XMemory
{
public:
    // -----------------------------------------------------------------------
    //  Constants
    // -----------------------------------------------------------------------
    enum AxisType { // Axis type
        AxisType_CHILD = 1,
        AxisType_ATTRIBUTE = 2,
        AxisType_SELF = 3,
        AxisType_DESCENDANT = 4,
        AxisType_UNKNOWN
    };

    // -----------------------------------------------------------------------
    //  Constructors/Destructor
    // -----------------------------------------------------------------------
    MatchStep(const unsigned short axisType, MatchNodeTest* const nodeTest);
    MatchStep(const MatchStep& other);
    ~MatchStep() { delete fNodeTest; }

    // -----------------------------------------------------------------------
    //  Operators
    // -----------------------------------------------------------------------
    MatchStep& operator= (const MatchStep& other);
    bool operator== (const MatchStep& other) const;
    bool operator!= (const MatchStep& other) const;

	// -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    unsigned short getAxisType() const { return fAxisType; }
    MatchNodeTest* getNodeTest() const { return fNodeTest; }

    /***
     * Support for Serialization/De-serialization
     ***/
    DECL_XSERIALIZABLE(MatchStep)

    MatchStep(MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

private:
    // -----------------------------------------------------------------------
    //  Data members
    // -----------------------------------------------------------------------
    unsigned short  fAxisType;
    MatchNodeTest* fNodeTest;
};


/**
  * A location path representation for an XPath expression.
  */
class VALIDATORS_EXPORT MatchLocationPath : public XSerializable, public XMemory
{
public:
    // -----------------------------------------------------------------------
    //  Constructors/Destructor
    // -----------------------------------------------------------------------
    MatchLocationPath(RefVectorOf<MatchStep>* const steps);
    ~MatchLocationPath() { delete fSteps; }

    // -----------------------------------------------------------------------
    //  Operators
    // -----------------------------------------------------------------------
    bool operator== (const MatchLocationPath& other) const;
    bool operator!= (const MatchLocationPath& other) const;

    // -----------------------------------------------------------------------
    //  Access methods
    // -----------------------------------------------------------------------
    XMLSize_t getStepSize() const;
    void addStep(MatchStep* const aStep);
    MatchStep* getStep(const XMLSize_t index) const;

    /***
     * Support for Serialization/De-serialization
     ***/
    DECL_XSERIALIZABLE(MatchLocationPath)

    MatchLocationPath(MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

private:
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    MatchLocationPath(const MatchLocationPath& other);
    MatchLocationPath& operator= (const MatchLocationPath& other);

    // -----------------------------------------------------------------------
    //  Data members
    // -----------------------------------------------------------------------
    RefVectorOf<MatchStep>* fSteps;
};


class VALIDATORS_EXPORT MatchXPath : public XSerializable, public XMemory
{
public:
    // -----------------------------------------------------------------------
    //  Constants
    // -----------------------------------------------------------------------
    /**
      * [28] ExprToken ::= '(' | ')' | '[' | ']' | '.' | '..' | '@' | ',' | '::'
      *                  | NameTest | NodeType | Operator | FunctionName
      *                  | AxisName | Literal | Number | VariableReference
      */
    enum {
        EXPRTOKEN_OPEN_PAREN                  =  0,
        EXPRTOKEN_CLOSE_PAREN                 =  1,
        EXPRTOKEN_OPEN_BRACKET                =  2,
        EXPRTOKEN_CLOSE_BRACKET               =  3,
        EXPRTOKEN_PERIOD                      =  4,
        EXPRTOKEN_DOUBLE_PERIOD               =  5,
        EXPRTOKEN_ATSIGN                      =  6,
        EXPRTOKEN_COMMA                       =  7,
        EXPRTOKEN_DOUBLE_COLON                =  8,
        EXPRTOKEN_NAMETEST_ANY                =  9,
        EXPRTOKEN_NAMETEST_NAMESPACE          = 10,
        EXPRTOKEN_NAMETEST_QNAME              = 11,
        EXPRTOKEN_NODETYPE_COMMENT            = 12,
        EXPRTOKEN_NODETYPE_TEXT               = 13,
        EXPRTOKEN_NODETYPE_PI                 = 14,
        EXPRTOKEN_NODETYPE_NODE               = 15,
        EXPRTOKEN_OPERATOR_AND                = 16,
        EXPRTOKEN_OPERATOR_OR                 = 17,
        EXPRTOKEN_OPERATOR_MOD                = 18,
        EXPRTOKEN_OPERATOR_DIV                = 19,
        EXPRTOKEN_OPERATOR_MULT               = 20,
        EXPRTOKEN_OPERATOR_SLASH              = 21,
        EXPRTOKEN_OPERATOR_DOUBLE_SLASH       = 22,
        EXPRTOKEN_OPERATOR_UNION              = 23,
        EXPRTOKEN_OPERATOR_PLUS               = 24,
        EXPRTOKEN_OPERATOR_MINUS              = 25,
        EXPRTOKEN_OPERATOR_EQUAL              = 26,
        EXPRTOKEN_OPERATOR_NOT_EQUAL          = 27,
        EXPRTOKEN_OPERATOR_LESS               = 28,
        EXPRTOKEN_OPERATOR_LESS_EQUAL         = 29,
        EXPRTOKEN_OPERATOR_GREATER            = 30,
        EXPRTOKEN_OPERATOR_GREATER_EQUAL      = 31,
        EXPRTOKEN_FUNCTION_NAME               = 32,
        EXPRTOKEN_AXISNAME_ANCESTOR           = 33,
        EXPRTOKEN_AXISNAME_ANCESTOR_OR_SELF   = 34,
        EXPRTOKEN_AXISNAME_ATTRIBUTE          = 35,
        EXPRTOKEN_AXISNAME_CHILD              = 36,
        EXPRTOKEN_AXISNAME_DESCENDANT         = 37,
        EXPRTOKEN_AXISNAME_DESCENDANT_OR_SELF = 38,
        EXPRTOKEN_AXISNAME_FOLLOWING          = 39,
        EXPRTOKEN_AXISNAME_FOLLOWING_SIBLING  = 40,
        EXPRTOKEN_AXISNAME_NAMESPACE          = 41,
        EXPRTOKEN_AXISNAME_PARENT             = 42,
        EXPRTOKEN_AXISNAME_PRECEDING          = 43,
        EXPRTOKEN_AXISNAME_PRECEDING_SIBLING  = 44,
        EXPRTOKEN_AXISNAME_SELF               = 45,
        EXPRTOKEN_LITERAL                     = 46,
        EXPRTOKEN_NUMBER                      = 47,
        EXPRTOKEN_VARIABLE_REFERENCE          = 48
    };

    // -----------------------------------------------------------------------
    //  Constructors/Destructor
    // -----------------------------------------------------------------------
    MatchXPath(const XMLCh* const xpathExpr,
                XMLStringPool* const stringPool,
                MatchNamespaceResolver* const scopeContext,
                const unsigned int emptyNamespaceId,
                const bool isSelector = false,
                MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
	~MatchXPath();

    // -----------------------------------------------------------------------
    //  Operators
    // -----------------------------------------------------------------------
    bool operator== (const MatchXPath& other) const;
    bool operator!= (const MatchXPath& other) const;

    // -----------------------------------------------------------------------
    //  Constructors/Destructor
    // -----------------------------------------------------------------------
    RefVectorOf<MatchLocationPath>* getLocationPaths() const;

    /***
     * Support for Serialization/De-serialization
     ***/
    DECL_XSERIALIZABLE(MatchXPath)

    MatchXPath(MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    XMLCh* getExpression();

private:
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    MatchXPath(const MatchXPath& other);
    MatchXPath& operator= (const MatchXPath& other);

    // -----------------------------------------------------------------------
    //  Helper methods
    // -----------------------------------------------------------------------
    void cleanUp();
    void checkForSelectedAttributes();
    void parseExpression(XMLStringPool* const stringPool,
                         MatchNamespaceResolver* const scopeContext);

    // -----------------------------------------------------------------------
    //  Data members
    // -----------------------------------------------------------------------
    unsigned int                     fEmptyNamespaceId;
    XMLCh*                           fExpression;
    RefVectorOf<MatchLocationPath>* fLocationPaths;
    MemoryManager*                   fMemoryManager;
};


class VALIDATORS_EXPORT XPathScanner : public XMemory
{
public:
    // -----------------------------------------------------------------------
    //  Constants
    // -----------------------------------------------------------------------
    enum {
        CHARTYPE_INVALID            =  0,   // invalid XML character
        CHARTYPE_OTHER              =  1,   // not special - one of "#%&;?\^`{}~" or DEL
        CHARTYPE_WHITESPACE         =  2,   // one of "\t\n\r " (0x09, 0x0A, 0x0D, 0x20)
        CHARTYPE_EXCLAMATION        =  3,   // '!' (0x21)
        CHARTYPE_QUOTE              =  4,   // '\"' or '\'' (0x22 and 0x27)
        CHARTYPE_DOLLAR             =  5,   // '$' (0x24)
        CHARTYPE_OPEN_PAREN         =  6,   // '(' (0x28)
        CHARTYPE_CLOSE_PAREN        =  7,   // ')' (0x29)
        CHARTYPE_STAR               =  8,   // '*' (0x2A)
        CHARTYPE_PLUS               =  9,   // '+' (0x2B)
        CHARTYPE_COMMA              = 10,   // ',' (0x2C)
        CHARTYPE_MINUS              = 11,   // '-' (0x2D)
        CHARTYPE_PERIOD             = 12,   // '.' (0x2E)
        CHARTYPE_SLASH              = 13,   // '/' (0x2F)
        CHARTYPE_DIGIT              = 14,   // '0'-'9' (0x30 to 0x39)
        CHARTYPE_COLON              = 15,   // ':' (0x3A)
        CHARTYPE_LESS               = 16,   // '<' (0x3C)
        CHARTYPE_EQUAL              = 17,   // '=' (0x3D)
        CHARTYPE_GREATER            = 18,   // '>' (0x3E)
        CHARTYPE_ATSIGN             = 19,   // '@' (0x40)
        CHARTYPE_LETTER             = 20,   // 'A'-'Z' or 'a'-'z' (0x41 to 0x5A and 0x61 to 0x7A)
        CHARTYPE_OPEN_BRACKET       = 21,   // '[' (0x5B)
        CHARTYPE_CLOSE_BRACKET      = 22,   // ']' (0x5D)
        CHARTYPE_UNDERSCORE         = 23,   // '_' (0x5F)
        CHARTYPE_UNION              = 24,   // '|' (0x7C)
        CHARTYPE_NONASCII           = 25   // Non-ASCII Unicode codepoint (>= 0x80)
    };

    // -----------------------------------------------------------------------
    //  Constructors/Destructor
    // -----------------------------------------------------------------------
    XPathScanner(XMLStringPool* const stringPool);
    virtual ~XPathScanner() {}

    // -----------------------------------------------------------------------
    //  Scan methods
    // -----------------------------------------------------------------------
    bool scanExpression(const XMLCh* const data, XMLSize_t currentOffset,
                        const XMLSize_t endOffset, ValueVectorOf<int>* const tokens);

protected:
    // -----------------------------------------------------------------------
    //  Helper methods
    // -----------------------------------------------------------------------
    /**
      * This method adds the specified token to the token list. By default,
      * this method allows all tokens. However, subclasses can can override
      * this method in order to disallow certain tokens from being used in the
      * scanned XPath expression. This is a convenient way of allowing only
      * a subset of XPath.
      */
    virtual void addToken(ValueVectorOf<int>* const tokens, const int aToken);

private:
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    XPathScanner(const XPathScanner& other);
    XPathScanner& operator= (const XPathScanner& other);

    // -----------------------------------------------------------------------
    //  Helper methods
    // -----------------------------------------------------------------------
    void init();

    // -----------------------------------------------------------------------
    //  Scan methods
    // -----------------------------------------------------------------------
    XMLSize_t scanNCName(const XMLCh* const data, const XMLSize_t endOffset,
                   XMLSize_t currentOffset);
    XMLSize_t scanNumber(const XMLCh* const data, const XMLSize_t endOffset,
                   XMLSize_t currentOffset, ValueVectorOf<int>* const tokens);

    // -----------------------------------------------------------------------
    //  Data members
    // -----------------------------------------------------------------------
    int fAndSymbol;
    int fOrSymbol;
    int fModSymbol;
    int fDivSymbol;
    int fCommentSymbol;
    int fTextSymbol;
    int fPISymbol;
    int fNodeSymbol;
    int fAncestorSymbol;
    int fAncestorOrSelfSymbol;
    int fAttributeSymbol;
    int fChildSymbol;
    int fDescendantSymbol;
    int fDescendantOrSelfSymbol;
    int fFollowingSymbol;
    int fFollowingSiblingSymbol;
    int fNamespaceSymbol;
    int fParentSymbol;
    int fPrecedingSymbol;
    int fPrecedingSiblingSymbol;
    int fSelfSymbol;
    XMLStringPool* fStringPool;

    static const XMLByte fASCIICharMap[128];
};


class VALIDATORS_EXPORT XPathScannerForSchema: public XPathScanner
{
public:
    // -----------------------------------------------------------------------
    //  Constructors/Destructor
    // -----------------------------------------------------------------------
    XPathScannerForSchema(XMLStringPool* const stringPool);
    ~XPathScannerForSchema() {}

protected:
    // -----------------------------------------------------------------------
    //  Helper methods
    // -----------------------------------------------------------------------
    void addToken(ValueVectorOf<int>* const tokens, const int aToken);

private:
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    XPathScannerForSchema(const XPathScannerForSchema& other);
    XPathScannerForSchema& operator= (const XPathScannerForSchema& other);
};

// ---------------------------------------------------------------------------
//  MatchLocationPath: Access methods
// ---------------------------------------------------------------------------
inline XMLSize_t MatchLocationPath::getStepSize() const {

    if (fSteps)
        return fSteps->size();

    return 0;
}

inline void MatchLocationPath::addStep(MatchStep* const aStep) {

    fSteps->addElement(aStep);
}

inline MatchStep* MatchLocationPath::getStep(const XMLSize_t index) const {

    if (fSteps)
        return fSteps->elementAt(index);

    return 0;
}

// ---------------------------------------------------------------------------
//  MatchScanner: Helper methods
// ---------------------------------------------------------------------------
inline void XPathScanner::addToken(ValueVectorOf<int>* const tokens,
                                   const int aToken) {
    tokens->addElement(aToken);
}


// ---------------------------------------------------------------------------
//  MatchXPath: Getter methods
// ---------------------------------------------------------------------------
inline RefVectorOf<MatchLocationPath>* MatchXPath::getLocationPaths() const {

    return fLocationPaths;
}

inline XMLCh* MatchXPath::getExpression() {
    return fExpression;
}

MATCH_CPP_NAMESPACE_END

#endif

/**
  * End of file MatchPath.hpp
  */

