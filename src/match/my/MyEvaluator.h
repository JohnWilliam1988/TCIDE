////////////////////////////////////2009.12.17  肖秀范修订//////////////////////////

#if !defined(MYEVALUATOR_H)
#define MYEVALUATOR_H

#include "match/My/MyValue.h"
MATCH_CPP_NAMESPACE_BEGIN

class PARSERS_EXPORT MyEvaluator
{
public:
    ///////////////////////////////////////////////实现////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //加法运算的实现
    static void Addition(_variant_t* left, _variant_t* right,_variant_t* result);

    //减法运算的实现
    static void Subtraction(_variant_t* left, _variant_t* right,_variant_t* result);

    //乘法运算的实现
    static void  Multiplication(_variant_t* left, _variant_t* right,_variant_t* result);

    //除法运算的实现
    static void  Division(_variant_t* left, _variant_t* right,_variant_t* result);

    //求余运算的实现
    static void  Mod(_variant_t* left, _variant_t* right,_variant_t* result);

    //>运算的实现
    static void  Bigger(_variant_t* left, _variant_t* right,_variant_t* result);

    //<运算的实现
    static void  Smaller(_variant_t* left, _variant_t* right,_variant_t* result);

    //==运算的实现
    static void  Equ(_variant_t* left, _variant_t* right,_variant_t* result);

    //!=运算的实现
    static void  NEqu(_variant_t* left, _variant_t* right,_variant_t* result);

    //>=运算的实现
    static void  BOE(_variant_t* left, _variant_t* right,_variant_t* result);

    //<=运算的实现
    static void  SOE(_variant_t* left, _variant_t* right,_variant_t* result);

    //&&运算的实现
    static void  And(_variant_t* left, _variant_t* right,_variant_t* result);

    //||运算的实现
    static void  Or(_variant_t* left, _variant_t* right,_variant_t* result);

    //!运算的实现
    static void  Not(_variant_t* right,_variant_t* result);


	static void Minus(_variant_t* right,_variant_t* result);

    //&运算的实现
    static void  StrAnd(_variant_t* left, _variant_t* right,_variant_t* result);

	 //|运算的实现
    static void  BitOr(_variant_t* left, _variant_t* right,_variant_t* result);

	 //^运算的实现
    static void  BitXor(_variant_t* left, _variant_t* right,_variant_t* result);

	//<<运算的实现
    static void  BitLeft(_variant_t* left, _variant_t* right,_variant_t* result);

	//>>运算的实现
    static void  BitRight(_variant_t* left, _variant_t* right,_variant_t* result);

    //~运算的实现
    static void  BitBack(_variant_t* left, _variant_t* right,_variant_t* result);

	//这些是在赋值语句中使用
	//--运算的实现
    static void  DEC(_variant_t* result);

	//++运算的实现
    static void  INC(_variant_t* result);

	static bool isEqual(_variant_t* left,_variant_t* rigth);
private:
    MyEvaluator();

};


MATCH_CPP_NAMESPACE_END


#endif