//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-15  Ф�㷶      ���#includeԤִ�п�ĳ�����MyIncludeBlock�����
//0002      2010-07-16  �ӱ�        �ڹ��캯��������˱�����кź��ļ�·����������
/////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYINCLUDEBLOCK_H
#define MYINCLUDEBLOCK_H

#include "BlockList.h"
#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//#includeԤ����ִ�п�ĳ�����
class PARSERS_EXPORT MyIncludeBlock :  public BaseBlock
{
public:
    //���캯��
    //����˵��:
    //��������                ��������                    ����
    //includeStatement        MyStatement *               #include���
    //id                      const unsigned int          ���ID
    //manager                 MemoryManager* const        �ڴ������
	MyIncludeBlock(MyStatement * includeStatement
		     ,const unsigned int    id = 0
			 ,const unsigned int    linenum=0
			 ,const XMLCh*          filepath=NULL
		     ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //����
	~MyIncludeBlock();                   

    //��������:����include�������ļ�·��
    //����ֵ::
    //��������                       ����
    //const XMLCh*                   include�������ļ�·��
	const XMLCh* getIncludeString() const;

private:
    //��ʼ����
	void initialize();                      

    //�����
	void cleanUp();                       

	MyStatement* fIncludeStatement;                       //include���

    /**
    * Copy constructor
    */
	MyIncludeBlock(const MyIncludeBlock& node);
    MyIncludeBlock& operator=(const MyIncludeBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END



#endif