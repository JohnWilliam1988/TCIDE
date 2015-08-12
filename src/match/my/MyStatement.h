//////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-15  肖秀范      完成语句的抽象基类MyStatement的设计
//////////////////////////////////////////////////////////////////////////

#ifndef MYSTATEMENT_H
#define MYSTATEMENT_H

#include <match/util/MatchDefs.hpp>
#include <match/util/PlatformUtils.hpp>

#include <match/util/XMLException.hpp>
#include <match/util/XMLExceptMsgs.hpp>
#include "MyValidException.h"

MATCH_CPP_NAMESPACE_BEGIN

//这里是火柴盒的抽象类,我们一般把每个语句都抽象成一个 mystatement 的继承类
class PARSERS_EXPORT MyStatement : public XMemory
{
public:
	//初始化函数
    //语句类型枚举类型
	enum StatementType
	{
		STATEMENT_INCLUDE =0,          //include語句
	    STATEMENT_IMPORT =0,           //import語句
		STATEMENT_NAMESPACE ,          //namespace語句
		STATEMENT_ENDNAMESPACE ,       //namesace结束語句
		STATEMENT_IF ,                 //if語句
		STATEMENT_ELSE,                //else语句
		STATEMENT_ENDIF,               //if结束语句
		STATEMENT_SELECT,              //select语句
		STATEMENT_ENDSELECT,           //endselect语句
		STATEMENT_CASE,                //case语句
		STATEMENT_DEFAULT,             //case语句
		STATEMENT_WHILE,               //while 语句
		STATEMENT_BREAK,               //循环break语句
		STATEMENT_CONTINUE,            //循环continue语句
		STATEMENT_ENDWHILE,            //while 结束语句
		STATEMENT_DO,                  //do 语句
		STATEMENT_ENDDOWHILE,          //enddowhile 结束语句
		STATEMENT_REPEAT,              //repeat重复操作语句
		STATEMENT_ENDREPEAT,           //repeat重复操作结束语句
		STATEMENT_FUNCTION,            //函数定义开头语句
		STATEMENT_ENDFUNCTION,         //函数结尾语句
		STATEMENT_VARIABLE,            //变量定义语句
		STATEMENT_ASSIGNMENT,          //赋值语句
		STATEMENT_CALL,                //函数调用语句
		STATEMENT_RETURN,              //函数返回语句
		STATEMENT_UNKNOWN,             //未知
	};

    //构造函数
    //参数说明:
    //参数名称      参数类型                    描述
    //type          const StatementType         语句类型
    //id            const unsigned int          语句ID
    //manager       MemoryManager* const        内存管理器
    MyStatement(const StatementType  type
        ,const unsigned int    id = 0
        ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);


    //方法描述:返回语句类型
    //返回值::
    //返回类型                描述
    //const StatementType     返回语句的类型
    const StatementType getStatementType() const;                                  

    //方法描述:返回语句ID
    //返回值::
    //返回类型                描述
    //const unsigned int      返回语句的ID
    const unsigned int getStatementID() const;

    //析构函数
    virtual ~MyStatement();

protected:
    //初始化
    void initialize();                 

    //清楚变量内存
    void cleanUp();

    MemoryManager*      fMemoryManager;      //内存管理器
    StatementType       fType;               //节点类型
    unsigned int        fId;                 //用于节点语句编号语句编号,预留
    /**
    * Copy constructor
    */
    MyStatement(const MyStatement& node);
    MyStatement& operator=(const MyStatement& toAssign);
};

MATCH_CPP_NAMESPACE_END

#endif