//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-17  Ф�㷶      ���endnamespace���ִ�п�ĳ�����MyEndNameSpaceBlock�����
//0002      2010-07-16  �ӱ�        �ڹ��캯��������˱�����кź��ļ�·����������
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYENDNAMESPACEBLOCK_H
#define MYENDNAMESPACEBLOCK_H

#include "BaseBlock.h"
#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//����������ʶһ�������ռ��Ľ�����.
class PARSERS_EXPORT MyEndNameSpaceBlock :  public BaseBlock
{
public:

    //���캯��
    //����˵��:
    //��������         ��������                    ����
    //endNSStatement   MyStatement *               endnamespace���
    //id               const unsigned int          ���ID
    //manager          MemoryManager* const        �ڴ������
	MyEndNameSpaceBlock(MyStatement* endNSStatement
		     ,const unsigned int    id = 0
			 ,const unsigned int    linenum=0
			 ,const XMLCh*          filepath=NULL
		     ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //��������
	~MyEndNameSpaceBlock();
private:
    //��ʼ����
	void initialize();                 

    //�����
	void cleanUp();           

	MyStatement* fEndNSStatement;                        //endnamespace��� 

    /**
    * Copy constructor
    */
	MyEndNameSpaceBlock(const MyEndNameSpaceBlock& node);
    MyEndNameSpaceBlock& operator=(const MyEndNameSpaceBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END



#endif