//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-15  Ф�㷶      ���varִ�п�ĳ�����MyVarBlock�����
//0002      2010-07-16  �ӱ�        �ڹ��캯��������˱�����кź��ļ�·����������
/////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYVARBLOCK_H
#define MYVARBLOCK_H

#include "BlockList.h"
#include "MyVarStatement.h"

#include "MyValue.h"

MATCH_CPP_NAMESPACE_BEGIN

//�����Ǳ��������
class PARSERS_EXPORT MyVarBlock : public BaseBlock
{
public:
    //���캯��
    //����˵��:
    //��������           ��������                    ����
    //parentBlock        BaseBlock *                 �����ָ��
    //varStatement       MyStatement *               �����������
    //id                 const unsigned int          ���ID
    //manager            MemoryManager* const        �ڴ������
    MyVarBlock(BaseBlock * parentBlock
        ,MyStatement * varStatement
        ,const unsigned int    id = 0
		,const unsigned int    linenum=0
		,const XMLCh*          filepath=NULL
        ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //��������
    ~MyVarBlock();                   

    //
    //��������:�õ�������
    //����ֵ::
    //��������                       ����
    //const XMLCh*                   ������
    const XMLCh* getVarNameString() const;

    //
    //��������:�õ�����ֵ
    //����ֵ::
    //��������                       ����
    //const XMLCh*                   ����ֵ
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

    //��������:�õ�����
    //����ֵ::
    //��������                ����
    //const BaseBlock*        ����ָ��
    const BaseBlock* GetParentBlock() const;

private:
    //��ʼ����
    void initialize();  

    //�����
    void cleanUp();       

    BaseBlock *           fParentBlock;               //��¼����

    MyVarStatement* fVarStatement;                       //Var���

    /**
    * Copy constructor
    */
    MyVarBlock(const MyVarBlock& node);
    MyVarBlock& operator=(const MyVarBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END



#endif