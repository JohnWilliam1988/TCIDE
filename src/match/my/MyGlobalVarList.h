/////////////////////////////////2009.12.16  肖秀范修订//////////////////////////

#ifndef MYGLOBALVARLIST_H
#define MYGLOBALVARLIST_H

#include"MyVar.h"
#include"MyVarList.h"

MATCH_CPP_NAMESPACE_BEGIN

//用来表示全局变量列表节点
class PARSERS_EXPORT  GlobalVarNode 
{
public:
    //
    //方法描述:构造函数                
    //参数说明:
    //参数名称              参数类型                    描述
    //memoryManager         MemoryManager* const        内存管理成员
	GlobalVarNode();

    //析构函数
	~GlobalVarNode();

    //方法描述:设置下一个全局链表节点
    //
    //返回值:
    //返回类型                              描述
    //void                                  无返回类型
    //
    //参数说明:
    //参数名称      参数类型                描述
    //node          GlobalVarNode * const   全局链表节点
	void setNextNode(GlobalVarNode * const node);

    //方法描述:得到下一个节点
    //
    //返回值:
    //返回类型                              描述
    //GlobalVarNode *                       全局链表节点
    //
    GlobalVarNode *getNextNode() const;

    //方法描述:设置命名空间
    //
    //返回值:
    //返回类型                              描述
    //void                                  无返回类型
    //
    //参数说明:
    //参数名称      参数类型                描述
    //spName        const XMLCh* const      空间名
	void setNameSpace(const XMLCh* const spName);

    //方法描述:得到命名空间
    //
    //返回值:
    //返回类型                              描述
    //const XMLCh *                         得到空间名
	const XMLCh *getNameSpace() const;

	//得到节点的变量列表
	MyVarList* getVarList();//const;
private:

	void initialize();
	void cleanUp();

    MyVarList             fList;                    //变量列表
	GlobalVarNode *       fNext;                    //下一个节点的指针
	XMLCh                 fNameSpace[NAMEMAXLENGTH];//空间名
};

//MyGlobalVarListManager主要用来存在和管理全局变量
class PARSERS_EXPORT  MyGlobalVarListManager
{
public:
    //方法描述:构造函数                
    //参数说明:
    //参数名称              参数类型                    描述
    //memoryManager         MemoryManager* const        内存管理成员
    MyGlobalVarListManager();

    //析构函数
    ~MyGlobalVarListManager();

    //方法描述:添加一个全局变量列表节点
    //
    //返回值:
    //返回类型                            描述
    //void                                无返回类型
    //
    //参数说明:
    //参数名称      参数类型              描述
    //list           GlobalVarNode *      全局变量列表节点
	void appendNode(GlobalVarNode *list);

    //方法描述:通过空间名字找到全局变量列表
    //
    //返回值:
    //返回类型                            描述
    //MyVarList *                         全局变量列表
    //
    //参数说明:
    //参数名称      参数类型              描述
    //nameSpace     const XMLCh * const   空间名
    MyVarList *findListByNameSpace(const XMLCh * nameSpace);

    //方法描述:得到全局变量表的头
    //返回值::
    //返回类型                            描述
    //GlobalVarNode *                    全局变量表的头
    GlobalVarNode *getListHead();

    //方法描述:得到全局变量表的尾
    //返回值::
    //返回类型                            描述
    //GlobalVarNode *                    全局变量表的尾
	GlobalVarNode *getListEnd();

    //方法描述:设置一个全局变量的值
    //
    //返回值:
    //返回类型                            描述
    //bool                                true:设置成功,false:设置失败
    //
    //参数说明:
    //参数名称      参数类型              描述
    //nameSpace     const XMLCh * const   空间名
    //varName       const XMLCh * const   变量名
    //value         XMLCh *               值的字符串表现形式
    //bool setVarValue(const XMLCh * const nameSpace,const XMLCh* const varName,XMLCh *value,VARTYPE type);

	  //清理
	void cleanUp();

	//链表初始化
	static void initializeStaticData();
	
    //链表初始化
	static void cleanUpStaticData();
private:
	//链表初始化
	void initialize();
  
	GlobalVarNode *           fListHead;                 //头节点
	GlobalVarNode *           fListEnd;                  //尾节点
};

MATCH_CPP_NAMESPACE_END

#endif