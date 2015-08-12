#if !defined(MYDEBUGINFOLIST_H)
#define MYDEBUGINFOLIST_H

#include <match/util/MatchDefs.hpp>
//#include <match/util/PlatformUtils.hpp>
#include <windows.h>

MATCH_CPP_NAMESPACE_BEGIN

//===============================================MyBreakPointInfo Node =====================================

//这里是断点信息的记录,记录当前的断点信息
class PARSERS_EXPORT MyBreakPointInfo
{
public:
    MyBreakPointInfo();
	//构造函数
	//参数说明:
	//参数名称      参数类型                    描述
	//filepath      XMLCh* const                文件名的路径
	//linenum       int                         行号
	MyBreakPointInfo(const XMLCh* const filepath
                    ,int linenum
                    );

	//析构函数
	~MyBreakPointInfo(void);

	//方法描述:获取文件名
	//返回值:  文件名
	//返回类型                          描述
	//XMLCh*                            文件名
	//参数说明:
	//参数名称      参数类型            描述
	//无
	const XMLCh*  getDebugFileName() const;

	//方法描述:获取行号
	//返回值:   行数
	//返回类型                          描述
	//int                               行号
	//参数说明:
	//参数名称      参数类型            描述
	//无
	int getDebugLineNum() const;

    //设置文件名路径
    void setFielName(const XMLCh * const name);

    //设置行号
    void setLineNum(XMLUInt32 lineNum);

private:
	//初始化
	void initialize();
	//清空
	void cleanUp();


	XMLCh             fFilename[MAX_PATH];      //文件路径
	XMLUInt32         fLineNum;                 //行号

};

//===============================================MyBPList ========================
//class PARSERS_EXPORT MyBPList
//{
//public:
//	//方法描述:构造函数
//	//参数说明:
//	//参数名称      参数类型              描述
//	//manager       MemoryManager* const  内存管理
//	MyBPList(void);
//
//	//析构函数
//	~MyBPList(void);
//
//	//方法描述:向调试列中加入一个调试信息节点
//	//返回值: void
//	//返回类型                                  描述
//	//无
//	//参数说明:
//	//参数名称      参数类型                    描述
//	//node          MyDebugInfo*                调试信息节点
//	bool inserNode(const XMLCh* const filepath,int linenum);
//
//
//    //方法描述:根据指定位置获取文件名
//    //返回值:  文件名
//    //返回类型                          描述
//    //XMLCh*                            文件名
//    //参数说明:
//    //参数名称      参数类型            描述
//    //无
//    const XMLCh*  getFileNameByIndex(unsigned int index) const;
//
//
//     //方法描述:根据指定位置获取行号
//	//返回值:   行数
//	//返回类型                          描述
//	//int                               行号
//	//参数说明:
//	//参数名称      参数类型            描述
//	//无
//	int getLineNumByIndex(unsigned int index) const;
//
//
//	//方法描述:获取列表头
//	//返回值: 调试信息节点
//	//返回类型                                  描述
//	//MyBreakPointInfo*                         断点指针
//	//参数说明:
//	//参数名称      参数类型                    描述
//	//无
//	unsigned int getLength() const;
//
//	//方法描述: 判断一个节点是否存在,
//	bool isExist(const XMLCh* const filepath,int linenum);
//
//	//去掉节点
//	bool removeNode(const XMLCh* const filepath,int linenum);
//
//	// 删除所有的节点
//	void removeAllNode();
//
//	// 判断列表是否为空
//	bool isEmpty();
//
//private:
//	//链表初始化
//	void initialize();
//	//清空
//	void cleanUp();
//
//	MyBreakPointInfo          fList[MATCH_MAXBREAKBPNUM];
//    unsigned int              fLeng;
//};

MATCH_CPP_NAMESPACE_END

#endif