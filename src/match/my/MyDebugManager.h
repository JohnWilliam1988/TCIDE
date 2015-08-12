#if !defined(MYEVENTSYNC_H)
#define MYEVENTSYNC_H

#include <match/util/MatchDefs.hpp>
#include <match/util/PlatformUtils.hpp>
#include <windows.h>

#include "MyBPList.h"
#include "MyVarListContainer.h"

MATCH_CPP_NAMESPACE_BEGIN


struct PARSERS_EXPORT CMyDebugInfor
{
    int                              fDebugType;			                         //ִ�����ͣ���һö������
	int                              fLeng;
	MyBreakPointInfo                 fList[MATCH_MAXBREAKBPNUM];
	MyBreakPointInfo                 fCurrentBP;
	XMLCh                            fVarInfo[MAXDEBUGVARBUFFLENGTH];
};

class PARSERS_EXPORT CMyDebugManager
{
public:
	//�������߽�����ִ�л��ǵ��ԣ��������Ի��߶ϵ���ԣ�
	enum MatchDebugFalg
	{
		MATCH_NODEBUG,                //������
		MATCH_STEPBYSTEP,             //��������
        MATCH_BREAKDEBUG,             //�ϵ����
	};

	//��ʼ��
	static void initialize();

	//����
	static void cleanUp();

	//��������:���������Կ��ƾ��,�ý���������ִ��
	//����ֵ:��
	//��������                ����
	//void                    ��
	//����˵��:
	//��������      ��������                    ����
	//��
	static void  setLibEvent();

    //��������:��ȡ���������Կ��ƾ��
    //����ֵ:   �¼����
    //��������                ����
    //HANDLE                  �¼����
    //����˵��:
    //��������      ��������                    ����
    //��
    static HANDLE getLibEventHandle();

    //��������:���ƽ������ʱ��ľ��
    //����ֵ:       �¼����
    //��������                 ����
    //HANDLE                   �¼����
    //����˵��:
    //��������      ��������   ����
    //��
    static HANDLE getWinEventHandle();

	//��������:���õ��Ե�����
	//����ֵ:  void
	//��������                          ����
	//void
	//����˵��:
	//��������      ��������                    ����
	//style         MatchDebugFalg              ����ִ�е�����
    static void   setDebugStyle(MatchDebugFalg style=MatchDebugFalg::MATCH_NODEBUG);

	//��������:��ȡִ�е�����
	//MATCH_NODEBUG,              ������
	//MATCH_STEPBYSTEP,           ��������
    //MATCH_BREAKDEBUG,           �ϵ����
	//��������                          ����
	//MatchDebugFalg                    ����ִ�е�����
	//����˵��:
	//��������      ��������            ����
	//��
	static MatchDebugFalg  getDebugStyle();

    //��������:���õ�ǰִ�еĶϵ�ڵ�
    //����ֵ:  void
    //��������                          ����
    //void
    //����˵��:
    //��������      ��������            ����
    //Node          MyBreakPointInfo*   ������Ϣ�ڵ�
    static void  setCurrentBPTransport(const XMLCh* const path,XMLUInt32 lineNum);

	//��������:��ǰ��Ч�����б�Ĵ��ݣ�Ϊ�����鿴����׼��
	//����ֵ:       void
	//��������                          
	///void     
	//����˵��:
	//��������      ��������                    ����
	//namesp        const XMLCh* const          �����ռ���
	//VarList       MyVarListContainer*         �ֲ���Ч�����б�
	static void varListTransport(const XMLCh* const namesp,MyVarListContainer* VarList,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager);

	//��������: �ж�һ���ڵ��Ƿ����,
	static bool isExist(const XMLCh* const filepath,int linenum);

	// �ж��б��Ƿ�Ϊ��
	static bool isEmpty();

public:
    static HANDLE						   fLibEventHandle;                              //�������еĵ��Դ����¼�,����ϵ��жϵ����
    static HANDLE						   fWinEventHandle;		                         //�����ͨ�ŵ��¼������߽����Ѿ����ϵ���
    static MatchDebugFalg                  fDebugType;			                         //ִ�����ͣ���һö������
	static HANDLE	        m_hFileMap;            //ӳ���ļ�
	static CMyDebugInfor*	m_pBuffer;             //ӳ���ļ��׵�ַ

protected:
	//���캯��
	CMyDebugManager();
};

MATCH_CPP_NAMESPACE_END

#endif
