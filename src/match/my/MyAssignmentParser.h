#if !defined(MYASSIGNMENTPARSER_H)
#define MYASSIGNMENTPARSER_H

#include"AbsBlockParser.h"
#include"BaseBlock.h"
#include"MyVarList.h"
#include"MyVarListContainer.h"

MATCH_CPP_NAMESPACE_BEGIN

//���������ֵ�����
class PARSERS_EXPORT MyAssignmentParser : public AbsBlockParser
{
public:
	//���캯��
    //����˵��:
    //��������      ��������                    ����
	//nameSpace     const XMLCh* const          �����ռ���
	//Block         const BaseBlock *           ģ���ָ��
    //manager       MemoryManager* const        �ڴ������
	MyAssignmentParser(const XMLCh * const nameSpace
		        ,const BaseBlock * Block
		        ,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager);

	~MyAssignmentParser();

	static void initializeStaticData();                                             //��ʼ��

	//��������: ��ֵ����ִ�й���
	//����ֵ    ��
    //����˵��:
    //��������      ��������                    ����
	//VarList     MyVarListContainer*          ��������ָ��
	void run(MyVarList * const VarList,MyVarListContainer* VarListCon);

	//��������: ��ֵ���GOTO��ִ�в���
	//����ֵ    ��
    //����˵��:
    //��������      ��������                    ����
	//VarList     MyVarListContainer*          ��������ָ��
	void gotoRun(MyVarList * const VarList,MyVarListContainer* VarListCon);

private:
    void initialize();                                             //��ʼ��
    void cleanUp();                                                //�������
	
	//CCF �������캯��
	MyAssignmentParser(const MyAssignmentParser& pool);
	MyAssignmentParser& operator=(const MyAssignmentParser& toAssign);
};


MATCH_CPP_NAMESPACE_END



#endif