//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-06-26  Ф�㷶      ��ɿ�ű�ɨ����MyEventScanner�����
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

//�ռ���Ϣ
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

//�ļ���Ϣ
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

//����ɨ��ű�Դ�ļ�
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

	//����ɨ�����
	void scanReset(const InputSource& src);                       
        
    //��������:������ɨ�������ռ��������еĶ���,���������ռ�
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������       ��������                              ����
    //list           MyBlockListPool* const                ����ʱ��ع�����
    void scanContent(const XMLCh* const ns=0,const XMLCh* const fun=0);               

    //��������:ɨ�������ռ�
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������       ��������                              ����
    //list           MyBlockListPool* const                ����ʱ��ع�����
	void scanNameSpace(const XMLCh* const ns=0,const XMLCh* const fun=0);              
    
    //��������:ɨ�躯������
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������                ��������                     ����
    //parentBlock             BaseBlock*                   ����
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

	//��������:ɨ��Var�����������
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������                ��������                     ����
    //parentBlock             BaseBlock*                  ����
    //type                    MyValue::DataType           ��������
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
    MyReaderMgr                 fReaderMgr;                         //�Զ�����ļ���ȡ��
    MemoryManager*              fMemoryManager;
    XMLBufferMgr                fBufMgr;
public:
    MySourceInfor               fScanInfor;

	/////////�õ��ռ�ͺ�����λ��,����Ŀ����������ʱ�õ�///////////////
	unsigned int fNSLineNum;
    unsigned int fNSEndLineNum;
    unsigned int fFunLineNum;
    unsigned int fFunEndLineNum;
    bool fFindNS;
    bool fFindFun;
    bool fFindNSEnd;
    bool fFindFunEnd;
	/////////�õ��ռ�ͺ�����λ��,����Ŀ����������ʱ�õ�///////////////

	////////��֤���º������߱�����ʱ��ʹ��//////
	 XMLCh fFunParam[MAXPARAMLENGTH];
	 bool fIsFindVar;
	 bool fFindVar;
	 const XMLCh *fWantFindNS;
	 const XMLCh *fWantFindVar;
	////////��֤���º������߱�����ʱ��ʹ��//////


    ///////////////////��֤GOTO����Ƿ�Ϸ�/////
	std::vector<MyLableStr>   fLableManager;
	MyLableInfor              fToFindLable;
	XMLFileLoc                fHelplineNum;
	bool                      fValidateGoto;
	bool                      fIsFindOver;
    ///////////////////��֤GOTO����Ƿ�Ϸ�/////

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