/////////////////////////////////2009.12.16  肖秀范修订//////////////////////////
#if !defined(MYVALIDATOR_H)
#define MYVALIDATOR_H

#include <match/util/MatchDefs.hpp>
#include <match/framework/MemoryManager.hpp>
#include <match/util/PlatformUtils.hpp>
#include "MyAssignmentStatement.h"
#include "MyValue.h"
#include "BaseBlock.h"

MATCH_CPP_NAMESPACE_BEGIN

//这个类用检验变量名 函数名命名的格式,已经检验他们的合法性(指这个名的定义是不是已经存在)
//以及检验条件表达试的格式,和检验他们的合法性
class PARSERS_EXPORT MyValidator
{
public :

	//写日志
	static bool writelog(const XMLCh* Name);


	//格式的检验,一般用来语句定义中
	//方法描述：验证变量名、空间命名或者函数名是否为关键字
	//返回值:  true 或者 false
    //返回类型                描述
    //bool                    布尔值，true表示为关键字，false表示非关键字
    //参数说明:
    //参数名称                参数类型                    描述
    //Name                    const XMLCh*               要效验的字符
	static bool validateKeyword(const XMLCh* Name);

    //验证常量的数据类型
	//方法描述：检验数据类型-验证长量用来
	//返回值:   
    //返回类型                描述
    //参数说明:
    //参数名称                参数类型                    描述
    //dtString            const XMLCh*  const         要效验的数据
	//manager             MemoryManager* const        内存管理成员
    static VARTYPE  validateConstant(const XMLCh* const dtString);

	//验证是否是COM类型定义
	//方法描述：验证是否是COM类型定义
	//返回值:   
    //返回类型                描述
    //参数说明:
    //参数名称                参数类型                    描述
    //dtString            const XMLCh*  const         要效验的数据
	//manager             MemoryManager* const        内存管理成员
    static bool validateComDef(const XMLCh* const dtString,XMLCh * comID);

	//验证是否是array类型定义
	//方法描述：验证是否是array类型定义
	//返回值:   
    //返回类型                描述
    //参数说明:
    //参数名称                参数类型                    描述
    //dtString            const XMLCh*  const         要效验的数据
	//manager             MemoryManager* const        内存管理成员
    static bool validateArrayDef(const XMLCh* const dtString/*,int * arraySub*/);


	//验证是否是数组类型定义,这里可用变量定义数组下标
	//方法描述：验证是否是COM类型定义
	//返回值:   
    //返回类型                描述
    //参数说明:
    //参数名称                参数类型                    描述
    //dtString            const XMLCh*  const         要效验的数据
	//manager             MemoryManager* const        内存管理成员
    static bool validateArrayDefByExp(const BaseBlock* const parent,const XMLCh* const dtString);


	//方法描述：检验函数调用实参的合法性
	//返回值:   true、 false
    //返回类型                描述
    //bool                true表示函数的实参合法，false表示不合法
    //参数说明:
    //参数名称                参数类型                    描述
	//parent              const BaseBlock* const      函数语句父块
    //argumentString      const XMLCh*  const         函数的实参
	//funcParmList        const XMLCh*  const         函数的形参
	//manager             MemoryManager* const        内存管理成员
	static bool validateCallArgumentLegality(const BaseBlock* const parent
		,const XMLCh* const argumentString
		,const XMLCh* const funcParmList
		,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager) ;

	//方法描述：检验COM调用参数的合法性,只能检测格式是否有效
	//返回值:   true、 false
    //返回类型                描述
    //bool                true表示函数的实参合法，false表示不合法
    //参数说明:
    //参数名称                参数类型                    描述
	//parent              const BaseBlock* const      函数语句父块
    //argumentString      const XMLCh*  const         函数的实参
	//manager             MemoryManager* const        内存管理成员
	static bool validateComArgumentLegality(const BaseBlock* const parent
		,const XMLCh* const argumentString
		,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager) ;

	//方法描述：验证变量名的格式,函数名,空间命名的格式是否正确
	//返回值:  true 或者 false
    //返回类型              描述
    //bool                 布尔值，true表示为格式正确，false表示不正确
    //参数说明:
    //参数名称              参数类型                    描述
    //Name                 const XMLCh*  const        要效验的函数名,空间命名字符
	//manager              MemoryManager* const        内存管理成员
	static bool validateName(const XMLCh* const Name,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager);

	//方法描述：验证变量名的合法性,检验变量名是否存在 tc4.0
	//返回值:   true、false
    //返回类型             描述
    //bool                true表示变量名已经存在，false表示不存在
    //参数说明:
    //参数名称             参数类型                    描述
	//parent              const BaseBlock* const      变量名的父块
    //varName             const XMLCh*  const         要效验的变量名
	//manager             MemoryManager* const        内存管理成员
	static bool validateVarNameExist(const BaseBlock* const parent,const XMLCh* const varName
		,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager) ;

	//方法描述：检验函数参数的合法性,检验里面的变量名是否合法 tc4.0
	//返回值:   true、false
    //返回类型                描述
    //bool                true表示函数参数合法，false表示不合法
    //参数说明:
    //参数名称                参数类型                    描述
	//parent              const BaseBlock* const      函数的父块
    //pramString          const XMLCh*  const         要效验的函数参数
	//manager             MemoryManager* const        内存管理成员
	static bool validateFuncPramLegality(const BaseBlock* const parent,const XMLCh* const pramString
		,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager) ;

    //方法描述：验证变量名的格式,函数名,空间命名的格式是否正确,这个方法不抛出异常
    //返回值:  true 或者 false
    //返回类型                描述
    //bool                    布尔值，true表示为格式正确，false表示不正确
    //参数说明:
    //参数名称                参数类型                    描述
    //Name                    const XMLCh*  const        要效验的函数名,空间命名字符
    //manager                 MemoryManager* const        内存管理成员
    static bool validateNameNoException(const XMLCh* const Name);

	//合法性的检验,一般用来块的定义中

	//方法描述：验证是否表示一个常量整数,用来做repeat语句的条件表达式使用
	//返回值:  true 或者 false
    //返回类型             描述
    //bool                布尔值，true表示为常量整数，false表示不是常量整数
    //参数说明:
    //参数名称             参数类型                    描述
    //IntegerString       const XMLCh*  const         要效验的字符
	//manager             MemoryManager* const        内存管理成员
	static bool validateIsUnsigendInteger(const XMLCh* const IntegerString,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager);

	//方法描述：验证全局变量的合法性,检验命名空间+变量名是否存在
	//返回值:   true、false
    //返回类型             描述
    //bool                true表示变量名已经存在，false表示不存在
    //参数说明:
    //参数名称             参数类型                    描述
	//parent              const BaseBlock* const      变量名的父块
    //varName             const XMLCh*  const         要效验的变量名
	//manager             MemoryManager* const        内存管理成员
	static bool validateGlobleVarNameExist(const XMLCh* const ns,const XMLCh* const varName
		,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager) ;

	static const BaseBlock* validateGlobleVarBlock(const XMLCh* const ns,const XMLCh* const varName,MemoryManager* const  manager) ;

	//方法描述：检验函数名的合法性,检验函数名是否存在
	//返回值:   true、false
    //返回类型                描述
    //bool                true表示函数名已经存在，false表示不存在
    //参数说明:
    //参数名称                参数类型                    描述
	//parent              const BaseBlock* const      函数名的父块
    //funcName            const XMLCh*  const         要效验的函数名
	//manager             MemoryManager* const        内存管理成员
	static bool validateFuncNameExist(const BaseBlock* const parent,const XMLCh* const funcName
		,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager) ;

