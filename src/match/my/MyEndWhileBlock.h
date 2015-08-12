//////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-17  肖秀范      完成endwhile语句执行块的抽象类MyEndWhileBlock的设计
//0002      2010-07-16  钟斌        在构造函数中添加了本块的行号和文件路径两个参数
//////////////////////////////////////////////////////////////////////////////////////////


#ifndef MYENDWHILEBLOCK_H
#define MYENDWHILEBLOCK_H

#include "BaseBlock.h"
#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//此类用来标识一个while块结束的地址.
class PARSERS_EXPORT MyEndWhileBlock :  public BaseBlock
{
public:
    //构造函数
    //参数说明:
    //参数名称            参数类型                    描述
    //endWhileStatement   MyStatement *               endwhile语句
    //id                  const unsigned int          块的ID
    //manager             MemoryManager* const        内存管理器
	MyEndWhileBlock(MyStatement* endWhileStatement
		     ,const unsigned int    id = 0
			 ,const unsigned int    linenum=0
			 ,const XMLCh*          filepath=NULL
		     ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //析构函数
	~MyEndWhileBlock();
private:
     //初始化块
	void initialize();                

    //清理块
	void cleanUp();                    

	MyStatement* fEndWhileStatement;                     //endWhile语句 

    /**
    * Copy constructor
    */
	MyEndWhileBlock(const MyEndWhileBlock& node);
    MyEndWhileBlock& operator=(const MyEndWhileBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END



#endif