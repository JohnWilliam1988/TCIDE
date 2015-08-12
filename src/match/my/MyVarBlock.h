//////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-15  肖秀范      完成var执行块的抽象类MyVarBlock的设计
//0002      2010-07-16  钟斌        在构造函数中添加了本块的行号和文件路径两个参数
/////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYVARBLOCK_H
#define MYVARBLOCK_H

#include "BlockList.h"
#include "MyVarStatement.h"

#include "MyValue.h"

MATCH_CPP_NAMESPACE_BEGIN

//这里是变量定义块
class PARSERS_EXPORT MyVarBlock : public BaseBlock
{
public:
    //构造函数
    //参数说明:
    //参数名称           参数类型                    描述
    //parentBlock        BaseBlock *                 父块的指针
    //varStatement       MyStatement *               变量定义语句
    //id                 const unsigned int          块的ID
    //manager            MemoryManager* const        内存管理器
    MyVarBlock(BaseBlock * parentBlock
        ,MyStatement * varStatement
        ,const unsigned int    id = 0
		,const unsigned int    linenum=0
		,const XMLCh*          filepath=NULL
        ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //析构函数
    ~MyVarBlock();                   

    //
    //方法描述:得到变量名
    //返回值::
    //返回类型                       描述
    //const XMLCh*                   变量名
    const XMLCh* getVarNameString() const;

    //
    //方法描述:得到变量值
    //返回值::
    //返回类型                       描述
    //const XMLCh*                   变量值
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

    //方法描述:得到父块
    //返回值::
    //返回类型                描述
    //const BaseBlock*        父块指针
    const BaseBlock* GetParentBlock() const;

private:
    //初始化块
    void initialize();  

    //清理块
    void cleanUp();       

    BaseBlock *           fParentBlock;               //记录父块

    MyVarStatement* fVarStatement;                       //Var语句

    /**
    * Copy constructor
    */
    MyVarBlock(const MyVarBlock& node);
    MyVarBlock& operator=(const MyVarBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END



#endif