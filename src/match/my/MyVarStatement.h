/////////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-16  肖秀范      完成var变量定义语句的抽象类MyVarStatement的设计
////////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYVARSTATEMENT_H
#define MYVARSTATEMENT_H

#include "MyStatement.h"
#include "match/My/MyValue.h"

MATCH_CPP_NAMESPACE_BEGIN

//变量定义语句,目前只支持用常量初始化的变量定义语句
class MyVarStatement : public MyStatement
{
public:  
    //构造函数
    //参数说明:
    //参数名称      参数类型                    描述
    //name          const XMLCh* const          变量名
    //value         const XMLCh* const          变量值的字符串表现形式
    //id            const unsigned int          语句ID
    //manager       MemoryManager* const        内存管理器
    MyVarStatement(const XMLCh* const    name
		,VARTYPE type 
        ,const XMLCh* const    value = 0
        ,const unsigned int    id = 0
        ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);


	//方法描述:返回变量名
    //返回值::
    //返回类型                   描述
    //const XMLCh*               变量名的字符串的表现形式
    const XMLCh* getVarNameString() const;

    //方法描述:返回变量值的字符串的表现形式
    //返回值::
    //返回类型                   描述
    //const XMLCh*               变量值的字符串的表现形式
    const XMLCh* getVarValueString() const;


	//方法描述:返回变量值的类型,一般是指常量的类型
	//返回值::
    //返回类型                   描述
    //VARTYPE                    变量值的类型
	//VT_NULL                    空值
	//VT_VARIANT                 可变类型,可能是变量或者函数调用返回
	//VT_I4
	//VT_R8
	//VT_BSTR
	VARTYPE getValueType() const;

    //析构函数
    virtual ~MyVarStatement();

private:
    //初始化
    void initialize();                

    //清楚变量内存
    void cleanUp();

    //方法描述:设置变量名
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //name           const XMLCh* const         变量名
    void setVarNameString(const XMLCh* const name);

    //方法描述:设置变量值,目前只支持常量值来初始化变量
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称       参数类型                    描述
    //value           const XMLCh* const         变量值
    void setVarValueString(const XMLCh* const value);


    XMLCh              fVarName[NAMEMAXLENGTH];   //变量名
    XMLCh*             fVarValue;                 //变量值

	//这里目前只表达全局变量,因为全局变量现在只能初始化为,常量,或者COM类型,或者是数组
	//如果是常量则是常量类型
	//如果是COM类型则是:VT_DISPATCH
	VARTYPE            fValueType;                //变量值的类型,一般是指常量的类型,这里有一个疑问,能不不是常量都设置一个值
    /**
    * Copy constructor
    */
    MyVarStatement(const MyVarStatement&);
    MyVarStatement& operator=(const MyVarStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif