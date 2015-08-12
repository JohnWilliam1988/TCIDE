//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-16  Ф�㷶      ��ɸ�ֵ��ʾ�����ִ�п�ĳ�����MyAssignmentBlock�����
//0002      2010-05-17  Ф�㷶      ȥ����fParentBlock��ָ��ļ�¼
//0003      2010-07-16  �ӱ�        �ڹ��캯��������˱�����кź��ļ�·����������
////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYASSIGNMENTBLOCK_H
#define MYASSIGNMENTBLOCK_H

#include "BlockList.h"
#include "MyStatement.h"
#include "MyAssignmentStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//��ֵ���ִ�п�
class PARSERS_EXPORT MyAssignmentBlock : public BaseBlock
{
public:

    //���캯��
    //����˵��:
    //��������      ��������                    ����
    //parentBlock   BaseBlock *                 �����ָ��
    //varStatement  MyStatement *               ��ֵ���
    //id            const unsigned int          ���ID
    //manager       MemoryManager* const        �ڴ������
    MyAssignmentBlock(/*BaseBlock *  parentBlock ��ʱԤ��*/
        MyStatement * varStatement
        ,const unsigned int    id = 0
		,const unsigned int    linenum=0
		,const XMLCh*          filepath=NULL
        ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //��������
    ~MyAssignmentBlock();                

	//bool getIsArrayReDef();

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

    //��������:���ظ�ֵ���ļ�������
    //����ֵ::
    //��������                ����
    //const XMLCh*            ��������
    const XMLCh* getExpressionString() const;

	//��������:���ر��ʽ������
    //����ֵ::
    //��������                ����
    //VARTYPE                 ���ر��ʽ������
	VARTYPE getValueType() const;

	//��������:���������±���ʽ
    //����ֵ::
    //��������                ����
    //const XMLCh*            ��������
	const XMLCh* getArraySub() const;

	MyAssignmentStatement::TC_LEFTVARTYPE getLeftType() const;

	MyAssignmentStatement::TC_ASSIGNTYPE getAssignType() const;

/*    //��������:�õ�����
    //����ֵ::
    //��������                ����
    //const BaseBlock*        ����ָ��
    const BaseBlock* GetParentBlock() const; */

private:
    //��ʼ����
    void initialize();           

    //�����
    void cleanUp();                    

    MyAssignmentStatement*               fAssignmentStatement; //Assignment��ֵ���

   /*    BaseBlock *                     fParentBlock;         //����ָ��     */

    /**
    * Copy constructor
    */
    MyAssignmentBlock(const MyAssignmentBlock& node);
    MyAssignmentBlock& operator=(const MyAssignmentBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END


#endif