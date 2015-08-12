////////////////////////////////////2009.12.19  Ф�㷶�޶�//////////////////////////

#if !defined(MYEXPRESSTION_H)
#define MYEXPRESSTION_H

#include <match/util/MatchDefs.hpp>
#include <match/util/PlatformUtils.hpp>
#include "BaseBlock.h"
#include "MyValue.h"
#include "MyVarListContainer.h"

MATCH_CPP_NAMESPACE_BEGIN

class PARSERS_EXPORT MyExpresstion
{
public:
    MyExpresstion();
    
    ~MyExpresstion();
    
    void reset();
    
    enum IDENTIFER_TYPE{
        NUMBER,         //  ��ֵ
        OPERATOR2,      //  OPERATOR2:��Ԫ�����
        OPERATOR1,      //  OPERATOR1��һԪ�����
        LB,             //  ������
        RB,			    //  ������
        END,			//  ���ʽ������ʾ
        IT_ERROR    	//  �����ʾ 
    };

    enum OPERATOR_TYPE{
        ATOM=0,			//  ATOM:��ԭ��(��)��һ�����ȼ�,����:����,����,��������,����Ԫ�ص��õ�,���ȴ���
        NOT,			//  ��(ȡ��)���� !   ��
        MUL,			//  �˷�����     *   ��
        DIV,			//  ��������     /   ��
        MOD,			//	�������     %   ��
        ADD,			//  �ӷ�����     +   ��
        SUB,			//  ��������     -   ��
        BIGGER,         //  ���ڷ���     >   ��
        SMALLER,		//  С�ڷ���     <   ��
        EQU,			//  ���ڷ���     ==  ��
        NEQU,			//  �����ڷ���   !=  ��
        BOE,			//  ���ڵ��ڷ��� >=  ��
        SOE,			//  С�ڵ��ڷ��� <=  ��
        AND,			//  ���������   &&  ��
        OR,				//  ���������   ||  ��

		MINUS,          //  ���������   -   ��
		STRAND,         //  λ���������� &   ��
		BITOR,          //  λ�������Ż� |   ��
		BITXOR,         //  λ����������� ^ ��
		BITLEFT,        //  λ��������   <<  ��
		BITRIGHT,       //  λ��������   >>  ��
		BITBACK,        //  λȡ������   ~   ��
        EXPR,
        OPEERROR			//  �����ʾ 
    };

    //////////////////////////////////////////////����ʱ���м���///////////////////////
    typedef struct tagNODE              //����һ���ṹ�����ڵ�
    {
        IDENTIFER_TYPE type;			//����
        _variant_t  value;				//�ڵ��ֵ
        OPERATOR_TYPE ope;						
        struct tagNODE * left, * right, * parent;
    }NODE;

    void Evalue(const XMLCh* namesp,const BaseBlock *  Block,const XMLCh* expression,MyVarListContainer * varlist,_variant_t* result);

    ///////////////////////////////////////////////////////////////////////////////////////
    typedef struct validateTagNODE             //����һ���ṹ�����ڵ�
    {
        IDENTIFER_TYPE type;			       //����
        VARTYPE       value;                   //�ڵ��ֵ
        OPERATOR_TYPE ope;						
        struct validateTagNODE * left, * right, * parent;
    }validateNODE;

    //������������Ƿ�Ϸ�,
    VARTYPE validateExpression(const BaseBlock* const parent,const XMLCh* const expresstion);
    //////////////////////////////////////////////////////////////////////////////////////

	 static void  setConstant(_variant_t* value, const XMLCh*  dtString);

	 static void  setArray(_variant_t* value,const XMLCh* const dtString);
private:
    ///////////////////////////////////////////////////////////////////////////////////////
    //��validate��ͷ�ķ����������м���ϱ�����Ƿ�Ϸ�
    int validateGetNextToken(const BaseBlock* const parent);

	void validateLastExp(const BaseBlock* const parent);

	//�������֤�ڱ��ʽ������һ���Ⱥ�,����ı��ʽ
    bool validateEueGetNextToken(const BaseBlock* const parent);

    validateNODE* validateBuildSyntaxTreeImpl(const BaseBlock* const parent);

    validateNODE* validateBuildSyntaxTree(const BaseBlock* const parent,const XMLCh* expression);

    validateNODE* validateMakeNode(IDENTIFER_TYPE type, VARTYPE value, OPERATOR_TYPE ope, validateNODE* l, validateNODE* r, validateNODE* p);

    VARTYPE validateEvalueTree(validateNODE* tree);

    void validateDestroyTree(validateNODE* tree);

    ///////////////////////////////////////////////////////////////////////////////////////////////


    ////////////////////////////////////////////////����ʱ������õ�/////////////////////////////////
    int GetNextToken(MyVarListContainer * varlist); 

	void EvalueLastExp(MyVarListContainer * varlist);

	void EueGetNextToken(MyVarListContainer * varlist,_variant_t* value);

    void EvalueTree(NODE* tree,_variant_t* value);

    void DestroyTree(NODE* tree);

    NODE* MakeNode(IDENTIFER_TYPE type, _variant_t* value, OPERATOR_TYPE ope, NODE* l, NODE* r, NODE* p); 

    NODE* BuildSyntaxTreeImpl(MyVarListContainer * varlist);

    NODE* BuildSyntaxTree(const XMLCh* expression,MyVarListContainer * varlist);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //��������������ȼ�
    unsigned int Level(OPERATOR_TYPE ope);

    //������
    void ShowError();

    void PutBack();

    /*�ʷ�����*/
private:
    IDENTIFER_TYPE      fType;
    VARTYPE             fValueType;
    _variant_t          fValue;
    OPERATOR_TYPE       fOpe;                    //������¼��������
    OPERATOR_TYPE       fPerope ;

    const XMLCh*        fStr;
    const XMLCh*        fLastStr;
    /*�﷨����*/
    int                 fError;                  //�����������
    int                 fLb;                     //��ʾ�Ƿ����������,���ֱ�ʾ�����׵ĸ���
    const XMLCh*        fPos;
    const XMLCh*        fnamesp;
    const BaseBlock *   fBlock;
};


MATCH_CPP_NAMESPACE_END


#endif