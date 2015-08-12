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
 * $Id: MatchXPath.cpp 903997 2010-01-28 08:28:06Z borisk $
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <match/validators/schema/identity/MatchXPath.hpp>
#include <match/validators/schema/identity/XPathSymbols.hpp>
#include <match/validators/schema/identity/XPathException.hpp>
#include <match/validators/schema/NamespaceScope.hpp>
#include <match/util/StringPool.hpp>
#include <match/util/Janitor.hpp>
#include <match/framework/XMLBuffer.hpp>
#include <match/internal/XMLReader.hpp>
#include <match/util/RuntimeException.hpp>
#include <match/util/OutOfMemoryException.hpp>

#include <match/internal/XTemplateSerializer.hpp>

MATCH_CPP_NAMESPACE_BEGIN


// ---------------------------------------------------------------------------
//  Static data
// ---------------------------------------------------------------------------
const XMLByte XPathScanner::fASCIICharMap[128] =
{
    0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  2,  0,  0,  2,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    2,  3,  4,  1,  5,  1,  1,  4,  6,  7,  8,  9, 10, 11, 12, 13,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 15,  1, 16, 17, 18,  1,
    19, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 21,  1, 22,  1, 23,
    1, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,  1, 24,  1,  1,  1
};


// ---------------------------------------------------------------------------
//  MatchNodeTest: Constructors and Destructor
// ---------------------------------------------------------------------------
MatchNodeTest::MatchNodeTest(const short aType,
                               MemoryManager* const manager)
    : fType(aType)
    , fName(new (manager) QName(manager))
{
}

MatchNodeTest::MatchNodeTest(const QName* const qName)
    : fType(NodeType_QNAME)
    , fName(new (qName->getMemoryManager()) QName(*qName))
{
}

MatchNodeTest::MatchNodeTest(const XMLCh* const prefix,
                               const unsigned int uriId,
                               MemoryManager* const manager)
    : fType(NodeType_NAMESPACE)
    , fName(new (manager) QName(manager))
{
    fName->setURI(uriId);
    fName->setPrefix(prefix);
}

MatchNodeTest::MatchNodeTest(const MatchNodeTest& other)
    : XSerializable(other)
    , XMemory(other)
    , fType(other.fType)
    , fName(new ((other.fName)->getMemoryManager()) QName(*other.fName))
{
}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_TOCREATE(MatchNodeTest)

void MatchNodeTest::serialize(XSerializeEngine& serEng)
{

    if (serEng.isStoring())
    {
        serEng<<fType;
        serEng<<fName;
    }
    else
    {
        serEng>>fType;
        serEng>>fName;
    }
}

MatchNodeTest::MatchNodeTest(MemoryManager* const)
:fType(NodeType_UNKNOWN)
,fName(0)
{
}

// ---------------------------------------------------------------------------
//  MatchNodeTest: Operators
// ---------------------------------------------------------------------------
MatchNodeTest& MatchNodeTest::operator=(const MatchNodeTest& other)
{
    if (this == &other)
        return *this;

    fType = other.fType;
    fName->setValues(*(other.fName));
    return *this;
}

bool MatchNodeTest::operator ==(const MatchNodeTest& other) const {

    if (this == &other)
        return true;

    if (fType != other.fType)
        return false;

    return (*fName == *(other.fName));
}


bool MatchNodeTest::operator !=(const MatchNodeTest& other) const {

    return !operator==(other);
}

// ---------------------------------------------------------------------------
//  MatchStep: Constructors and Destructor
// ---------------------------------------------------------------------------
MatchStep::MatchStep(const unsigned short axisType, MatchNodeTest* const nodeTest)
    : fAxisType(axisType)
    , fNodeTest(nodeTest)
{
}

MatchStep::MatchStep(const MatchStep& other)
    : XSerializable(other)
    , XMemory(other)
    , fAxisType(other.fAxisType)
    , fNodeTest(0)
{
    fNodeTest = new (other.fNodeTest->getName()->getMemoryManager()) MatchNodeTest(*(other.fNodeTest));
}


// ---------------------------------------------------------------------------
//  MatchStep: Operators
// ---------------------------------------------------------------------------
MatchStep& MatchStep::operator=(const MatchStep& other)
{
    if (this == &other)
        return *this;

    fAxisType = other.fAxisType;
    *fNodeTest = *(other.fNodeTest);
    return *this;
}

bool MatchStep::operator==(const MatchStep& other) const {

    if (this == &other)
        return true;

    if (fAxisType != other.fAxisType)
        return false;

    if (fAxisType == MatchStep::AxisType_CHILD ||
        fAxisType == MatchStep::AxisType_ATTRIBUTE) {
        return (*fNodeTest == *(other.fNodeTest));
    }

    return true;
}

bool MatchStep::operator!=(const MatchStep& other) const {

    return !operator==(other);
}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_TOCREATE(MatchStep)

void MatchStep::serialize(XSerializeEngine& serEng)
{
    if (serEng.isStoring())
    {
        serEng<<(int)fAxisType;
        serEng<<fNodeTest;
    }
    else
    {
        int i;
        serEng>>i;
        fAxisType = (unsigned short) i;

        serEng>>fNodeTest;
    }
}

MatchStep::MatchStep(MemoryManager* const)
:fAxisType(AxisType_UNKNOWN)
,fNodeTest(0)
{
}

// ---------------------------------------------------------------------------
//  MatchLocationPath: Constructors and Destructor
// ---------------------------------------------------------------------------
MatchLocationPath::MatchLocationPath(RefVectorOf<MatchStep>* const steps)
    : fSteps(steps)
{
}

// ---------------------------------------------------------------------------
//  MatchLocationPath: Operators
// ---------------------------------------------------------------------------
bool MatchLocationPath::operator==(const MatchLocationPath& other) const {

    XMLSize_t stepsSize = fSteps->size();

    if (stepsSize != other.fSteps->size())
        return false;

    for (XMLSize_t i=0; i < stepsSize; i++) {
        if (*(fSteps->elementAt(i)) != *(other.fSteps->elementAt(i)))
            return false;
    }

    return true;
}

bool MatchLocationPath::operator!=(const MatchLocationPath& other) const {

    return !operator==(other);
}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_TOCREATE(MatchLocationPath)

void MatchLocationPath::serialize(XSerializeEngine& serEng)
{
    if (serEng.isStoring())
    {
        /***
         * Serialize RefVectorOf<MatchStep>* fSteps;
         ***/
        XTemplateSerializer::storeObject(fSteps, serEng);
    }
    else
    {
        /***
         * Deserialize RefVectorOf<MatchStep>* fSteps;
         ***/
        XTemplateSerializer::loadObject(&fSteps, 8, true, serEng);
    }
}

MatchLocationPath::MatchLocationPath(MemoryManager* const)
:fSteps(0)
{
}

typedef JanitorMemFunCall<MatchXPath>  CleanupType;

// ---------------------------------------------------------------------------
//  MatchPath: Constructors and Destructor
// ---------------------------------------------------------------------------
MatchXPath::MatchXPath(const XMLCh* const xpathExpr,
                         XMLStringPool* const stringPool,
                         MatchNamespaceResolver* const scopeContext,
                         const unsigned int emptyNamespaceId,
                         const bool isSelector,
                         MemoryManager* const manager)
    : fEmptyNamespaceId(emptyNamespaceId)
    , fExpression(0)
    , fLocationPaths(0)
    , fMemoryManager(manager)
{
    CleanupType cleanup(this, &MatchXPath::cleanUp);

    try
    {
        fExpression = XMLString::replicate(xpathExpr, fMemoryManager);
        parseExpression(stringPool, scopeContext);

        if (isSelector) {
            checkForSelectedAttributes();
        }
    }
    catch(const OutOfMemoryException&)
    {
        cleanup.release();

        throw;
    }

    cleanup.release();
}

MatchXPath::~MatchXPath() {
    cleanUp();
}


// ---------------------------------------------------------------------------
//  MatchXPath: Operators
// ---------------------------------------------------------------------------
bool MatchXPath::operator==(const MatchXPath& other) const {

    XMLSize_t locPathSize = fLocationPaths->size();

    if (locPathSize != other.fLocationPaths->size())
        return false;

    for (XMLSize_t i=0; i < locPathSize; i++) {
        if (*(fLocationPaths->elementAt(i)) != *(other.fLocationPaths->elementAt(i)))
            return false;
    }

    return true;
}

bool MatchXPath::operator!=(const MatchXPath& other) const {

    return !operator==(other);
}

// ---------------------------------------------------------------------------
//  MatchPath: Helper methods
// ---------------------------------------------------------------------------
void MatchXPath::cleanUp() {

    fMemoryManager->deallocate(fExpression);//delete [] fExpression;
    delete fLocationPaths;
}

void MatchXPath::checkForSelectedAttributes() {

    // verify that an attribute is not selected
    XMLSize_t locSize = (fLocationPaths) ? fLocationPaths->size() : 0;

    for (XMLSize_t i = 0; i < locSize; i++) {

        MatchLocationPath* locPath = fLocationPaths->elementAt(i);
        XMLSize_t stepSize = locPath->getStepSize();

        if (stepSize) {
            if (locPath->getStep(stepSize - 1)->getAxisType() == MatchStep::AxisType_ATTRIBUTE) {
                ThrowXMLwithMemMgr(XPathException, XMLExcepts::XPath_NoAttrSelector, fMemoryManager);
            }
		}
    }
}

