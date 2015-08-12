/////////////////////////////////2009.12.20  Ф�㷶�޶�//////////////////////////

#ifndef MYRETURNSTATEMENT_H
#define MYRETURNSTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//���ǰ� return ��� ����� mystatement  �̳���
//return�����Ҫ���������� ��������Է��ص�
class PARSERS_EXPORT MyReturnStatement : public MyStatement
{
public:
    //���캯��
    //����˵��:
    //��������       ��������                    ����
    //expression     const XMLCh* const          ��������
    //id             const unsigned int          ���ID
    //manager        MemoryManager* const        �ڴ������
    MyReturnStatement(const XMLCh* const     expression
        ,const unsigned int    id = 0
        ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //��������:����return���ļ�������
    //����ֵ::
    //��������                ����
    //const XMLCh*            ��������
    const XMLCh* getReturnExpressionString() const;

    //����
    virtual ~MyReturnStatement();

private:
    //��ʼ��
    void initialize();                 

    //��������ڴ�
    void cleanUp();

    //��������:����return�������Ե��ַ���
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������      ��������                    ����
    //exp           const XMLCh* const          return�ļ�������
    void setReturnExpressionString(const XMLCh* const  expression);

    XMLCh*            fReturnExpression;         //��¼flag�����ļ���·��

    /**
    * Copy constructor
    */
    MyReturnStatement(const MyReturnStatement&);
    MyReturnStatement& operator=(const MyReturnStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif