//////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-15  肖秀范      完成赋值表示试语句的抽象类MyAssignmentStatement的设计
////////////////////////////////////////////////////////////////////////////////////////
#ifndef MYASSIGNMENTSTATEMENT_H
#define MYASSIGNMENTSTATEMENT_H

#include "MyStatement.h"
#include "match/My/MyValue.h"
MATCH_CPP_NAMESPACE_BEGIN

//我们这里把 赋值表达试语句 抽象成一个 MyStatement 继承类
class PARSERS_EXPORT MyAssignmentStatement : public MyStatement
{
public:
	enum TC_LEFTVARTYPE
	{
		 TC_VAR                 =0
		,TC_ARRAYSUB            
		,TC_GLOBALVAR
		,TC_GLOBALARRAYSUB
		,TC_DISPROP
		,TC_ERRORLEFTTYPE       
	};

	enum TC_ASSIGNTYPE
	{
		 TC_DEC                 =0   
		,TC_INC                  
		,TC_MULLEFT     
		,TC_DIVLEFT
		,TC_ADDLEFT
		,TC_SUBLEFT
		,TC_STRADDLEFT
		,TC_ASSIGN
		,TC_ASSIGNMAX
	};

public:
    //构造函数
    //参数说明:
    //参数名称      参数类型                    描述
    //name          const XMLCh* const          左变量的名字
    //expression    const XMLCh* const          计算表达试
    //id            const unsigned int          语句ID
    //manager       MemoryManager* const        内存管理器
	MyAssignmentStatement(const XMLCh* const name
		                 ,const XMLCh* const expression
					     ,TC_LEFTVARTYPE  leftType
					     ,TC_ASSIGNTYPE   assignType
						 ,VARTYPE expType 
						 ,const XMLCh* const nameSP=0
						 ,const XMLCh* const arraySub=0
			             ,const unsigned int    id = 0
		                 ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);  


    //方法描述:返回赋值语句的左操作数的变量名
    //返回值::
    //返回类型                描述
    //const XMLCh*            左操作数的变量名
	const XMLCh* getVarNameString() const;

	//方法描述:返回空间名
    //返回值::
    //返回类型                描述
    //const XMLCh*            返回空间名
	const XMLCh* getSpNameString() const;

	
	//方法描述:返回表达式的类型
    //返回值::
    //返回类型                描述
    //VARTYPE                 返回表达式的类型
	VARTYPE getValueType() const;

    //方法描述:返回赋值语句的计算表达试
    //返回值::
    //返回类型                描述
    //const XMLCh*            计算表达试
	const XMLCh* getExpressionString() const;


	//方法描述:返回数组下标表达式
    //返回值::
    //返回类型                描述
    //const XMLCh*            计算表达试
	const XMLCh* getArraySub() const;

	MyAssignmentStatement::TC_LEFTVARTYPE getLeftType() const;

	MyAssignmentStatement::TC_ASSIGNTYPE getAssignType() const;

    //析构函数
	virtual ~MyAssignmentStatement();

private:
    //初始化
	void initialize();                 

   //清楚变量内存
    void cleanUp();  

    //方法描述:设置赋值表达试的左操作数的字符串
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //name          const XMLCh* const          左操作数字符串
	void setVarNameString(const XMLCh* const name);

	//方法描述:设置空间名
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //name          const XMLCh* const          空间名
	void setNameSPString(const XMLCh* const name);


    //方法描述:设置赋值表达试的计算表达试
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称         参数类型              描述
    //expression       const XMLCh* const    计算表达试
	void setExpressionString(const XMLCh* const expression);


	//方法描述:设置数组下标表达式
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称         参数类型              描述
    //expression       const XMLCh* const    数组下标表达式
	void setArraySub(const XMLCh* const exp);


	XMLCh             fVarName[NAMEMAXLENGTH];           //左操作数
	XMLCh             fNameSp[NAMEMAXLENGTH];            //空间名           //跨空间访问的时候,会用到
	XMLCh*            fExpression;                       //计算表达试
	XMLCh*            fArraySub;                         //鼠标的下标表达式
	TC_LEFTVARTYPE    fLeftType;                         //左参数类型
	TC_ASSIGNTYPE     fAssignType;                       //操作类型

	//表达式类型: VT_VARIANT
	//常量      : 常量类型
	//COM类型   : VT_DISPATCH
	VARTYPE            fExpType;                         //变量值的类型
    /**
    * Copy constructor
    */
    MyAssignmentStatement(const MyAssignmentStatement&);
    MyAssignmentStatement& operator=(const MyAssignmentStatement&);

};

MATCH_CPP_NAMESPACE_END

#endif