void MatchXPath::parseExpression(XMLStringPool* const stringPool,
                                  MatchNamespaceResolver* const scopeContext) {

    XMLSize_t length = XMLString::stringLen(fExpression);

    if (!length) {
        return;
    }

    ValueVectorOf<int>                tokens(16, fMemoryManager);
    XPathScannerForSchema             scanner(stringPool);
    if(!scanner.scanExpression(fExpression, 0, length, &tokens))
        ThrowXMLwithMemMgr(XPathException, XMLExcepts::XPath_TokenNotSupported, fMemoryManager);

    bool                              firstTokenOfLocationPath=true;
    XMLSize_t                         tokenCount = tokens.size();
    RefVectorOf<MatchStep>*          stepsVector = new (fMemoryManager) RefVectorOf<MatchStep>(16, true, fMemoryManager);
    Janitor<RefVectorOf<MatchStep> > janSteps(stepsVector);

    if (tokenCount) {
        fLocationPaths = new (fMemoryManager) RefVectorOf<MatchLocationPath>(8, true, fMemoryManager);
    }

    for (XMLSize_t i = 0; i < tokenCount; i++) {

        int  aToken = tokens.elementAt(i);
        bool isNamespace=false;

        switch (aToken) {
        case  MatchXPath::EXPRTOKEN_OPERATOR_UNION:
            {
                if (i == 0) {
                    ThrowXMLwithMemMgr(XPathException, XMLExcepts::XPath_NoUnionAtStart, fMemoryManager);
                }

                XMLSize_t stepsSize = stepsVector->size();

                if (stepsSize == 0) {
                    ThrowXMLwithMemMgr(XPathException, XMLExcepts::XPath_NoMultipleUnion, fMemoryManager);
                }

                if(stepsVector->elementAt(0)->getAxisType()!=MatchStep::AxisType_SELF)
                {
                    // prepend ./
                    MatchNodeTest* nodeTest = new (fMemoryManager) MatchNodeTest(MatchNodeTest::NodeType_NODE, fMemoryManager);
                    MatchStep* step = new (fMemoryManager) MatchStep(MatchStep::AxisType_SELF, nodeTest);
                    stepsVector->insertElementAt(step, 0);
                }
                MatchLocationPath* newPath = new (fMemoryManager) MatchLocationPath(stepsVector);
                janSteps.orphan();
                bool bFound=false;
                for(XMLSize_t i=0;i<fLocationPaths->size();i++)
                    if((*(fLocationPaths->elementAt(i)))==(*newPath))
                    {
                        bFound=true;
                        break;
                    }
                if(bFound)
                    delete newPath;
                else
                    fLocationPaths->addElement(newPath);
                stepsVector = new (fMemoryManager) RefVectorOf<MatchStep>(16, true, fMemoryManager);
                janSteps.reset(stepsVector);
                firstTokenOfLocationPath = true;
            }
            break;
        case MatchXPath::EXPRTOKEN_AXISNAME_ATTRIBUTE:
            {
                // consume "::" token and drop through
                i++;
            }
        case MatchXPath::EXPRTOKEN_ATSIGN:
            {
                // consume QName token
                if (i == tokenCount - 1) {
                    ThrowXMLwithMemMgr(XPathException, XMLExcepts::XPath_MissingAttr, fMemoryManager);
                }

                aToken = tokens.elementAt(++i);

                if (aToken != MatchXPath::EXPRTOKEN_NAMETEST_QNAME
                    && aToken!= MatchXPath::EXPRTOKEN_NAMETEST_ANY
                    && aToken!= MatchXPath::EXPRTOKEN_NAMETEST_NAMESPACE) {
                        ThrowXMLwithMemMgr(XPathException, XMLExcepts::XPath_ExpectedToken1, fMemoryManager);
                }

                bool isNamespaceAtt=false;

                switch (aToken) {

                case MatchXPath::EXPRTOKEN_NAMETEST_ANY:
                    {
                        MatchNodeTest* nodeTest = new (fMemoryManager) MatchNodeTest(MatchNodeTest::NodeType_WILDCARD, fMemoryManager);
                        MatchStep* step = new (fMemoryManager) MatchStep(MatchStep::AxisType_ATTRIBUTE, nodeTest);
                        stepsVector->addElement(step);
                        break;
                    }
                case MatchXPath::EXPRTOKEN_NAMETEST_NAMESPACE:
                    {
                        isNamespaceAtt = true;
                    }
                case MatchXPath::EXPRTOKEN_NAMETEST_QNAME:
                    {
                        aToken = tokens.elementAt(++i);

                        const XMLCh* prefix = XMLUni::fgZeroLenString;
                        unsigned int uri = fEmptyNamespaceId;

                        if (scopeContext && aToken != -1) {

                            prefix = stringPool->getValueForId(aToken);
                            uri = scopeContext->getNamespaceForPrefix(prefix);
                        }

                        if (aToken != -1 && scopeContext && uri == fEmptyNamespaceId) {
                            ThrowXMLwithMemMgr1(XPathException, XMLExcepts::XPath_PrefixNoURI, prefix, fMemoryManager);
                        }

                        if (isNamespaceAtt) {

                            // build step
                            MatchNodeTest* nodeTest = new (fMemoryManager) MatchNodeTest(prefix, uri, fMemoryManager);
                            MatchStep* step = new (fMemoryManager) MatchStep(MatchStep::AxisType_ATTRIBUTE, nodeTest);
                            stepsVector->addElement(step);
                            break;
                        }

                        aToken = tokens.elementAt(++i);

                        const XMLCh* localPart = stringPool->getValueForId(aToken);
                        QName aQName(prefix, localPart, uri, fMemoryManager);

                        // build step
                        MatchNodeTest* nodeTest = new (fMemoryManager) MatchNodeTest(&aQName);
                        MatchStep* step = new (fMemoryManager) MatchStep(MatchStep::AxisType_ATTRIBUTE, nodeTest);
                        stepsVector->addElement(step);
                        break;
                    }
				}

                firstTokenOfLocationPath=false;
                break;
            }
        case MatchXPath::EXPRTOKEN_DOUBLE_COLON:
            {
                // should never have a bare double colon
                ThrowXMLwithMemMgr(XPathException, XMLExcepts::XPath_NoDoubleColon, fMemoryManager);
            }
        case MatchXPath::EXPRTOKEN_AXISNAME_CHILD:
            {
                // consume "::" token and drop through
                i++;

                if (i == tokenCount - 1) {
                    ThrowXMLwithMemMgr(XPathException, XMLExcepts::XPath_ExpectedStep1, fMemoryManager);
                }

                firstTokenOfLocationPath=false;
                break;
            }
        case MatchXPath::EXPRTOKEN_NAMETEST_ANY:
            {
                MatchNodeTest* nodeTest = new (fMemoryManager) MatchNodeTest(MatchNodeTest::NodeType_WILDCARD, fMemoryManager);
                MatchStep* step = new (fMemoryManager) MatchStep(MatchStep::AxisType_CHILD, nodeTest);
                stepsVector->addElement(step);
                firstTokenOfLocationPath = false;
                break;
            }
        case MatchXPath::EXPRTOKEN_NAMETEST_NAMESPACE:
            {
                isNamespace=true;
            }
        case MatchXPath::EXPRTOKEN_NAMETEST_QNAME:
            {
                // consume QName token
                aToken = tokens.elementAt(++i);

                const XMLCh* prefix = XMLUni::fgZeroLenString;
                unsigned int uri = fEmptyNamespaceId;

                if (scopeContext && aToken != -1) {

                    prefix = stringPool->getValueForId(aToken);
                    uri = scopeContext->getNamespaceForPrefix(prefix);
                }

                if (aToken != -1 && scopeContext && uri == fEmptyNamespaceId) {
                    ThrowXMLwithMemMgr1(XPathException, XMLExcepts::XPath_PrefixNoURI, prefix, fMemoryManager);
                }

                if (isNamespace) {

                    // build step
                    MatchNodeTest* nodeTest = new (fMemoryManager) MatchNodeTest(prefix, uri, fMemoryManager);
                    MatchStep* step = new (fMemoryManager) MatchStep(MatchStep::AxisType_CHILD, nodeTest);
                    stepsVector->addElement(step);
                    break;
                }

                aToken = tokens.elementAt(++i);
                const XMLCh* localPart = stringPool->getValueForId(aToken);
                QName aQName(prefix, localPart, uri, fMemoryManager);

                // build step
                MatchNodeTest* nodeTest = new (fMemoryManager) MatchNodeTest(&aQName);
                MatchStep* step = new (fMemoryManager) MatchStep(MatchStep::AxisType_CHILD, nodeTest);
                stepsVector->addElement(step);
                firstTokenOfLocationPath = false;
                break;
            }
        case MatchXPath::EXPRTOKEN_PERIOD:
            {
                // build step
                MatchNodeTest* nodeTest = new (fMemoryManager) MatchNodeTest(MatchNodeTest::NodeType_NODE, fMemoryManager);
                MatchStep* step = new (fMemoryManager) MatchStep(MatchStep::AxisType_SELF, nodeTest);
                stepsVector->addElement(step);

                if (firstTokenOfLocationPath && i+1 < tokenCount) {

                    aToken = tokens.elementAt(i+1);

                    if (aToken == MatchXPath::EXPRTOKEN_OPERATOR_DOUBLE_SLASH){

                        if (++i == tokenCount - 1) {
                            ThrowXMLwithMemMgr(XPathException, XMLExcepts::XPath_ExpectedStep2, fMemoryManager);
                        }

                        if (i+1 < tokenCount)	{

                            aToken = tokens.elementAt(i+1);

                            if (aToken == MatchXPath::EXPRTOKEN_OPERATOR_SLASH) {
                                ThrowXMLwithMemMgr(XPathException, XMLExcepts::XPath_NoForwardSlash, fMemoryManager);
                            }
                        }
                        // build step
                        nodeTest = new (fMemoryManager) MatchNodeTest(MatchNodeTest::NodeType_NODE, fMemoryManager);
                        step = new (fMemoryManager) MatchStep(MatchStep::AxisType_DESCENDANT, nodeTest);
                        stepsVector->addElement(step);
                    }
                }
                firstTokenOfLocationPath=false;
                break;
            }
        case MatchXPath::EXPRTOKEN_OPERATOR_DOUBLE_SLASH:
            {
                ThrowXMLwithMemMgr(XPathException, XMLExcepts::XPath_NoDoubleForwardSlash, fMemoryManager);
            }
        case MatchXPath::EXPRTOKEN_OPERATOR_SLASH:
            {
                if (i == 0) {
                    ThrowXMLwithMemMgr(XPathException, XMLExcepts::XPath_NoForwardSlashAtStart, fMemoryManager);
                }

                // keep on truckin'
                if (firstTokenOfLocationPath) {
                    ThrowXMLwithMemMgr(XPathException, XMLExcepts::XPath_NoSelectionOfRoot, fMemoryManager);
                }

                if (i == tokenCount - 1) {
                    ThrowXMLwithMemMgr(XPathException, XMLExcepts::XPath_ExpectedStep3, fMemoryManager);
                }

                aToken = tokens.elementAt(i+1);
                if(aToken == MatchXPath::EXPRTOKEN_OPERATOR_SLASH || aToken == MatchXPath::EXPRTOKEN_OPERATOR_DOUBLE_SLASH || aToken == MatchXPath::EXPRTOKEN_OPERATOR_UNION)
                    ThrowXMLwithMemMgr(XPathException, XMLExcepts::XPath_ExpectedStep3, fMemoryManager);

                firstTokenOfLocationPath=false;
                break;
            }
        default:
            firstTokenOfLocationPath=false;
        }
    }

    XMLSize_t stepsSize = stepsVector->size();

    if (stepsSize == 0) {
        if (!fLocationPaths || fLocationPaths->size() == 0) {
            ThrowXMLwithMemMgr(XPathException, XMLExcepts::XPath_EmptyExpr, fMemoryManager);
        }
        else {
            ThrowXMLwithMemMgr(XPathException, XMLExcepts::XPath_NoUnionAtEnd, fMemoryManager);
        }
    }

    if(stepsVector->elementAt(0)->getAxisType()!=MatchStep::AxisType_SELF)
    {
        // prepend ./
        MatchNodeTest* nodeTest = new (fMemoryManager) MatchNodeTest(MatchNodeTest::NodeType_NODE, fMemoryManager);
        MatchStep* step = new (fMemoryManager) MatchStep(MatchStep::AxisType_SELF, nodeTest);
        stepsVector->insertElementAt(step, 0);
    }
    MatchLocationPath* newPath = new (fMemoryManager) MatchLocationPath(stepsVector);
    janSteps.orphan();
    bool bFound=false;
    for(XMLSize_t j=0;j<fLocationPaths->size();j++)
        if((*(fLocationPaths->elementAt(j)))==(*newPath))
        {
            bFound=true;
            break;
        }
    if(bFound)
        delete newPath;
    else
        fLocationPaths->addElement(newPath);
}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_TOCREATE(MatchXPath)

