/////////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-16  Ф�㷶      ���endrepeat���ĳ�����MyEndRepeatStatement�����
////////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYENDREPEATSTATEMENT_H
#define MYENDREPEATSTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN
//���������endrepeat������� mystatement ������
class PARSERS_EXPORT MyEndRepeatStatement : public MyStatement
{
public:
    //���캯��
    //����˵��:
    //��������       ��������                    ����
    //id             const unsigned int          ���ID
    //manager        MemoryManager* const        �ڴ������
    MyEndRepeatStatement(const unsigned int    id = 0
        ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //��������
    virtual ~MyEndRepeatStatement();

private:
    //��ʼ��
    void initialize();      

    //��������ڴ� 
    void cleanUp();                     

    /**
    * Copy constructor
    */
    MyEndRepeatStatement(const MyEndRepeatStatement&);
    MyEndRepeatStatement& operator=(const MyEndRepeatStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif