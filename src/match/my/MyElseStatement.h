//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-15  Ф�㷶      ���else���ĳ�����MyElseStatement�����
////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYELSESTATEMENT_H
#define MYELSESTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//���ǰ�else��� ����� mystatement �̳���
class PARSERS_EXPORT MyElseStatement : public MyStatement
{
public:
    //���캯��
    //����˵��:
    //��������       ��������                    ����
    //id             const unsigned int          ���ID
    //manager        MemoryManager* const        �ڴ������
	MyElseStatement(const unsigned int    id = 0
		                  ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
 


    //��������
	virtual ~MyElseStatement();

private:

	void initialize();                 //��ʼ��

    void cleanUp();                    	//��������ڴ�

	MyElseStatement(const MyElseStatement&);
    MyElseStatement& operator=(const MyElseStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif