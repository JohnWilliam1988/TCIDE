//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-17  Ф�㷶      ���endwhile���ִ�п�ĳ�����MyEndWhileBlock�����
//0002      2010-07-16  �ӱ�        �ڹ��캯��������˱�����кź��ļ�·����������
//////////////////////////////////////////////////////////////////////////////////////////


#ifndef MYENDWHILEBLOCK_H
#define MYENDWHILEBLOCK_H

#include "BaseBlock.h"
#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//����������ʶһ��while������ĵ�ַ.
class PARSERS_EXPORT MyEndWhileBlock :  public BaseBlock
{
public:
    //���캯��
    //����˵��:
    //��������            ��������                    ����
    //endWhileStatement   MyStatement *               endwhile���
    //id                  const unsigned int          ���ID
    //manager             MemoryManager* const        �ڴ������
	MyEndWhileBlock(MyStatement* endWhileStatement
		     ,const unsigned int    id = 0
			 ,const unsigned int    linenum=0
			 ,const XMLCh*          filepath=NULL
		     ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //��������
	~MyEndWhileBlock();
private:
     //��ʼ����
	void initialize();                

    //�����
	void cleanUp();                    

	MyStatement* fEndWhileStatement;                     //endWhile��� 

    /**
    * Copy constructor
    */
	MyEndWhileBlock(const MyEndWhileBlock& node);
    MyEndWhileBlock& operator=(const MyEndWhileBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END



#endif