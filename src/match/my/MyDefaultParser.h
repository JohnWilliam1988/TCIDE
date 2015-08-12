#if !defined(MYDEFAULTPARSER_H)
#define MYDEFAULTPARSER_H

#include "MyDataList.h"

#include"AbsBlockParser.h"
#include"BaseBlock.h"
#include"MyVarListContainer.h"
#include"match/My/MyValue.h"

#include "MyWhileParser.h"


MATCH_CPP_NAMESPACE_BEGIN

class PARSERS_EXPORT MyDefaultParser  : public AbsBlockParser
{
public:
	//���캯��
    //����˵��:
    //��������      ��������                    ����
	//nameSpace     const XMLCh* const          �����ռ���
	//Block         const BaseBlock *           ģ���ָ��
    //manager       MemoryManager* const        �ڴ������
	MyDefaultParser(const XMLCh *nameSpace
		        ,const BaseBlock * Block
		        ,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager);
	~MyDefaultParser();
	//��������:if��ִ�����
    //����ֵ:   MyValue
    //��������                ����
    //MyValue*              MyValue�ľ�����������
    //����˵��:
    //��������          ��������                    ����
	//varableList       MyVarListContainer*         ��������
	//isFindReturn      bool                        return��ʶ����return��Ϊ�棬����Ϊ��
	//isFindLoopReturn  LoopReturnType              �������
	void run(MyVarListContainer* varableList,bool &isFindReturn,LoopReturnType &isFindLoopReturn,_variant_t* result);


	//��������:if��ִ�����
    //����ֵ:   MyValue
    //��������                ����
    //MyValue*              MyValue�ľ�����������
    //����˵��:
    //��������          ��������                    ����
	//varableList       MyVarListContainer*         ��������
	//isFindReturn      bool                        return��ʶ����return��Ϊ�棬����Ϊ��
	void run(MyVarListContainer* varableList,bool &isFindReturn,_variant_t* result);

private:
	void initialize();                         //��ʼ��
    void cleanUp();                            //�������

	MyVarList       fOwnvarlist;               //�ֲ������б�

	//CCF �������캯��
	MyDefaultParser(const MyDefaultParser& pool);
	MyDefaultParser& operator=(const MyDefaultParser& toAssign);
};


MATCH_CPP_NAMESPACE_END

#endif