	//方法描述：检验表达试的合法性,检查里面的变量名或者函数名是否合法
	//返回值:   MyValue::DataType
    //返回类型                描述
    //MyValue::DataType   MyValue::DataType的具体值，失败返回的是MyValue::dt_MAXCOUNT
    //参数说明:
    //参数名称                参数类型                    描述
	//parent              const BaseBlock* const      表达式的父块
    //funcName            const XMLCh*  const         要效验的表达式
	//manager             MemoryManager* const        内存管理成员
	static VARTYPE validateExpressionLegality(const BaseBlock* const parent,const XMLCh* const expName
		,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager) ;

	//方法描述：检验一个函数参数列表中是否包含一个变量名
	//返回值:   true、false
    //返回类型                描述
    //bool                true表示已经包含，false表示没有包含
    //参数说明:
    //参数名称                参数类型                    描述
	//pramString          const XMLCh* const          函数参数字符串
    //varName             const XMLCh*  const         要效验的变量名
	//manager             MemoryManager* const        内存管理成员
	static bool validatePramListContainsName(const XMLCh* const pramString,const XMLCh* const varName
		,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager) ;

	//方法描述：检验函数调用的合法性
	//返回值:   MyValue::DataType
    //返回类型                描述
    //MyValue::DataType   MyValue::DataType的具体值，MyValue::dt_MAXCOUNT表示函数调用语句不合法
    //参数说明:
    //参数名称                参数类型                    描述
	//parent              const BaseBlock* const      函数调用语句父块
    //callSataement       const XMLCh*  const         调用的函数语句
	//manager             MemoryManager* const        内存管理成员
	static bool validateCallLegality(const BaseBlock* const parent
		,const XMLCh* const callSataement
		,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager);

	//方法描述：验证 Break或者Continue 的合法性
	//返回值:   true、false
    //返回类型                描述
    //bool                true表示Break或者Continue合法性，false表示不合法
    //参数说明:
    //参数名称                参数类型                    描述
	//parent              const BaseBlock* const      函数返回语句父块
	//manager             MemoryManager* const        内存管理成员
	static bool  validateBreakOrContinue(const BaseBlock* const parent,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager);

	/////////////////////////////////////////////数组相关/////////////////////////////////////////
	//方法描述：验证数组成员调用的格式是否正确
	//返回值:  true 或者 false
    //返回类型              描述
    //bool                 布尔值，true表示为格式正确，false表示不正确
    //参数说明:
    //参数名称              参数类型                    描述
    //Name                 const XMLCh*  const        要效验的数组成员调用
	//manager              MemoryManager* const        内存管理成员
	static bool validateArraySubCall(const BaseBlock* const parent,const XMLCh* const buff,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager);

    //方法描述：验证format(格式化)字符串数据的参数合法性
    //返回值:   true、false
    //返回类型                描述
    //bool                true表示可以赋值，false表示不可以赋值
    //参数说明:
    //参数名称                参数类型                    描述
    //express             const XMLCh* const          格式化函数的参数
    //manager             MemoryManager* const        内存管理成员
    //如果可以返回真
    static bool validateFormatArgument(const BaseBlock* const parent,const XMLCh* const argumentStr,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager);

    //得到类型的字符串
	//方法描述：得到类型的字符串
	//返回值:   
    //返回类型                描述
    //参数说明:
    //参数名称                参数类型                    描述
    static const XMLCh * getVarTypeString(VARTYPE vt);

	//方法描述：验证goto的标记合法性
    //返回值:   true、false
    //返回类型            描述
    //bool                true表示可以赋值，false表示不可以赋值
    //参数说明:
    //参数名称            参数类型                    描述
    //lable               const XMLCh* const          格式化函数的参数
    //manager             MemoryManager* const        内存管理成员
    //如果可以返回真
    static bool validateGoto(const BaseBlock* const parent,const XMLCh* const lable,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager);


private:
	MyValidator();
};

MATCH_CPP_NAMESPACE_END

#endif
