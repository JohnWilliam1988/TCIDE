//////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-06-26  肖秀范      完成块脚本扫描类MyEventScanner的设计
//////////////////////////////////////////////////////////////////////////////////////////
#if !defined(MyEventScanner_H)
#define MyEventScanner_H

#include <match/util/PlatformUtils.hpp>
#include <match/framework/XMLErrorReporter.hpp>
#include <match/sax/ErrorHandler.hpp>
#include <match/framework/XMLErrorCodes.hpp>
#include <match/framework/XMLBufferMgr.hpp>

#include "MyReaderMgr.h"
#include "BlockList.h"
#include "MyBlockListPool.h"
#include "DependBlockList.h"
#include "MyValue.h"
#include "MyRuntimeManager.h"
#include <vector>
MATCH_CPP_NAMESPACE_BEGIN


class PARSERS_EXPORT MyFunInfor
{
public:
    MyFunInfor();
public:
    XMLCh fFunName[NAMEMAXLENGTH];
    XMLCh fFunParam[MAXPARAMLENGTH];
    int fLine;
};

//空间信息
class PARSERS_EXPORT MyNsInfor
{
public:
    MyNsInfor();
    unsigned int getLength() const;
    const XMLCh * getNSName() const;
    int getLine() const;
    const MyFunInfor * getFunPrototype(unsigned int index) const;

    bool setNsName(const XMLCh * const name,int line);
    bool addFunName(const XMLCh * const funname,const XMLCh * const param,int line);
private:  
    XMLCh fNS[NAMEMAXLENGTH];
    MyFunInfor fFunList[MAXFUNLISTLENGTH];
    int fFunLength;
    int fLine;
};

//文件信息
class PARSERS_EXPORT MySourceInfor
{
public:
    MySourceInfor();
    unsigned int getLength() const; 
    bool addNameSpace(const XMLCh * const ns,int line);
    bool addNsAndFun(const XMLCh * const ns,const XMLCh * const fun,const XMLCh * const param,int line); 

    const MyNsInfor * getNsInfor(unsigned int index) const; 
    int getLine(const XMLCh * const ns,const XMLCh * const fun);
    int getLine(const XMLCh * const ns);
private:
    MyNsInfor fNsList[MAXNSLISTLENGTH];
    int fNsLength;
};

//用来扫描脚本源文件
class PARSERS_EXPORT MyEventScanner : 
	public XMemory
  // ,public XMLBufferFullHandler
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    MyEventScanner
    (MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    virtual ~MyEventScanner();


	// -----------------------------------------------------------------------
    //  Document scanning methods
    //
    //  scanDocument() does the entire source document. 
    // -----------------------------------------------------------------------

	void scanDocument                         
		(
		const   InputSource&    src,const XMLCh* const ns=0,const XMLCh* const fun=0
		);

	void scanDocument
		(
		const   XMLCh* const    systemId,const XMLCh* const ns=0,const XMLCh* const fun=0
		);

	void scanDocument
		(
		const   char* const     systemId,const XMLCh* const ns=0,const XMLCh* const fun=0
		);

	void setStandardUriConformant(const bool newValue);

	//重置扫描参数
	void scanReset(const InputSource& src);                       
        
    //方法描述:包含了扫描命名空间里面所有的东西,包含命名空间
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称       参数类型                              描述
    //list           MyBlockListPool* const                运行时块池管理类
    void scanContent(const XMLCh* const ns=0,const XMLCh* const fun=0);               

    //方法描述:扫描命名空间
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称       参数类型                              描述
    //list           MyBlockListPool* const                运行时块池管理类
	void scanNameSpace(const XMLCh* const ns=0,const XMLCh* const fun=0);              
    
    //方法描述:扫描函数定义
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称                参数类型                     描述
    //parentBlock             BaseBlock*                   父块
	void scanFunction(const XMLCh* const fun=0,const XMLCh * const inNS=0);  

	void scanNameSpaceByGoto();  

	void scanFunctionByGoto();  

	void scanIfByGoto();                              

    void scanElseByGoto();   

	void scanSelectByGoto();  

	void scanCaseByGoto(); 

	void scanDefaultByGoto(); 

	void scanWhileByGoto();                            

	void scanRepeatByGoto();   

	void scanLableByGoto(const XMLCh * const lable,std::vector<MyLableStr> *pLableVector);

	bool scanGotoByGoto();

	bool isLable(const XMLCh * const lable);   

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
	void scanGlobalVar(const XMLCh * const ns);

	void setCalculateSrcOfs(const bool newValue);
    bool getCalculateSrcOfs() const;

    // -----------------------------------------------------------------------
    //  Notification that lazy data has been deleted
    // -----------------------------------------------------------------------
	//static void reinitScannerMutex();
	//static void reinitMsgLoader();
private:
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    MyEventScanner(const MyEventScanner&);
    MyEventScanner& operator=(const MyEventScanner&);

    // -----------------------------------------------------------------------
    //  Private helper methods
    // -----------------------------------------------------------------------
    void commonInit();
    void cleanUp();

	unsigned int                fScannerId;
    bool                        fCalculateSrcOfs;
    bool                        fStandardUriConformant;
    XMLUInt32                   fSequenceId;
    bool                        fInException;
    bool                        fExitOnFirstFatal;
    MyReaderMgr                 fReaderMgr;                         //自定义的文件读取类
    MemoryManager*              fMemoryManager;
    XMLBufferMgr                fBufMgr;
public:
    MySourceInfor               fScanInfor;

	/////////得到空间和函数的位置,在项目里面插入代码时得到///////////////
	unsigned int fNSLineNum;
    unsigned int fNSEndLineNum;
    unsigned int fFunLineNum;
    unsigned int fFunEndLineNum;
    bool fFindNS;
    bool fFindFun;
    bool fFindNSEnd;
    bool fFindFunEnd;
	/////////得到空间和函数的位置,在项目里面插入代码时得到///////////////

	////////验证上下函数或者变量的时候使用//////
	 XMLCh fFunParam[MAXPARAMLENGTH];
	 bool fIsFindVar;
	 bool fFindVar;
	 const XMLCh *fWantFindNS;
	 const XMLCh *fWantFindVar;
	////////验证上下函数或者变量的时候使用//////


    ///////////////////验证GOTO语句是否合法/////
	std::vector<MyLableStr>   fLableManager;
	MyLableInfor              fToFindLable;
	XMLFileLoc                fHelplineNum;
	bool                      fValidateGoto;
	bool                      fIsFindOver;
    ///////////////////验证GOTO语句是否合法/////

};

inline void MyEventScanner::setStandardUriConformant(const bool newValue)
{
    fStandardUriConformant = newValue;
    fReaderMgr.setStandardUriConformant(newValue);
}

// ---------------------------------------------------------------------------
//  XMLScanner: Mutator methods
// ---------------------------------------------------------------------------

inline bool MyEventScanner::getCalculateSrcOfs() const
{
    return fCalculateSrcOfs;
}

inline void MyEventScanner::setCalculateSrcOfs(const bool newValue)
{
    fCalculateSrcOfs = newValue;
}

MATCH_CPP_NAMESPACE_END


#endif