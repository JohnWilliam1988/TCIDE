/////////////////////////////////2009.12.19  肖秀范修订//////////////////////////

#ifndef MYVARLISTCONTAINER_H
#define MYVARLISTCONTAINER_H

#include "MyVarList.h"

MATCH_CPP_NAMESPACE_BEGIN

//变量列表的一个容器,不负责清楚变量的内存,只是用来变量列表对子块传递的时候
//把变量列表重新装载一次,然后传给子块,这样就不会影响父块中的变量列表的结构
class PARSERS_EXPORT MyVarListContainer
{
public:
    //
    //方法描述:构造函数                
    //参数说明:
    //参数名称              参数类型                    描述
    //memoryManager         MemoryManager* const        内存管理成员
	MyVarListContainer();

    //析构函数
	~MyVarListContainer();
	
    //在根据一个变量名 链表中查选 一个变量
    //方法描述:向链表中添加一个节点
    //
    //返回值:
    //返回类型                描述
    //MyVar *                 变量指针
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //name          const XMLCh * const         变量名
	MyVar *findVarByName(const XMLCh *const name);


	//在根据一个数组名和一个数组下标获得数组成员的句柄
    //方法描述:向链表中添加一个节点
    //
    //返回值:
    //返回类型                描述
    //MyVar *                 变量指针
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //name         const XMLCh * const         变量名
	//int          数组的下标                   ...
    //MyVar *findArraySubVarByName(const XMLCh * const name,int c1,int c2=-1,int c3=-1);

    //判断一个变量是否存在
    bool VarNameExist(const XMLCh *const name) const;

    // 
    //方法描述:返回变量链表的头节点
    //返回值::
    //返回类型                            描述
    //const varNode *                     变量列表的头节点
	varNode *getListHead() const;

    // 
    //方法描述:返回变量链表的尾节点
    //返回值::
    //返回类型                            描述
    //const varNode *                     变量列表的尾节点
	varNode *getListEnd();

    //方法描述:连接一个变量链表到容器里面去
    //
    //返回值:
    //返回类型                描述
    //void                    无
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //list          const MyVarList * const     变量列表指针
	void catVarList(const MyVarList * const list);

    //方法描述:把一个变量容器连接到变量容器后面去
    //
    //返回值:
    //返回类型                描述
    //void                    无
    //
    //参数说明:
    //参数名称      参数类型                             描述
    //list          const MyVarListContainer * const     变量列表容器指针
	void catVarList(const MyVarListContainer * const list);

    //方法描述:根据变量名,修改变量的值
    //
    //返回值:
    //返回类型                描述
    //bool                    true表示修改成功,false表示修改失败
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //name          const XMLCh* const          变量名
    //value         const XMLCh* const          值的字符串表现形式
    //bool setVarValueByName(const XMLCh * const  name, const  XMLCh * const  value,VARTYPE type);

    //方法描述:根据变量名,修改变量的值
    //
    //返回值:
    //返回类型                描述
    //bool                    true表示修改成功,false表示修改失败
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //name          const XMLCh* const          变量名
    //value         const MyValue* const        值的MyValue类型的指针
    bool setVarValueByName(const XMLCh * const  name,const  _variant_t * const  value);

    //方法描述:在容器中添加一个变量
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //var           varNode *                   变量节点
	void appendVar(varNode *var);

private:

	//链表初始化
	void initialize();

    //清理
	void cleanUp();

	varNode *           fVarHead;                      //容器列表头节点
	varNode *           fVarEnd;                       //容器列表尾节点

    /**
    * Copy constructor
    */
    MyVarListContainer(const MyVarListContainer& node);
    MyVarListContainer& operator=(const MyVarListContainer& toAssign);
};

MATCH_CPP_NAMESPACE_END

#endif