/////////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-16  Ф�㷶      ���function���ĳ�����MyFunctionHeadStatement�����
////////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYFUNCTIONHEADSTATEMENT_H
#define MYFUNCTIONHEADSTATEMENT_H

#include "match/My/MyValue.h"
#include "MyStatement.h"


MATCH_CPP_NAMESPACE_BEGIN
//�������� �Ѻ�������ĺ���ͷ������� ����� mystatement �̳���
//MyFunctionHeadStatement ��¼�˺������������,�β��б�,�ͺ�����������
class PARSERS_EXPORT MyFunctionHeadStatement : public MyStatement
{
public:
    //���캯��
    //����˵��:
    //��������       ��������                    ����
    //name           const XMLCh* const          ������
    //pramList       const XMLCh* const          �����б�
    //id             const unsigned int          ���ID
    //manager        MemoryManager* const        �ڴ������
	MyFunctionHeadStatement(
		               const XMLCh* const    name
					  ,const XMLCh* const    pramList
			          ,const unsigned int    id = 0
		              ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
 
    //��������:���غ�����
    //����ֵ::
    //��������                ����
    //const XMLCh*            �������ַ���
	const XMLCh* getFunctionNameString() const;

	//��������:���غ����Ĳ����б�
    //����ֵ::
    //��������                       ����
    //const XMLCh*                   �õ������Ĳ����б�
	const XMLCh* getFunctionPramString() const;

    //��������
	virtual ~MyFunctionHeadStatement();
private:
    //��ʼ��
	void initialize();                 

	//�����ڴ�
     void cleanUp();

     //��������:���ú�����
     //
     //����ֵ:
     //��������                ����
     //void                    �޷�������
     //
     //����˵��:
     //��������      ��������                    ����
     //name          const XMLCh* const          ������
	void setFunctionNameString(const XMLCh* const name);

     //��������:���ò����б���
     //
     //����ֵ:
     //��������                ����
     //void                    �޷�������
     //
     //����˵��:
     //��������      ��������                    ����
     //pramList          const XMLCh* const      ������
	void setFunctionPramString(const XMLCh* const pramList);

	XMLCh             fFunctionName[NAMEMAXLENGTH];//��¼������
	XMLCh*            fFunctionPram;               //��¼���������б�
    /**
    * Copy constructor
    */
	MyFunctionHeadStatement(const MyFunctionHeadStatement&);
    MyFunctionHeadStatement& operator=(const MyFunctionHeadStatement&);
};



MATCH_CPP_NAMESPACE_END

#endif