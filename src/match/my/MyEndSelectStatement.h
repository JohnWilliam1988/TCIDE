//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-15  Ф�㷶      ���endif���ĳ�����MyEndIfStatement�����
////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYENDSELECTSTATEMENT_H
#define MYENDSELECTSTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN
//���������endif������� mystatement �̳���
class PARSERS_EXPORT MyEndSelectStatement : public MyStatement
{
public:
    //���캯��
    //����˵��:
    //��������       ��������                    ����
    //id             const unsigned int          ���ID
    //manager        MemoryManager* const        �ڴ������
	MyEndSelectStatement(const unsigned int    id = 0
		                  ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
 

    //��������
	virtual ~MyEndSelectStatement();

private:
    //��ʼ��
	void initialize();                 

    //�����ڴ�
	void cleanUp();                    

    /**
    * Copy constructor
    */
	MyEndSelectStatement(const MyEndSelectStatement&);
    MyEndSelectStatement& operator=(const MyEndSelectStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif