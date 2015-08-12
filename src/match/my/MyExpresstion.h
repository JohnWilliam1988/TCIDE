////////////////////////////////////2009.12.19  肖秀范修订//////////////////////////

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
        NUMBER,         //  数值
        OPERATOR2,      //  OPERATOR2:二元运算符
        OPERATOR1,      //  OPERATOR1：一元运算符
        LB,             //  左括号
        RB,			    //  右括号
        END,			//  表达式结束标示
        IT_ERROR    	//  错误标示 
    };

    enum OPERATOR_TYPE{
        ATOM=0,			//  ATOM:给原子(数)赋一个优先级,比如:变量,常数,函数调用,数组元素调用等,优先处理
        NOT,			//  非(取反)符号 !   右
        MUL,			//  乘法操作     *   左
        DIV,			//  除法操作     /   左
        MOD,			//	求余操作     %   左
        ADD,			//  加法操作     +   左
        SUB,			//  减法操作     -   左
        BIGGER,         //  大于符号     >   非
        SMALLER,		//  小于符号     <   非
        EQU,			//  等于符号     ==  非
        NEQU,			//  不等于符号   !=  非
        BOE,			//  大于等于符号 >=  非
        SOE,			//  小于等于符号 <=  非
        AND,			//  与运算符号   &&  左
        OR,				//  或运算符号   ||  左

		MINUS,          //  负号运算符   -   非
		STRAND,         //  位操作符号与 &   左
		BITOR,          //  位操作符号或 |   左
		BITXOR,         //  位操作符号异或 ^ 左
		BITLEFT,        //  位操作左移   <<  左
		BITRIGHT,       //  位操作右移   >>  左
		BITBACK,        //  位取反操作   ~   非
        EXPR,
        OPEERROR			//  错误标示 
    };

    //////////////////////////////////////////////运行时进行计算///////////////////////
    typedef struct tagNODE              //定义一个结构体做节点
    {
        IDENTIFER_TYPE type;			//类型
        _variant_t  value;				//节点的值
        OPERATOR_TYPE ope;						
        struct tagNODE * left, * right, * parent;
    }NODE;

    void Evalue(const XMLCh* namesp,const BaseBlock *  Block,const XMLCh* expression,MyVarListContainer * varlist,_variant_t* result);

    ///////////////////////////////////////////////////////////////////////////////////////
    typedef struct validateTagNODE             //定义一个结构体做节点
    {
        IDENTIFER_TYPE type;			       //类型
        VARTYPE       value;                   //节点的值
        OPERATOR_TYPE ope;						
        struct validateTagNODE * left, * right, * parent;
    }validateNODE;

    //用来检查表达试是否合法,
    VARTYPE validateExpression(const BaseBlock* const parent,const XMLCh* const expresstion);
    //////////////////////////////////////////////////////////////////////////////////////

	 static void  setConstant(_variant_t* value, const XMLCh*  dtString);

	 static void  setArray(_variant_t* value,const XMLCh* const dtString);
private:
    ///////////////////////////////////////////////////////////////////////////////////////
    //带validate开头的方法用来进行检验合表达试是否合法
    int validateGetNextToken(const BaseBlock* const parent);

	void validateLastExp(const BaseBlock* const parent);

	//这个是验证在表达式后面又一个等号,后面的表达式
    bool validateEueGetNextToken(const BaseBlock* const parent);

    validateNODE* validateBuildSyntaxTreeImpl(const BaseBlock* const parent);

    validateNODE* validateBuildSyntaxTree(const BaseBlock* const parent,const XMLCh* expression);

    validateNODE* validateMakeNode(IDENTIFER_TYPE type, VARTYPE value, OPERATOR_TYPE ope, validateNODE* l, validateNODE* r, validateNODE* p);

    VARTYPE validateEvalueTree(validateNODE* tree);

    void validateDestroyTree(validateNODE* tree);

    ///////////////////////////////////////////////////////////////////////////////////////////////


    ////////////////////////////////////////////////运行时计算调用的/////////////////////////////////
    int GetNextToken(MyVarListContainer * varlist); 

	void EvalueLastExp(MyVarListContainer * varlist);

	void EueGetNextToken(MyVarListContainer * varlist,_variant_t* value);

    void EvalueTree(NODE* tree,_variant_t* value);

    void DestroyTree(NODE* tree);

    NODE* MakeNode(IDENTIFER_TYPE type, _variant_t* value, OPERATOR_TYPE ope, NODE* l, NODE* r, NODE* p); 

    NODE* BuildSyntaxTreeImpl(MyVarListContainer * varlist);

    NODE* BuildSyntaxTree(const XMLCh* expression,MyVarListContainer * varlist);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //返回运算符的优先级
    unsigned int Level(OPERATOR_TYPE ope);

    //错误处理
    void ShowError();

    void PutBack();

    /*词法分析*/
private:
    IDENTIFER_TYPE      fType;
    VARTYPE             fValueType;
    _variant_t          fValue;
    OPERATOR_TYPE       fOpe;                    //用来记录操作符号
    OPERATOR_TYPE       fPerope ;

    const XMLCh*        fStr;
    const XMLCh*        fLastStr;
    /*语法分析*/
    int                 fError;                  //代表错误类型
    int                 fLb;                     //表示是否存在左括号,数字表示起镶套的个数
    const XMLCh*        fPos;
    const XMLCh*        fnamesp;
    const BaseBlock *   fBlock;
};


MATCH_CPP_NAMESPACE_END


#endif