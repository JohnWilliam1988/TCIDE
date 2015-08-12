//////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-17  肖秀范      完成continue语句执行块的抽象类MyContinueBlock的设计
//0002      2010-07-16  钟斌        在构造函数中添加了本块的行号和文件路径两个参数
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYCONTINUEBLOCK_H
#define MYCONTINUEBLOCK_H

#include "BaseBlock.h"
#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//这里抽象的是 continue 执行块 
class PARSERS_EXPORT MyContinueBlock :  public BaseBlock
{
public:
    //构造函数
    //参数说明:
    //参数名称       参数类型                    描述
    //continueState  MyStatement *               continue语句
    //id             const unsigned int          块的ID
    //manager        MemoryManager* const        内存管理器
	MyContinueBlock(MyStatement * continueState
		,const unsigned int    id = 0
		,const unsigned int    linenum=0
		,const XMLCh*          filepath=NULL
		,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //析构函数
	~MyContinueBlock();

private:
     //初始化块
	void initialize();                

    //清理块
	void cleanUp();                    

	MyStatement* fContinueStatement;                    //contine语句

    /**
    * Copy constructor
    */
	MyContinueBlock(const MyContinueBlock& node);
    MyContinueBlock& operator=(const MyContinueBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END



#endif