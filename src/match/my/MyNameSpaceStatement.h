//////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-15  肖秀范      完成#include预处理语句的抽象基类MyIncludeStatement的设计
//////////////////////////////////////////////////////////////////////////

#ifndef MYNAMESPACESTATEMENT_H
#define MYNAMESPACESTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN
//这里我们把 namespace 语句 抽象成 mystatement 继承类
//namespace 语句主要是用来定义命名空间的
class PARSERS_EXPORT MyNameSpaceStatement : public MyStatement
{
public:
    //构造函数
    //参数说明:
    //参数名称      参数类型                    描述
    //spaceName     const XMLCh* const          空间名
    //id            const unsigned int          语句ID
    //manager       MemoryManager* const        内存管理器
	MyNameSpaceStatement(const XMLCh* const    spaceName
			          ,const unsigned int    id = 0
		              ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
 
	//得到命名空间名
    //方法描述:得到命名空间名
    //返回值::
    //返回类型                描述
    //const XMLCh*            空间名
	const XMLCh* getSpaceName() const;

    //析构函数
	virtual ~MyNameSpaceStatement();

private:
    //初始化
	void initialize();                 

	//清楚变量内存
    void cleanUp();

//
    //方法描述:设置空间名
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称       参数类型                    描述
    //includeString  const XMLCh* const          空间名
	void setSpaceName(const XMLCh* const spaceName);

	
	XMLCh            fNameSpace[NAMEMAXLENGTH];                  //记录空间名称

    /**
    * Copy constructor
    */
	MyNameSpaceStatement(const MyNameSpaceStatement&);
    MyNameSpaceStatement& operator=(const MyNameSpaceStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif