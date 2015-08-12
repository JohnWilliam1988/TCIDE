//////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-16  肖秀范      完成Call语句执行块的抽象类MyCallBlock的设计
//0002      2010-07-16  钟斌        在构造函数中添加了本块的行号和文件路径两个参数
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYGOTOBLOCK_H
#define MYGOTOBLOCK_H

#include "BlockList.h"
#include "MyGotoStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//我们这里把call执行块 抽象成 MyCallBlock 抽象类
//call执行块 主要是用来调用本地API或者是用户自己定义的API
class PARSERS_EXPORT MyGotoBlock : public BaseBlock
{
public:
    //构造函数
    //参数说明:
    //参数名称       参数类型                    描述
    //callStatement  MyStatement *               call语句
    //id             const unsigned int          块的ID
    //manager        MemoryManager* const        内存管理器
    MyGotoBlock(MyStatement * gotoStatement
        ,const unsigned int    id = 0
		,const unsigned int    linenum=0
		,const XMLCh*          filepath=NULL
        ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //析构函数
    ~MyGotoBlock();   

    //方法描述:返回函数调用表达试的字符串
    //返回值::
    //返回类型                描述
    //const XMLCh*            函数调用表达试字符串
    const XMLCh* getGotolable() const;

private:
    //初始化块
    void initialize();                 

    //清理块
    void cleanUp();                   

    MyGotoStatement* fGotoStatement;                      //Call语句

    /**
    * Copy constructor
    */
    MyGotoBlock(const MyGotoBlock& block);
    MyGotoBlock& operator=(const MyGotoBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END

#endif