void MatchXPath::serialize(XSerializeEngine& serEng)
{

    if (serEng.isStoring())
    {
        serEng<<fEmptyNamespaceId;
        serEng.writeString(fExpression);

        /***
         * Serialize RefVectorOf<MatchLocationPath>* fLocationPaths;
         ***/
        XTemplateSerializer::storeObject(fLocationPaths, serEng);
    }
    else
    {
        serEng>>fEmptyNamespaceId;
        serEng.readString(fExpression);

        /***
         * Deserialize RefVectorOf<MatchLocationPath>* fLocationPaths;
         ***/
        XTemplateSerializer::loadObject(&fLocationPaths, 8, true, serEng);
    }
}

MatchXPath::MatchXPath(MemoryManager* const manager)
:fEmptyNamespaceId(0)
,fExpression(0)
,fLocationPaths(0)
,fMemoryManager(manager)
{
}

// ---------------------------------------------------------------------------
//  XPathScanner: Constructors and Destructor
// ---------------------------------------------------------------------------
XPathScanner::XPathScanner(XMLStringPool* const stringPool)
    : fAndSymbol (0)
    , fOrSymbol(0)
    , fModSymbol(0)
    , fDivSymbol(0)
    , fCommentSymbol(0)
    , fTextSymbol(0)
    , fPISymbol(0)
    , fNodeSymbol(0)
    , fAncestorSymbol(0)
    , fAncestorOrSelfSymbol(0)
    , fAttributeSymbol(0)
    , fChildSymbol(0)
    , fDescendantSymbol(0)
    , fDescendantOrSelfSymbol(0)
    , fFollowingSymbol(0)
    , fFollowingSiblingSymbol(0)
    , fNamespaceSymbol(0)
    , fParentSymbol(0)
    , fPrecedingSymbol(0)
    , fPrecedingSiblingSymbol(0)
    , fSelfSymbol(0)
    , fStringPool(stringPool)
{
    init();
}

// ---------------------------------------------------------------------------
//  XPathScanner: Helper methods
// ---------------------------------------------------------------------------
void XPathScanner::init() {

    fAndSymbol = fStringPool->addOrFind(XPathSymbols::fgSYMBOL_AND);
    fOrSymbol = fStringPool->addOrFind(XPathSymbols::fgSYMBOL_OR);
    fModSymbol = fStringPool->addOrFind(XPathSymbols::fgSYMBOL_MOD);
    fDivSymbol = fStringPool->addOrFind(XPathSymbols::fgSYMBOL_DIV);
    fCommentSymbol = fStringPool->addOrFind(XPathSymbols::fgSYMBOL_COMMENT);
    fTextSymbol = fStringPool->addOrFind(XPathSymbols::fgSYMBOL_TEXT);
    fPISymbol = fStringPool->addOrFind(XPathSymbols::fgSYMBOL_PI);
    fNodeSymbol = fStringPool->addOrFind(XPathSymbols::fgSYMBOL_NODE);
    fAncestorSymbol = fStringPool->addOrFind(XPathSymbols::fgSYMBOL_ANCESTOR);
    fAncestorOrSelfSymbol = fStringPool->addOrFind(XPathSymbols::fgSYMBOL_ANCESTOR_OR_SELF);
    fAttributeSymbol = fStringPool->addOrFind(XPathSymbols::fgSYMBOL_ATTRIBUTE);
    fChildSymbol = fStringPool->addOrFind(XPathSymbols::fgSYMBOL_CHILD);
    fDescendantSymbol = fStringPool->addOrFind(XPathSymbols::fgSYMBOL_DESCENDANT);
    fDescendantOrSelfSymbol = fStringPool->addOrFind(XPathSymbols::fgSYMBOL_DESCENDANT_OR_SELF);
    fFollowingSymbol = fStringPool->addOrFind(XPathSymbols::fgSYMBOL_FOLLOWING);
    fFollowingSiblingSymbol = fStringPool->addOrFind(XPathSymbols::fgSYMBOL_FOLLOWING_SIBLING);
    fNamespaceSymbol = fStringPool->addOrFind(XPathSymbols::fgSYMBOL_NAMESPACE);
    fParentSymbol = fStringPool->addOrFind(XPathSymbols::fgSYMBOL_PARENT);
    fPrecedingSymbol = fStringPool->addOrFind(XPathSymbols::fgSYMBOL_PRECEDING);
    fPrecedingSiblingSymbol = fStringPool->addOrFind(XPathSymbols::fgSYMBOL_PRECEDING_SIBLING);
    fSelfSymbol = fStringPool->addOrFind(XPathSymbols::fgSYMBOL_SELF);
}


