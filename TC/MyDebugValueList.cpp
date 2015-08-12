#include "MyDebugValueList.h"

#include "../src/match/util/OutOfMemoryException.hpp"
#include <match/util/XMLString.hpp>

#include <match/util/XMLException.hpp>
#include <match/util/XMLExceptMsgs.hpp>
#include "MyValidException.h"
#include "MyValidator.h"
#include <wchar.h>
#include <stdio.h>

MATCH_CPP_NAMESPACE_BEGIN
//==============变量节点=======================

//构造函数
//参数说明:
//参数名称            参数类型                    描述
//namesp              const XMLCh* const          命名空间
//name                const XMLCh* const          变量名
//value               const MyValue*              变量
//manager             MemoryManager* const        内存管理
MyDebugValue::MyDebugValue(const XMLCh* namesp,const XMLCh* name,const _variant_t* value)
{
    initialize();

    //设置命名空间名
    setNamesp(namesp);

    //设置变量名
    setVarName(name);

    //设置变量类型
	setVarType(MyValidator::getVarTypeString(value->vt));

    setVarValue(value);
}


    
MyDebugValue::MyDebugValue()
{
    initialize();
}


//析构函数
MyDebugValue::~MyDebugValue(void)
{
    cleanUp();
}

//初始化
void MyDebugValue::initialize()
{
    ::memset(fNamesp,0,sizeof(XMLCh)*NAMEMAXLENGTH);
    ::memset(fVarName,0,sizeof(XMLCh)*NAMEMAXLENGTH);
    ::memset(fDatatype,0,sizeof(XMLCh)*TYPEMAXLENGTH);
    ::memset(fVarValue,0,sizeof(XMLCh)*VALUEMAXLENGTH);
}

//清空
void MyDebugValue::cleanUp()
{
    //--------修改,因为内存分配是由XMLString 来分配,但是没有由XMLString 来释放,所以有不安全代码--钟斌 9-19
    ::memset(fNamesp,0,sizeof(XMLCh)*NAMEMAXLENGTH);
    ::memset(fVarName,0,sizeof(XMLCh)*NAMEMAXLENGTH);
    ::memset(fDatatype,0,sizeof(XMLCh)*TYPEMAXLENGTH);
    ::memset(fVarValue,0,sizeof(XMLCh)*VALUEMAXLENGTH);
    //--------修改,因为内存分配是由XMLString 来分配,但是没有由XMLString 来释放,所以有不安全代码--钟斌 9-19---
}


//设置命名空间名
void MyDebugValue::setNamesp(const XMLCh * const namesp)
{
    ::memset(fNamesp,0,sizeof(XMLCh)*NAMEMAXLENGTH);

    if(!namesp || !(*namesp))
        return;

    ::memcpy(fNamesp,namesp,::wcslen(namesp)*sizeof(XMLCh));
}

//设置变量名
void MyDebugValue::setVarName(const XMLCh * const name)
{
    ::memset(fVarName,0,sizeof(XMLCh)*NAMEMAXLENGTH);

    if(!name || !(*name))
        return;

    ::memcpy(fVarName,name,::wcslen(name)*sizeof(XMLCh));
}

// 设置变量类型
void MyDebugValue::setVarType(const XMLCh* type)
{
    ::memset(fDatatype,0,sizeof(XMLCh)*TYPEMAXLENGTH);

    if(!type || !(*type))
        return;
    ::memcpy(fDatatype,type,::wcslen(type)*sizeof(XMLCh));
}

//设置变量的值
void MyDebugValue::setVarValue(const _variant_t* const value)
{
    // 设置变量值
	if(value->vt==VT_BOOL)
	{
		if(value->vt)
		{
			::memset(fVarValue,0,sizeof(XMLCh)*VALUEMAXLENGTH);
			::wcsncpy(fVarValue,L"true",VALUEMAXLENGTH);
		}
		else
		{
			::memset(fVarValue,0,sizeof(XMLCh)*VALUEMAXLENGTH);
			::wcsncpy(fVarValue,L"false",VALUEMAXLENGTH);
		}
	}
	else if(value->vt==VT_DISPATCH)
	{
		::memset(fVarValue,0,sizeof(XMLCh)*VALUEMAXLENGTH);
		::wsprintfW(fVarValue,L"%d",(int)value->pdispVal);
	}
	else
	{
		_variant_t dest;
		if(S_OK==VariantChangeType(&dest,value,VARIANT_NOVALUEPROP,VT_BSTR))
		{
			::memset(fVarValue,0,sizeof(XMLCh)*VALUEMAXLENGTH);
			::wcsncpy(fVarValue,dest.bstrVal,VALUEMAXLENGTH);
		}
	}
    return;
}


//方法描述:获取命名空间名
//返回值:  命名空间名
//返回类型                  描述
//const XMLCh*             命名空间名
//参数说明:
//参数名称      参数类型                    描述
//无
const XMLCh*  MyDebugValue::getDebugNameSpace() const
{
    return fNamesp;
}

//方法描述:获取变量名
//返回值:  变量名
//返回类型                  描述
//const XMLCh*             变量名
//参数说明:
//参数名称      参数类型                    描述
//无
const XMLCh*  MyDebugValue::getDebugVarNames() const
{
    return fVarName;
}

//方法描述:获取变量的类型
//返回值:  变量的类型
//返回类型                  描述
//const XMLCh*          变量的类型
//参数说明:
//参数名称      参数类型                    描述
//无
const XMLCh*  MyDebugValue::getValueType() const
{
    return fDatatype;
}

//方法描述:获取变量的值
//返回值:  变量的值
//返回类型                  描述
//const XMLCh*          变量的值
//参数说明:
//参数名称      参数类型                    描述
//无
const XMLCh*  MyDebugValue::getDebugVarValueString() const
{
    return fVarValue;
}

//==================================================================
//  由多个变量
//=================变量列表==================================

//构造函数
//参数说明:
//参数名称            参数类型                    描述
//manager             MemoryManager* const        内存管理
MyDebugValueList::MyDebugValueList(/*MemoryManager* const  manager*/)
{
    initialize();
}

//析构函数
MyDebugValueList::~MyDebugValueList(void)
{
    cleanUp();
}

//初始化
void MyDebugValueList::initialize()
{
    ::memset(fList,0,sizeof(MyDebugValue)*DEBUGVALUELISTMAXLENGTH);
    flength=0;
}

//清空
void MyDebugValueList::cleanUp()
{
    ::memset(fList,0,sizeof(MyDebugValue)*DEBUGVALUELISTMAXLENGTH);
    flength=0;
}


//变量节点插入的实现
bool MyDebugValueList::inserNode(const XMLCh* namesp,const XMLCh* varname,const _variant_t* value)
{

    if( !namesp || !varname )
    {
        throw;
        return false;
    }

    if(flength>=DEBUGVALUELISTMAXLENGTH)
        return false;

	if(value->vt&VT_ARRAY)
	{
		return false;
	}

	//if(value->fData.f_datatype==MyValue::dt_TCArray)
	//{
	//	if(value->fData.fValue.f_tcarray)
	//	{
	//		MyValue::MyArrayValue * pArray=value->fData.fValue.f_tcarray;
	//		if(pArray->fCount==1)
	//		{
	//			int length=pArray->fLength;
	//			for(int i=0;i<length;i++)
	//			{				
	//				if(flength>=DEBUGVALUELISTMAXLENGTH)
	//					return false;

	//				//调试的时候,打印变量数组
	//				XMLCh helpName[32]={0};
	//				::swprintf(helpName,32,L"%s[%d]",varname,i);
	//				fList[flength].setNamesp(namesp);
	//				fList[flength].setVarName(helpName);
	//				fList[flength].setVarType(value->getDataTypeString(value->fData.fValue.f_tcarray->fType));
	//				fList[flength].setVarValue(&(value->fData.fValue.f_tcarray->fValue[i]));
	//				flength++;
	//			}
	//		}
	//		else if(pArray->fCount==2)
	//		{
	//			int c1=pArray->fArrayCount[0];
	//			int c2=pArray->fArrayCount[1];

	//			for(int i=0;i<c1;i++)
	//			{
	//				for(int j=0;j<c2;j++)
	//				{				
	//					if(flength>=DEBUGVALUELISTMAXLENGTH)
	//						return false;

	//					XMLCh helpName[32]={0};
	//					::swprintf(helpName,32,L"%s[%d][%d]",varname,i,j);
	//					fList[flength].setNamesp(namesp);
	//					fList[flength].setVarName(helpName);
	//					fList[flength].setVarType(value->getDataTypeString(value->fData.fValue.f_tcarray->fType));
	//					fList[flength].setVarValue(&(value->fData.fValue.f_tcarray->fValue[i*c2+j]));
	//					flength++;
	//				}
	//			}

	//		}
	//		else if(pArray->fCount==3)
	//		{
	//			int c1=pArray->fArrayCount[0];
	//			int c2=pArray->fArrayCount[1];
	//			int c3=pArray->fArrayCount[2];
	//			for(int i=0;i<c1;i++)
	//			{
	//				for(int j=0;j<c2;j++)
	//				{				
	//					for(int k=0;k<c3;k++)
	//					{
	//						if(flength>=DEBUGVALUELISTMAXLENGTH)
	//							return false;

	//						XMLCh helpName[32]={0};
	//						::swprintf(helpName,32,L"%s[%d][%d][%d]",varname,i,j,k);
	//						fList[flength].setNamesp(namesp);
	//						fList[flength].setVarName(helpName);
	//						fList[flength].setVarType(value->getDataTypeString(value->fData.fValue.f_tcarray->fType));
	//						fList[flength].setVarValue(&(value->fData.fValue.f_tcarray->fValue[i*c2*c3+j*c3+k]));
	//						flength++;
	//					}
	//				}
	//			}

	//		}
	//	}
	//	
	//	return true;
	//}

    fList[flength].setNamesp(namesp);
    fList[flength].setVarName(varname);
	fList[flength].setVarType(MyValidator::getVarTypeString(value->vt));
    fList[flength].setVarValue(value);
    //设置变量类型
    flength++;
    return true;
}

