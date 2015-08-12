#if !defined(MYSERIALIZEBLOCKLIST_H)
#define MYSERIALIZEBLOCKLIST_H

#include <match/util/MatchDefs.hpp>
#include <match/util/PlatformUtils.hpp>
#include "MyRuntimeManager.h"
#include "MyBlockListPool.h"
#include "MyIncludeBlock.h"
#include "MyNameSpaceBlock.h"
#include "MyEndNameSpaceBlock.h"
#include "MyFunctionBlock.h"
#include "MyEndFunctionBlock.h"
#include "MyIfBlock.h"
#include "MyElseBlock.h"
#include "MyEndIfBlock.h"
#include "MyVarBlock.h"
#include "MyAssignmentBlock.h"
#include "MyCallBlock.h"
#include "MyRepeatBlock.h"
#include "MyEndRepeatBlock.h"
#include "MyWhileBlock.h"
#include "MyEndWhileBlock.h"
#include "MyEndWhileBlock.h"
#include "MyBreakBlock.h"
#include "MyContinueBlock.h"
#include "MyReturnBlock.h"

MATCH_CPP_NAMESPACE_BEGIN

const unsigned int MAXTWINLENGTH=256;

class PARSERS_EXPORT MySerializeBlockList
{
public:
	//初始化
	static void initialize();
	//清理
	static void cleanUp();

	//方法描述: 序列化写入操作
	//返回值    bool
    //返回类型                描述
    //bool                    序列化成功或者失败
    //参数说明:
    //参数名称      参数类型                    描述
	//path       const XMLCh* const             文件路径
	static bool serializeList(const XMLCh* const path,const unsigned char* const key=NULL);

	//方法描述: 反序列化文件，使之成为模块链表
	//返回值    bool
    //返回类型                描述
    //bool                    反序列化成功或者失败
    //参数说明:
    //参数名称      参数类型                    描述
	//path       const XMLCh* const             文件路径
	static bool deserializeFileToBlockList(const XMLCh* const path);

	//方法描述: 反序列化包含文件
	//返回值    bool
    //返回类型                描述
    //bool                    反序列化成功或者失败
    //参数说明:
    //参数名称      参数类型                    描述
	//path       const XMLCh* const             文件路径
	static bool deserializeImportFile(const XMLCh* const path);


	//方法描述: 设置编译相关的两个路径
	//返回值    bool
    //返回类型                描述
    //bool                    反序列化成功或者失败
    //参数说明:
    //参数名称      参数类型                    描述
	//compilepath    const XMLCh* const         编译路径
	//temppath       const XMLCh* const         临时路径
	static bool setCompilePath(const XMLCh* const compilepath,const XMLCh* const srcPath);

	static void initialSrcTll();

private:
	//遍历一个命名空间的方法
	static void traversalBlocklist( const BaseBlock * templist);
	//写入一行数据
	static void serializeWriteFileLine(const XMLCh* const word);
	//读取一行数据
	static XMLCh* deserializeReadFileLine();
	//读取一行数据
	static XMLCh* ReadFileLine();

	static void  scanfFunctionBlock(BaseBlock * parentBlock,XMLCh * filepath);
	static void  scanfVarBlock( BaseBlock * parentBlock,XMLCh * filepath);
	static void  scanfAssigBlock( BaseBlock * parentBlock,XMLCh * filepath);
	static void  scanfCallBlock( BaseBlock * parentBlock,XMLCh * filepath);
	static void  scanfIfBlock( BaseBlock * parentBlock,XMLCh * filepath);
    static void  scanfElseBlock( BaseBlock * parentBlock,XMLCh * filepath,bool &isElseEnd);
	static void  scanfSelectBlock( BaseBlock * parentBlock,XMLCh * filepath);
    static void  scanfCaseBlock( BaseBlock * parentBlock,XMLCh * filepath);
    static void  scanfDefaultBlock( BaseBlock * parentBlock,XMLCh * filepath);
	static void  scanfWhileBlock( BaseBlock * parentBlock,XMLCh * filepath);
	static void  scanfRepeatBlock( BaseBlock * parentBlock,XMLCh * filepath);
	static void  scanfReturnBlock( BaseBlock * parentBlock,XMLCh * filepath);
	static void  scanfBreakBlock( BaseBlock * parentBlock,XMLCh * filepath);
	static void  scanfContinueBlock(BaseBlock * parentBlock,XMLCh * filepath);
	static void  scanfLableBlock(BaseBlock * parentBlock,XMLCh * filepath);
	static void  scanfGotoBlock(BaseBlock * parentBlock,XMLCh * filepath);

	static HANDLE fWriteFile;
    static HANDLE fReadFile;
	static XMLCh *fBuffer;
	static XMLCh *fBufferEnd;

	static XMLCh fCompilePath[MAX_PATH];
	static XMLCh fSourcePath[MAX_PATH];
	static XMLCh fTWinName[MAXTWINLENGTH][NAMEMAXLENGTH];
	static unsigned fTWinLength;
	MySerializeBlockList(void);
	~MySerializeBlockList(void);
};


MATCH_CPP_NAMESPACE_END

#endif