// ---------------------------------------------------------------------------
//  XPathScanner: Scan methods
// ---------------------------------------------------------------------------
bool XPathScanner::scanExpression(const XMLCh* const data,
                                  XMLSize_t currentOffset,
                                  const XMLSize_t endOffset,
                                  ValueVectorOf<int>* const tokens) {

    bool      starIsMultiplyOperator = false;
    XMLSize_t nameOffset = 0;
    int       nameHandle = -1;
    int       prefixHandle = -1;
    XMLCh     ch;
    XMLBuffer dataBuffer(128, tokens->getMemoryManager());

    while (currentOffset != endOffset) {

        ch = data[currentOffset];

        while (XMLChar1_0::isWhitespace(ch)) {

            if (++currentOffset == endOffset) {
                break;
            }

            ch = data[currentOffset];
        }

        if (currentOffset == endOffset) {
            break;
        }
        //
        // [28] ExprToken ::= '(' | ')' | '[' | ']' | '.' | '..' | '@' | ',' | '::'
        //                  | NameTest | NodeType | Operator | FunctionName
        //                  | AxisName | Literal | Number | VariableReference
        //
        XMLByte chartype = (ch >= 0x80) ? (XMLByte)CHARTYPE_NONASCII : fASCIICharMap[ch];

        switch (chartype) {
        case CHARTYPE_OPEN_PAREN:       // '('
            addToken(tokens, MatchXPath::EXPRTOKEN_OPEN_PAREN);
            starIsMultiplyOperator = false;
            ++currentOffset;
            break;
        case CHARTYPE_CLOSE_PAREN:      // ')'
            addToken(tokens, MatchXPath::EXPRTOKEN_CLOSE_PAREN);
            starIsMultiplyOperator = true;
            ++currentOffset;
            break;
        case CHARTYPE_OPEN_BRACKET:     // '['
            addToken(tokens, MatchXPath::EXPRTOKEN_OPEN_BRACKET);
            starIsMultiplyOperator = false;
            ++currentOffset;
            break;
        case CHARTYPE_CLOSE_BRACKET:    // ']'
            addToken(tokens, MatchXPath::EXPRTOKEN_CLOSE_BRACKET);
            starIsMultiplyOperator = true;
            ++currentOffset;
            break;
                //
                // [30] Number ::= Digits ('.' Digits?)? | '.' Digits
                //                                         ^^^^^^^^^^
                //
        case CHARTYPE_PERIOD:           // '.', '..' or '.' Digits
            if (currentOffset + 1 == endOffset) {
                addToken(tokens, MatchXPath::EXPRTOKEN_PERIOD);
                starIsMultiplyOperator = true;
                currentOffset++;
                break;
            }

            ch = data[currentOffset + 1];

            if (ch == chPeriod) {            // '..'
                addToken(tokens, MatchXPath::EXPRTOKEN_DOUBLE_PERIOD);
                starIsMultiplyOperator = true;
                currentOffset += 2;
            } else if (ch >= chDigit_0 && ch <= chDigit_9) {
                addToken(tokens, MatchXPath::EXPRTOKEN_NUMBER);
                starIsMultiplyOperator = true;
                currentOffset = scanNumber(data, endOffset, currentOffset, tokens);
            } else if (ch == chForwardSlash) {
                addToken(tokens, MatchXPath::EXPRTOKEN_PERIOD);
                starIsMultiplyOperator = true;
                currentOffset++;
            } else if (ch == chPipe) { // '|'
                addToken(tokens, MatchXPath::EXPRTOKEN_PERIOD);
                starIsMultiplyOperator = true;
                currentOffset++;
            } else if (XMLChar1_0::isWhitespace(ch)) {
                do {
                    if (++currentOffset == endOffset)
                        break;

                    ch = data[currentOffset];
                } while (XMLChar1_0::isWhitespace(ch));

                if (currentOffset == endOffset || ch == chPipe || ch == chForwardSlash) {
				    addToken(tokens, MatchXPath::EXPRTOKEN_PERIOD);
                    starIsMultiplyOperator = true;
                    break;
                }
            } else {
                XMLCh str[2]= {ch, 0 };
                ThrowXMLwithMemMgr1(XPathException, XMLExcepts::XPath_InvalidChar, str, tokens->getMemoryManager());
            }

            break;
        case CHARTYPE_ATSIGN:           // '@'
            addToken(tokens, MatchXPath::EXPRTOKEN_ATSIGN);
            starIsMultiplyOperator = false;
            ++currentOffset;
            break;
        case CHARTYPE_COMMA:            // ','
            addToken(tokens, MatchXPath::EXPRTOKEN_COMMA);
            starIsMultiplyOperator = false;
            ++currentOffset;
            break;
        case CHARTYPE_COLON:            // '::'
            if (++currentOffset == endOffset) {
                return false; // REVISIT
            }
            ch = data[currentOffset];

            if (ch != chColon) {
                return false; // REVISIT
            }
            addToken(tokens, MatchXPath::EXPRTOKEN_DOUBLE_COLON);
            starIsMultiplyOperator = false;
            ++currentOffset;
            break;
        case CHARTYPE_SLASH:            // '/' and '//'
            if (++currentOffset == endOffset) {
                addToken(tokens, MatchXPath::EXPRTOKEN_OPERATOR_SLASH);
                starIsMultiplyOperator = false;
                break;
            }

            ch = data[currentOffset];

            if (ch == chForwardSlash) { // '//'
                addToken(tokens, MatchXPath::EXPRTOKEN_OPERATOR_DOUBLE_SLASH);
                starIsMultiplyOperator = false;
                ++currentOffset;
            } else {
                addToken(tokens, MatchXPath::EXPRTOKEN_OPERATOR_SLASH);
                starIsMultiplyOperator = false;
            }
            break;
        case CHARTYPE_UNION:            // '|'
            addToken(tokens, MatchXPath::EXPRTOKEN_OPERATOR_UNION);
            starIsMultiplyOperator = false;
            ++currentOffset;
            break;
        case CHARTYPE_PLUS:             // '+'
            addToken(tokens, MatchXPath::EXPRTOKEN_OPERATOR_PLUS);
            starIsMultiplyOperator = false;
            ++currentOffset;
            break;
        case CHARTYPE_MINUS:            // '-'
            addToken(tokens, MatchXPath::EXPRTOKEN_OPERATOR_MINUS);
            starIsMultiplyOperator = false;
            ++currentOffset;
            break;
        case CHARTYPE_EQUAL:            // '='
            addToken(tokens, MatchXPath::EXPRTOKEN_OPERATOR_EQUAL);
            starIsMultiplyOperator = false;
            ++currentOffset;
            break;
        case CHARTYPE_EXCLAMATION:      // '!='
            if (++currentOffset == endOffset) {
                return false; // REVISIT
            }

            ch = data[currentOffset];

            if (ch != chEqual) {
                return false; // REVISIT
            }

            addToken(tokens, MatchXPath::EXPRTOKEN_OPERATOR_NOT_EQUAL);
            starIsMultiplyOperator = false;
            ++currentOffset;
            break;
        case CHARTYPE_LESS: // '<' and '<='
            if (++currentOffset == endOffset) {
                addToken(tokens, MatchXPath::EXPRTOKEN_OPERATOR_LESS);
                starIsMultiplyOperator = false;
                break;
            }

            ch = data[currentOffset];

            if (ch == chEqual) { // '<='
                addToken(tokens, MatchXPath::EXPRTOKEN_OPERATOR_LESS_EQUAL);
                starIsMultiplyOperator = false;
                ++currentOffset;
            } else {
                addToken(tokens, MatchXPath::EXPRTOKEN_OPERATOR_LESS);
                starIsMultiplyOperator = false;
            }
            break;
        case CHARTYPE_GREATER: // '>' and '>='
            if (++currentOffset == endOffset) {
                addToken(tokens, MatchXPath::EXPRTOKEN_OPERATOR_GREATER);
                starIsMultiplyOperator = false;
                break;
            }

            ch = data[currentOffset];

            if (ch == chEqual) { // '>='
                addToken(tokens, MatchXPath::EXPRTOKEN_OPERATOR_GREATER_EQUAL);
                starIsMultiplyOperator = false;
                ++currentOffset;
            } else {
                addToken(tokens, MatchXPath::EXPRTOKEN_OPERATOR_GREATER);
                starIsMultiplyOperator = false;
            }
            break;
        //
        // [29] Literal ::= '"' [^"]* '"' | "'" [^']* "'"
        //
        case CHARTYPE_QUOTE:            // '\"' or '\''
            {
                XMLCh qchar = ch;
                if (++currentOffset == endOffset) {
                    return false; // REVISIT
                }

                ch = data[currentOffset];

                XMLSize_t litOffset = currentOffset;
                while (ch != qchar) {
                    if (++currentOffset == endOffset) {
                        return false; // REVISIT
                    }

                    ch = data[currentOffset];
                }

                addToken(tokens, MatchXPath::EXPRTOKEN_LITERAL);
                starIsMultiplyOperator = true;

                dataBuffer.set(data + litOffset, currentOffset - litOffset);
                tokens->addElement(fStringPool->addOrFind(dataBuffer.getRawBuffer()));
                ++currentOffset;
                break;
            }
        //
        // [30] Number ::= Digits ('.' Digits?)? | '.' Digits
        // [31] Digits ::= [0-9]+
        //
        case CHARTYPE_DIGIT:
            addToken(tokens, MatchXPath::EXPRTOKEN_NUMBER);
            starIsMultiplyOperator = true;
            currentOffset = scanNumber(data, endOffset, currentOffset, tokens);
            break;
        //
        // [36] VariableReference ::= '$' QName
        //
        case CHARTYPE_DOLLAR:
            if (++currentOffset == endOffset) {
                return false; // REVISIT
            }
            nameOffset = currentOffset;
            currentOffset = scanNCName(data, endOffset, currentOffset);

            if (currentOffset == nameOffset) {
                return false; // REVISIT
            }

            if (currentOffset < endOffset) {
                ch = data[currentOffset];
            }
            else {
                ch = 0;
            }

            dataBuffer.set(data + nameOffset, currentOffset - nameOffset);
            nameHandle = fStringPool->addOrFind(dataBuffer.getRawBuffer());
            prefixHandle = -1;

            if (ch == chColon) {

                prefixHandle = nameHandle;
                if (++currentOffset == endOffset) {
                    return false; // REVISIT
                }
                nameOffset = currentOffset;
                currentOffset = scanNCName(data, endOffset, currentOffset);

                if (currentOffset == nameOffset) {
                    return false; // REVISIT
                }

                dataBuffer.set(data + nameOffset, currentOffset - nameOffset);
                nameHandle = fStringPool->addOrFind(dataBuffer.getRawBuffer());
            }
            addToken(tokens, MatchXPath::EXPRTOKEN_VARIABLE_REFERENCE);
            starIsMultiplyOperator = true;
            tokens->addElement(prefixHandle);
            tokens->addElement(nameHandle);
            break;
        //
        // [37] NameTest ::= '*' | NCName ':' '*' | QName
        // [34] MultiplyOperator ::= '*'
        //
        case CHARTYPE_STAR:             // '*'
            //
            // 3.7 Lexical Structure
            //
            //  If there is a preceding token and the preceding token is not one of @, ::, (, [, , or
            //  an Operator, then a * must be recognized as a MultiplyOperator.
            //
            // Otherwise, the token must not be recognized as a MultiplyOperator.
            //
            if (starIsMultiplyOperator) {
                addToken(tokens, MatchXPath::EXPRTOKEN_OPERATOR_MULT);
                starIsMultiplyOperator = false;
            } else {
                addToken(tokens, MatchXPath::EXPRTOKEN_NAMETEST_ANY);
                starIsMultiplyOperator = true;
            }

            ++currentOffset;
            break;
        //
        // NCName, QName and non-terminals
        //
        case CHARTYPE_NONASCII: // possibly a valid non-ascii 'Letter' (BaseChar | Ideographic)
        case CHARTYPE_LETTER:
        case CHARTYPE_UNDERSCORE:
            {
            //
            // 3.7 Lexical Structure
            //
            //  If there is a preceding token and the preceding token is not one of @, ::, (, [, , or
            //  an Operator, then an NCName must be recognized as an OperatorName.
            //
            //  If the character following an NCName (possibly after intervening ExprWhitespace) is (,
            //  then the token must be recognized as a NodeType or a FunctionName.
            //
            //  If the two characters following an NCName (possibly after intervening ExprWhitespace)
            //  are ::, then the token must be recognized as an AxisName.
            //
            //  Otherwise, the token must not be recognized as an OperatorName, a NodeType, a
            //  FunctionName, or an AxisName.
            //
            // [33] OperatorName ::= 'and' | 'or' | 'mod' | 'div'
            // [38] NodeType ::= 'comment' | 'text' | 'processing-instruction' | 'node'
            // [35] FunctionName ::= QName - NodeType
            // [6] AxisName ::= (see above)
            //
            // [37] NameTest ::= '*' | NCName ':' '*' | QName
            // [5] NCName ::= (Letter | '_') (NCNameChar)*
            // [?] NCNameChar ::= Letter | Digit | '.' | '-' | '_'  (ascii subset of 'NCNameChar')
            // [?] QName ::= (NCName ':')? NCName
            // [?] Letter ::= [A-Za-z]                              (ascii subset of 'Letter')
            // [?] Digit ::= [0-9]                                  (ascii subset of 'Digit')
            //
            nameOffset = currentOffset;
            currentOffset = scanNCName(data, endOffset, currentOffset);
            if (currentOffset == nameOffset) {
                return false; // REVISIT
			}

            if (currentOffset < endOffset) {
                ch = data[currentOffset];
            }
            else {
                ch = 0;
            }

            dataBuffer.set(data + nameOffset, currentOffset - nameOffset);
            nameHandle = fStringPool->addOrFind(dataBuffer.getRawBuffer());

            bool isNameTestNCName = false;
            bool isAxisName = false;
            prefixHandle = -1;

            if (ch == chColon) {

                if (++currentOffset == endOffset) {
                    return false; // REVISIT
                }

                ch = data[currentOffset];

                if (ch == chAsterisk) {
                    if (++currentOffset < endOffset) {
                        ch = data[currentOffset];
                    }

                    isNameTestNCName = true;
                } else if (ch == chColon) {
                    if (++currentOffset < endOffset) {
                        ch = data[currentOffset];
                    }

                    isAxisName = true;
                } else {
                    prefixHandle = nameHandle;
                    nameOffset = currentOffset;
                    currentOffset = scanNCName(data, endOffset, currentOffset);
                    if (currentOffset == nameOffset) {
                        return false; // REVISIT
                    }
                    if (currentOffset < endOffset) {
                        ch = data[currentOffset];
                    }
                    else {
                        ch = 0;
                    }

                    dataBuffer.set(data + nameOffset, currentOffset - nameOffset);
                    nameHandle = fStringPool->addOrFind(dataBuffer.getRawBuffer());
                }
            }
            //
            // [39] ExprWhitespace ::= S
            //
            while (XMLChar1_0::isWhitespace(ch)) {
                if (++currentOffset == endOffset) {
                    break;
                }
                ch = data[currentOffset];
            }

            //
            //  If there is a preceding token and the preceding token is not one of @, ::, (, [, , or
            //  an Operator, then an NCName must be recognized as an OperatorName.
            //
            if (starIsMultiplyOperator) {
                if (nameHandle == fAndSymbol) {
                    addToken(tokens, MatchXPath::EXPRTOKEN_OPERATOR_AND);
                    starIsMultiplyOperator = false;
                } else if (nameHandle == fOrSymbol) {
                    addToken(tokens, MatchXPath::EXPRTOKEN_OPERATOR_OR);
                    starIsMultiplyOperator = false;
                } else if (nameHandle == fModSymbol) {
                    addToken(tokens, MatchXPath::EXPRTOKEN_OPERATOR_MOD);
                    starIsMultiplyOperator = false;
                } else if (nameHandle == fDivSymbol) {
                    addToken(tokens, MatchXPath::EXPRTOKEN_OPERATOR_DIV);
                    starIsMultiplyOperator = false;
                } else {
                    return false; // REVISIT
                }

                if (isNameTestNCName) {
                    return false; // REVISIT - NCName:* where an OperatorName is required
                } else if (isAxisName) {
                    return false; // REVISIT - AxisName:: where an OperatorName is required
                }
                break;
            }
            //
            //  If the character following an NCName (possibly after intervening ExprWhitespace) is (,
            //  then the token must be recognized as a NodeType or a FunctionName.
            //
            if (ch == chOpenParen && !isNameTestNCName && !isAxisName) {
                if (nameHandle == fCommentSymbol) {
                    addToken(tokens, MatchXPath::EXPRTOKEN_NODETYPE_COMMENT);
                } else if (nameHandle == fTextSymbol) {
                    addToken(tokens, MatchXPath::EXPRTOKEN_NODETYPE_TEXT);
                } else if (nameHandle == fPISymbol) {
                    addToken(tokens, MatchXPath::EXPRTOKEN_NODETYPE_PI);
                } else if (nameHandle == fNodeSymbol) {
                    addToken(tokens, MatchXPath::EXPRTOKEN_NODETYPE_NODE);
                } else {
                    addToken(tokens, MatchXPath::EXPRTOKEN_FUNCTION_NAME);
                    tokens->addElement(prefixHandle);
                    tokens->addElement(nameHandle);
                }
                addToken(tokens, MatchXPath::EXPRTOKEN_OPEN_PAREN);
                starIsMultiplyOperator = false;
                ++currentOffset;
                break;
            }

            //
            //  If the two characters following an NCName (possibly after intervening ExprWhitespace)
            //  are ::, then the token must be recognized as an AxisName.
            //
            if (isAxisName ||
                (ch == chColon && currentOffset + 1 < endOffset &&
                 data[currentOffset + 1] == chColon)) {

                if (nameHandle == fAncestorSymbol) {
                    addToken(tokens, MatchXPath::EXPRTOKEN_AXISNAME_ANCESTOR);
                } else if (nameHandle == fAncestorOrSelfSymbol) {
                    addToken(tokens, MatchXPath::EXPRTOKEN_AXISNAME_ANCESTOR_OR_SELF);
                } else if (nameHandle == fAttributeSymbol) {
                    addToken(tokens, MatchXPath::EXPRTOKEN_AXISNAME_ATTRIBUTE);
                } else if (nameHandle == fChildSymbol) {
                    addToken(tokens, MatchXPath::EXPRTOKEN_AXISNAME_CHILD);
                } else if (nameHandle == fDescendantSymbol) {
                    addToken(tokens, MatchXPath::EXPRTOKEN_AXISNAME_DESCENDANT);
                } else if (nameHandle == fDescendantOrSelfSymbol) {
                    addToken(tokens, MatchXPath::EXPRTOKEN_AXISNAME_DESCENDANT_OR_SELF);
                } else if (nameHandle == fFollowingSymbol) {
                    addToken(tokens, MatchXPath::EXPRTOKEN_AXISNAME_FOLLOWING);
                } else if (nameHandle == fFollowingSiblingSymbol) {
                    addToken(tokens, MatchXPath::EXPRTOKEN_AXISNAME_FOLLOWING_SIBLING);
                } else if (nameHandle == fNamespaceSymbol) {
                    addToken(tokens, MatchXPath::EXPRTOKEN_AXISNAME_NAMESPACE);
                } else if (nameHandle == fParentSymbol) {
                    addToken(tokens, MatchXPath::EXPRTOKEN_AXISNAME_PARENT);
                } else if (nameHandle == fPrecedingSymbol) {
                    addToken(tokens, MatchXPath::EXPRTOKEN_AXISNAME_PRECEDING);
                } else if (nameHandle == fPrecedingSiblingSymbol) {
                    addToken(tokens, MatchXPath::EXPRTOKEN_AXISNAME_PRECEDING_SIBLING);
                } else if (nameHandle == fSelfSymbol) {
                    addToken(tokens, MatchXPath::EXPRTOKEN_AXISNAME_SELF);
                } else {
                    return false; // REVISIT
                }

                if (isNameTestNCName) {
                    return false; // REVISIT - "NCName:* ::" where "AxisName ::" is required
                }

                addToken(tokens, MatchXPath::EXPRTOKEN_DOUBLE_COLON);
                starIsMultiplyOperator = false;
                if (!isAxisName) {
                    currentOffset += 2;
                }
                break;
            }
            //
            //  Otherwise, the token must not be recognized as an OperatorName, a NodeType, a
            //  FunctionName, or an AxisName.
            //
            if (isNameTestNCName) {
                addToken(tokens, MatchXPath::EXPRTOKEN_NAMETEST_NAMESPACE);
                tokens->addElement(nameHandle);
            } else {
                addToken(tokens, MatchXPath::EXPRTOKEN_NAMETEST_QNAME);
                tokens->addElement(prefixHandle);
                tokens->addElement(nameHandle);
            }

            starIsMultiplyOperator = true;
            break;
            }
        default:
            {
            XMLCh str[2]= {ch, 0 };
            ThrowXMLwithMemMgr1(XPathException, XMLExcepts::XPath_InvalidChar, str, tokens->getMemoryManager());
            break;
            }
        }
    }

    return true;
}


