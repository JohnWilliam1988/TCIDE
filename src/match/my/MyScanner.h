//////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-06-26  肖秀范      完成块脚本扫描类MyScanner的设计
//////////////////////////////////////////////////////////////////////////////////////////

#if !defined(MYSCANNER_H)
#define MYSCANNER_H

#include <match/util/PlatformUtils.hpp>
#include <match/framework/XMLErrorReporter.hpp>
#include <match/sax/ErrorHandler.hpp>
#include <match/framework/XMLErrorCodes.hpp>
#include <match/framework/XMLBufferMgr.hpp>

#include "MyReaderMgr.h"
#include "BlockList.h"
#include "MyBlockListPool.h"
#include "DependBlockList.h"

#include "match/My/MyValue.h"

MATCH_CPP_NAMESPACE_BEGIN

enum HelpStatementType
{
    HELPSTATETYPE_SET=0,
    HELPSTATETYPE_CALL,
	HELPSTATETYPE_LABLE,
    HELPSTATETYPE_ERROR
};


//用来扫描脚本源文件
class PARSERS_EXPORT MyScanner : 
	public XMemory
  // ,public XMLBufferFullHandler
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    MyScanner
    (MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    virtual ~MyScanner();

	// -----------------------------------------------------------------------
	//  Error emitter methods
	// -----------------------------------------------------------------------
	bool emitErrorWillThrowException(const XMLErrs::Codes toEmit);
	void emitError(const XMLErrs::Codes toEmit);
	void emitError
		(
		  const XMLErrs::Codes      toEmit
		, const XMLCh* const        text1
		, const XMLCh* const        text2 = 0
		, const XMLCh* const        text3 = 0
		, const XMLCh* const        text4 = 0
		);
	void emitError
		(
		  const XMLErrs::Codes      toEmit
		, const char* const         text1
		, const char* const         text2 = 0
		, const char* const         text3 = 0
		, const char* const         text4 = 0
		);
	void emitError
		(
		  const XMLErrs::Codes      toEmit
		, const XMLExcepts::Codes   originalErrorCode
		, const XMLCh* const        text1 = 0
		, const XMLCh* const        text2 = 0
		, const XMLCh* const        text3 = 0
		, const XMLCh* const        text4 = 0
		);

	// -----------------------------------------------------------------------
    //  Document scanning methods
    //
    //  scanDocument() does the entire source document. 
    // -----------------------------------------------------------------------

	void scanDocument                         
		(
		const   InputSource&    src
		);

	void scanDocument
		(
		const   XMLCh* const    systemId
		);

	void scanDocument
		(
		const   char* const     systemId
		);


	//重置扫描参数
	void scanReset(const InputSource& src);                       


    //方法描述:此方法用来扫描预定义语句
    //
    //返回值:
    //返回类型              描述
    //void                 无返回类型
    //
    //参数说明:
    //参数名称              参数类型                              描述
    //DependList           DependBlockList* const                运行时头文件管理类 
    void scanPredefined(DependBlockList* const DependList);         

    //方法描述:此方法用来扫include语句
    //
    //返回值:
    //返回类型              描述
    //void                 无返回类型
    //
    //参数说明:
    //参数名称              参数类型                              描述
    //DependList           DependBlockList* const                运行时头文件管理类 
	void scanInclude(DependBlockList* const DependList);                    

    //方法描述:此方法用来扫import语句
    //
    //返回值:
    //返回类型              描述
    //void                 无返回类型
    //
    //参数说明:
    //参数名称              参数类型                              描述
    //DependList           DependBlockList* const                运行时头文件管理类 
	void scanImport(DependBlockList* const DependList);      

    //
    //方法描述:扫描注释,扫描所有的注释,直接跳过
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //void scanAnnotation();            //这里战时注释掉,因为注释已经在内部做了处理 -match 8-29

    //方法描述:包含了扫描命名空间里面所有的东西,包含命名空间
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称       参数类型                              描述
    //list           MyBlockListPool* const                运行时块池管理类
    void scanContent(MyBlockListPool* const pool);               

    //方法描述:扫描命名空间
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称       参数类型                              描述
    //list           MyBlockListPool* const                运行时块池管理类
	void scanNameSpace(MyBlockListPool* const pool);              
    
    //方法描述:扫描函数定义
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称                参数类型                     描述
    //parentBlock             BaseBlock*                   父块
	void scanFunction(BaseBlock* parentBlock);           

    //方法描述:扫描函数返回语句
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称                参数类型                     描述
    //parentBlock             BaseBlock*                   父块
	void scanReturn(BaseBlock* parentBlock);   

	//方法描述:扫描if语句, isInWhileOrRepeat为真表示 if语句如果包含在while or repeat里面的话,就可以包含break 和 continue 语句
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称                参数类型                     描述
    //parentBlock             BaseBlock*                   父块
    //isInWhileOrRepeat       bool                         是否包含在while语句中的标识位
    void scanIf(BaseBlock* parentBlock,bool isInWhileOrRepeat=false);                              

	//方法描述:扫描select语句, isInWhileOrRepeat为真表示 select语句如果包含在while or repeat里面的话,就可以包含break 和 continue 语句
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称                参数类型                     描述
    //parentBlock             BaseBlock*                  父块
    //isInWhileOrRepeat       bool                        是否包含在while语句中的标识位
    void scanSelect(BaseBlock* parentBlock,bool isInWhileOrRepeat=false);  


	//方法描述:扫描case语句, isInWhileOrRepeat为真表示 select语句如果包含在while or repeat里面的话,就可以包含break 和 continue 语句
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称                参数类型                     描述
    //parentBlock             BaseBlock*                  父块
    //isInWhileOrRepeat       bool                        是否包含在while语句中的标识位
    void scanCase(BaseBlock* parentBlock,bool isInWhileOrRepeat=false); 


	//方法描述:扫描case语句, isInWhileOrRepeat为真表示 select语句如果包含在while or repeat里面的话,就可以包含break 和 continue 语句
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称                参数类型                     描述
    //parentBlock             BaseBlock*                  父块
    //isInWhileOrRepeat       bool                        是否包含在while语句中的标识位
    void scanDefault(BaseBlock* parentBlock,bool isInWhileOrRepeat=false); 

	//方法描述:扫描else语句, isInWhileOrRepeat为真表示 if语句如果包含在while or repeat里面的话,就可以包含break 和 continue 语句
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称                参数类型                     描述
    //parentBlock             BaseBlock*                   父块
    //isInWhileOrRepeat       bool                         是否包含在while语句中的标识位
	void scanElse(BaseBlock* parentBlock,bool isInWhileOrRepeat=false);   

    //方法描述:扫描while语句
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称                参数类型                     描述
    //parentBlock             BaseBlock*                  父块
	void scanWhile(BaseBlock* parentBlock);                            

    //方法描述:扫描break语句
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称                参数类型                     描述
    //parentBlock             BaseBlock*                  父块
	void scanBreak(BaseBlock* parentBlock);   

    //方法描述:扫描continue语句
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称                参数类型                     描述
    //parentBlock             BaseBlock*                  父块
	void scanContinue(BaseBlock* parentBlock);   

    //方法描述:扫描repeat语句
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称                参数类型                     描述
    //parentBlock             BaseBlock*                  父块
	void scanRepeat(BaseBlock* parentBlock);   
 
    //方法描述:扫描Var变量定义语句
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称                参数类型                     描述
    //parentBlock             BaseBlock*                  父块
    //type                    MyValue::DataType           变量类型
	void scanVar(BaseBlock* parentBlock);


    //方法描述:扫描全局Var变量定义语句
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称                参数类型                     描述
    //parentBlock             BaseBlock*                  父块
	void scanGlobalVar(BaseBlock* parentBlock);


    //方法描述:扫描Assignment语句
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称                参数类型                     描述
    //assign                  const XMLCh * const         脚本一行的内容
    //parentBlock             BaseBlock*                  父块
	void scanAssignment(const XMLCh * const assign,BaseBlock* parentBlock);

    //方法描述:扫描Call语句
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称                参数类型                     描述
    //call                    const XMLCh * const         脚本一行的内容
    //parentBlock             BaseBlock*                  父块
	void scanCall(const XMLCh * const call,BaseBlock* parentBlock);

	//方法描述:扫描Lable语句
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称                参数类型                     描述
    //call                    const XMLCh * const         脚本一行的内容
    //parentBlock             BaseBlock*                  父块
	void scanLable(const XMLCh * const lable,BaseBlock* parentBlock);


    //方法描述:扫描Goto语句
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称                参数类型                     描述
    //call                    const XMLCh * const         脚本一行的内容
    //parentBlock             BaseBlock*                  父块
	void scanGoto(BaseBlock* parentBlock);


    //方法描述:判断是否是一个语句,这里用来判断是不是 set语句 和 call语句
    //
    //返回值:
    //返回类型                描述
    //HelpStatementType       脚本上一行内容的语句类型
    //
    //参数说明:
    //参数名称                参数类型                     描述
    //state                    const XMLCh * const        脚本一行的内容
    //parentBlock             BaseBlock*                  父块
    HelpStatementType getStatementType(const XMLCh * const state,BaseBlock* parentBlock);
    

    void setErrorReporter(XMLErrorReporter* const errHandler);

    void setErrorHandler(ErrorHandler* const handler);

	void setStandardUriConformant(const bool newValue);

	const XMLErrorReporter* getErrorReporter() const;

    XMLErrorReporter* getErrorReporter();

    const ErrorHandler* getErrorHandler() const;

    ErrorHandler* getErrorHandler();

	void setCalculateSrcOfs(const bool newValue);
    bool getCalculateSrcOfs() const;
    // -----------------------------------------------------------------------
    //  Mutator methods
    // -----------------------------------------------------------------------
    void incrementErrorCount(void);			// For use by XMLValidator

    // -----------------------------------------------------------------------
    //  Notification that lazy data has been deleted
    // -----------------------------------------------------------------------
	//static void reinitScannerMutex();
	//static void reinitMsgLoader();

private:
	bool isLable(const XMLCh * const lable,BaseBlock* parentBlock);              //判断是不是标记语句
    bool isAssignment(const XMLCh * const assign,BaseBlock* parentBlock);         //判断是不是赋值语句
    bool isCall(const XMLCh * const call,BaseBlock* parentBlock);                 //判断是不是函数调用语句

    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    MyScanner(const MyScanner&);
    MyScanner& operator=(const MyScanner&);

    // -----------------------------------------------------------------------
    //  Private helper methods
    // -----------------------------------------------------------------------
    void commonInit();
    void cleanUp();

	unsigned int                fScannerId;
    int                         fErrorCount;
    bool                        fCalculateSrcOfs;
    bool                        fStandardUriConformant;
    XMLUInt32                   fSequenceId;
    bool                        fInException;
    bool                        fExitOnFirstFatal;
	XMLErrorReporter*           fErrorReporter;
	ErrorHandler*               fErrorHandler;
    MyReaderMgr                 fReaderMgr;                         //自定义的文件读取类
    MemoryManager*              fMemoryManager;
    XMLBufferMgr                fBufMgr;
};

inline void MyScanner::setStandardUriConformant(const bool newValue)
{
    fStandardUriConformant = newValue;
    fReaderMgr.setStandardUriConformant(newValue);
}

// ---------------------------------------------------------------------------
//  XMLScanner: Mutator methods
// ---------------------------------------------------------------------------

inline void MyScanner::incrementErrorCount()
{
    ++fErrorCount;
}


inline bool MyScanner::getCalculateSrcOfs() const
{
    return fCalculateSrcOfs;
}

inline void MyScanner::setCalculateSrcOfs(const bool newValue)
{
    fCalculateSrcOfs = newValue;
}

MATCH_CPP_NAMESPACE_END


#endif