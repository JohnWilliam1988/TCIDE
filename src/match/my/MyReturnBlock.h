//////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-15  肖秀范      完成return执行块的抽象类MyReturnBlock的设计
//0002      2010-07-16  钟斌        在构造函数中添加了本块的行号和文件路径两个参数
/////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYRETURNBLOCK_H
#define MYRETURNBLOCK_H

#include "BlockList.h"
#include "MyReturnStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//这里是return执行块
class PARSERS_EXPORT MyReturnBlock :  public BaseBlock
{
public:
    //构造函数
    //参数说明:
    //参数名称           参数类型                    描述
    //returnStatement    MyStatement *               return语句
    //id                 const unsigned int          块的ID
    //manager            MemoryManager* const        内存管理器
	MyReturnBlock(MyStatement * returnStatement
		     ,const unsigned int    id = 0
			 ,const unsigned int    linenum=0
			 ,const XMLCh*          filepath=NULL
		     ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //析构
	~MyReturnBlock();                   

    //方法描述:得到return块的计算表达试
    //返回值::
    //返回类型                       描述
    //const XMLCh*                   条件表达试
	const XMLCh* getReturnExpressionString() const;

private:
    //初始化块
	void initialize();                      

    //清理块
	void cleanUp();                    

	MyReturnStatement* fReturnStatement;                         //return语句

    /**
    * Copy constructor
    */
	MyReturnBlock(const MyReturnBlock& node);
    MyReturnBlock& operator=(const MyReturnBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END



#endif