XMLSize_t XPathScanner::scanNCName(const XMLCh* const data,
                             const XMLSize_t endOffset,
                             XMLSize_t currentOffset) {

    XMLCh ch = data[currentOffset];

    if (!XMLChar1_0::isFirstNCNameChar(ch)) {
        return currentOffset;
    }

    while (++currentOffset < endOffset) {

        ch = data[currentOffset];

        if (!XMLChar1_0::isNCNameChar(ch)) {
            break;
        }
    }

    return currentOffset;
}


XMLSize_t XPathScanner::scanNumber(const XMLCh* const data,
                             const XMLSize_t endOffset,
                             XMLSize_t currentOffset,
                             ValueVectorOf<int>* const tokens) {

    XMLCh ch = data[currentOffset];
    int   whole = 0;
    int   part = 0;

    while (ch >= chDigit_0 && ch <= chDigit_9) {

        whole = (whole * 10) + (ch - chDigit_0);

        if (++currentOffset == endOffset) {
            break;
        }

        ch = data[currentOffset];
    }

    if (ch == chPeriod) {

        if (++currentOffset < endOffset) {

            ch = data[currentOffset];

            while (ch >= chDigit_0 && ch <= chDigit_9) {

                part = (part * 10) + (ch - chDigit_0);

                if (++currentOffset == endOffset) {
                    break;
                }

                ch = data[currentOffset];
            }

            if (part != 0) {
                ThrowXMLwithMemMgr(RuntimeException, XMLExcepts::XPath_FindSolution, tokens->getMemoryManager());
            }
        }
    }

    tokens->addElement(whole);
    tokens->addElement(part);

    return currentOffset;
}


// ---------------------------------------------------------------------------
//  XPathScannerForSchema: Constructors and Destructor
// ---------------------------------------------------------------------------
XPathScannerForSchema::XPathScannerForSchema(XMLStringPool* const stringPool)
    : XPathScanner(stringPool)
{
}


// ---------------------------------------------------------------------------
//  XPathScannerForSchema: Helper methods
// ---------------------------------------------------------------------------
void XPathScannerForSchema::addToken(ValueVectorOf<int>* const tokens,
                                     const int aToken) {

    if (aToken == MatchXPath::EXPRTOKEN_ATSIGN ||
        aToken == MatchXPath::EXPRTOKEN_AXISNAME_ATTRIBUTE ||
        aToken == MatchXPath::EXPRTOKEN_AXISNAME_CHILD ||
        //token == MatchXPath::EXPRTOKEN_AXISNAME_SELF ||
        aToken == MatchXPath::EXPRTOKEN_DOUBLE_COLON ||
        aToken == MatchXPath::EXPRTOKEN_NAMETEST_QNAME ||
        //token == MatchXPath::EXPRTOKEN_NODETYPE_NODE ||
        aToken == MatchXPath::EXPRTOKEN_OPERATOR_SLASH ||
        aToken == MatchXPath::EXPRTOKEN_PERIOD ||
        aToken == MatchXPath::EXPRTOKEN_NAMETEST_ANY ||
        aToken == MatchXPath::EXPRTOKEN_NAMETEST_NAMESPACE ||
        aToken == MatchXPath::EXPRTOKEN_OPERATOR_DOUBLE_SLASH ||
        aToken == MatchXPath::EXPRTOKEN_OPERATOR_UNION) {

        tokens->addElement(aToken);
        return;
    }

    ThrowXMLwithMemMgr(XPathException, XMLExcepts::XPath_TokenNotSupported, tokens->getMemoryManager());
}

MATCH_CPP_NAMESPACE_END

/**
  * End of file MatchPath.cpp
  */
