//////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-07-15  肖秀范      完成实参抽象类MyDataNode的设计
////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYDATALIST_H
#define MYDATALIST_H
#include"MyValue.h"
MATCH_CPP_NAMESPACE_BEGIN
//我们用MyDataNode 用来表示一个实参节点,在函数调用的时候传值的时候用
class PARSERS_EXPORT MyDataNode 
{
public:
    //
    //方法描述:构造函数                
    //参数说明:
    //参数名称              参数类型                    描述
    //type                  const MyValue::DataType     数据类型
    //value                 const XMLCh * const         数据值的字符串表现形式
    MyDataNode();

    //析构函数
    ~MyDataNode();


    //方法描述:设置当我们是处理变量的时候,直接记录它的指针就好,方便引用处理
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //data          const MyValue* const        MyValue值的指针
    void setQuoteData(_variant_t* data);


    //方法描述:设置实参值
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //toSet         const XMLCh* const          值的字符串表现形式
    void  setValue(const XMLCh * const toSet,VARTYPE type);

    //
    // 
    //方法描述:得到实参的值的指针
    //返回值::
    //返回类型                            描述
    //const MyValue*                      实参的值的指针
    /*const*/ _variant_t* getData() /*const*/;

    //方法描述:设置下一个节点
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //node          MyDataNode * const          下一个节点的指针
    void setNextNode(MyDataNode * const node);

    // 
    //方法描述:得到下一个节点
    //返回值::
    //返回类型                            描述
    //MyDataNode *                        下一个节点的指针
    MyDataNode *getNextNode() const;
private:
    //初始化
    void initialize();
    //清理
    void cleanUp();

	//引用类型处理
	bool                      fQuote;                    //标示是否是引用变量
	_variant_t *              fQuoteData;               //引用变量
	//引用类型处理

    _variant_t                fData;                     //实参指针
    MyDataNode *              fNext;                     //下个节点

    /**
    * Copy constructor
    */
    MyDataNode(const MyDataNode& node);
    MyDataNode& operator=(const MyDataNode& toAssign);
};

//这个类主要用于处理实参列表的处理,过期时会销毁实参变量
class PARSERS_EXPORT MyDataList
{
public:
    //方法描述:构造函数                
    //参数说明:
    //参数名称              参数类型                    描述
    //memoryManager         MemoryManager* const        内存管理成员
    MyDataList(bool needDel=true);

    //析构函数
    ~MyDataList();

    //方法描述:向实参列表中加入一个实参
    //
    //返回值:
    //返回类型                            描述
    //void                                无返回类型
    //
    //参数说明:
    //参数名称      参数类型              描述
    //var           MyDataNode *          实参指针
    void appendNode(MyDataNode *var);

	//方法描述:把一个参数插入到倒数第二个参数里面去,这个是用来调WIN32时,调整参数位置使用的
    //
    //返回值:
    //返回类型                            描述
    //void                                无返回类型
    //
    //参数说明:
    //参数名称      参数类型              描述
    //var           MyDataNode *          实参指针
    void insertNodeByNode(MyDataNode *forwardNode,MyDataNode *var);


    //方法描述:得到实参数链表的头
    //返回值::
    //返回类型                            描述
    //MyDataNode *                        实参头的指针
    MyDataNode *getListHead();

    //方法描述:得到实参数链表的尾
    //返回值::
    //返回类型                            描述
    //MyDataNode *                        实参头的指针
    MyDataNode *getListEnd();

    //清除实参链表
    void clear();
private:
    //链表初始化
    void initialize();

    //内存清理
    void cleanUp();

    MyDataNode *           fNodeHead;                 //实参链表头
    MyDataNode *           fNodeEnd;                  //实参链表尾
	bool                   fNeedDel;

    /**
    * Copy constructor
    */
    MyDataList(const MyDataList& node);
    MyDataList& operator=(const MyDataList& toAssign);
};

MATCH_CPP_NAMESPACE_END

#endif