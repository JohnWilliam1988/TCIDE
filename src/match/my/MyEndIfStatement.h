//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-15  Ф�㷶      ���endif���ĳ�����MyEndIfStatement�����
////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYENDIFSTATEMENT_H
#define MYENDIFSTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN
//���������endif������� mystatement �̳���
class PARSERS_EXPORT MyEndIfStatement : public MyStatement
{
public:
    //���캯��
    //����˵��:
    //��������       ��������                    ����
    //id             const unsigned int          ���ID
    //manager        MemoryManager* const        �ڴ������
	MyEndIfStatement(const unsigned int    id = 0
		                  ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
 

    //��������
	virtual ~MyEndIfStatement();

private:
    //��ʼ��
	void initialize();                 

    //�����ڴ�
	void cleanUp();                    

    /**
    * Copy constructor
    */
	MyEndIfStatement(const MyEndIfStatement&);
    MyEndIfStatement& operator=(const MyEndIfStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif