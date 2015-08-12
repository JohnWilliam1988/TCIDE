#if !defined(MYDEBUGVALUELIST_H)
#define MYDEBUGVALUELIST_H

#include "MyVarListContainer.h"
#include "MyValue.h"

MATCH_CPP_NAMESPACE_BEGIN

// =============调试变量节点=================
class PARSERS_EXPORT MyDebugValue
{
public:
	//构造函数
	//参数说明:
	//参数名称            参数类型                    描述
	//namesp              const XMLCh* const          命名空间
	//name                const XMLCh* const          变量名
	//value               const MyValue*              变量
	//manager             MemoryManager* const        内存管理
	MyDebugValue(const XMLCh* namesp,const XMLCh* name,const _variant_t* value/*,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager*/);

    MyDebugValue();

	//析构函数
	~MyDebugValue(void);

	//方法描述:获取命名空间名
	//返回值:  命名空间名
	//返回类型                  描述
	//const XMLCh*             命名空间名
	//参数说明:
	//参数名称      参数类型                    描述
	//无
	const XMLCh*  getDebugNameSpace() const;

	//方法描述:获取变量名
	//返回值:  变量名
	//返回类型                  描述
	//const XMLCh*             变量名
	//参数说明:
	//参数名称      参数类型                    描述
	//无
	const XMLCh*  getDebugVarNames() const;	

	//方法描述:获取变量的类型
	//返回值:  变量的类型
	//返回类型                  描述
	//const XMLCh*           变量的类型
	//参数说明:
	//参数名称      参数类型                    描述
	//无
	const XMLCh* getValueType() const;

	//方法描述:获取变量的值
	//返回值:  变量的值
	//返回类型                  描述
	//const XMLCh*          变量的值
	//参数说明:
	//参数名称      参数类型                    描述
	//无
	const XMLCh*  getDebugVarValueString() const;

	
	//方法描述:设置下个变量节点
	//返回值:  void
	//返回类型                  描述
	//void
	//参数说明:
	//参数名称      参数类型                    描述
	//node         MyDebugValue* const         变量节点
	void  setNextNode(MyDebugValue* const node);

    //设置命名空间名
	void setNamesp(const XMLCh * const namesp);

	//设置变量名
	void setVarName(const XMLCh * const name);

	//设置变量类型
	void setVarType(const XMLCh*  type);

	//设置变量的值
	void setVarValue(const _variant_t* const value);
private:

	//初始化
	void initialize();
	//清空
	void cleanUp();

	XMLCh               fNamesp[NAMEMAXLENGTH];
	XMLCh               fVarName[NAMEMAXLENGTH];
	XMLCh               fDatatype[TYPEMAXLENGTH];
	XMLCh               fVarValue[VALUEMAXLENGTH];
};

//==============调试变量链表=======================

class PARSERS_EXPORT MyDebugValueList
{
public:

	//构造函数
	//参数说明:
	//参数名称            参数类型                    描述
	//manager             MemoryManager* const        内存管理
	MyDebugValueList(/*MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager*/);

	//析构函数
	~MyDebugValueList(void);

	//方法描述:连接变量节点
	//返回值:  void
	//返回类型                  描述
	//void
	//参数说明:
	//参数名称      参数类型                    描述
	//namesp        const XMLCh*               命名空间名
	//VarList       MyVarList*                 局部变量列表
	void catDebugValueToList(const XMLCh* namesp,MyVarList* VarList);

	//方法描述:连接变量节点
	//返回值:  void
	//返回类型                  描述
	//void
	//参数说明:
	//参数名称      参数类型                    描述
	//namesp        const XMLCh*               命名空间名
	//VarList       MyVarListContainer*        局部变量列表
	void catDebugValueToList(const XMLCh* namesp,MyVarListContainer* VarList);

    unsigned int getLength() const;

	//清空变量信息链表
	void cleanUp();

	// 判断变量信息链表是否为空
	bool isEmpty();

    	//方法描述:获取命名空间名
	//返回值:  命名空间名
	//返回类型                  描述
	//const XMLCh*             命名空间名
	//参数说明:
	//参数名称      参数类型                    描述
	//无
	const XMLCh*  getDebugNameSpaceByIndex(unsigned int) const;

	//方法描述:获取变量名
	//返回值:  变量名
	//返回类型                  描述
	//const XMLCh*             变量名
	//参数说明:
	//参数名称      参数类型                    描述
	//无
	const XMLCh*  getDebugVarNamesByIndex(unsigned int) const;	

	//方法描述:获取变量的类型
	//返回值:  变量的类型
	//返回类型                  描述
	//const XMLCh*           变量的类型
	//参数说明:
	//参数名称      参数类型                    描述
	//无
	const XMLCh* getValueTypeByIndex(unsigned int) const;

	//方法描述:获取变量的值
	//返回值:  变量的值
	//返回类型                  描述
	//const XMLCh*          变量的值
	//参数说明:
	//参数名称      参数类型                    描述
	//无
	const XMLCh*  getDebugVarValueStringByIndex(unsigned int) const;

private:

	//初始化
	void initialize();
	

	//变量节点插入的实现
	bool inserNode(const XMLCh* namesp,const XMLCh* varname,const _variant_t* value);
	
	MyDebugValue         fList[DEBUGVALUELISTMAXLENGTH];
	unsigned int         flength;
};

MATCH_CPP_NAMESPACE_END

#endif