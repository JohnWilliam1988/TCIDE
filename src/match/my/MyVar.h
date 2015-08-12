//////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-07-15  肖秀范      完成变量抽象类MyVar的设计
////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYVAR_H
#define MYVAR_H

#include <match/util/XMemory.hpp>
#include <match/util/PlatformUtils.hpp>
#include "MyValue.h"
#include <stdio.h>
#include <wchar.h>

//定义了com链表的节点类型
struct comNode
{
    void   *              pDis;                  //变量指针
	comNode *             next;                 //指向下一个节点
    comNode()
    {
        pDis=0;
        next=0;
    }
};

MATCH_CPP_NAMESPACE_BEGIN

class MyComInfor;
//我们把MyVar抽象成一个变量,存储变量名和变量存储的值
class PARSERS_EXPORT MyVar
{
public:
    //
    //方法描述:构造函数                
    //参数说明:
    //参数名称              参数类型                    描述
    MyVar(const XMLCh * const  name
        );

	 MyVar(const XMLCh * const  name
        ,_variant_t *  value
		,bool isQuote=false
        );

    ~MyVar();

    //方法描述:取得变量类型
    //返回值::
    //返回类型                            描述
    //VARTYPE            返回变量类型
    //VARTYPE  getDataType() const;

    //方法描述:得到变量名
    //返回值::
    //返回类型                            描述
    //const XMLCh*                        返回变量名
    const XMLCh*   getName() const;

    //方法描述:得到变量的值的指针
    //返回值::
    //返回类型                            描述
    //const MyValue*                      得到变量值的指针
     _variant_t*   getVarData();


	//方法描述:向链表中添加一个节点
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //var           varNode *                   变量节点
	void appendCom(comNode *com);


    //方法描述:记录一个dispatch接口,用来释放
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称      参数类型                    描述
 //   void  initialComInfor();

	////方法描述:返回一个MyComInfor接口
	//MyComInfor *GetComInfo();

    //方法描述:设置变量值
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //toSet         const MyValue* const        另一个变量值的指针
    void  setValue(const _variant_t * const toSet);

private:
    //方法描述:设置变量名
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //toSet         const XMLCh* const          变量名
    void  setName(const XMLCh * const toSet);

    //初始化
    void initialize();

    //清除变量内存
    void cleanUp();

    // -----------------------------------------------------------------------
    //  data members
    // -----------------------------------------------------------------------

	//引用类型处理
	bool                      fQuote;                    //标示是否是引用变量
	_variant_t *              fQuoteData;               //引用变量
	//引用类型处理

	//MyComInfor *              fDispinFor;
		
	//记录分配的COM对象的链表
	comNode *           fComHead;
	comNode *           fComEnd;

    _variant_t                fValue;               //变量值
    XMLCh			          fName[NAMEMAXLENGTH]; //变量名

    /**
    * Copy constructor
    */
    MyVar(const MyVar& node);
    MyVar& operator=(const MyVar& toAssign);
};


MATCH_CPP_NAMESPACE_END

#endif



