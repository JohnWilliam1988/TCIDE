///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-06-26  Ф�㷶      ��ɹؼ��ֹ�����MyUni
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(MYUNI_H)
#define MYUNI_H

#include <match/util/MatchDefs.hpp>

MATCH_CPP_NAMESPACE_BEGIN

class PARSERS_EXPORT MyUni
{
public:
	//my�﷨�ؼ��ֶ���
	//ע���������"//"
	static const XMLCh fgAnnotationString[];
	//main��������
	static const XMLCh fgMainString[];
	//include
	static const XMLCh fgIncludeString[];
    //import
	static const XMLCh fgImportString[];
	//namespace
	static const XMLCh fgNameSpaceString[];
	//endNamespace
	static const XMLCh fgEndNameSapceString[];
	//if
	static const XMLCh fgIfString[];
	//else
	static const XMLCh fgElseString[];
	//endif
	static const XMLCh fgEndifString[];
	//while
	static const XMLCh fgWhileString[];
	//continue
	static const XMLCh fgContinueString[];
	//break
	static const XMLCh fgBreakString[];
	//endWhile
	static const XMLCh fgEndWhileString[];
	//repeat
	static const XMLCh fgRepeatString[];
	//endrepeat
	static const XMLCh fgEndRepeatString[];
	//function
	static const XMLCh fgFunctionString[];
	//return
	static const XMLCh fgReturnString[];
	//endfunction
	static const XMLCh fgEndFunctionString[];
	//assignment
	static const XMLCh fgSetString[];
	//call
	static const XMLCh fgCallString[];
	//select
	static const XMLCh fgSelectString[];
	//case
	static const XMLCh fgCaseString[];
	//default
	static const XMLCh fgDefaultString[];
	//endselect
	static const XMLCh fgEndSelectString[];
	//goto
	static const XMLCh fgGotoString[];


	//�������͹ؼ��ֶ���
	//Ŀǰ֧������
	////byte
	//static const XMLCh fgByteString[];
	////short
	//static const XMLCh fgShortString[];
	//int
	static const XMLCh fgIntString[];
	////long
	static const XMLCh fgVarString[];
	//double
	static const XMLCh fgDoubleString[];
	//string
	static const XMLCh fgStringString[];
	//bool
	static const XMLCh fgBoolString[];
	//array
	static const XMLCh fgArrayString[];
	//com
	static const XMLCh fgComString[];
	//true
	static const XMLCh fgTrueString[];
	//false
	static const XMLCh fgFalseString[];
	//null
	static const XMLCh fgNullString[];
    //���ַ���
	static const XMLCh fgVoidStringString[];
	//Ԥ������,��ʱ��û��ʹ��
	//Void
	static const XMLCh fgVoidString[];
	//Unknown
	static const XMLCh fgUnknownString[];
	//my�﷨�ؼ��ֶ���

    /////////////////////���Ĺؼ��ֶ���/////////////////////
	//main��������
	static const XMLCh fgChineseMain[];
	//include
	static const XMLCh fgChineseInclude[];
	//import
	static const XMLCh fgChineseImport[];
	//namespace
	static const XMLCh fgChineseNameSpace[];
	//endNamespace
	static const XMLCh fgChineseEndNameSapce[];
	//if
	static const XMLCh fgChineseIf[];
	//else
	static const XMLCh fgChineseElse[];
	//endif
	static const XMLCh fgChineseEndif[];
	//while
	static const XMLCh fgChineseWhile[];
	//continue
	static const XMLCh fgChineseContinue[];
	//break
	static const XMLCh fgChineseBreak[];
	//endWhile
	static const XMLCh fgChineseEndWhile[];
	//repeat
	static const XMLCh fgChineseRepeat[];
	//endrepeat
	static const XMLCh fgChineseEndRepeat[];
	//function
	static const XMLCh fgChineseFunction[];
	//return
	static const XMLCh fgChineseReturn[];
	//endfunction
	static const XMLCh fgChineseEndFunction[];
	//select
	static const XMLCh fgChineseSelect[];
	//case
	static const XMLCh fgChineseCase[];
	//default
	static const XMLCh fgChineseDefault[];
	//endselect
	static const XMLCh fgChineseEndSelect[];

	//��ת
	static const XMLCh fgChineseGotoString[];

	//�������͹ؼ��ֶ���
	//����
	static const XMLCh fgChineseVar[];
	//int
	static const XMLCh fgChineseInt[];
	//double
	static const XMLCh fgChineseDouble[];
	//string
	static const XMLCh fgChineseString[];
	//bool
	static const XMLCh fgChineseBool[];
	//true
	static const XMLCh fgChineseTrue[];
	//false
	static const XMLCh fgChineseFalse[];
	//���
	static const XMLCh fgChineseCom[];
	//����
	static const XMLCh fgChineseArray[];
	//��
	static const XMLCh fgChineseNull[];
	//����
	static const XMLCh fgSpaceString[];

private:
	// -----------------------------------------------------------------------
	//  Unimplemented constructors and operators
	// -----------------------------------------------------------------------
	MyUni();

};

MATCH_CPP_NAMESPACE_END



#endif