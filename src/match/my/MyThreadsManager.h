//�����ǿ���ڲ����̴߳���
#if !defined(_MYTHREADMANAGER_H_)
#define _MYTHREADMANAGER_H_

#include <match/util/MatchDefs.hpp>
#include <windows.h>

MATCH_CPP_NAMESPACE_BEGIN

//�̹߳���ڵ�
struct CMyThreadsNode
{
public:
	XMLCh                fFunDestory[NAMEMAXLENGTH];
	XMLCh                fFunArg[VALUEMAXLENGTH];
	XMLCh                fLable[NAMEMAXLENGTH];
	bool                 fIsGoto;
    HANDLE               fHandle;
    DWORD                fThreadID;
    bool                 fStop;
    CMyThreadsNode()
	{
		::memset(fFunArg,0,VALUEMAXLENGTH*sizeof(XMLCh));
		::memset(fFunDestory,0,NAMEMAXLENGTH*sizeof(XMLCh));
		::memset(fLable,0,NAMEMAXLENGTH*sizeof(XMLCh));
		fIsGoto=false;
		fStop=false;
		fHandle=NULL;
		fThreadID=NULL;
	}
};

//======================================================================================================
//����ʱ,�̹߳�����,�������Ҫ��������������ʱ,�̵߳�ֹͣ,�ͼ�¼�̵߳�ID,���
//======================================================================================================
class PARSERS_EXPORT CMyThreadsManager
{
public:
    CMyThreadsManager(void);

    ~CMyThreadsManager(void);

    //��ʼ��
    void initialize();
    //���
    void cleanUp();

    void addNewHandle(HANDLE handle,DWORD id);

	bool makeSureNewThread();

    HANDLE getThreadHandle(DWORD threadID);

	//�����߳�ID����̵߳����ٹ���
	void  setDestoryByID(DWORD threadID,const XMLCh * const fun,const XMLCh * const arg);

	//�����߳�ID����̵߳����ٹ���
	void  getDestoryByID(DWORD threadID, XMLCh *  fun, XMLCh *  arg);

	void removeThreadID(DWORD threadID);

	void removeThreadByHandle(HANDLE handle);

	bool setThreadStopFlag(HANDLE handle,bool stop);

	bool isCloseThread(DWORD threadID);
		
	//����GOTO���
	void  setGotoLable(DWORD threadID,const XMLCh *  lable);

	//����GOTO���
	void  resetGotoLable(DWORD threadID);

	//�жϲ�����gotoָ���ľ��
	bool getGotoLable(DWORD threadID, XMLCh *  lable);

    bool haveDestoryFun(DWORD threadID);

private:
	CMyThreadsNode*   fThreadPool;
	int               fLength;
	int               fMaxLength;
};

MATCH_CPP_NAMESPACE_END

#endif
