#if !defined(MYDLLCALL_H)
#define MYDLLCALL_H
#include"BaseBlock.h"
#include "MyValue.h"
#include "MyDataList.h"
#include"MyVarListContainer.h"
MATCH_CPP_NAMESPACE_BEGIN
	
class MyDllCall
{
public:
	static bool Cmd(MyDataList *argumentList,_variant_t* result);
	static VARTYPE GetType(const XMLCh*const typeString);
	static bool ValidateDllCall(const XMLCh*const fun);
private:
	//¹¹Ôìº¯Êý
	MyDllCall(void);

	/**
	* Copy constructor
	*/
    MyDllCall(const MyDllCall& pool);
    MyDllCall& operator=(const MyDllCall& toAssign);
};

MATCH_CPP_NAMESPACE_END
#endif