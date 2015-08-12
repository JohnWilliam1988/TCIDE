#if !defined(MYEXCUTOR_H)
#define MYEXCUTOR_H

#include <match/util/MatchDefs.hpp>
#include <match/util/PlatformUtils.hpp>
#include "MyGlobalVarList.h"
#include "MyBlockListPool.h"

MATCH_CPP_NAMESPACE_BEGIN

class PARSERS_EXPORT MyExcutor : public XMemory
{
public :
    static MemoryManager*              fMemoryManager;
    
    ////��������
    //virtual ~MyExcutor();

	//��������: ��������ִ�нӿ�
	//����ֵ    void
    //����˵��:
    //��������      ��������                    ����
	//��
    //static void execute();

    //��������: ��������ִ�нӿ�
    //����ֵ    void
    //����˵��:
    //��������      ��������                    ����
    //nameSpace         const XMLCh * const         Ҫִ�е������ռ�
    //funcName          const XMLCh * const         Ҫִ�еĺ�����
    static bool execute(const XMLCh * const nameSpace,const XMLCh* const funcName);

	//��������: ��ȡȫ�ֱ�������
	//����ֵ    void
    //����˵��:
    //��������      ��������                    ����
	//��
	static void getGlobalVarList() ; 

	//��������: ���õ�ǰ����Ϊmain����
	//����ֵ    ģ��ָ��
	//��������                ����
    //const BaseBlock *       main�Ŀ�ָ��
    //����˵��:
    //��������      ��������                    ����
	//��
    //static const BaseBlock * getMailFcuntion() ; 

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
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    //���캯��
    //����˵��:
    //��������      ��������                    ����
    //manager       MemoryManager* const        �ڴ������
    MyExcutor();

	//MyExcutor(const MyExcutor& excutor);
	//MyExcutor& operator=(const MyExcutor& toAssign);
};

MATCH_CPP_NAMESPACE_END

#endif