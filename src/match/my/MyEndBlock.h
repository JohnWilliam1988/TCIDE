//////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人       动作以及目的 
//0001      2010-05-17  肖秀范      设计一个通用的结束块,此结束块在语法中是不存在的
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYENDBLOCK_H
#define MYENDBLOCK_H

#include "BaseBlock.h"
#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//此类是用来标识一个case执行块的结束块,这个块比较特殊,实际是没有这个语句的,为了整体的结构,所以加了这个语句
//endSelectStatement 暂时不给endcase准备语句
class PARSERS_EXPORT MyEndBlock :  public BaseBlock
{
public:
    //构造函数
    //参数说明:
    //参数名称             参数类型                     描述
    //endSelectStatement   MyStatement *               endrepeat语句
    //id                   const unsigned int          块的ID
    //manager              MemoryManager* const        内存管理器
	MyEndBlock(MyStatement* endStatement=NULL
		     ,const unsigned int    id = 0
			 ,const unsigned int    linenum=0
			 ,const XMLCh*          filepath=0
		     ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //析构函数
	~MyEndBlock();
private:
    //初始化块
	void initialize();             

     //清理块
	void cleanUp();                   

	MyStatement* fEndStatement;                             //endrepeat语句 

	MyEndBlock(const MyEndBlock& node);
    MyEndBlock& operator=(const MyEndBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END



#endif