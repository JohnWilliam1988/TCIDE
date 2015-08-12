//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-17  Ф�㷶      ���endrepeat���ִ�п�ĳ�����MyEndRepeatBlock�����
//0002      2010-07-16  �ӱ�        �ڹ��캯��������˱�����кź��ļ�·����������
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYENDREPEATBLOCK_H
#define MYENDREPEATBLOCK_H

#include "BaseBlock.h"
#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//������������ʶһ��repeatִ�п�Ľ�����.
class PARSERS_EXPORT MyEndRepeatBlock :  public BaseBlock
{
public:
    //���캯��
    //����˵��:
    //��������             ��������                    ����
    //endRepeatStatement   MyStatement *               endrepeat���
    //id                   const unsigned int          ���ID
    //manager              MemoryManager* const        �ڴ������
	MyEndRepeatBlock(MyStatement* endRepeatStatement
		     ,const unsigned int    id = 0
			 ,const unsigned int    linenum=0
			 ,const XMLCh*          filepath=NULL
		     ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //��������
	~MyEndRepeatBlock();
private:
    //��ʼ����
	void initialize();             

     //�����
	void cleanUp();                   

	MyStatement* fEndRepeatStatement;                             //endrepeat��� 

	MyEndRepeatBlock(const MyEndRepeatBlock& node);
    MyEndRepeatBlock& operator=(const MyEndRepeatBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END



#endif