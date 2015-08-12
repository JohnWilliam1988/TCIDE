//////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-15  Ф�㷶      ���select���ĳ������MySelectStatement�����
//////////////////////////////////////////////////////////////////////////
#ifndef MYSELECTSTATEMENT_H
#define MYSELECTSTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//���ǰ� repeatstatement������� mystatement �ļ̳���
//repeat�����Ҫ������ִ�й涨�������ظ�����
class PARSERS_EXPORT MySelectStatement : public MyStatement
{
public:

    //���캯��
    //����˵��:
    //��������      ��������                    ����
    //num           unsigned int                �ظ�����    
    //id            const unsigned int          ���ID
    //manager       MemoryManager* const        �ڴ������
	MySelectStatement(const XMLCh* const     expression
			          ,const unsigned int    id = 0
		              ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

	//��������:����Select�������������
    //����ֵ::
    //��������                ����
    //const XMLCh*            if�������������
	const XMLCh* getExpressionString() const;

    //��������
	virtual ~MySelectStatement();

private:
	void initialize();         //��ʼ��

	//��������ڴ�
    void cleanUp();

	//��������:����Select�������������
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������      ��������                    ����
    //exp           const XMLCh* const          if�������������
	void setExpressionString(const XMLCh* const exp);


	XMLCh*            fExpression;                        //��¼���������

	MySelectStatement(const MySelectStatement&);
    MySelectStatement& operator=(const MySelectStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif