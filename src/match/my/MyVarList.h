/////////////////////////////////2009.12.19  肖秀范修订//////////////////////////

#ifndef MYVARLIST_H
#define MYVARLIST_H

#include"MyVar.h"
#include"match/My/MyValue.h"

MATCH_CPP_NAMESPACE_BEGIN

//定义了变量链表的节点类型
struct varNode
{
    MyVar   *             var;                  //变量指针
	varNode *             next;                 //指向下一个节点
    varNode()
    {
        var=0;
        next=0;
    }
};

//这里定义了一个变量链表管理类,主要用来管理局部变量
class PARSERS_EXPORT MyVarList 
{
public:
    //
    //方法描述:构造函数                
    //参数说明:
    //参数名称              参数类型                    描述
    MyVarList();

    //析构函数
	~MyVarList();

    //方法描述:向链表中添加一个节点
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //var           varNode *                   变量节点
	void appendVar(varNode *var);
	

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
    MyVar *findVarByName(const XMLCh * const name);

    //判断一个变量是否存在
    bool VarNameExist(const XMLCh * const name) const;

    //方法描述:返回变量链表的头节点
    //返回值::
    //返回类型                            描述
    //const varNode *                     变量列表的头节点
    const varNode *getListHead() const;

    // 
    //方法描述:返回变量链表的尾节点
    //返回值::
    //返回类型                            描述
    //const varNode *                     变量列表的尾节点
	const varNode *getListEnd() const;

    //方法描述:根据变量名,修改变量的值
    //
    //返回值:
    //返回类型                描述
    //bool                    true表示修改成功,false表示修改失败
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //name          const XMLCh* const          变量名
    //toSet         const XMLCh* const          值的字符串表现形式
    //bool setVarValueByName(const XMLCh * const name,const XMLCh * const value,VARTYPE type);

    //方法描述:根据变量名,修改变量的值
    //
    //返回值:
    //返回类型                描述
    //bool                    true表示修改成功,false表示修改失败
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //name          const XMLCh* const          变量名
    //toSet         const MyValue* const        值的MyValue类型的指针
	bool setVarValueByName(const XMLCh * const name,const _variant_t * const value);

    void reset();
private:
	//链表初始化
	void initialize();

    //清理,会清理包含的所有的变量的内存
	void cleanUp();

	varNode *           fVarHead;              //变量链表头节点
	varNode *           fVarEnd;               //变量链条尾节点


    /**
    * Copy constructor
    */
    MyVarList(const MyVarList& node);
    MyVarList& operator=(const MyVarList& toAssign);
};

MATCH_CPP_NAMESPACE_END

#endif