//////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-15  肖秀范      完成#include预执行块的抽象类MyImportBlock的设计
/////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYIMPORTBLOCK_H
#define MYIMPORTBLOCK_H

#include "BlockList.h"
#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//#import预处理执行块的抽象类
class PARSERS_EXPORT MyImportBlock :  public BaseBlock
{
public:
    //构造函数
    //参数说明:
    //参数名称                参数类型                    描述
    //importStatement         MyStatement *               #import语句
    //id                      const unsigned int          块的ID
    //manager                 MemoryManager* const        内存管理器
	MyImportBlock(MyStatement * importStatement
		     ,const unsigned int    id = 0
			 ,const unsigned int    linenum=0
			 ,const XMLCh*          filepath=NULL
		     ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //析构
	~MyImportBlock();                   

    //方法描述:返回ProgID
    //返回值::
    //返回类型                描述
    //const XMLCh*           COM组件的ID名称
    const XMLCh* getProgIDString() const;

    //方法描述:返回com组件的的空间名
    //返回值::
    //返回类型                描述
    //const XMLCh*            com组件的的空间名
    const XMLCh* getNameSpaceString() const;

private:
    //初始化块
	void initialize();                      

    //清理块
	void cleanUp();                       

	MyStatement* fImportStatement;                       //Import语句

    /**
    * Copy constructor
    */
	MyImportBlock(const MyImportBlock& node);
    MyImportBlock& operator=(const MyImportBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END



#endif