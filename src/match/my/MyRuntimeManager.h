///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-06-26  Ф�㷶      �������ʱ������MyRuntimeManager,����ʱ���еĲ����������������������
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYRUNTIMEMANAGER_H
#define MYRUNTIMEMANAGER_H

#include <match/util/PlatformUtils.hpp>
#include <wchar.h>
#include <windows.h>
#include "BaseBlock.h"

MATCH_CPP_NAMESPACE_BEGIN

class MemoryManager;
class MyGlobalVarListManager;
class MyBlockListPool;
class BaseBlock;
class DependBlockList;
class MyFunInfor;
class MyComFunInfor;

//MyRuntimeStruct ������¼����ʱ��һЩ��������,����ȫ�ֱ���,�����ռ�,��ǰ���п�,my����������ǰ,�����ȳ��Ի���ǰ��
class XMLPARSER_EXPORT MyRuntimeManager: public XMemory
{
public:
	static MemoryManager*                     fMemoryManager;                   //�ڴ����
	static MyGlobalVarListManager *           fGlobalVarList;                   //�洢ȫ�ֱ�����
	static MyBlockListPool *                  fListPool;                        //�����洢ִ�п����
	static DependBlockList *                  fDependentFile;                   //��һ��������õ����е������ļ�
    static HWND                               fWndHandle;                       //��ǰ����ľ��,�����ؼ��ͽ���ͨѶ��
    static XMLCh                              fTempPath[MAX_PATH];              //��ǰ����ʱĿ¼��·��

    //��ȡ���ڵľ��
    static HWND getWndHandle();

    //���ô��ڵľ��
    static void setWndHandle(HWND handle);

    //
    //��������:�жϿռ����Ƿ��Ѿ��ڱ���API�д���
    //����ֵ:������ ��ʾ����,�� ��ʾ������
    //��������                ����
    //bool                   ������ ��ʾ����,�� ��ʾ������
    //����˵��:
    //��������          ��������                    ����
    //nameSpace         const XMLCh * const         Ҫ���ҵ������ռ�
	static bool isNameSpaceExist(const XMLCh * const nameSpace);

    //
    //��������:�ڿ���в���һ����
    //����ֵ:�ҵ��Ŀ�
    //��������                ����
    //const BaseBlock *       �����ҵ��Ŀ�,���� NULL,��ʾʧ��
    //����˵��:
    //��������          ��������                    ����
    //nameSpace         const XMLCh * const         Ҫ���ҵ������ռ�
    //funcName          const XMLCh * const         Ҫ���ҵĺ�����
	static const BaseBlock * findFuncBlockByName(const XMLCh * const nameSpace,const XMLCh* const funcName);

	//
    //��������:�������ļ�,��ͷ�ļ��в��Һ���ԭ��,�����Ҫ��Ϊ�˽�����»���,�����ļ���������������
	//����ֵ:�ҵ��Ŀ�
	//��������                ����
	//const BaseBlock *       �����ҵ��Ŀ�,���� NULL,��ʾʧ��
	//����˵��:
	//��������          ��������                    ����
	//nameSpace        const XMLCh * const         Ҫ���ҵ������ռ�
	//funcName         const XMLCh * const         Ҫ���ҵĺ�����
	//pFun             MyFunInfor *                ����ԭ����Ϣ
	static bool findFunInforByName(const XMLCh * const nameSpace,const XMLCh* const funcName,XMLCh * funParam);


	//���ұ���
    static bool findVarInforByName(const XMLCh * const nameSpace,const XMLCh* const varName);


    //
    //��������:��ʼ��ִ�л���ľ�̬����
    //����ֵ:��
    //��������                ����
    //void                    ��
    //����˵��:
    //��������              ��������                    ����
	static void initializeAbsBlockStaticData();

    //
    //��������:����ִ�л���ľ�̬����
    //����ֵ:��
    //��������                ����
    //void                    ��
	static void TerminateAbsBlockStaticData();

    //
    //��������:��ʼ�����й�����
    //����ֵ:��
    //��������                ����
    //void                    ��
    //����˵��:
    //��������              ��������                    ����
    //memoryManager         MemoryManager* const        �ڴ�����Ա
	static void initialize(MemoryManager* const memoryManager = XMLPlatformUtils::fgMemoryManager);

    //
    //��������:�������й�����
    //����ֵ:��
    //��������                ����
    //void                    ��
	static void Terminate();

private:

    //��̬��,���췽��˽�л�
	MyRuntimeManager();
};


//��¼�����ı��
struct MyLableInfor
{
    XMLCh fNS[NAMEMAXLENGTH];
    XMLCh fFun[MAXPARAMLENGTH];
	XMLCh fLable[NAMEMAXLENGTH];
	MyLableInfor()
	{
		::wmemset(fNS,0,NAMEMAXLENGTH);
		::wmemset(fFun,0,NAMEMAXLENGTH);
		::wmemset(fLable,0,NAMEMAXLENGTH);
	}
};

struct MyLableStr
{
	XMLCh fLable[NAMEMAXLENGTH];
	MyLableStr()
	{
		::wmemset(fLable,0,NAMEMAXLENGTH);
	}
};

MATCH_CPP_NAMESPACE_END


#endif