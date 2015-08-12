//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-15  Ф�㷶      ���break��ʾ�����ĳ�����MyBreakStatement�����
////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYBREAKSTATEMENT_H
#define MYBREAKSTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//�������ǰ� break��� ����� MyStatement �ļ̳���
class PARSERS_EXPORT MyBreakStatement : public MyStatement
{
public:
    //���캯��
    //����˵��:
    //��������      ��������                    ����
    //id            const unsigned int          ���ID
    //manager       MemoryManager* const        �ڴ������
	MyBreakStatement(const unsigned int    id = 0
		                  ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
 

    //��������
	virtual ~MyBreakStatement();

private:
    //��ʼ��
	void initialize();                

    //��������ڴ�
    void cleanUp();     

    /**
    * Copy constructor
    */
	MyBreakStatement(const MyBreakStatement&);
    MyBreakStatement& operator=(const MyBreakStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif