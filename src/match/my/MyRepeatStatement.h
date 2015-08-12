//////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-15  Ф�㷶      ���repeat���ĳ������MyRepeatStatement�����
//////////////////////////////////////////////////////////////////////////
#ifndef MYREPEATSTATEMENT_H

#define MYREPEATSTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//���ǰ� repeatstatement������� mystatement �ļ̳���
//repeat�����Ҫ������ִ�й涨�������ظ�����
class PARSERS_EXPORT MyRepeatStatement : public MyStatement
{
public:

    //���캯��
    //����˵��:
    //��������      ��������                    ����
    //num           unsigned int                �ظ�����    
    //id            const unsigned int          ���ID
    //manager       MemoryManager* const        �ڴ������
	MyRepeatStatement(bool isCondition
		              ,const XMLCh* const    conditionExp
			          ,const unsigned int    id = 0
		              ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

	//���캯��
    //����˵��:
    //��������      ��������                    ����
    //num           unsigned int                �ظ�����    
    //id            const unsigned int          ���ID
    //manager       MemoryManager* const        �ڴ������
	MyRepeatStatement( const XMLCh* const    conditionExp
		              ,const XMLCh* const    VarName 
					  ,const XMLCh* const    initValue 
					  ,const XMLCh* const    forStep 
			          ,const unsigned int    id    =0
		              ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);


	bool isContant();

	const XMLCh* getVarName();

	const XMLCh* getInitValue();

	const XMLCh* getExp();

	const XMLCh* getStep();

	unsigned int getContantNum();

	//void setIsContant(bool isContant);

    void setVarName(const XMLCh* const name);

	void setInitValue(const XMLCh* const initial);

	void setExp(const XMLCh* const exp);

	void setStep(const XMLCh* const step);

    //��������:�õ�ѭ���Ĵ���
    //����ֵ::
    //��������                ����
    //unsigned int            ѭ���Ĵ���
    //unsigned int getRepeatNum() const;

    //��������
	virtual ~MyRepeatStatement();

private:
	 //��ʼ��
	void initialize();                

	//��������ڴ�
    void cleanUp();

	//void setVarName(const XMLCh* const name);

	//void setInitValue(const XMLCh* const initial);

	//void setExp(const XMLCh* const exp);

	//void setStep(const XMLCh* const step);


	bool                    fIsContant;
	unsigned int            fRepeatContant;
	XMLCh                   fVarName[NAMEMAXLENGTH];
	XMLCh*                  fInitValue;
	XMLCh*                  fExp;
	XMLCh*                  fStep;

	MyRepeatStatement(const MyRepeatStatement&);
    MyRepeatStatement& operator=(const MyRepeatStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif