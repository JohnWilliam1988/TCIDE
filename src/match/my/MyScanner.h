//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-06-26  Ф�㷶      ��ɿ�ű�ɨ����MyScanner�����
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


//����ɨ��ű�Դ�ļ�
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


	//����ɨ�����
	void scanReset(const InputSource& src);                       


    //��������:�˷�������ɨ��Ԥ�������
    //
    //����ֵ:
    //��������              ����
    //void                 �޷�������
    //
    //����˵��:
    //��������              ��������                              ����
    //DependList           DependBlockList* const                ����ʱͷ�ļ������� 
    void scanPredefined(DependBlockList* const DependList);         

    //��������:�˷�������ɨinclude���
    //
    //����ֵ:
    //��������              ����
    //void                 �޷�������
    //
    //����˵��:
    //��������              ��������                              ����
    //DependList           DependBlockList* const                ����ʱͷ�ļ������� 
	void scanInclude(DependBlockList* const DependList);                    

    //��������:�˷�������ɨimport���
    //
    //����ֵ:
    //��������              ����
    //void                 �޷�������
    //
    //����˵��:
    //��������              ��������                              ����
    //DependList           DependBlockList* const                ����ʱͷ�ļ������� 
	void scanImport(DependBlockList* const DependList);      

    //
    //��������:ɨ��ע��,ɨ�����е�ע��,ֱ������
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //void scanAnnotation();            //����սʱע�͵�,��Ϊע���Ѿ����ڲ����˴��� -match 8-29

    //��������:������ɨ�������ռ��������еĶ���,���������ռ�
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������       ��������                              ����
    //list           MyBlockListPool* const                ����ʱ��ع�����
    void scanContent(MyBlockListPool* const pool);               

    //��������:ɨ�������ռ�
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������       ��������                              ����
    //list           MyBlockListPool* const                ����ʱ��ع�����
	void scanNameSpace(MyBlockListPool* const pool);              
    
    //��������:ɨ�躯������
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������                ��������                     ����
    //parentBlock             BaseBlock*                   ����
	void scanFunction(BaseBlock* parentBlock);           

    //��������:ɨ�躯���������
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������                ��������                     ����
    //parentBlock             BaseBlock*                   ����
	void scanReturn(BaseBlock* parentBlock);   

	//��������:ɨ��if���, isInWhileOrRepeatΪ���ʾ if������������while or repeat����Ļ�,�Ϳ��԰���break �� continue ���
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������                ��������                     ����
    //parentBlock             BaseBlock*                   ����
    //isInWhileOrRepeat       bool                         �Ƿ������while����еı�ʶλ
    void scanIf(BaseBlock* parentBlock,bool isInWhileOrRepeat=false);                              

	//��������:ɨ��select���, isInWhileOrRepeatΪ���ʾ select������������while or repeat����Ļ�,�Ϳ��԰���break �� continue ���
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������                ��������                     ����
    //parentBlock             BaseBlock*                  ����
    //isInWhileOrRepeat       bool                        �Ƿ������while����еı�ʶλ
    void scanSelect(BaseBlock* parentBlock,bool isInWhileOrRepeat=false);  


	//��������:ɨ��case���, isInWhileOrRepeatΪ���ʾ select������������while or repeat����Ļ�,�Ϳ��԰���break �� continue ���
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������                ��������                     ����
    //parentBlock             BaseBlock*                  ����
    //isInWhileOrRepeat       bool                        �Ƿ������while����еı�ʶλ
    void scanCase(BaseBlock* parentBlock,bool isInWhileOrRepeat=false); 


	//��������:ɨ��case���, isInWhileOrRepeatΪ���ʾ select������������while or repeat����Ļ�,�Ϳ��԰���break �� continue ���
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������                ��������                     ����
    //parentBlock             BaseBlock*                  ����
    //isInWhileOrRepeat       bool                        �Ƿ������while����еı�ʶλ
    void scanDefault(BaseBlock* parentBlock,bool isInWhileOrRepeat=false); 

	//��������:ɨ��else���, isInWhileOrRepeatΪ���ʾ if������������while or repeat����Ļ�,�Ϳ��԰���break �� continue ���
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������                ��������                     ����
    //parentBlock             BaseBlock*                   ����
    //isInWhileOrRepeat       bool                         �Ƿ������while����еı�ʶλ
	void scanElse(BaseBlock* parentBlock,bool isInWhileOrRepeat=false);   

    //��������:ɨ��while���
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������                ��������                     ����
    //parentBlock             BaseBlock*                  ����
	void scanWhile(BaseBlock* parentBlock);                            

    //��������:ɨ��break���
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������                ��������                     ����
    //parentBlock             BaseBlock*                  ����
	void scanBreak(BaseBlock* parentBlock);   

    //��������:ɨ��continue���
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������                ��������                     ����
    //parentBlock             BaseBlock*                  ����
	void scanContinue(BaseBlock* parentBlock);   

    //��������:ɨ��repeat���
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������                ��������                     ����
    //parentBlock             BaseBlock*                  ����
	void scanRepeat(BaseBlock* parentBlock);   
 
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
	void scanVar(BaseBlock* parentBlock);


    //��������:ɨ��ȫ��Var�����������
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������                ��������                     ����
    //parentBlock             BaseBlock*                  ����
	void scanGlobalVar(BaseBlock* parentBlock);


    //��������:ɨ��Assignment���
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������                ��������                     ����
    //assign                  const XMLCh * const         �ű�һ�е�����
    //parentBlock             BaseBlock*                  ����
	void scanAssignment(const XMLCh * const assign,BaseBlock* parentBlock);

    //��������:ɨ��Call���
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������                ��������                     ����
    //call                    const XMLCh * const         �ű�һ�е�����
    //parentBlock             BaseBlock*                  ����
	void scanCall(const XMLCh * const call,BaseBlock* parentBlock);

	//��������:ɨ��Lable���
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������                ��������                     ����
    //call                    const XMLCh * const         �ű�һ�е�����
    //parentBlock             BaseBlock*                  ����
	void scanLable(const XMLCh * const lable,BaseBlock* parentBlock);


    //��������:ɨ��Goto���
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������                ��������                     ����
    //call                    const XMLCh * const         �ű�һ�е�����
    //parentBlock             BaseBlock*                  ����
	void scanGoto(BaseBlock* parentBlock);


    //��������:�ж��Ƿ���һ�����,���������ж��ǲ��� set��� �� call���
    //
    //����ֵ:
    //��������                ����
    //HelpStatementType       �ű���һ�����ݵ��������
    //
    //����˵��:
    //��������                ��������                     ����
    //state                    const XMLCh * const        �ű�һ�е�����
    //parentBlock             BaseBlock*                  ����
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
	bool isLable(const XMLCh * const lable,BaseBlock* parentBlock);              //�ж��ǲ��Ǳ�����
    bool isAssignment(const XMLCh * const assign,BaseBlock* parentBlock);         //�ж��ǲ��Ǹ�ֵ���
    bool isCall(const XMLCh * const call,BaseBlock* parentBlock);                 //�ж��ǲ��Ǻ����������

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
    MyReaderMgr                 fReaderMgr;                         //�Զ�����ļ���ȡ��
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