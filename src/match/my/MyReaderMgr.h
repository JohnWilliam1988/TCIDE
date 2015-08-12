///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-06-26  肖秀范      完成块文件读取管理类MyReaderMgr的设计,在原计划上添加一个 bool getLine(XMLBuffer& toFill); 方法
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(MATCHC_INCLUDE_GUARD_MYREADERMGR_HPP)
#define MATCHC_INCLUDE_GUARD_MYREADERMGR_HPP

#include "MyReader.h"

#include <match/util/PlatformUtils.hpp>
#include <match/util/RefStackOf.hpp>
#include <match/sax/Locator.hpp>
#include <match/framework/XMLBuffer.hpp>

MATCH_CPP_NAMESPACE_BEGIN

class XMLEntityDecl;
class XMLEntityHandler;
class XMLDocumentHandler;
class XMLScanner;


// ---------------------------------------------------------------------------
//  This class is used by the scanner. The scanner must deal with expansion
//  of entities, some of which are totally different files (external parsed
//  entities.) It does so by pushing readers onto a stack. The top reader is
//  the one it wants to read out of, but that one must be popped when it is
//  empty. To keep that logic from being all over the place, the scanner
//  talks to the reader manager, which handles the stack and popping off
//  used up readers.
// ---------------------------------------------------------------------------
class XMLPARSER_EXPORT MyReaderMgr :   public XMemory
                                   , public Locator
{
public :
    // -----------------------------------------------------------------------
    //  Class specific types
    // -----------------------------------------------------------------------
    struct LastExtEntityInfo : public XMemory
    {
        const   XMLCh*          systemId;
        const   XMLCh*          publicId;
                XMLFileLoc      lineNumber;
                XMLFileLoc      colNumber;
    };


    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    MyReaderMgr(MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
    ~MyReaderMgr();


    // -----------------------------------------------------------------------
    //  Convenience scanning methods
    //
    //  This are all convenience methods that work in terms of the core
    //  character spooling methods.
    // -----------------------------------------------------------------------

    /////////////////////////////////////////////////////////自定义语句////////////////////////////////////////////
    bool getLine(XMLBuffer& toFill);                                //添加一个读取一行的代码 -match 6-30
	bool isVarDef(); 
    /////////////////////////////////////////////////////////自定义语句////////////////////////////////////////////

    bool atEOF() const;
    bool getName(XMLBuffer& toFill);
    bool getQName(XMLBuffer& toFill, int* colonPosition);
    bool getNameToken(XMLBuffer& toFill);
    XMLCh getNextChar();
    bool getNextCharIfNot(const XMLCh chNotToGet, XMLCh& chGotten);
    void movePlainContentChars(XMLBuffer &dest);
    void getSpaces(XMLBuffer& toFill);
    void getUpToCharOrWS(XMLBuffer& toFill, const XMLCh toCheck);
    bool isEmpty() const;
    bool lookingAtChar(const XMLCh toCheck);
    bool lookingAtSpace();
    XMLCh peekNextChar();
    bool skipIfQuote(XMLCh& chGotten);
    void skipPastChar(const XMLCh toSkip);
    void skipPastSpaces(bool& skippedSomething, bool inDecl = false);
    void skipPastSpaces();
    void skipToChar(const XMLCh toSkipTo);
    bool skippedChar(const XMLCh toSkip);
    bool skippedSpace();
    bool skippedString(const XMLCh* const toSkip);
    bool skippedStringLong(const XMLCh* const toSkip);
    void skipQuotedString(const XMLCh quoteCh);
    XMLCh skipUntilIn(const XMLCh* const listToSkip);
    XMLCh skipUntilInOrWS(const XMLCh* const listToSkip);
    bool peekString(const XMLCh* const toPeek);


    // -----------------------------------------------------------------------
    //  Control methods
    // -----------------------------------------------------------------------
    void cleanStackBackTo(const XMLSize_t readerNum);
    MyReader* createReader
    (
        const   InputSource&        src
        , const bool                xmlDecl
        , const MyReader::RefFrom  refFrom
        , const MyReader::Types    type
        , const MyReader::Sources  source
        , const bool                calcSrsOfs = true
        ,       XMLSize_t           lowWaterMark = 100
    );
    MyReader* createReader
    (
        const   XMLCh* const        sysId
        , const XMLCh* const        pubId
        , const bool                xmlDecl
        , const MyReader::RefFrom  refFrom
        , const MyReader::Types    type
        , const MyReader::Sources  source
        ,       InputSource*&       srcToFill
        , const bool                calcSrcOfs = true
        ,       XMLSize_t           lowWaterMark = 100
        , const bool                disableDefaultEntityResolution = false
    );
    MyReader* createReader
    (
        const   XMLCh* const        baseURI
        , const XMLCh* const        sysId
        , const XMLCh* const        pubId
        , const bool                xmlDecl
        , const MyReader::RefFrom  refFrom
        , const MyReader::Types    type
        , const MyReader::Sources  source
        ,       InputSource*&       srcToFill
        , const bool                calcSrcOfs = true
        ,       XMLSize_t           lowWaterMark = 100
        , const bool                disableDefaultEntityResolution = false
    );
    MyReader* createIntEntReader
    (
        const   XMLCh* const        sysId
        , const MyReader::RefFrom  refFrom
        , const MyReader::Types    type
        , const XMLCh* const        dataBuf
        , const XMLSize_t           dataLen
        , const bool                copyBuf
        , const bool                calcSrcOfs = true
        ,       XMLSize_t           lowWaterMark = 100
    );
    bool isScanningPERefOutOfLiteral() const;
    bool pushReader
    (
                MyReader* const        reader
        ,       XMLEntityDecl* const    entity
    );
    void reset();


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    const XMLCh* getCurrentEncodingStr() const;
    const XMLEntityDecl* getCurrentEntity() const;
    XMLEntityDecl* getCurrentEntity();
    const MyReader* getCurrentReader() const;
    MyReader* getCurrentReader();
    XMLSize_t getCurrentReaderNum() const;
    XMLSize_t getReaderDepth() const;
    void getLastExtEntityInfo(LastExtEntityInfo& lastInfo) const;
    XMLFilePos getSrcOffset() const;
    bool getThrowEOE() const;


    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void setEntityHandler(XMLEntityHandler* const newHandler);
    void setThrowEOE(const bool newValue);
    void setXMLVersion(const MyReader::XMLVersion version);
    void setStandardUriConformant(const bool newValue);

    // -----------------------------------------------------------------------
    //  Implement the SAX Locator interface
    // -----------------------------------------------------------------------
    virtual const XMLCh* getPublicId() const;
    virtual const XMLCh* getSystemId() const;
    virtual XMLFileLoc getLineNumber() const;
    virtual XMLFileLoc getColumnNumber() const;


private :
    // -----------------------------------------------------------------------
    //  Private helper methods
    // -----------------------------------------------------------------------
    const MyReader* getLastExtEntity(const XMLEntityDecl*& itsEntity) const;
    bool popReader();

    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    MyReaderMgr(const MyReaderMgr&);
    MyReaderMgr& operator=(const MyReaderMgr&);

    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fCurEntity
    //      This is the current top of stack entity. We pull it off the stack
    //      and store it here for efficiency.
    //
    //  fCurReader
    //      This is the current top of stack reader. We pull it off the
    //      stack and store it here for efficiency.
    //
    //  fEntityHandler
    //      This is the installed entity handler. Its installed via the
    //      scanner but he passes it on to us since we need it the most, in
    //      process of creating external entity readers.
    //
    //  fEntityStack
    //      We need to keep up with which of the pushed readers are pushed
    //      entity values that are being spooled. This is done to avoid the
    //      problem of recursive definitions. This stack consists of refs to
    //      EntityDecl objects for the pushed entities.
    //
    //  fNextReaderNum
    //      This is the reader serial number value. Each new reader that is
    //      created from this reader is given a successive number. This lets
    //      us catch things like partial markup errors and such.
    //
    //  fReaderStack
    //      This is the stack of reader references. We own all the readers
    //      and destroy them when they are used up.
    //
    //  fThrowEOE
    //      This flag controls whether we throw an exception when we hit an
    //      end of entity. The scanner doesn't really need to know about ends
    //      of entities in the int/ext subsets, so it will turn this flag off
    //      until it gets into the content usually.
    //
    //  fXMLVersion
    //      Enum to indicate if each Reader should be created as XML 1.1 or
    //      XML 1.0 conformant
    //
    //  fStandardUriConformant
    //      This flag controls whether we force conformant URI
    // -----------------------------------------------------------------------
    XMLEntityDecl*              fCurEntity;
    MyReader*                   fCurReader;
    XMLEntityHandler*           fEntityHandler;
    RefStackOf<XMLEntityDecl>*  fEntityStack;
    unsigned int                fNextReaderNum;
    RefStackOf<MyReader>*      fReaderStack;
    bool                        fThrowEOE;
    MyReader::XMLVersion       fXMLVersion;
    bool                        fStandardUriConformant;
    MemoryManager*              fMemoryManager;
};



// ---------------------------------------------------------------------------
//  MyReaderMgr: Inlined methods
//
//  NOTE: We cannot put these in alphabetical and type order as we usually
//  do because some of the compilers we have to support are too stupid to
//  understand out of order inlines!
// ---------------------------------------------------------------------------
inline XMLSize_t MyReaderMgr::getCurrentReaderNum() const
{
    return fCurReader->getReaderNum();
}

inline const MyReader* MyReaderMgr::getCurrentReader() const
{
    return fCurReader;
}

inline MyReader* MyReaderMgr::getCurrentReader()
{
    return fCurReader;
}

inline bool MyReaderMgr::getName(XMLBuffer& toFill)
{
    toFill.reset();
    return fCurReader->getName(toFill, false);
}

inline bool MyReaderMgr::getQName(XMLBuffer& toFill, int *colonPosition)
{
    toFill.reset();
    return fCurReader->getQName(toFill, colonPosition);
}

inline bool MyReaderMgr::getNameToken(XMLBuffer& toFill)
{
    toFill.reset();
    return fCurReader->getName(toFill, true);
}

inline bool MyReaderMgr::getNextCharIfNot(const XMLCh chNotToGet, XMLCh& chGotten)
{
    return fCurReader->getNextCharIfNot(chNotToGet, chGotten);
}

inline void MyReaderMgr::movePlainContentChars(XMLBuffer &dest)
{
    fCurReader->movePlainContentChars(dest);
}

inline bool MyReaderMgr::getThrowEOE() const
{
    return fThrowEOE;
}

inline XMLFilePos MyReaderMgr::getSrcOffset() const
{
    return fCurReader? fCurReader->getSrcOffset() : 0;
}

inline bool MyReaderMgr::lookingAtChar(const XMLCh chToCheck)
{
    return (chToCheck == peekNextChar());
}

inline bool MyReaderMgr::lookingAtSpace()
{
    XMLCh c = peekNextChar();
    return fCurReader->isWhitespace(c);
}

inline void MyReaderMgr::setThrowEOE(const bool newValue)
{
    fThrowEOE = newValue;
}

inline void MyReaderMgr::setStandardUriConformant(const bool newValue)
{
    fStandardUriConformant = newValue;
}

inline bool MyReaderMgr::skippedString(const XMLCh* const toSkip)
{
    return fCurReader->skippedString(toSkip);
}

inline bool MyReaderMgr::skippedStringLong(const XMLCh* const toSkip)
{
    return fCurReader->skippedStringLong(toSkip);
}

inline void MyReaderMgr::skipToChar(const XMLCh toSkipTo)
{
	XMLCh nextCh = 0;
    do
    {
        // Get chars until we find the one to skip
        nextCh = getNextChar();
	}
    // Break out at end of input or the char to skip
	while((nextCh != toSkipTo) && nextCh!=0);
}

inline void MyReaderMgr::skipPastChar(const XMLCh toSkipPast)
{
	XMLCh nextCh = 0;
    do
    {
        // Get chars until we find the one to skip
        nextCh = getNextChar();
	}
	while((nextCh != toSkipPast) && nextCh!=0);
}

inline bool MyReaderMgr::peekString(const XMLCh* const toPeek)
{
    return fCurReader->peekString(toPeek);
}

inline void MyReaderMgr::setEntityHandler(XMLEntityHandler* const newHandler)
{
    fEntityHandler = newHandler;
}

inline void MyReaderMgr::setXMLVersion(const MyReader::XMLVersion version)
{
    fXMLVersion = version;
    fCurReader->setXMLVersion(version);
}

//
//  This is a simple class to temporarily change the 'throw at end of entity'
//  flag of the reader manager. There are some places where we need to
//  turn this on and off on a scoped basis.
//
class XMLPARSER_EXPORT ThrowEOEJanitor
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and destructor
    // -----------------------------------------------------------------------
    ThrowEOEJanitor(MyReaderMgr* mgrTarget, const bool newValue) :

        fOld(mgrTarget->getThrowEOE())
        , fMgr(mgrTarget)
    {
        mgrTarget->setThrowEOE(newValue);
    }

    ~ThrowEOEJanitor()
    {
        fMgr->setThrowEOE(fOld);
    };

private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    ThrowEOEJanitor(const ThrowEOEJanitor&);
    ThrowEOEJanitor& operator=(const ThrowEOEJanitor&);

    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fOld
    //      The previous value of the flag, which we replaced during ctor,
    //      and will replace during dtor.
    //
    //  fMgr
    //      A pointer to the reader manager we are going to set/reset the
    //      flag on.
    // -----------------------------------------------------------------------
    bool        fOld;
    MyReaderMgr*  fMgr;
};

MATCH_CPP_NAMESPACE_END

#endif
