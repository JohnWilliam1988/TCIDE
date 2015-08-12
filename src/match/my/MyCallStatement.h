//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-15  Ф�㷶      ���call��ʾ�����ĳ�����MyCallStatement�����
////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYCALLSTATEMENT_H
#define MYCALLSTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//��������� call�������ñ���� ����� MyStatement �ļ̳���
//call�������ñ���� ��Ҫ������ ���� ����API���û�API
class PARSERS_EXPORT MyCallStatement : public MyStatement
{
public:
    //���캯��
    //����˵��:
    //��������       ��������                    ����
    //callExpress    const XMLCh* const          �������ñ����
    //id             const unsigned int          ���ID
    //manager        MemoryManager* const        �ڴ������
	MyCallStatement(  const XMLCh* const    callExpress
			          ,const unsigned int    id = 0
		              ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
 
    //��������:���غ������ñ���Ե��ַ���
    //����ֵ::
    //��������                ����
    //const XMLCh*            �������ñ�����ַ���
	const XMLCh* getExpressionString() const;

    //��������
	virtual ~MyCallStatement();

private:
    //��ʼ��
	void initialize();                 

    //��������ڴ�
    void cleanUp();                    	

    //��������:���ú������ñ���Ե��ַ���
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������      ��������                    ����
    //pram          const XMLCh* const          �������ñ���Ե��ַ���
	void setExpressionString(const XMLCh* const pram);

	XMLCh*            fCallExpression;          //���õı����

    /**
    * Copy constructor
    */
	MyCallStatement(const MyCallStatement&);
    MyCallStatement& operator=(const MyCallStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif