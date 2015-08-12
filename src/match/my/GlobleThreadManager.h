#if !defined(_MYGLOBLETHREADMANAGER_H_)
#define _MYGLOBLETHREADMANAGER_H_

#include <match/util/MatchDefs.hpp>
#include <match/util/PlatformUtils.hpp>
#include <windows.h>

#include "MyThreadsManager.h"

MATCH_CPP_NAMESPACE_BEGIN

class XMLPARSER_EXPORT CGlobleThreadManager
{
public:
	//��ʼ��
	static void initialize();
	//����
	static void cleanUp();

	static HANDLE getThreadHandle(DWORD threadID);

	//���µ��߳̾����ID�Լ�״̬���뵽�߳�ID����
	static void  addNewHandle(HANDLE handle,DWORD id);

	//ɾ���̹߳���ڵ�
	static void  removeThreadNode(DWORD threadID);

	//ɾ���̹߳���ڵ�
	static void  removeThreadByHandle(HANDLE hnd);

    //���߳�ID��״̬λ��λ-ִ��
    static bool setThreadFlag(HANDLE hnd,bool isStop);

	//��λֹͣ��Ƿ�
	static void setStopFlag();

	//��ȡֹͣ��Ƿ�״̬
	static bool getStopFlagState();

	//�Ƿ�ر��߳�
	static bool isCloseThread();

	//�����߳�ID����̵߳����ٹ���
	static void setDestoryByID(DWORD threadID,const XMLCh * const ns,const XMLCh * const fun);

	//
	static void runDestoryFunByID(DWORD threadID);

	//����GOTO���
	static void  setGotoLable(DWORD threadID, const XMLCh *  lable);

	//����GOTO���
	static void  resetGotoLable(DWORD threadID);

	static bool getGotoLable(DWORD threadID,  XMLCh *  lable);

public:
	static bool                                     gStop;              //������ִ���̵߳ı��λ
    static CMyThreadsManager                        gThreadManager;          //�̹߳�����

private:
	CGlobleThreadManager(void);

	~CGlobleThreadManager(void);
};

MATCH_CPP_NAMESPACE_END
#endif