//////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-15  肖秀范      完成#include预处理语句的抽象基类MyIncludeStatement的设计
//////////////////////////////////////////////////////////////////////////

#ifndef MYINCLUDESTATEMENT_H
#define MYINCLUDESTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//我们把include预定义指令 抽象成 mystatement 继承类
//include 预定义指令 主要是用来 包含其他文件，是在执行源文件前事先执行的 
class PARSERS_EXPORT MyIncludeStatement : public MyStatement
{
public:
    //构造函数
    //参数说明:
    //参数名称      参数类型                    描述
    //includeString const XMLCh* const          include语句包含的路径,注意这里是带双引号括起来的字符串
    //id            const unsigned int          语句ID
    //manager       MemoryManager* const        内存管理器
	MyIncludeStatement(const XMLCh* const    includeString
			          ,const unsigned int    id = 0
		              ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
 
    //方法描述:得到include,注意这里是带双引号括起来的字符串
    //返回值::
    //返回类型                描述
    //const XMLCh*            include包含的文件路径
    const XMLCh* getIncludeString() const;

    //析构函数
	virtual ~MyIncludeStatement();

private:
    //初始化
	void initialize();                

	//清楚变量内存
    void cleanUp();

	//
    //方法描述:设置include 包含的文件路径,注意这里是带双引号括起来的字符串
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称       参数类型                    描述
    //includeString  const XMLCh* const          include包含的文件路径的字符串,注意这里是带双引号括起来的字符串
	void setIncludeString(const XMLCh* const includeString);

	XMLCh*            fIncludeString;                   //记录include包含文件的路径例如  "file.txt"

    /**
    * Copy constructor
    */
	MyIncludeStatement(const MyIncludeStatement&);
    MyIncludeStatement& operator=(const MyIncludeStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif