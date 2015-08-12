//////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-15  肖秀范      完成repeat语句的抽象基类MyRepeatStatement的设计
//////////////////////////////////////////////////////////////////////////
#ifndef MYREPEATSTATEMENT_H

#define MYREPEATSTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//我们把 repeatstatement语句抽象成 mystatement 的继承类
//repeat语句主要是用来执行规定次数的重复操作
class PARSERS_EXPORT MyRepeatStatement : public MyStatement
{
public:

    //构造函数
    //参数说明:
    //参数名称      参数类型                    描述
    //num           unsigned int                重复次数    
    //id            const unsigned int          语句ID
    //manager       MemoryManager* const        内存管理器
	MyRepeatStatement(bool isCondition
		              ,const XMLCh* const    conditionExp
			          ,const unsigned int    id = 0
		              ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

	//构造函数
    //参数说明:
    //参数名称      参数类型                    描述
    //num           unsigned int                重复次数    
    //id            const unsigned int          语句ID
    //manager       MemoryManager* const        内存管理器
	MyRepeatStatement( const XMLCh* const    conditionExp
		              ,const XMLCh* const    VarName 
					  ,const XMLCh* const    initValue 
					  ,const XMLCh* const    forStep 
			          ,const unsigned int    id    =0
		              ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);


	bool isContant();

	const XMLCh* getVarName();

	const XMLCh* getInitValue();

	const XMLCh* getExp();

	const XMLCh* getStep();

	unsigned int getContantNum();

	//void setIsContant(bool isContant);

    void setVarName(const XMLCh* const name);

	void setInitValue(const XMLCh* const initial);

	void setExp(const XMLCh* const exp);

	void setStep(const XMLCh* const step);

    //方法描述:得到循环的次数
    //返回值::
    //返回类型                描述
    //unsigned int            循环的次数
    //unsigned int getRepeatNum() const;

    //析构函数
	virtual ~MyRepeatStatement();

private:
	 //初始化
	void initialize();                

	//清楚变量内存
    void cleanUp();

	//void setVarName(const XMLCh* const name);

	//void setInitValue(const XMLCh* const initial);

	//void setExp(const XMLCh* const exp);

	//void setStep(const XMLCh* const step);


	bool                    fIsContant;
	unsigned int            fRepeatContant;
	XMLCh                   fVarName[NAMEMAXLENGTH];
	XMLCh*                  fInitValue;
	XMLCh*                  fExp;
	XMLCh*                  fStep;

	MyRepeatStatement(const MyRepeatStatement&);
    MyRepeatStatement& operator=(const MyRepeatStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif