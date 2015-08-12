//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-15  Ф�㷶      ���call��ʾ�����ĳ�����MyCallStatement�����
////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYGOTOSTATEMENT_H
#define MYGOTOSTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//��������� goto�������ñ���� ����� MyStatement �ļ̳���
//goto�������ñ���� ��Ҫ������ ���� ����API���û�API
class PARSERS_EXPORT MyGotoStatement : public MyStatement
{
public:
    //���캯��
    //����˵��:
    //��������       ��������                    ����
    //callExpress    const XMLCh* const          �������ñ����
    //id             const unsigned int          ���ID
    //manager        MemoryManager* const        �ڴ������
	MyGotoStatement(  const XMLCh* const    gotoLable
			          ,const unsigned int    id = 0
		              ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
 
    //��������:���غ������ñ���Ե��ַ���
    //����ֵ::
    //��������                ����
    //const XMLCh*            �������ñ�����ַ���
	const XMLCh* getGotolable() const;

    //��������
	virtual ~MyGotoStatement();

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
	void setGotolable(const XMLCh* const pram);

    XMLCh            fLable[NAMEMAXLENGTH];          //���õı����

    /**
    * Copy constructor
    */
	MyGotoStatement(const MyGotoStatement&);
    MyGotoStatement& operator=(const MyGotoStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif