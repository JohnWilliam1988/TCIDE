//////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-15  肖秀范      完成#include预执行块的抽象类MyIncludeBlock的设计
//0002      2010-07-16  钟斌        在构造函数中添加了本块的行号和文件路径两个参数
/////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYINCLUDEBLOCK_H
#define MYINCLUDEBLOCK_H

#include "BlockList.h"
#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//#include预处理执行块的抽象类
class PARSERS_EXPORT MyIncludeBlock :  public BaseBlock
{
public:
    //构造函数
    //参数说明:
    //参数名称                参数类型                    描述
    //includeStatement        MyStatement *               #include语句
    //id                      const unsigned int          块的ID
    //manager                 MemoryManager* const        内存管理器
	MyIncludeBlock(MyStatement * includeStatement
		     ,const unsigned int    id = 0
			 ,const unsigned int    linenum=0
			 ,const XMLCh*          filepath=NULL
		     ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //析构
	~MyIncludeBlock();                   

    //方法描述:返回include包含的文件路径
    //返回值::
    //返回类型                       描述
    //const XMLCh*                   include包含的文件路径
	const XMLCh* getIncludeString() const;

private:
    //初始化块
	void initialize();                      

    //清理块
	void cleanUp();                       

	MyStatement* fIncludeStatement;                       //include语句

    /**
    * Copy constructor
    */
	MyIncludeBlock(const MyIncludeBlock& node);
    MyIncludeBlock& operator=(const MyIncludeBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END



#endif