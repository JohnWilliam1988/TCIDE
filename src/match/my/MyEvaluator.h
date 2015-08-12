////////////////////////////////////2009.12.17  Ф�㷶�޶�//////////////////////////

#if !defined(MYEVALUATOR_H)
#define MYEVALUATOR_H

#include "match/My/MyValue.h"
MATCH_CPP_NAMESPACE_BEGIN

class PARSERS_EXPORT MyEvaluator
{
public:
    ///////////////////////////////////////////////ʵ��////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //�ӷ������ʵ��
    static void Addition(_variant_t* left, _variant_t* right,_variant_t* result);

    //���������ʵ��
    static void Subtraction(_variant_t* left, _variant_t* right,_variant_t* result);

    //�˷������ʵ��
    static void  Multiplication(_variant_t* left, _variant_t* right,_variant_t* result);

    //���������ʵ��
    static void  Division(_variant_t* left, _variant_t* right,_variant_t* result);

    //���������ʵ��
    static void  Mod(_variant_t* left, _variant_t* right,_variant_t* result);

    //>�����ʵ��
    static void  Bigger(_variant_t* left, _variant_t* right,_variant_t* result);

    //<�����ʵ��
    static void  Smaller(_variant_t* left, _variant_t* right,_variant_t* result);

    //==�����ʵ��
    static void  Equ(_variant_t* left, _variant_t* right,_variant_t* result);

    //!=�����ʵ��
    static void  NEqu(_variant_t* left, _variant_t* right,_variant_t* result);

    //>=�����ʵ��
    static void  BOE(_variant_t* left, _variant_t* right,_variant_t* result);

    //<=�����ʵ��
    static void  SOE(_variant_t* left, _variant_t* right,_variant_t* result);

    //&&�����ʵ��
    static void  And(_variant_t* left, _variant_t* right,_variant_t* result);

    //||�����ʵ��
    static void  Or(_variant_t* left, _variant_t* right,_variant_t* result);

    //!�����ʵ��
    static void  Not(_variant_t* right,_variant_t* result);


	static void Minus(_variant_t* right,_variant_t* result);

    //&�����ʵ��
    static void  StrAnd(_variant_t* left, _variant_t* right,_variant_t* result);

	 //|�����ʵ��
    static void  BitOr(_variant_t* left, _variant_t* right,_variant_t* result);

	 //^�����ʵ��
    static void  BitXor(_variant_t* left, _variant_t* right,_variant_t* result);

	//<<�����ʵ��
    static void  BitLeft(_variant_t* left, _variant_t* right,_variant_t* result);

	//>>�����ʵ��
    static void  BitRight(_variant_t* left, _variant_t* right,_variant_t* result);

    //~�����ʵ��
    static void  BitBack(_variant_t* left, _variant_t* right,_variant_t* result);

	//��Щ���ڸ�ֵ�����ʹ��
	//--�����ʵ��
    static void  DEC(_variant_t* result);

	//++�����ʵ��
    static void  INC(_variant_t* result);

	static bool isEqual(_variant_t* left,_variant_t* rigth);
private:
    MyEvaluator();

};


MATCH_CPP_NAMESPACE_END


#endif