//////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-16  肖秀范      完成赋值表示试语句执行块的抽象类MyAssignmentBlock的设计
//0002      2010-05-17  肖秀范      去掉了fParentBlock父指针的记录
//0003      2010-07-16  钟斌        在构造函数中添加了本块的行号和文件路径两个参数
////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYASSIGNMENTBLOCK_H
#define MYASSIGNMENTBLOCK_H

#include "BlockList.h"
#include "MyStatement.h"
#include "MyAssignmentStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//赋值语句执行块
class PARSERS_EXPORT MyAssignmentBlock : public BaseBlock
{
public:

    //构造函数
    //参数说明:
    //参数名称      参数类型                    描述
    //parentBlock   BaseBlock *                 父块的指针
    //varStatement  MyStatement *               赋值语句
    //id            const unsigned int          块的ID
    //manager       MemoryManager* const        内存管理器
    MyAssignmentBlock(/*BaseBlock *  parentBlock 暂时预留*/
        MyStatement * varStatement
        ,const unsigned int    id = 0
		,const unsigned int    linenum=0
		,const XMLCh*          filepath=NULL
        ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //析构函数
    ~MyAssignmentBlock();                

	//bool getIsArrayReDef();

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

    //方法描述:返回赋值语句的计算表达试
    //返回值::
    //返回类型                描述
    //const XMLCh*            计算表达试
    const XMLCh* getExpressionString() const;

	//方法描述:返回表达式的类型
    //返回值::
    //返回类型                描述
    //VARTYPE                 返回表达式的类型
	VARTYPE getValueType() const;

	//方法描述:返回数组下标表达式
    //返回值::
    //返回类型                描述
    //const XMLCh*            计算表达试
	const XMLCh* getArraySub() const;

	MyAssignmentStatement::TC_LEFTVARTYPE getLeftType() const;

	MyAssignmentStatement::TC_ASSIGNTYPE getAssignType() const;

/*    //方法描述:得到父块
    //返回值::
    //返回类型                描述
    //const BaseBlock*        父块指针
    const BaseBlock* GetParentBlock() const; */

private:
    //初始化块
    void initialize();           

    //清理块
    void cleanUp();                    

    MyAssignmentStatement*               fAssignmentStatement; //Assignment赋值语句

   /*    BaseBlock *                     fParentBlock;         //父块指针     */

    /**
    * Copy constructor
    */
    MyAssignmentBlock(const MyAssignmentBlock& node);
    MyAssignmentBlock& operator=(const MyAssignmentBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END


#endif