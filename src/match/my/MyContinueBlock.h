//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-17  Ф�㷶      ���continue���ִ�п�ĳ�����MyContinueBlock�����
//0002      2010-07-16  �ӱ�        �ڹ��캯��������˱�����кź��ļ�·����������
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYCONTINUEBLOCK_H
#define MYCONTINUEBLOCK_H

#include "BaseBlock.h"
#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//���������� continue ִ�п� 
class PARSERS_EXPORT MyContinueBlock :  public BaseBlock
{
public:
    //���캯��
    //����˵��:
    //��������       ��������                    ����
    //continueState  MyStatement *               continue���
    //id             const unsigned int          ���ID
    //manager        MemoryManager* const        �ڴ������
	MyContinueBlock(MyStatement * continueState
		,const unsigned int    id = 0
		,const unsigned int    linenum=0
		,const XMLCh*          filepath=NULL
		,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //��������
	~MyContinueBlock();

private:
     //��ʼ����
	void initialize();                

    //�����
	void cleanUp();                    

	MyStatement* fContinueStatement;                    //contine���

    /**
    * Copy constructor
    */
	MyContinueBlock(const MyContinueBlock& node);
    MyContinueBlock& operator=(const MyContinueBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END



#endif