// 判断变量信息链表是否为空
bool MyDebugValueList::isEmpty()
{
    return (flength==0);
}

unsigned int MyDebugValueList::getLength() const
{
    return flength;
}

//方法描述:获取命名空间名
//返回值:  命名空间名
//返回类型                  描述
//const XMLCh*             命名空间名
//参数说明:
//参数名称      参数类型                    描述
//无
const XMLCh*  MyDebugValueList::getDebugNameSpaceByIndex(unsigned int index) const
{
    return fList[index].getDebugNameSpace();

}
//方法描述:获取变量名
//返回值:  变量名
//返回类型                  描述
//const XMLCh*             变量名
//参数说明:
//参数名称      参数类型                    描述
//无
const XMLCh*  MyDebugValueList::getDebugVarNamesByIndex(unsigned int index) const
{
    return fList[index].getDebugVarNames();
}
//方法描述:获取变量的类型
//返回值:  变量的类型
//返回类型                  描述
//const XMLCh*           变量的类型
//参数说明:
//参数名称      参数类型                    描述
//无
const XMLCh* MyDebugValueList::getValueTypeByIndex(unsigned int index) const
{
    return fList[index].getValueType();
}

//方法描述:获取变量的值
//返回值:  变量的值
//返回类型                  描述
//const XMLCh*          变量的值
//参数说明:
//参数名称      参数类型                    描述
//无
const XMLCh*  MyDebugValueList::getDebugVarValueStringByIndex(unsigned int index) const
{
    return fList[index].getDebugVarValueString();
}
//方法描述:连接变量节点
//返回值:  void
//返回类型                  描述
//void
//参数说明:
//参数名称      参数类型                    描述
//namesp        const XMLCh*               命名空间名
//VarList       MyVarList*                 局部变量列表
void MyDebugValueList::catDebugValueToList(const XMLCh* namesp,MyVarList* VarList)
{
    const varNode *helpvarNode =VarList->getListHead();

    while(helpvarNode)
    {
        inserNode(namesp,helpvarNode->var->getName(),helpvarNode->var->getVarData());

        helpvarNode = helpvarNode->next;
    }
}

//方法描述:连接变量节点
//返回值:  void
//返回类型                  描述
//void
//参数说明:
//参数名称      参数类型                    描述
//namesp        const XMLCh*               命名空间名
//VarList       MyVarListContainer*        局部变量列表
void MyDebugValueList::catDebugValueToList(const XMLCh* namesp,MyVarListContainer* VarList)
{
    varNode *helpvarNode =VarList->getListHead();

    while(helpvarNode)
    {
        inserNode(namesp,helpvarNode->var->getName(),helpvarNode->var->getVarData());

        helpvarNode = helpvarNode->next;
    }
}


MATCH_CPP_NAMESPACE_END
