/////////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-16  Ф�㷶      ���var�����������ĳ�����MyVarStatement�����
////////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYVARSTATEMENT_H
#define MYVARSTATEMENT_H

#include "MyStatement.h"
#include "match/My/MyValue.h"

MATCH_CPP_NAMESPACE_BEGIN

//�����������,Ŀǰֻ֧���ó�����ʼ���ı����������
class MyVarStatement : public MyStatement
{
public:  
    //���캯��
    //����˵��:
    //��������      ��������                    ����
    //name          const XMLCh* const          ������
    //value         const XMLCh* const          ����ֵ���ַ���������ʽ
    //id            const unsigned int          ���ID
    //manager       MemoryManager* const        �ڴ������
    MyVarStatement(const XMLCh* const    name
		,VARTYPE type 
        ,const XMLCh* const    value = 0
        ,const unsigned int    id = 0
        ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);


	//��������:���ر�����
    //����ֵ::
    //��������                   ����
    //const XMLCh*               ���������ַ����ı�����ʽ
    const XMLCh* getVarNameString() const;

    //��������:���ر���ֵ���ַ����ı�����ʽ
    //����ֵ::
    //��������                   ����
    //const XMLCh*               ����ֵ���ַ����ı�����ʽ
    const XMLCh* getVarValueString() const;


	//��������:���ر���ֵ������,һ����ָ����������
	//����ֵ::
    //��������                   ����
    //VARTYPE                    ����ֵ������
	//VT_NULL                    ��ֵ
	//VT_VARIANT                 �ɱ�����,�����Ǳ������ߺ������÷���
	//VT_I4
	//VT_R8
	//VT_BSTR
	VARTYPE getValueType() const;

    //��������
    virtual ~MyVarStatement();

private:
    //��ʼ��
    void initialize();                

    //��������ڴ�
    void cleanUp();

    //��������:���ñ�����
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������      ��������                    ����
    //name           const XMLCh* const         ������
    void setVarNameString(const XMLCh* const name);

    //��������:���ñ���ֵ,Ŀǰֻ֧�ֳ���ֵ����ʼ������
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������       ��������                    ����
    //value           const XMLCh* const         ����ֵ
    void setVarValueString(const XMLCh* const value);


    XMLCh              fVarName[NAMEMAXLENGTH];   //������
    XMLCh*             fVarValue;                 //����ֵ

	//����Ŀǰֻ���ȫ�ֱ���,��Ϊȫ�ֱ�������ֻ�ܳ�ʼ��Ϊ,����,����COM����,����������
	//����ǳ������ǳ�������
	//�����COM��������:VT_DISPATCH
	VARTYPE            fValueType;                //����ֵ������,һ����ָ����������,������һ������,�ܲ����ǳ���������һ��ֵ
    /**
    * Copy constructor
    */
    MyVarStatement(const MyVarStatement&);
    MyVarStatement& operator=(const MyVarStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif