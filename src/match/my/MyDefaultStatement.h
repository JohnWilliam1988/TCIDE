/////////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-16  Ф�㷶      ���Default���ĳ�����MyDefaultStatement�����
////////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYDEFAULTSTATEMENT_H
#define MYDEFAULTSTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN
//���ǰ�if��� ����� mystatement �̳���
class PARSERS_EXPORT MyDefaultStatement : public MyStatement
{
public:
    //���캯��
    //����˵��:
    //��������       ��������                     ����
    //id             const unsigned int          ���ID
    //manager        MemoryManager* const        �ڴ������
	MyDefaultStatement(const unsigned int    id = 0
		              ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
 
    //��������
	virtual ~MyDefaultStatement();

private:

    //��ʼ��
	void initialize();                 

	//�����ڴ�
    void cleanUp();

    /**
    * Copy constructor
    */
	MyDefaultStatement(const MyDefaultStatement&);
    MyDefaultStatement& operator=(const MyDefaultStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif