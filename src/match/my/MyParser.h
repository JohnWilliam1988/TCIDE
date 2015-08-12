//////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-06-26  肖秀范      完成块脚本解释类MyParser的设计
//////////////////////////////////////////////////////////////////////////////////////////

#if !defined(MYPARSER_H)
#define MYPARSER_H

#include <match/framework/XMLErrorReporter.hpp>
#include <match/sax/InputSource.hpp>

#include "MyBlockListPool.h"
#include "MyScanner.h"
#include "MyGlobalVarList.h"

MATCH_CPP_NAMESPACE_BEGIN

//MyParser类主要负责解析match源文件的作用
class PARSERS_EXPORT MyParser  : public XMemory
                               , public XMLErrorReporter
{ 
public:
	MyParser(MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);


    // -----------------------------------------------------------------------
    //  Parsing methods
    // -----------------------------------------------------------------------

	    // -----------------------------------------------------------------------
    //  Implementation of the XMLErrorReporter interface.
    // -----------------------------------------------------------------------

    /** @name Implementation of the XMLErrorReporter interface. */
    //@{

    /** Handle errors reported from the parser
      *
      * This method is used to report back errors found while parsing the
      * XML file. This method is also borrowed from the SAX specification.
      * It calls the corresponding user installed Error Handler method:
      * 'fatal', 'error', 'warning' depending on the severity of the error.
      * This classification is defined by the XML specification.
      *
      * @param errCode An integer code for the error.
      * @param msgDomain A const pointer to an Unicode string representing
      *                  the message domain to use.
      * @param errType An enumeration classifying the severity of the error.
      * @param errorText A const pointer to an Unicode string representing
      *                  the text of the error message.
      * @param systemId  A const pointer to an Unicode string representing
      *                  the system id of the XML file where this error
      *                  was discovered.
      * @param publicId  A const pointer to an Unicode string representing
      *                  the public id of the XML file where this error
      *                  was discovered.
      * @param lineNum   The line number where the error occurred.
      * @param colNum    The column number where the error occurred.
      * @see ErrorHandler
      */
    virtual void error
    (
        const   unsigned int                errCode
        , const XMLCh* const                msgDomain
        , const XMLErrorReporter::ErrTypes  errType
        , const XMLCh* const                errorText
        , const XMLCh* const                systemId
        , const XMLCh* const                publicId
        , const XMLFileLoc                  lineNum
        , const XMLFileLoc                  colNum
    );

    virtual void resetErrors();

	 /** Get a pointer to the error handler
      *
      * This method returns the installed error handler. If no handler
      * has been installed, then it will be a zero pointer.
      *
      * @return The pointer to the installed error handler object.
      */
    ErrorHandler* getErrorHandler();

    /** Get a const pointer to the error handler
      *
      * This method returns the installed error handler.  If no handler
      * has been installed, then it will be a zero pointer.
      *
      * @return A const pointer to the installed error handler object.
      */
    const ErrorHandler* getErrorHandler() const;


	 /** Set the error handler
      *
      * This method allows applications to install their own error handler
      * to trap error and warning messages.
      *
      * <i>Any previously set handler is merely dropped, since the parser
      * does not own them.</i>
      *
      * @param handler  A const pointer to the user supplied error
      *                 handler.
      *
      * @see #getErrorHandler
      */
    void setErrorHandler(ErrorHandler* const handler);

    //三个重载的接口

    //解析文件
    //方法描述:解析脚本的接口
    //返回值:无
    //返回类型                描述
    //void                    无
    //参数说明:
    //参数名称                参数类型                    描述
    //source                  const InputSource&          脚本源文件路径的对象
    void parse(const InputSource& source);                        

    //解析文件
    //方法描述:解析脚本的接口
    //返回值:无
    //返回类型                描述
    //void                    无
    //参数说明:
    //参数名称      参数类型                    描述
    //systemId      const XMLCh*                脚本源文件路径的XMLCh字符串
    void parse(const XMLCh* const systemId);                      

    //解析文件
    //方法描述:解析脚本的接口
    //返回值:无
    //返回类型                描述
    //void                    无
    //参数说明:
    //参数名称      参数类型                    描述
    //systemId      const char*                脚本源文件路径的char字符串
    void parse(const char* const systemId);             

    //解析文件
    //方法描述:重置解析参数,表示是否在脚本的解析中
    //返回值:无
    //返回类型                描述
    //void                    无
    void reset();                                                 

    //得到内存管理器对象
    //方法描述:重置解析参数
    //返回值:内存管理对象
    //返回类型                描述
    //MemoryManager*          内存管理对象
    MemoryManager* getMemoryManager() const;


	~MyParser();

private:
	// -----------------------------------------------------------------------
    //  Initialize/Cleanup methods
    // -----------------------------------------------------------------------
    void initialize();                                             //初始化
    void cleanUp();                                                //清除工作

	void resetInProgress();

	bool                          fParseInProgress;       //是否在处理中
    MemoryManager*                fMemoryManager;         //内存管理
	MyScanner*                    fScanner;               //扫描源文件
    ErrorHandler*                 fErrorHandler;          //错误处理机制
    /**
    * Copy constructor
    */
    MyParser(const MyParser& pool);
    MyParser& operator=(const MyParser& toAssign);
};

inline MemoryManager* MyParser::getMemoryManager() const
{
    return fMemoryManager;
}

inline ErrorHandler* MyParser::getErrorHandler()
{
    return fErrorHandler;
}

inline const ErrorHandler* MyParser::getErrorHandler() const
{
    return fErrorHandler;
}

MATCH_CPP_NAMESPACE_END

#endif