//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-15  Ф�㷶      ���endfunction���ĳ�����MyEndFunctionStatement�����
////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYENDFUNCTIONSTATEMENT_H
#define MYENDFUNCTIONSTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN
//���������endfunction������� mystatement �̳���
class PARSERS_EXPORT MyEndFunctionStatement : public MyStatement
{
public:
    //���캯��
    //����˵��:
    //��������       ��������                    ����
    //id             const unsigned int          ���ID
    //manager        MemoryManager* const        �ڴ������
	MyEndFunctionStatement(const unsigned int    id = 0
		                  ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
 
    //��������
	virtual ~MyEndFunctionStatement();

private:

    //��ʼ��
	void initialize();                 

    //�����ڴ�
    void cleanUp();      

	MyEndFunctionStatement(const MyEndFunctionStatement&);
    MyEndFunctionStatement& operator=(const MyEndFunctionStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif