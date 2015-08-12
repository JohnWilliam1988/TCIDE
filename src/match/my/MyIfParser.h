#if !defined(MYIFPARSER_H)
#define MYIFPARSER_H

#include "MyDataList.h"

#include"AbsBlockParser.h"
#include"BaseBlock.h"
#include"MyVarListContainer.h"
#include"match/My/MyValue.h"

#include "MyWhileParser.h"


MATCH_CPP_NAMESPACE_BEGIN

class PARSERS_EXPORT MyIfParser  : public AbsBlockParser
{
public:
	//���캯��
    //����˵��:
    //��������      ��������                    ����
	//nameSpace     const XMLCh* const          �����ռ���
	//Block         const BaseBlock *           ģ���ָ��
    //manager       MemoryManager* const        �ڴ������
	MyIfParser(const XMLCh *nameSpace
		        ,const BaseBlock * Block
		        ,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager);
	~MyIfParser();

	//��������:����else��
    //����ֵ:�ҵ��Ŀ�
    //��������                ����
    //const BaseBlock *       �����ҵ��Ŀ�,���� NULL,��ʾʧ��
    //����˵��:
    //��������          ��������                    ����
    //helpBlock         const BaseBlock *           Ҫ���ҵ�ģ��
	const BaseBlock* FindElseSta(const BaseBlock * helpBlock) const;

	//��������:ִ��if��
    //����ֵ:   MyValue
    //��������                ����
    //MyValue*              MyValue�ľ�����������
    //����˵��:
    //��������          ��������                    ����
    //helpBlock         const BaseBlock *           ��ǰģ��
	//varableList       MyVarListContainer*         ��������
	//isFindReturn      bool                        return��ʶ����return��Ϊ�棬����Ϊ��
	//isFindLoopReturn  LoopReturnType              �������
	void ProcessIfBlock(const BaseBlock * helpBlock,MyVarListContainer* varableList,bool &isFindReturn,LoopReturnType &isFindLoopReturn,_variant_t* result);

	//��������:ִ��else��
    //����ֵ:   MyValue
    //��������                ����
    //MyValue*              MyValue�ľ�����������
    //����˵��:
    //��������          ��������                    ����
    //helpBlock         const BaseBlock *           ��ǰģ��
	//varableList       MyVarListContainer*         ��������
	//isFindReturn      bool                        return��ʶ����return��Ϊ�棬����Ϊ��
	//isFindLoopReturn  LoopReturnType              �������
	void ProcessElseBlock(const BaseBlock * helpBlock,MyVarListContainer* varableList,bool &isFindReturn,LoopReturnType &isFindLoopReturn,_variant_t* result);

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



	//�������������غ���,��Ҫ������function���е��õ�

	//��������:ִ��if��
    //����ֵ:   MyValue
    //��������                ����
    //MyValue*              MyValue�ľ�����������
    //����˵��:
    //��������          ��������                    ����
    //helpBlock         const BaseBlock *           ��ǰģ��
	//varableList       MyVarListContainer*         ��������
	//isFindReturn      bool                        return��ʶ����return��Ϊ�棬����Ϊ��
	void ProcessIfBlock(const BaseBlock * helpBlock,MyVarListContainer* varableList,bool &isFindReturn,_variant_t* result);

	//��������:ִ��else��
    //����ֵ:   MyValue
    //��������                ����
    //MyValue*              MyValue�ľ�����������
    //����˵��:
    //��������          ��������                    ����
    //helpBlock         const BaseBlock *           ��ǰģ��
	//varableList       MyVarListContainer*         ��������
	//isFindReturn      bool                        return��ʶ����return��Ϊ�棬����Ϊ��
	void ProcessElseBlock(const BaseBlock * helpBlock,MyVarListContainer* varableList,bool &isFindReturn,_variant_t* result);


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
	MyIfParser(const MyIfParser& pool);
	MyIfParser& operator=(const MyIfParser& toAssign);
};


MATCH_CPP_NAMESPACE_END

#endif