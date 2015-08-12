/////////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-16  Ф�㷶      ���endwhile���ĳ�����MyEndWhileStatement�����
////////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYENDWHILESTATEMENT_H
#define MYENDWHILESTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN
//��������� endwhile ����� mystatement �̳���
class PARSERS_EXPORT MyEndWhileStatement : public MyStatement
{
public:
    //���캯��
    //����˵��:
    //��������       ��������                    ����
    //id             const unsigned int          ���ID
    //manager        MemoryManager* const        �ڴ������
	MyEndWhileStatement(const unsigned int    id = 0
		                  ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
 

    //��������
	virtual ~MyEndWhileStatement();

private:

    //��ʼ��
	void initialize();                 

    //�����ڴ�
	void cleanUp();                    

    /**
    * Copy constructor
    */
	MyEndWhileStatement(const MyEndWhileStatement&);
    MyEndWhileStatement& operator=(const MyEndWhileStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif