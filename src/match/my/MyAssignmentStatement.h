//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-15  Ф�㷶      ��ɸ�ֵ��ʾ�����ĳ�����MyAssignmentStatement�����
////////////////////////////////////////////////////////////////////////////////////////
#ifndef MYASSIGNMENTSTATEMENT_H
#define MYASSIGNMENTSTATEMENT_H

#include "MyStatement.h"
#include "match/My/MyValue.h"
MATCH_CPP_NAMESPACE_BEGIN

//��������� ��ֵ�������� �����һ�� MyStatement �̳���
class PARSERS_EXPORT MyAssignmentStatement : public MyStatement
{
public:
	enum TC_LEFTVARTYPE
	{
		 TC_VAR                 =0
		,TC_ARRAYSUB            
		,TC_GLOBALVAR
		,TC_GLOBALARRAYSUB
		,TC_DISPROP
		,TC_ERRORLEFTTYPE       
	};

	enum TC_ASSIGNTYPE
	{
		 TC_DEC                 =0   
		,TC_INC                  
		,TC_MULLEFT     
		,TC_DIVLEFT
		,TC_ADDLEFT
		,TC_SUBLEFT
		,TC_STRADDLEFT
		,TC_ASSIGN
		,TC_ASSIGNMAX
	};

public:
    //���캯��
    //����˵��:
    //��������      ��������                    ����
    //name          const XMLCh* const          �����������
    //expression    const XMLCh* const          ��������
    //id            const unsigned int          ���ID
    //manager       MemoryManager* const        �ڴ������
	MyAssignmentStatement(const XMLCh* const name
		                 ,const XMLCh* const expression
					     ,TC_LEFTVARTYPE  leftType
					     ,TC_ASSIGNTYPE   assignType
						 ,VARTYPE expType 
						 ,const XMLCh* const nameSP=0
						 ,const XMLCh* const arraySub=0
			             ,const unsigned int    id = 0
		                 ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);  


    //��������:���ظ�ֵ������������ı�����
    //����ֵ::
    //��������                ����
    //const XMLCh*            ��������ı�����
	const XMLCh* getVarNameString() const;

	//��������:���ؿռ���
    //����ֵ::
    //��������                ����
    //const XMLCh*            ���ؿռ���
	const XMLCh* getSpNameString() const;

	
	//��������:���ر��ʽ������
    //����ֵ::
    //��������                ����
    //VARTYPE                 ���ر��ʽ������
	VARTYPE getValueType() const;

    //��������:���ظ�ֵ���ļ�������
    //����ֵ::
    //��������                ����
    //const XMLCh*            ��������
	const XMLCh* getExpressionString() const;


	//��������:���������±���ʽ
    //����ֵ::
    //��������                ����
    //const XMLCh*            ��������
	const XMLCh* getArraySub() const;

	MyAssignmentStatement::TC_LEFTVARTYPE getLeftType() const;

	MyAssignmentStatement::TC_ASSIGNTYPE getAssignType() const;

    //��������
	virtual ~MyAssignmentStatement();

private:
    //��ʼ��
	void initialize();                 

   //��������ڴ�
    void cleanUp();  

    //��������:���ø�ֵ����Ե�����������ַ���
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������      ��������                    ����
    //name          const XMLCh* const          ��������ַ���
	void setVarNameString(const XMLCh* const name);

	//��������:���ÿռ���
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������      ��������                    ����
    //name          const XMLCh* const          �ռ���
	void setNameSPString(const XMLCh* const name);


    //��������:���ø�ֵ����Եļ�������
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������         ��������              ����
    //expression       const XMLCh* const    ��������
	void setExpressionString(const XMLCh* const expression);


	//��������:���������±���ʽ
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������         ��������              ����
    //expression       const XMLCh* const    �����±���ʽ
	void setArraySub(const XMLCh* const exp);


	XMLCh             fVarName[NAMEMAXLENGTH];           //�������
	XMLCh             fNameSp[NAMEMAXLENGTH];            //�ռ���           //��ռ���ʵ�ʱ��,���õ�
	XMLCh*            fExpression;                       //��������
	XMLCh*            fArraySub;                         //�����±���ʽ
	TC_LEFTVARTYPE    fLeftType;                         //���������
	TC_ASSIGNTYPE     fAssignType;                       //��������

	//���ʽ����: VT_VARIANT
	//����      : ��������
	//COM����   : VT_DISPATCH
	VARTYPE            fExpType;                         //����ֵ������
    /**
    * Copy constructor
    */
    MyAssignmentStatement(const MyAssignmentStatement&);
    MyAssignmentStatement& operator=(const MyAssignmentStatement&);

};

MATCH_CPP_NAMESPACE_END

#endif