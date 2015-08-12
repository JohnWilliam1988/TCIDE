//////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-15  肖秀范      完成执行块的抽象基类BaseBlock的设计
//0002      2010-7-16   钟斌        添加了成员变量fLineNum,fSourceFilePath以及成员函数getLineNum(),getBlockFilePath()
////////////////////////////////////////////////////////////////////////////////////////

#ifndef BASEBLOCK_H
#define BASEBLOCK_H

#include <match/util/MatchDefs.hpp>
#include <match/util/PlatformUtils.hpp>

#include <match/util/XMLException.hpp>
#include <match/util/XMLExceptMsgs.hpp>
#include "MyValidException.h"
#include <Windows.h>

MATCH_CPP_NAMESPACE_BEGIN

//块是火柴盒用来执行的抽象对象,BaseBlock块是所有执行块的抽象类
//块与语句的区别是:一个while语句 和 一个 endwhile 语句组成了一个while块的框架,
//块可以包含子块,但是语句不能包含语句,但不是块都需要包含子块

class PARSERS_EXPORT BaseBlock : public XMemory
{
public:

    //块类型的枚举类型
    enum BlockType
    {
        BLOCK_INCLUDE =0,            //include块
		BLOCK_IMPORT ,               //import块
        BLOCK_NAMESPACE,             //namespace块
        BLOCK_ENDNAMESPACE,          //endnamespace块
        BLOCK_VARIABLE,              //变量定义块
        BLOCK_FUNCTION,              //函数块
        BLOCK_ENDFUNCTION,           //end函数块
        BLOCK_IF ,                   //if块
        BLOCK_ELSE ,                 //else块
        BLOCK_ENDIF ,                //endif块
		BLOCK_SELECT ,               //select块
		BLOCK_ENDSELECT ,            //endselect块
		BLOCK_CASE ,                 //case块
		BLOCK_DEFAULT,               //default块
        BLOCK_WHILE,                 //while块
        BLOCK_ENDWHILE,              //endwhile块
        BLOCK_BREAK,                 //循环语句跳出
        BLOCK_CONTINUE,              //循环语句跳下一条
        BLOCK_REPEAT,                //repeat块
        BLOCK_ENDREPEAT,             //endrepeat块
        BLOCK_ASSIGNMENT,            //赋值块
        BLOCK_CALL,                  //函数调用块
		BLOCK_LABLE,                 //标记语句
		BLOCK_GOTO,                  //GOTO语句
        BLOCK_RETURN,                //函数返回块
		BLOCK_END,                   //公共的结束块
        BLOCK_UNKNOWN,               //未知
    };

    //构造函数
    //参数说明:
    //参数名称      参数类型                    描述
    //type          const BlockType             块类型
    //id            const unsigned int          块的ID
    //linenum       const unsigned int          行数
    //filepath      const XMLCh*   const        文件路径
    //manager       MemoryManager* const        内存管理器
    BaseBlock(const BlockType  type
        ,const unsigned int    id=0
        ,const unsigned int    linenum=0
        ,const XMLCh* const    filepath=NULL
        ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //方法描述:返回块的类型
    //返回值::
    //返回类型                描述
    //const BlockType         返回块的类型
    const BlockType getBlockType() const;    

    //析构函数
    virtual ~BaseBlock();

    //方法描述:返回块的下一个块
    //返回值::
    //返回类型                 描述
    //BlockType*         下一个块的指针
    BaseBlock* getNextBlock() const;

    //方法描述:返回块的上一个块
    //返回值::
    //返回类型                 描述
    //BlockType*         上一个块的指针
    BaseBlock* getPreBlock() const;

    //方法描述:得到此块的id
    //返回值::
    //返回类型                 描述
    //unsigned int             ID值
    unsigned int getID() const;

    //设置块的id
    //方法描述:得到此块的id
    //返回值::
    //返回类型                 描述
    //void                     无
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //id            unsigned int                ID值
    void setID(unsigned int id);

    //设置块的下一个块
    //方法描述:得到此块的id
    //返回值::
    //返回类型                 描述
    //void                      无
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //node          BaseBlock*                  块指针
    void setNextBlock(BaseBlock* node);

    //
    //方法描述:设置块的上一个块的id
    //返回值::
    //返回类型                 描述
    //无                       无
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //node          BaseBlock*                  块指针
    void setPreBlock(BaseBlock*  node);

    //方法描述:得到块行号
    //返回值:: 行号，整型数据
    //返回类型                 描述
    //XMLFileLoc               行号
    //
    //参数说明:无

    XMLFileLoc getBlockLineNum() const;

    //方法描述:得到块的所在文件路径
    //返回值:: 块所在的文件路径
    //返回类型                 描述
    //XMLCh                    文件路径
    //
    //参数说明:无   
    const XMLCh* getBlockFilePath() const;

protected:
    //清理内存
    void cleanUp();

    //块的初始化
    void initialize();

    //方法描述:设置文件路径
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //name          const XMLCh* const          文件路径
    void setFilePath(const XMLCh* const name);

    /**
    * Copy constructor
    */
    BaseBlock(const BaseBlock& node);
    BaseBlock& operator=(const BaseBlock& toAssign);

    MemoryManager*    fMemoryManager;           //内存管理
    BlockType         fType;                    //节点类型
    BaseBlock*        fNext;                    //上一个块的指针
    BaseBlock*        fPre;                     //下一个块的指针
    unsigned int      fId;                      //用于块的编号,在一个块表中是一个唯一标识
    XMLFileLoc        fLineNum;                 //标识块所在的行数
	XMLCh  	          fSourceFilePath[MAX_PATH];                  //记录该块所在的文件路径
};

MATCH_CPP_NAMESPACE_END


#endif