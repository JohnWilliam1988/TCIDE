/////////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-16  Ф�㷶      ���while���ĳ�����MyWhileStatement�����
////////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYWHILESTATEMENT_H
#define MYWHILESTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//�������ǰ�while��� ����� mystatement �ļ̳���
//
class PARSERS_EXPORT MyWhileStatement : public MyStatement
{
public:
    //���캯��
    //����˵��:
    //��������       ��������                    ����
    //expression     const XMLCh* const          ���������
    //id             const unsigned int          ���ID
    //manager        MemoryManager* const        �ڴ������
    MyWhileStatement(const XMLCh* const    expression
        ,const unsigned int    id = 0
        ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //��������:����while�������������
    //����ֵ::
    //��������                ����
    //const XMLCh*            while�������������
    const XMLCh* getExpressionString() const;


    //��������
    virtual ~MyWhileStatement();

private:

    //��ʼ��
    void initialize();                 
    //��������ڴ�
    void cleanUp();

    //��������:����while�������������
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������      ��������                    ����
    //exp           const XMLCh* const          while�������������
    void setExpressionString(const XMLCh* const expression);

    XMLCh*            fExpression;                          //��¼���������

    /**
    * Copy constructor
    */
    MyWhileStatement(const MyWhileStatement&);
    MyWhileStatement& operator=(const MyWhileStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif