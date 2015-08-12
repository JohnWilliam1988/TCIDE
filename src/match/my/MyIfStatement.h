/////////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-16  Ф�㷶      ���if���ĳ�����MyIfStatement�����
////////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYIFSTATEMENT_H
#define MYIFSTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN
//���ǰ�if��� ����� mystatement �̳���
class PARSERS_EXPORT MyIfStatement : public MyStatement
{
public:
    //���캯��
    //����˵��:
    //��������       ��������                    ����
    //expression     const XMLCh* const          ���������
    //id             const unsigned int          ���ID
    //manager        MemoryManager* const        �ڴ������
	MyIfStatement(const XMLCh* const    expression
			          ,const unsigned int    id = 0
		              ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
 
    //��������:����if�������������
    //����ֵ::
    //��������                ����
    //const XMLCh*            if�������������
	const XMLCh* getExpressionString() const;

    //��������
	virtual ~MyIfStatement();

private:

    //��ʼ��
	void initialize();                 

	//�����ڴ�
    void cleanUp();

    //��������:����if�������������
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

    /**
    * Copy constructor
    */
	MyIfStatement(const MyIfStatement&);
    MyIfStatement& operator=(const MyIfStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif