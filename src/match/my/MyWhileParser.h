#if !defined(MYWHILEPARSER_H)
#define MYWHILEPARSER_H

#include"AbsBlockParser.h"
#include"BaseBlock.h"
#include"MyVarListContainer.h"
#include"match/My/MyValue.h"
#include "MyDataList.h"

MATCH_CPP_NAMESPACE_BEGIN

//��ö��������Ҫ��������ѭ�����ڲ���ִ�п��break��continue���ص����
//��Ҫ����ѭ����������,Ŀǰ��Ҫ���ڿ��֧�ֵ�repeat��while������
//
enum LoopReturnType
{
	LOOP_BREAK,
	LOOP_CONTINUE,
	LOOP_NORMAL,
};

class PARSERS_EXPORT MyWhileParser  : public AbsBlockParser
{
public:

	//���캯��
    //����˵��:
    //��������      ��������                    ����
	//nameSpace     const XMLCh* const          �����ռ���
	//Block         const BaseBlock *           ģ���ָ��
    //manager       MemoryManager* const        �ڴ������
	MyWhileParser(const XMLCh * const nameSpace
		        ,const BaseBlock * Block
		        ,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager);
	~MyWhileParser();

	//��������:ִ��while���ܿ�
    //����ֵ:   MyValue
    //��������                ����
    //MyValue*                MyValue�ľ�����������
    //����˵��:
    //��������                 ��������                     ����
    //helpBlock               const BaseBlock *           ��ǰģ��
	//varableList             MyVarListContainer*         ��������
	//isFindReturn            bool                        return��ʶ,��return��Ϊ��,����Ϊ��
	void ProcessWhileBlock(const BaseBlock * helpBlock,MyVarListContainer* varableList,bool &isFindReturn,_variant_t* result);

	//��������:while��ִ�����
    //����ֵ:   MyValue
    //��������                ����
    //MyValue*                MyValue�ľ�����������
    //����˵��:
    //��������                ��������                    ����
	//varableList             MyVarListContainer*         ��������
	//isFindReturn            bool                        return��ʶ����return��Ϊ�棬����Ϊ��
	void run(MyVarListContainer* varableList,bool &isFindReturn,_variant_t* result);

private:
	void initialize();                                             //��ʼ��
    void cleanUp();                                                //�������

	MyVarList          fOwnvarlist;                  //�ֲ������б�
	LoopReturnType     fReturnType;                  //while����жϵ�����,break,continue,

	//CCF �������캯��
	MyWhileParser(const MyWhileParser& pool);
	MyWhileParser& operator=(const MyWhileParser& toAssign);
};


MATCH_CPP_NAMESPACE_END

#endif