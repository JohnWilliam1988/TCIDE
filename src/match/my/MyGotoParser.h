#if !defined(MYGOTOPARSER_H)
#define MYGOTOPARSER_H

#include"AbsBlockParser.h"
#include"BaseBlock.h"
#include"MyValue.h"
#include"MyDataList.h"
#include"MyVarList.h"

MATCH_CPP_NAMESPACE_BEGIN


//gotoִ�п�ĳ�����,�̳���AbsBlockParser
class PARSERS_EXPORT MyGotoParser : public AbsBlockParser
{

public:

	//���캯��
    //����˵��:
    //��������      ��������                    ����
	//nameSpace     const XMLCh* const          �����ռ���
	//Block         const BaseBlock *           ģ���ָ��
    //manager       MemoryManager* const        �ڴ������
	MyGotoParser(const XMLCh * const nameSpace,
		             const BaseBlock * Block,
		             MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager);

    //��������
	~MyGotoParser();

	//��������: gotoִ�нӿڡ�
	//����ֵ    ��
    //��������                ����
    //void                    ��
    //����˵��:
    //��������      ��������                    ����
	//varableList   MyVarListContainer*         ʵ�α�������ָ��
	void run(MyVarListContainer* varableList);
private:
    void initialize();   //��ʼ��
    void cleanUp();     //�������


	MyVarList  fOwnVarList;                            //������ź�����ľֲ�����

	//CCF �������캯��
	MyGotoParser(const MyGotoParser& pool);
	MyGotoParser& operator=(const MyGotoParser& toAssign);
};


MATCH_CPP_NAMESPACE_END



#endif