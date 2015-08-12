/////////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-09-13  肖秀范      完成#import语句的抽象类MyImportStatement的设计
////////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYIMPORTSTATEMENT_H
#define MYIMPORTSTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//这里我们把import语句 抽象成 mystatement 的继承类
//
class PARSERS_EXPORT MyImportStatement : public MyStatement
{
public:
    //构造函数
    //参数说明:
    //参数名称        参数类型                    描述
    //progID         const XMLCh* const          com组件的接口ID
    //id             const unsigned int          语句ID
    //manager        MemoryManager* const        内存管理器
    MyImportStatement(const XMLCh* const    progID
		,const XMLCh* const    csNS
        ,const unsigned int    id = 0
        ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

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

    //析构函数
    virtual ~MyImportStatement();

private:

    //初始化
    void initialize();                 
    //清楚变量内存
    void cleanUp();

    //方法描述:设置ProgID名称
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //exp           const XMLCh* const         COM组件的ID名称
    void setProgIDString(const XMLCh* const id);

	//方法描述:设置空间名称
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //ns           const XMLCh* const          空间名称
    void setNameSpaceString(const XMLCh* const ns);


    XMLCh*            fProgID;                          //COM组件ID名
    XMLCh*            fNamespace;                       //空间名

    /**
    * Copy constructor
    */
    MyImportStatement(const MyImportStatement&);
    MyImportStatement& operator=(const MyImportStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif