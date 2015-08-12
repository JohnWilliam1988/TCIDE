//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��       �����Լ�Ŀ�� 
//0001      2010-05-17  Ф�㷶      ���һ��ͨ�õĽ�����,�˽��������﷨���ǲ����ڵ�
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYENDBLOCK_H
#define MYENDBLOCK_H

#include "BaseBlock.h"
#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//������������ʶһ��caseִ�п�Ľ�����,�����Ƚ�����,ʵ����û���������,Ϊ������Ľṹ,���Լ���������
//endSelectStatement ��ʱ����endcase׼�����
class PARSERS_EXPORT MyEndBlock :  public BaseBlock
{
public:
    //���캯��
    //����˵��:
    //��������             ��������                     ����
    //endSelectStatement   MyStatement *               endrepeat���
    //id                   const unsigned int          ���ID
    //manager              MemoryManager* const        �ڴ������
	MyEndBlock(MyStatement* endStatement=NULL
		     ,const unsigned int    id = 0
			 ,const unsigned int    linenum=0
			 ,const XMLCh*          filepath=0
		     ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //��������
	~MyEndBlock();
private:
    //��ʼ����
	void initialize();             

     //�����
	void cleanUp();                   

	MyStatement* fEndStatement;                             //endrepeat��� 

	MyEndBlock(const MyEndBlock& node);
    MyEndBlock& operator=(const MyEndBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END



#endif