//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-15  Ф�㷶      ���call��ʾ�����ĳ�����MyCallStatement�����
////////////////////////////////////////////////////////////////////////////////////////
#ifndef MYLABLESTATEMENT_H
#define MYLABLESTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//��������� call�������ñ���� ����� MyStatement �ļ̳���
//call�������ñ���� ��Ҫ������ ���� ����API���û�API
class PARSERS_EXPORT MyLableStatement : public MyStatement
{
public:
    //���캯��
    //����˵��:
    //��������       ��������                    ����
    //lable          const XMLCh* const          �������ñ����
    //id             const unsigned int          ���ID
    //manager        MemoryManager* const        �ڴ������
	MyLableStatement(  const XMLCh* const     lable
			          ,const unsigned int    id = 0
		              ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
 
    //��������:���غ������ñ���Ե��ַ���
    //����ֵ::
    //��������                ����
    //const XMLCh*            �������ñ�����ַ���
	const XMLCh* getLable() const;

    //��������
	virtual ~MyLableStatement();

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
	void setLable(const XMLCh* const pram);

	XMLCh            fLable[NAMEMAXLENGTH];          //���õı����

    /**
    * Copy constructor
    */
	MyLableStatement(const MyLableStatement&);
    MyLableStatement& operator=(const MyLableStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif