/////////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-15  Ф�㷶      ���endnamespace���ĳ�����MyEndNameSpaceStatement�����
////////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYENDNAMESPACESTATEMENT_H
#define MYENDNAMESPACESTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN
//���������endnamespace �����mystatement �̳���
class PARSERS_EXPORT MyEndNameSpaceStatement : public MyStatement
{
public:
    //���캯��
    //����˵��:
    //��������       ��������                    ����
    //id             const unsigned int          ���ID
    //manager        MemoryManager* const        �ڴ������
	MyEndNameSpaceStatement(const unsigned int    id = 0
		                  ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
 
    //��������
	virtual ~MyEndNameSpaceStatement();

private:
    //��ʼ��
	void initialize();                 

    //�����ڴ�
    void cleanUp();                    

    /**
    * Copy constructor
    */
	MyEndNameSpaceStatement(const MyEndNameSpaceStatement&);
    MyEndNameSpaceStatement& operator=(const MyEndNameSpaceStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif