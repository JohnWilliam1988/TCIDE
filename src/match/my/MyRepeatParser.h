#if !defined(MYREPEATPARSER_H)
#define MYREPEATPARSER_H

#include"AbsBlockParser.h"
#include"MyWhileParser.h"
#include"BaseBlock.h"
#include"MyVarListContainer.h"
#include"MyValue.h"
#include"MyDataList.h"

MATCH_CPP_NAMESPACE_BEGIN

class PARSERS_EXPORT MyRepeatParser  : public AbsBlockParser
{
public:
	//���캯��
    //����˵��:
    //��������      ��������                    ����
	//nameSpace     const XMLCh* const          �����ռ���
	//Block         const BaseBlock *           ģ���ָ��
    //manager       MemoryManager* const        �ڴ������
	MyRepeatParser(const XMLCh *nameSpace
		        ,const BaseBlock * Block
		        ,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager);
	~MyRepeatParser();

	//��������:ִ��repeat���ܿ�
    //����ֵ:   MyValue
    //��������                ����
    //MyValue*              MyValue�ľ�����������
    //����˵��:
    //��������          ��������                    ����
    //helpBlock         const BaseBlock *           ��ǰģ��
	//varableList       MyVarListContainer*         ��������
	//isFindReturn      bool                        return��ʶ����return��Ϊ�棬����Ϊ��
	void ProcessRepeatBlock(const BaseBlock * helpBlock,MyVarListContainer* varableList,bool &isFindReturn,_variant_t* result);

	//��������:repeat��ִ�����
    //����ֵ:   MyValue
    //��������                ����
    //MyValue*              MyValue�ľ�����������
    //����˵��:
    //��������          ��������                    ����
	//varableList       MyVarListContainer*         ��������
	//isFindReturn      bool                        return��ʶ����return��Ϊ�棬����Ϊ��
	void run(MyVarListContainer* varableList,bool &isFindReturn,_variant_t* result);

private:
	void initialize();                                             //��ʼ��
    void cleanUp();                                                //�������

	MyVarList      fOwnvarlist;   

	LoopReturnType  fReturnType;

	//CCF �������캯��
	MyRepeatParser(const MyRepeatParser& pool);
	MyRepeatParser& operator=(const MyRepeatParser& toAssign);
};


MATCH_CPP_NAMESPACE_END

#endif