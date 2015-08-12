#if !defined(MYFUNCTIONPARSER_H)
#define MYFUNCTIONPARSER_H

#include"AbsBlockParser.h"
#include"BaseBlock.h"
#include"MyValue.h"
#include"MyDataList.h"
#include"MyVarList.h"

MATCH_CPP_NAMESPACE_BEGIN


//����ִ�п�ĳ�����,�̳���AbsBlockParser
class PARSERS_EXPORT MyFunctionParser : public AbsBlockParser
{

public:

	//���캯��
    //����˵��:
    //��������      ��������                    ����
	//nameSpace     const XMLCh* const          �����ռ���
	//Block         const BaseBlock *           ģ���ָ��
    //manager       MemoryManager* const        �ڴ������
	MyFunctionParser(const XMLCh * const nameSpace,
		             const BaseBlock * Block,
		             MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager);

    //��������
	~MyFunctionParser();

	//��������: ����ִ�нӿڡ�
	//����ֵ    ��
    //��������                ����
    //void                    ��
    //����˵��:
    //��������      ��������                    ����
	//argumentList  MyDataList*                 ʵ�α�������ָ��
    //result        MyValue*                    ����ֵ
	void run(MyDataList *argumentList,_variant_t* result/*out*/);

    //�β�ת��
    //��������: ��������������ת��:ʵ��->�β�
    //����ֵ    ��
    //��������                ����
    //void                    ��
    //����˵��:
    //��������      ��������                    ����
    //argumentList  MyDataList*                 ʵ�α���ֵ����ָ��
    void pramTransport(MyDataList *argumentList/*out*/);

private:
    void initialize();   //��ʼ��
    void cleanUp();     //�������


	MyVarList  fOwnVarList;                            //������ź�����ľֲ�����

	//CCF �������캯��
	MyFunctionParser(const MyFunctionParser& pool);
	MyFunctionParser& operator=(const MyFunctionParser& toAssign);
};


MATCH_CPP_NAMESPACE_END



#endif