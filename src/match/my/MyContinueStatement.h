//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-15  Ф�㷶      ���continue���ĳ�����MyContinueStatement�����
////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYCONTINUESTATEMENT_H
#define MYCONTINUESTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN
//���ǰ� continue��� ����� MyStatement �ļ̳���
class PARSERS_EXPORT MyContinueStatement : public MyStatement
{
public:
    //���캯��
    //����˵��:
    //��������       ��������                    ����
    //id             const unsigned int          ���ID
    //manager        MemoryManager* const        �ڴ������
	MyContinueStatement(const unsigned int    id = 0
		                  ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
 
    //��������
	virtual ~MyContinueStatement();

private:
    //��ʼ��
	void initialize();                 

    //��������ڴ�
    void cleanUp();    

    /**
    * Copy constructor
    */
	MyContinueStatement(const MyContinueStatement&);
    MyContinueStatement& operator=(const MyContinueStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif