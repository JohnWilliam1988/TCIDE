#if !defined(MYCALLPARSER_H)
#define MYCALLPARSER_H

#include"AbsBlockParser.h"
#include"BaseBlock.h"
#include"MyVarListContainer.h"
#include"match/My/MyValue.h"
#include"MyDataList.h"

MATCH_CPP_NAMESPACE_BEGIN

class PARSERS_EXPORT MyCallParser  : public AbsBlockParser
{
public:
	//���캯��
    //����˵��:
    //��������      ��������                    ����
	//nameSpace     const XMLCh* const          �����ռ���
	//Block         const BaseBlock *           ģ���ָ��
    //manager       MemoryManager* const        �ڴ������
	MyCallParser(const XMLCh *nameSpace
		        ,const BaseBlock * Block
		        ,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager);

    //��������
	~MyCallParser();

    //��������:��ʵ��ת��Ϊ����֮����뵽��������
    //���캯��
    //����ֵ        void
    //����˵��:
    //��������      ��������                    ����
    //nameSpace     const XMLCh* const          �����ռ���
    //argumentStr   const XMLCh* const          ʵ���ַ���
    //varableList   MyVarListContainer*         ��������
    //argumentList  MyDataList*                 ����ֵ����
    //Block         const BaseBlock *           ģ��ָ��
    //manager       MemoryManager* const        �ڴ������,
    static void argumentTransportNoCheck(const XMLCh* nameSp, XMLCh * const argumentStr,MyVarListContainer* varableList,MyDataList* argumentList,const BaseBlock * Block,MemoryManager* const memoryManager = XMLPlatformUtils::fgMemoryManager);

	//��������: ������������ֵ��䡣
	//����ֵ    �����ķ���ֵ
    //��������                ����
    //MyValue*                �������͵�����
    //����˵��:
    //��������      ��������                    ����
	//varableList   MyVarListContainer*          ��������ָ��
	 void run(MyVarListContainer* varableList,_variant_t* result);

	//��������: ��������ֱ�ӵ��ú�������䣬����Ҫ����ֵ
	//����ֵ        void
    //����˵��:
    //��������      ��������                    ����
	//varableList   MyVarListContainer*          ��������ָ��
	void  NoReturnrun(MyVarListContainer* varableList);

	static void ArrayGetMsg(const XMLCh* namesp,MyVarListContainer* varableList,MyDataList* list,MyValue* ret);

private:
	void initialize();                                             //��ʼ��
    void cleanUp();                                                //�������

	//�ڴ�����Ա


	//CCF �������캯��
	MyCallParser(const MyCallParser& pool);
	MyCallParser& operator=(const MyCallParser& toAssign);
};


MATCH_CPP_NAMESPACE_END

#endif