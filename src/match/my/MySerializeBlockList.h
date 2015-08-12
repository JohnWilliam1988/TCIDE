#if !defined(MYSERIALIZEBLOCKLIST_H)
#define MYSERIALIZEBLOCKLIST_H

#include <match/util/MatchDefs.hpp>
#include <match/util/PlatformUtils.hpp>
#include "MyRuntimeManager.h"
#include "MyBlockListPool.h"
#include "MyIncludeBlock.h"
#include "MyNameSpaceBlock.h"
#include "MyEndNameSpaceBlock.h"
#include "MyFunctionBlock.h"
#include "MyEndFunctionBlock.h"
#include "MyIfBlock.h"
#include "MyElseBlock.h"
#include "MyEndIfBlock.h"
#include "MyVarBlock.h"
#include "MyAssignmentBlock.h"
#include "MyCallBlock.h"
#include "MyRepeatBlock.h"
#include "MyEndRepeatBlock.h"
#include "MyWhileBlock.h"
#include "MyEndWhileBlock.h"
#include "MyEndWhileBlock.h"
#include "MyBreakBlock.h"
#include "MyContinueBlock.h"
#include "MyReturnBlock.h"

MATCH_CPP_NAMESPACE_BEGIN

const unsigned int MAXTWINLENGTH=256;

class PARSERS_EXPORT MySerializeBlockList
{
public:
	//��ʼ��
	static void initialize();
	//����
	static void cleanUp();

	//��������: ���л�д�����
	//����ֵ    bool
    //��������                ����
    //bool                    ���л��ɹ�����ʧ��
    //����˵��:
    //��������      ��������                    ����
	//path       const XMLCh* const             �ļ�·��
	static bool serializeList(const XMLCh* const path,const unsigned char* const key=NULL);

	//��������: �����л��ļ���ʹ֮��Ϊģ������
	//����ֵ    bool
    //��������                ����
    //bool                    �����л��ɹ�����ʧ��
    //����˵��:
    //��������      ��������                    ����
	//path       const XMLCh* const             �ļ�·��
	static bool deserializeFileToBlockList(const XMLCh* const path);

	//��������: �����л������ļ�
	//����ֵ    bool
    //��������                ����
    //bool                    �����л��ɹ�����ʧ��
    //����˵��:
    //��������      ��������                    ����
	//path       const XMLCh* const             �ļ�·��
	static bool deserializeImportFile(const XMLCh* const path);


	//��������: ���ñ�����ص�����·��
	//����ֵ    bool
    //��������                ����
    //bool                    �����л��ɹ�����ʧ��
    //����˵��:
    //��������      ��������                    ����
	//compilepath    const XMLCh* const         ����·��
	//temppath       const XMLCh* const         ��ʱ·��
	static bool setCompilePath(const XMLCh* const compilepath,const XMLCh* const srcPath);

	static void initialSrcTll();

private:
	//����һ�������ռ�ķ���
	static void traversalBlocklist( const BaseBlock * templist);
	//д��һ������
	static void serializeWriteFileLine(const XMLCh* const word);
	//��ȡһ������
	static XMLCh* deserializeReadFileLine();
	//��ȡһ������
	static XMLCh* ReadFileLine();

	static void  scanfFunctionBlock(BaseBlock * parentBlock,XMLCh * filepath);
	static void  scanfVarBlock( BaseBlock * parentBlock,XMLCh * filepath);
	static void  scanfAssigBlock( BaseBlock * parentBlock,XMLCh * filepath);
	static void  scanfCallBlock( BaseBlock * parentBlock,XMLCh * filepath);
	static void  scanfIfBlock( BaseBlock * parentBlock,XMLCh * filepath);
    static void  scanfElseBlock( BaseBlock * parentBlock,XMLCh * filepath,bool &isElseEnd);
	static void  scanfSelectBlock( BaseBlock * parentBlock,XMLCh * filepath);
    static void  scanfCaseBlock( BaseBlock * parentBlock,XMLCh * filepath);
    static void  scanfDefaultBlock( BaseBlock * parentBlock,XMLCh * filepath);
	static void  scanfWhileBlock( BaseBlock * parentBlock,XMLCh * filepath);
	static void  scanfRepeatBlock( BaseBlock * parentBlock,XMLCh * filepath);
	static void  scanfReturnBlock( BaseBlock * parentBlock,XMLCh * filepath);
	static void  scanfBreakBlock( BaseBlock * parentBlock,XMLCh * filepath);
	static void  scanfContinueBlock(BaseBlock * parentBlock,XMLCh * filepath);
	static void  scanfLableBlock(BaseBlock * parentBlock,XMLCh * filepath);
	static void  scanfGotoBlock(BaseBlock * parentBlock,XMLCh * filepath);

	static HANDLE fWriteFile;
    static HANDLE fReadFile;
	static XMLCh *fBuffer;
	static XMLCh *fBufferEnd;

	static XMLCh fCompilePath[MAX_PATH];
	static XMLCh fSourcePath[MAX_PATH];
	static XMLCh fTWinName[MAXTWINLENGTH][NAMEMAXLENGTH];
	static unsigned fTWinLength;
	MySerializeBlockList(void);
	~MySerializeBlockList(void);
};


MATCH_CPP_NAMESPACE_END

#endif

