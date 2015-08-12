//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-15  Ф�㷶      ���returnִ�п�ĳ�����MyReturnBlock�����
//0002      2010-07-16  �ӱ�        �ڹ��캯��������˱�����кź��ļ�·����������
/////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYRETURNBLOCK_H
#define MYRETURNBLOCK_H

#include "BlockList.h"
#include "MyReturnStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//������returnִ�п�
class PARSERS_EXPORT MyReturnBlock :  public BaseBlock
{
public:
    //���캯��
    //����˵��:
    //��������           ��������                    ����
    //returnStatement    MyStatement *               return���
    //id                 const unsigned int          ���ID
    //manager            MemoryManager* const        �ڴ������
	MyReturnBlock(MyStatement * returnStatement
		     ,const unsigned int    id = 0
			 ,const unsigned int    linenum=0
			 ,const XMLCh*          filepath=NULL
		     ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //����
	~MyReturnBlock();                   

    //��������:�õ�return��ļ�������
    //����ֵ::
    //��������                       ����
    //const XMLCh*                   ���������
	const XMLCh* getReturnExpressionString() const;

private:
    //��ʼ����
	void initialize();                      

    //�����
	void cleanUp();                    

	MyReturnStatement* fReturnStatement;                         //return���

    /**
    * Copy constructor
    */
	MyReturnBlock(const MyReturnBlock& node);
    MyReturnBlock& operator=(const MyReturnBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END



#endif