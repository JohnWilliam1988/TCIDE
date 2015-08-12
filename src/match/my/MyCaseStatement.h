/////////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-16  Ф�㷶      ���Case���ĳ�����MyCaseStatement�����
////////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYCASESTATEMENT_H
#define MYCASESTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN
//���ǰ�if��� ����� mystatement �̳���
class PARSERS_EXPORT MyCaseStatement : public MyStatement
{
public:
    //���캯��
    //����˵��:
    //��������       ��������                     ����
    //expression     const XMLCh* const          ���������
    //id             const unsigned int          ���ID
    //manager        MemoryManager* const        �ڴ������
	MyCaseStatement(const XMLCh* const    expression
			          ,const unsigned int    id = 0
		              ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
 
    //��������:����if�������������
    //����ֵ::
    //��������                ����
    //const XMLCh*            if�������������
	const XMLCh* getExpressionString() const;

    //��������
	virtual ~MyCaseStatement();

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
	MyCaseStatement(const MyCaseStatement&);
    MyCaseStatement& operator=(const MyCaseStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif