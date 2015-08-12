#if !defined(MYVARPARSER_H)
#define MYVARPARSER_H

#include"AbsBlockParser.h"
#include"BaseBlock.h"
#include"MyVarList.h"

MATCH_CPP_NAMESPACE_BEGIN

class PARSERS_EXPORT MyVarParser : public AbsBlockParser
{
public:
	//���캯��
    //����˵��:
    //��������      ��������                    ����
	//nameSpace     const XMLCh* const          �����ռ���
	//Block         const BaseBlock *           ģ���ָ��
    //manager       MemoryManager* const        �ڴ������
	MyVarParser(const XMLCh* const    nameSpace
		        ,const BaseBlock * Block
		        ,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager);

    //��������
	~MyVarParser();

	//��������:return��ִ�����                                                                                    
    //����ֵ:   void
    //��������                ����
    //void                    ��
    //����˵��:
    //��������          ��������                    ����
	//VarList           MyVarList*  const          ��������
	void run(MyVarList * const VarList,MyVarListContainer* const listCon);

private:
    void initialize();                                             //��ʼ��
    void cleanUp();                                                //�������

	//CCF �������캯��
	MyVarParser(const MyVarParser& pool);
	MyVarParser& operator=(const MyVarParser& toAssign);

};


MATCH_CPP_NAMESPACE_END



#endif