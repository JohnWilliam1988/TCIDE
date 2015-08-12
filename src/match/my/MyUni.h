///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-06-26  肖秀范      完成关键字管理类MyUni
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(MYUNI_H)
#define MYUNI_H

#include <match/util/MatchDefs.hpp>

MATCH_CPP_NAMESPACE_BEGIN

class PARSERS_EXPORT MyUni
{
public:
	//my语法关键字定义
	//注释语句表达试"//"
	static const XMLCh fgAnnotationString[];
	//main函数定义
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


	//数据类型关键字定义
	//目前支持类型
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
    //空字符串
	static const XMLCh fgVoidStringString[];
	//预留类型,暂时还没有使用
	//Void
	static const XMLCh fgVoidString[];
	//Unknown
	static const XMLCh fgUnknownString[];
	//my语法关键字定义

    /////////////////////中文关键字定义/////////////////////
	//main函数定义
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

	//跳转
	static const XMLCh fgChineseGotoString[];

	//数据类型关键字定义
	//变量
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
	//插件
	static const XMLCh fgChineseCom[];
	//数组
	static const XMLCh fgChineseArray[];
	//空
	static const XMLCh fgChineseNull[];
	//其他
	static const XMLCh fgSpaceString[];

private:
	// -----------------------------------------------------------------------
	//  Unimplemented constructors and operators
	// -----------------------------------------------------------------------
	MyUni();

};

MATCH_CPP_NAMESPACE_END



#endif