#if !defined(MYRETURNPARSER_H)
#define MYRETURNPARSER_H

#include"AbsBlockParser.h"
#include"BaseBlock.h"
#include"MyVarListContainer.h"
#include"match/My/MyValue.h"

MATCH_CPP_NAMESPACE_BEGIN

class PARSERS_EXPORT MyReturnParser  : public AbsBlockParser
{
public:
	//���캯��
    //����˵��:
    //��������      ��������                    ����
	//nameSpace     const XMLCh* const          �����ռ���
	//Block         const BaseBlock *           ģ���ָ��
    //manager       MemoryManager* const        �ڴ������
	MyReturnParser(const XMLCh *nameSpace
		        ,const BaseBlock * Block
		        ,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager);
	~MyReturnParser();

	//��������:return��ִ�����
    //����ֵ:   MyValue
    //��������                ����
    //MyValue*              MyValue�ľ�����������
    //����˵��:
    //��������          ��������                    ����
	//varableList       MyVarListContainer*         ��������
	void run(MyVarListContainer* varableList,_variant_t* result);

private:
	void initialize();                                             //��ʼ��
    void cleanUp();                                                //�������

	//�ڴ�����Ա


	//CCF �������캯��
	MyReturnParser(const MyReturnParser& pool);
	MyReturnParser& operator=(const MyReturnParser& toAssign);
};


MATCH_CPP_NAMESPACE_END

#endif