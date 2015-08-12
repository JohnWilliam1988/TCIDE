#ifndef PROTOCOL_H__
#define PROTOCOL_H__

#ifdef PROTOCOL_EXPORTS
#define PROTOCOL_API _declspec(dllexport)
#else
#define PROTOCOL_API _declspec(dllimport)
#endif

//////////////////////////////////////////////////////////////////////////
// Common definitions for both client and server
// Application defined error codes
// Error codes are 32-bit values (bit 31 is the most significant bit). 
// Bit 29 is reserved for application-defined error codes; no system error code has this bit set.
// If you are defining an error code for your application, set this bit to one. 
//#define GET_PROTO_ERROR_CODE OA_ERROR_CODE-ERROR_OA_BASE - ERROR_PROTO_RANGE_FROM
enum OA_ERROR_CODE {

	ERROR_SERVER_RANGE_FROM				= 0,					// Error code of server module range from 0 to 4999
	ERROR_SERVER_BASE					= 0x20000000+ERROR_SERVER_RANGE_FROM,				
	ERROR_SERVER_RANGE_TO				= 4999,					// Error code of server module range from 0 to 4999 
	// Do not beyond this!
	ERROR_SERVER_BASE_END				= ERROR_SERVER_BASE+ERROR_SERVER_RANGE_TO,	

	ERROR_PROTO_RANGE_FROM				= 5000,					// Error code of protocol module range from 5000 to 6000
	ERROR_OA_BASE						= 0x20000000+ERROR_PROTO_RANGE_FROM,				

	ERROR_OA_CREATE_SOCKET_FAILED		= ERROR_OA_BASE+1,		// Failed to create socket.
	ERROR_OA_CONNECT_SERVER_FAILED		= ERROR_OA_BASE+2,		// Failed to connect to the server.
	ERROR_OA_INVALID_IP_ADDRESS         = ERROR_OA_BASE+3,		// Invalid IP address.
	ERROR_OA_INVALID_HEADER_FLAG		= ERROR_OA_BASE+4,		// Invalid flag in packet header.
	ERROR_OA_INVALID_HEADER_VER			= ERROR_OA_BASE+5,		// Invalid version in packet header.
	ERROR_OA_INVALID_HEADER_CMD			= ERROR_OA_BASE+6,		// Invalid command in packet header.
	ERROR_OA_SOCKET_ERROR				= ERROR_OA_BASE+7,		// Socket error occurred.
	ERROR_OA_INVALID_SOCKET				= ERROR_OA_BASE+8,		// Invalid socket.
	ERROR_OA_WSA_NOT_INITIALISED		= ERROR_OA_BASE+9,		// Either the application has not called WSAStartup, or WSAStartup failed.
	ERROR_OA_CONNECTION_CLOSED			= ERROR_OA_BASE+10,		// Socket connection closed.
	ERROR_OA_INVALID_KEY_LENGTH			= ERROR_OA_BASE+11,		// Invalid key length. (must be multiple of 16)
	ERROR_OA_SET_SOCKET_TIMEOUT_FAILED	= ERROR_OA_BASE+12,		// Failed to set socket timeout.
	//ERROR_OA_xxx						= ERROR_OA_BASE+13,		// 
	ERROR_OA_SET_BACK_BLOCK_MODE_FAILED = ERROR_OA_BASE+14,		// Failed to set back to block mode.
	ERROR_OA_INVALID_SESSION_ID			= ERROR_OA_BASE+15,		// Invalid session id in packet header.
	ERROR_OA_INVALID_DATASET_SEQ		= ERROR_OA_BASE+16,		// Invalid sequence number for extend SQL dataset.
	ERROR_OA_INVALID_DATASET_FLAG		= ERROR_OA_BASE+17,		// Invalid flag for extend SQL dataset.
	ERROR_OA_INVALID_DATASET_SLOT_SIZE	= ERROR_OA_BASE+18,		// Invalid size of SQL dataset slot.
	ERROR_OA_INVALID_TCP_PACKET_POINTER = ERROR_OA_BASE+19,		// Invalid pointer of tcp packet.
	ERROR_OA_PARSE_TCP_PACKET_FAILED    = ERROR_OA_BASE+20,		// Failed to parse tcp packet.
	ERROR_OA_INVALID_DATASET_BUFFER		= ERROR_OA_BASE+21,		// Invalid or corrupt dataset buffer.
	ERROR_OA_NOT_ENOUGH_BUFFER			= ERROR_OA_BASE+22,		// Not enough or corrupt dataset buffer.
	ERROR_OA_INVALID_BUFFER_OFFSET		= ERROR_OA_BASE+23,		// Invalid buffer offset.
	ERROR_OA_INVALID_BUFFER_LENGTH		= ERROR_OA_BASE+24,		// Invalid buffer length.
	ERROR_OA_INVALID_KEY_POINTER		= ERROR_OA_BASE+25,		// Invalid key pointer.
	//add by hegang
	ERROR_OA_USERNAMEORPSW_ERROR        = ERROR_OA_BASE+26,		// 用户名或密码错误
	//add end
	ERROR_OA_LISTEN_THREAD_CANCELLED	= ERROR_OA_BASE+25,		// Listen thread was cancelled.


	// ... 
	ERROR_PROTO_RANGE_TO				= 6000,					// Error code of protocol module range from 5000 to 6000
	// Do not beyond this!
	ERROR_OA_BASE_END					= ERROR_OA_BASE+ERROR_PROTO_RANGE_TO,	

	ERROR_OA_SUCCESS					= 0x20000000 + 0x0FFFFFFF,	

	////诚诚 2012-2-13 验证注册码错误信息
	ERROR_CHECKCODE_SUCCESS				=8000,//通过验证
	ERROR_INVALID_USER					=8100,//无效用户(用户VIP过期或者用户不是VIP)
	ERROR_INVALID_KEY					=8200,//无效KEY
	ERROR_EXPIRED_KEY					=8300,//过期KEY
	ERROR_FREEZE_KEY					=8400,//冻结KEY
	ERROR_INVALID_MACHINECODE           =8500,//KEY和机器码不匹配
	////诚诚 2012-2-13 验证注册码错误信息
	////诚诚 2012-2-15 解绑错误信息
	ERROR_UNBUNDLING_SUCCESS			=9000,//解绑成功
	ERROR_INVALID_UNBUNDLING			=9100,//解绑失败
	////诚诚 2012-2-15 解绑错误信息
	////诚诚 2012-3-10 检测更新错误信息
	ERROR_CHECKUPDATA_SUCCESS			=9500,//通过检测更新
	ERROR_CHECKUPDATA_UNSUCCESS			=9600, //检测更新失败
	////诚诚 2012-3-10 检测更新错误信息

	////诚诚 2012-3-13 检测更新错误信息
	ERROR_FINDSERVER_SUCCESS			=9700,//寻址成功
	ERROR_FINDSERVER_UNSUCCESS			=9800,//寻址失败

	////诚诚 2012-3-14 上传下载删除项目文件错误信息
	ERROR_DOWNPROFILE_SUCCESS			=9900,//获取服务器下载信息成功
	ERROR_DOWNPROFILE_UNSUCCESS,			  //获取服务器下载信息失败

	ERROR_UPPROFILE_SUCCESS,				//获取服务器上传信息成功
	ERROR_UPPROFILE_UNSUCCESS,				//获取服务器上传信息失败

	ERROR_DELETEFILEPRO_SUCCESS,			//获取服务器删除信息成功
	ERROR_DELETEFILEPRO_UNSUCCESS,			//获取服务器删除信息失败
	////诚诚 2012-3-14 上传下载删除项目文件错误信息
};

// String beyond the max length will be truncated
enum MAX_STRING_LENGTH {

//	MAX_SCRIPT_NAME             = 64,
//	MAX_SCRIPT_DESCRIPTION      = 302,
//	MAX_SHOP_NAME               = 42,
	MAX_USER_NAME               = 64,
	MAX_PASSWORD                = 36,
	MAX_MACHINE_CODE            = 64,
	MAX_CITY_NAME               = 64,
	MAX_QQ_IDS                  = 128,
	MAX_USER_NAME_ENCRYPTED	    = 64,
	MAX_RANDOM_NUMBER           = 32,
	MAX_KEY                     = 64,
	MAX_IP_ADDRESS              = 16,
	MAX_ERROR_MSG               = 260,
	MAX_NOTIFY_MSG              = 260,
	MAX_TOKEN                   = 32,
	MAX_DATASET_SLOT            = 2*1024,
	MAX_SQL_STRING              = 15*1024,
	//match-2012.2.11-加注册码的长度
	MAX_REGCODE					= 64,          //注册码长度
	MAX_PRONAME                 = 32,
	MAX_ERROR_MSG_CLIENT		= 260,
	MAX_DATASET_LEN				= 1000*1024,
	MAX_REGMACHINE_CODE			= 64,				//诚诚 2012-2-11 机器码长度
	MAX_PROFILENAMEINFO			= 1024,			//诚诚 2012-3-24 用户所有项目信息长度
};

// Timeout definitions (in seconds)
// Note: ExecuteSQL timeout was provided by client
enum TIMEOUT_VALUE {
	MAX_TIMEOUT					= 180,
	LOGIN_TIMEOUT_DEFAULT		= 60,
	LOGOUT_TIMEOUT_DEFAULT		= 10,									
};

// Flag and version
enum HEADER_FIELDS {
	HEADER_SIGNATURE			= 0xC4B6,
	PROTOCOL_VERSION			= 0x01
};

// Commands
enum PACKET_COMMANDS {
	CMD_UNKNOWN					= 0x00,
	CMD_LOGIN_REQUEST			= 0x01,
	CMD_LOGIN_ACK,
	CMD_VALIDATE_REQUEST,
	CMD_VALIDATE_ACK,
	CMD_LOGOUT_REQUEST,			
	CMD_TRANSFER_REQUEST		= 0x10,
	CMD_TRANSFER_ACK,
	//add by hegang
	CMD_GETSCRIPTTIME_REQUEST   = 0x20,
	CMD_GETSCRIPTTIME_ACK,
	//add end
	CMD_INDICATE_REQUEST		= 0x30,		// server send to client
	CMD_INDICATE_ACK,						// client send to server


	CMD_USERINFO_REQUEST =0x50,				//诚诚 2012-3-17 获取用户信息
	CMD_USERINFO_ACK,					    //诚诚 2012-3-17 获取用户信息

	CMD_FINDSERVER_REQUEST,					//诚诚 2012-3-13 服务器寻址
	CMD_FINDSERVER_ACK,						//诚诚 2012-3-13 服务器寻址

	CMD_CHECKCODE_REQUEST,                  //诚诚 2012-2-13 验证注册码（连接验证服务器）
	CMD_CHECKCODE_ACK,					    //诚诚 2012-2-13 验证注册码（连接验证服务器）
	CMD_UNBUNDLING_REQUEST,					//诚诚 2012-2-13 解绑机器码（连接验证服务器）
	CMD_UNBUNDLING_ACK,	                    //诚诚 2012-2-13 解绑机器码（连接验证服务器）

	CMD_GETINFOPROFILE_REQUEST,				//诚诚 2012-2-24 获取用户项目文件信息（连接验证服务器）
	CMD_GETINFOPROFILE_ACK,					//诚诚 2012-2-24 获取用户项目文件信息（连接验证服务器）

	CMD_DOWNPROFILE_REQUEST,				//诚诚 2012-3-13 下载项目文件（连接更新服务器）
	CMD_DOWNPROFILE_ACK,					//诚诚 2012-3-13 下载项目文件（连接更新服务器）

	CMD_UPPROFILE_REQUEST,					//诚诚 2012-3-13 上传项目文件（连接更新服务器）
	CMD_UPPROFILE_ACK,						//诚诚 2012-3-13 上传项目文件（连接更新服务器）

	CMD_DELETEFILEPRO_REQUEST,				//诚诚 2012-3-13 删除项目文件（连接更新服务器）
	CMD_DELETEUPFILEPRO_ACK,				//诚诚 2012-3-13 删除项目文件（连接更新服务器）

	CMD_CHECKUPDATA_REQUEST,				//诚诚 2012-3-10 检测更新
	CMD_CHECKUPDATA_ACK,					//诚诚 2012-3-10 检测更新

	CMD_UPDATAEXE_REQUEST,					//诚诚 2012-3-12 更新EXE（连接更新服务器）
	CMD_UPDATAEXE_ACK,						//诚诚 2012-3-12 更新EXE（连接更新服务器）

	/////文件下载///////////////
    MSG_DOWNSCRIPT_REQ=139, 
    MSG_DOWNSCRIPT_RSP,  
    MSG_DOWNSCRIPTINFO_REQ,
	MSG_DOWNSCRIPTINFO_RSP,
	////文件下载////////////////

	//王崧百 获取新闻信息 2012.12.29
	MSG_GETNOWNEWS_REQ,
	MSG_GETNOWNEWS_RSP,
	//王崧百 获取新闻信息 2012.12.29

	//王崧百 心跳包,防止重复登陆及多开 2013.1.5
	MSG_HEARTBEAT_REQ,
	MSG_HEARTBEAT_RSP,
	MSG_HEARTBEATSPECIAL_RSP,   //特殊处理
	//王崧百 心跳包,防止重复登陆及多开 2013.1.5

	MSG_GENERAL_ERR_RSP			= 0x58+60		// return from server for general error which is not specified for above process 
};

/////////////////////////////////////////////////////////////诚诚 2012-3-13 
// 寻址服务器类型
enum FINDSERVER_TYPE {
	FIND_REGCODESERVER					=0x10,
	FIND_UPDOWNFILESERVER,
	FIND_UPDATASERVER
};
/////////////////////////////////////////////////////////////诚诚 2012-3-13 

//王崧百 服务器返回值 2013.1.5
typedef enum
{
	ERR_APP_BASE        = 0x20000000,
	ERR_SECID_OL		= ERR_APP_BASE + 1,
	ERR_NOT_LOGIN,
	ERR_USER_LOGINED,
	ERR_USER_INFO,
	ERR_ALLOC_DB,
	ERR_ALLOC_MSG,
	ERR_ALLOC_OVERLAPPED,
	ERR_UNMATCH_TOKEN,
	ERR_LOGIN_STATE,
	ERR_IP_ADDR,
	ERR_TOKEN_OL,
	ERR_MAC_ADDR_OL,
	ERR_MAC_ADDR,
	ERR_RND_OL,
	ERR_PASSWORD_OL,
	ERR_PASSWORD_WRONG,
	ERR_USER_NAME_OL,
	ERR_USER_NAME_UNKNOW,
	EMPTY_CONTEXT,
	ERR_PAKCET_CMD,
	ERR_PAKCET_VERSION,
	ERR_PAKCET_FLAG,
	ERR_PAKCET_LENGTH,
	ERR_NULL_POINT,
	ERR_PUSH_MSG,
	ERR_SND_SOCKET,
	ERR_UDP_IND,
	ERR_SEC_ID,
	ERR_INDICATE_INUSING,
	ERR_TIMER_OUT_SET,
	KILL_FSM,
	//add by hegang
	ERR_CANNOTFIND_USER,
	ERR_DEC_ERROR,                        //解密时发生错误
	//add end
	//match = ERR_APP_BASE + 3000

	ERR_BUSINESS_BASE   = ERR_APP_BASE + 3000,
	ERR_DB_ILLEGAL_MAC  =ERR_BUSINESS_BASE +1,           //非法的网卡地址
	ERR_DB_ILLEGAL_USER,                                 //非法的用户名  
	ERR_DB_ERROR_PROJNAME,                               //注册码验证的项目类型错误
	ERR_DB_ILLEGAL_REG,                                  //非法的注册码
	ERR_DB_ERRREGTOUSER,                                 //注册码和用户不对应
	ERR_DB_OVERDUE_REG,                                  //过期的注册码
	ERR_DB_BINDED_MAC,                                   //机器码已绑定
	ERR_DB_FREEZING_REGCODE,                             //冻结的注册码
	ERR_DB_DELETE_REGCODE,                               //已删除的注册码
	ERR_DB_UNBIND_FAULT,                                 //解绑失败
	ERR_SERVER_TYPE,                                     //错误的服务器类型
	ERR_FORMAT_XML,                                      //CDataSetFormat::format发生了异常
	ERR_SQL_EXCUTE,                                      //CSqlCmd::Excute发生了异常
	ERR_DB_INTICOM_FAULT,                                //初始化COM组件失败
	ERR_DB_CREATECON_FAULT,                              //创建_ConnectionPtr连接对象
	ERR_DB_NOT_CREATECMD_FAULT,                          //创建_CommandPtr对象失败
	ERR_DB_NODEFINED_ERROR,                              //DB操作未定义的错误
	ERR_DB_GETPWD_FAULT,                                 //获取密码失败

	//KEVIN	= ERR_APP_BASE + 5000
	//ERR_APP_DB_BASE   = ERR_APP_BASE + 3000,
	//ERR_DB_NOT_READ     = ERR_APP_DB_BASE + 1,
	//ERR_DB_SQL,

	PROCESS = ERR_APP_BASE + 0x0FFFFFFF,
}SERVERERRORACK;


// Packet structures
#pragma pack(1)
typedef struct _TCP_SEND_HEADER {
	unsigned char head1;	// 0xA5
	unsigned char head2;	// 0xA6
	int len;				// length of TCP_PACKET
} TCP_SEND_HEADER, *PTCP_SEND_HEADER;

typedef struct _TCP_SEND_TAIL {
	unsigned char tail1;	// 0xB1
	unsigned char tail2;	// 0xB2
} TCP_SEND_TAIL, *PTCP_SEND_TAIL;

typedef struct _TCP_HEADER {
	unsigned short flag;
	unsigned char ver;
	unsigned char cmd;
	int len;
	int si[2];	// session id
} TCP_HEADER, *PTCP_HEADER;

typedef struct _TCP_PACKET {
	TCP_HEADER header;
	char body[];
} TCP_PACKET, *PTCP_PACKET;
#pragma pack()

// Body structure of login_request packet
typedef struct _LOGIN_REQ_BODY {

	char szUserName[MAX_USER_NAME];						// Username (Encrypted with AES)
	unsigned int uiLocalIPAddress;						// IP Address of local machine
	unsigned int uiExternalIPAddress;					// IP Address of external machine
	char szMachineCode[MAX_MACHINE_CODE];				// Hardware information(include MAC, HDD SN, etc. each one connect with '#')
	char szCity[MAX_CITY_NAME];							// City name
	char szQQIDs[MAX_QQ_IDS];							// QQ numbers gathered from local machine
	int nClientType;									// Type of client (OF/MF/FD/ZB)

} LOGIN_REQ_BODY, *PLOGIN_REQ_BODY;

// Body structure of login_ack packet
typedef struct _LOGIN_ACK_BODY {

	char szRandomNumber[MAX_RANDOM_NUMBER];				// RandomNumber (Encrypted with MD5)
	int nErrorCode;										// Error code
	char szErrorMsg[MAX_ERROR_MSG];						// Error code description

} LOGIN_ACK_BODY , *PLOGIN_ACK_BODY;

// Body structure of validate_req packet
typedef struct _VALIDATE_REQ_BODY {

	char szUserName[MAX_USER_NAME];						// Username (Encrypted with AES)
	char szRandomNumber[MAX_RANDOM_NUMBER];				// RandomNumber (Encrypted with MD5)

} VALIDATE_REQ_BODY , *PVALIDATE_REQ_BODY;

// Body structure of validate_ack packet
typedef struct _VALIDATE_ACK_BODY {

	char szKey[MAX_KEY];								// Key (Encrypted with MD5)
	char szToken[MAX_TOKEN];							// Token (Encrypted with MD5)
	int nErrorCode;										// Error code
	char szErrorMsg[MAX_ERROR_MSG];						// Error code description

} VALIDATE_ACK_BODY , *PVALIDATE_ACK_BODY;

// Body structure of sql_req packet
typedef struct _SQL_REQ_BODY {

	char szToken[MAX_TOKEN];				// Token to identify this client
	int nDestDB;							// Destination database No.
	//unsigned int nDBQueryTimeout;			// TODO: add this
	char szSQL[MAX_SQL_STRING];				// SQL string

} SQL_REQ_BODY, *PSQL_REQ_BODY;

// Body structure ok sql_ack packet
enum {
	SQL_FLAG_SINGLE = 0,
	SQL_FLAG_START = 1,
	SQL_FLAG_BODY = 2,
	SQL_FLAG_END = 3,
};
typedef struct _SQL_ACK_BODY {

	int nErrorCode;						// Error code
	char szErrorMsg[MAX_ERROR_MSG];		// Error code description
	int nIsDataset;						// Indicate the result is dataset
	int nFlag;							// Can be SINGLE, START, BODY and END
	int nRowsOfDataset;					// Rows of dataset
	int nColsOfDataset;					// Cols of dataSet
	int nDatasetBufSize;				// Dataset string buffer size, must be less than MAX_DATASET_SLOT
	char szDataset[MAX_DATASET_SLOT];	// Dataset string

} SQL_ACK_BODY , *PSQL_ACK_BODY;

// Extend body structure of sql_ack packet 
// Used when the dataset string can NOT be sent in one packet
typedef struct _SQL_ACK_BODY_EXT {
	
	int nFlag;							// Indicate start/end of sending for this kind of packets
	int nErrorCode;						// Error code
	char szErrorMsg[MAX_ERROR_MSG];		// Error code description
	int nDatasetSeq;					// Dataset string sequence, used to ensure the packet sequence is in order 
	int nDatasetBufSize;				// Dataset string buffer size, must be less than MAX_DATASET_SLOT
	char szDataset[MAX_DATASET_SLOT];	// Dataset string
	
} SQL_ACK_BODY_EXT , *PSQL_ACK_BODY_EXT;

// Body structure of error_ack packet
// Used to report general error from server to client
typedef struct _GENERAL_ERROR_BODY {

	int nErrorCode;						// Error code
	char szErrorMsg[MAX_ERROR_MSG];		// Error code description

} GERNERAL_ERROR_BODY, *PGERNERAL_ERROR_BODY;

//////////////////////////////////////////////////////////////////////////
// Structures used in Login(...)
typedef struct _LOGIN_INPUT {

	char *pszUserName;				// Username
	char *pszPassword;				// Password
	char *pszServerIPAddress;		// IP Address of remote server
	char *pszLocalIPAddress;		// IP Address of local machine
	char *pszExternalIPAddress;		// IP Address of external
	char *pszMachineCode;			// Hardware information(include MAC, HDD SN, etc. each one connect with '#')
	char *pszCity;					// City name
	char *pszQQIDs;					// QQ numbers gathered from local machine
	int nClientType;				// Type of client (OF/MF/FD/ZB)

} LOGIN_INPUT, *PLOGIN_INPUT;

typedef struct _LOGIN_OUTPUT {

	int nErrorCode;					// Error code
	int nErrorMsgBufSize;			// Error message buffer size
	char* pszErrorMsg;				// Error message

} LOGIN_OUTPUT, *PLOGIN_OUTPUT;

//////////////////////////////////////////////////////////////////////////
// Structures used in Login(...)
typedef struct _LOGOUT_REQ_BODY {

	char szToken[MAX_TOKEN];		// Token to identify this client

} LOGOUT_REQ_BODY, *PLOGOUT_REQ_BODY;

//////////////////////////////////////////////////////////////////////////
// Body structure of heart_beat_req packet
typedef struct _HEART_BEAT_REQ_BODY {

	char szToken[MAX_TOKEN];		// Token to identify this client

} HEART_BEAT_REQ_BODY, *PHEART_BEAT_REQ_BODY;

// Body structure ok heart_beat_ack packet
typedef struct _HEART_BEAT_ACK_BODY {

	int nErrorCode;					// Error code
	char szErrorMsg[MAX_ERROR_MSG];	// Error code description

	_HEART_BEAT_ACK_BODY()
	{
		memset(this,0,sizeof(_HEART_BEAT_ACK_BODY));
	}

} HEART_BEAT_ACK_BODY , *PHEART_BEAT_ACK_BODY;

//////////////////////////////////////////////////////////////////////////
// Structures used in ListenThread(...)
// Body structure ok indicate_req packet (server send to client)
typedef struct _INDICATE_REQ_BODY {

	int nNotifyCode;					// Notification code
	char szNotifyMsg[MAX_NOTIFY_MSG];	// Notification message

} INDICATE_REQ_BODY , *PINDICATE_REQ_BODY;

// Body structure of indicate_ack packet (client send to server)
typedef struct _INDICATE_ACK_BODY {

	int nNotifyCode;					// Notification code

} INDICATE_ACK_BODY , *PINDICATE_ACK_BODY;

/////////////////////////////////////////////////////////////诚诚 2012-2-10  验证码发包，收包结构体
//验证函数接收服务器回包结构体
typedef struct _CHECK_DATA_OUTPUT{
	int nDatatime;					//剩余倒数时间（分）
	int nErrorCode;					//注册码错误信息
}CHECK_DATA_OUTPUT ,*PCHECK_DATA_OUTPUT;

//验证函数入参结构体
typedef struct _CHECK_CODE_INPUT{
	char *pszUserName;									//用户名			
	char *pszKeyCode;									//注册码
	char *pszServerIPAddress;							//服务器IP地址
	char *pszMachineCode;								//机器码信息(include MAC, HDD SN, etc. each one connect with '#')
}CHECK_CODE_INPUT ,*PCHECK_CODE_INPUT;

//验证函数发包数据结构体
typedef struct _CHECK_CODE_BODY {
	char szUserName[MAX_USER_NAME];						//用户名 (AES加密)
	char RegCode[MAX_REGCODE];							//注册码(AES加密)
	char szMachineCode[MAX_REGMACHINE_CODE];			//机器码信息(include MAC, HDD，MD5加密)
	char szToken[MAX_TOKEN];							//动态密匙
} CHECK_CODE_BODY, *PCHECK_CODE_BODY;

//2012-2-13 解绑机器码函数接收服务器回包结构体
typedef struct _UNBUNDLING_OUTPUT{
	int UnbundlingCode;									//接收服务器回包解绑信息
	//int nErrorCode;										//错误信息							
}UNBUNDLING_OUTPUT ,*PUNBUNDLING_OUTPUT;

//2012-2-13 解绑机器码函数入参结构体
typedef struct _UNBUNDLING_INPUT{
	char *pszUserName;									//用户名			
	char *pszKeyCode;									//注册码
	char *pszServerIPAddress;							//服务器IP地址
	char *pszMachineCode;								//机器码信息(include MAC, HDD SN, etc. each one connect with '#')
}UNBUNDLING_INPUT ,*PUNBUNDLING_INPUT;

//2012-2-13 解绑机器码函数发包数据结构体
typedef struct _UNBUNDLING_BODY {
	char szUserName[MAX_USER_NAME];						//用户名 (AES加密)
	char RegCode[MAX_REGCODE];							//注册码(AES加密)
	char szMachineCode[MAX_REGMACHINE_CODE];			//机器码信息(include MAC, HDD，MD5加密)
	char szToken[MAX_TOKEN];							//动态密匙
} UNBUNDLING_BODY, *PUNBUNDLING_BODY;
/////////////////////////////////////////////////////////////诚诚 2012-2-10  验证码发包，收包结构体

/////////////////////////////////////////////////////////////诚诚 2012-3-10  检测更新发包，收包结构体
//2012-3-10 检测更新函数接收服务器回包结构体
typedef struct _CHECKUPDATA_OUTPUT{							//接收服务器回包检测更新信息
	int nErrorCode;										//返回信息
	char *pszServerIPAddress;							//服务器IP地址
	int nPort;                                            //服务器端口
}CHECKUPDATA_OUTPUT ,*PCHECKUPDATA_OUTPUT;

//2012-3-10 检测更新函数入参结构体
typedef struct _CHECKUPDATA_INPUT{
	char *pszUserName;									//用户名			
	char *pszKeyCode;									//注册码
	char *pszServerIPAddress;							//服务器IP地址
	char *pszMachineCode;								//机器码信息(include MAC, HDD SN, etc. each one connect with '#')
}CHECKUPDATA_INPUT ,*PCHECKUPDATA_INPUT;

//2012-3-10 检测更新函数发包数据结构体
typedef struct _CHECKUPDATA_BODY {
	char szUserName[MAX_USER_NAME];						//用户名 (AES加密)
	char RegCode[MAX_REGCODE];							//注册码(AES加密)
	char szMachineCode[MAX_REGMACHINE_CODE];			//机器码信息(include MAC, HDD，MD5加密)
	char szToken[MAX_TOKEN];							//动态密匙
} CHECKUPDATA_BODY, *PCHECKUPDATA_BODY;

//2012-3-10 检测更新函数接收服务器回包结构体
typedef struct _UPDATAEXE_OUTPUT{							//接收服务器回包检测更新信息
	int nErrorCode;										//返回信息
	int nLastFileLen;									//断点续传文件长度
}UPDATAEXE_OUTPUT ,*PUPDATAEXE_OUTPUT;

//2012-3-12 检测更新：发送更新服务器函数入参结构体
typedef struct _UPDATAEXE_INPUT {
	char *szToken;										//动态密匙（访问令牌）
	int nLastFileLen;									//断点续传文件长度
} UPDATAEXE_INPUT, *PUPDATAEXE_INPUT;
//2012-3-12 检测更新：发送更新服务器的发包数据结构体
typedef struct _UPDATAEXE_BODY {
	char szToken[MAX_TOKEN];							//动态密匙（访问令牌）
	int LastFileLen;									//断点续传文件长度
} UPDATAEXE_BODY, *PUPDATAEXE_BODY;

/////////////////////////////////////////////////////////////诚诚 2012-3-10  检测更新发包，收包结构体


////////////////////////文件下载////////////////////////////////////////
typedef struct _DOWNSCRIPTINFO_INPUT{

	char username[MAX_USER_NAME];                   //用户名最大20个中文
	char userpwd[MAX_PASSWORD];                     //用户密码最长32位
	char tokenTemp[MAX_TOKEN]; 					    //权限匹配字串
	float scriptVersion;							//资源文件版本
	char savePath[MAX_PATH];					    //本地保存路径
	int scriptID;								    //脚本ID唯一标示符
	int fileClinetsize;								//本地版本大小
	int userType;								    //用户类型

	_DOWNSCRIPTINFO_INPUT()//初始化
	{
		memset(this,0,sizeof(_DOWNSCRIPTINFO_INPUT)); 
	}

}DOWNSCRIPTINFO_INPUT ,*PDOWNSCRIPTINFO_INPUT;


typedef struct _DOWNSCRIPTINFO_OUTPUT{

	char serverIP[MAX_IP_ADDRESS];					//服务器IP
	int type;                                       //0:无更新 1:有更新
	char randKey[MAX_KEY];                          //下载验证key
	float newestVersion;							//最新资源文件版本
	char savePath[MAX_PATH];						//服务器资源文件保存路径
	int fileLenth;								    //服务器文件大小
	int fileClinetsize;								//本地版本大小
	float ptRunVersion;                             //平台版本
	int scriptTime;                                 //脚本剩余时间,0:脚本到期,正整数:剩余时间,负数:免费脚本
	int useType;                                    //脚本使用类型,0:试用,1:正常使用
	int resultValue;                                //存储过程执行结果返回值
// 	char scriptName[MAX_SCRIPT_NAME];				//脚本名
// 	char scriptDescription[MAX_SCRIPT_DESCRIPTION];	//脚本简介
// 	char shopName[MAX_SHOP_NAME];					//店铺名

	_DOWNSCRIPTINFO_OUTPUT()//初始化
	{
		memset(this,0,sizeof(_DOWNSCRIPTINFO_OUTPUT)); 
	}

}DOWNSCRIPTINFO_OUTPUT ,*PDOWNSCRIPTINFO_OUTPUT;

typedef struct _DOWNSCRIPT_INPUT{

	char serverIP[MAX_IP_ADDRESS];					//服务器IP
	int type;                                       //0:无更新 1:有更新
	char randKey[MAX_KEY];                          //下载验证key
	float newestVersion;							//最新资源文件版本
	char savePath[MAX_PATH];						//服务器资源文件保存路径
	int fileLenth;								    //服务器文件大小
	int fileClinetsize;								//本地版本大小

	_DOWNSCRIPT_INPUT()//初始化
	{
		memset(this,0,sizeof(_DOWNSCRIPT_INPUT)); 
	}

}DOWNSCRIPT_INPUT ,*PDOWNSCRIPT_INPUT;


typedef struct _DOWNSCRIPT_OUTPUT{

	int port;								        //服务器端口
	int resultValue;                                //存储过程执行结果返回值

	_DOWNSCRIPT_OUTPUT()//初始化
	{
		memset(this,0,sizeof(_DOWNSCRIPT_OUTPUT)); 
	}

}DOWNSCRIPT_OUTPUT ,*PDOWNSCRIPT_OUTPUT;

//返回信息
typedef struct _SCRIPTRINFO_OUTPUT{

	float newestVersion;							//最新资源文件版本
	int scriptTime;                                 //脚本剩余时间,0:脚本到期,正整数:剩余时间,负数:免费脚本
	int useType;                                    //脚本使用类型,0:试用,1:正常使用
	int resultValue;                                //存储过程执行结果返回值

	_SCRIPTRINFO_OUTPUT()//初始化
	{
		memset(this,0,sizeof(_SCRIPTRINFO_OUTPUT)); 
	}

}SCRIPTRINFO_OUTPUT ,*PSCRIPTRINFO_OUTPUT;

/////////////////////////文件下载//////////////////////////////////////////


#pragma pack(1)
///////////////////王崧百 心跳检测脚本剩余时间包 2012.11.10///////////////
typedef struct _GETSCRIPT_INPUT{

	char username[MAX_USER_NAME];                   //用户名最大20个中文
	char userpwd[MAX_PASSWORD];                     //用户密码最长32位
	int  scriptID;								    //脚本ID唯一标示符

	_GETSCRIPT_INPUT()//初始化
	{
		memset(this,0,sizeof(_GETSCRIPT_INPUT)); 
	}

}GETSCRIPT_INPUT ,*PGETSCRIPT_INPUT;

typedef struct _GETSCRIPT_OUTPUT{

	int scriptTime;                                 //脚本剩余时间,0:脚本到期,正整数:剩余时间,负数:免费脚本
	int resultValue;                                //存储过程执行结果返回值

	_GETSCRIPT_OUTPUT()//初始化
	{
		memset(this,0,sizeof(_GETSCRIPT_OUTPUT)); 
	}

}GETSCRIPT_OUTPUT ,*PGETSCRIPT_OUTPUT;
///////////////////王崧百 心跳检测脚本剩余时间包 2012.11.10///////////////

///////////////////王崧百 获取新闻信息 2012.12.29///////////////
typedef struct _GETNOWNEWS_INPUT{
	int  newsID;								   
	//新闻ID判断是否有更新
	_GETNOWNEWS_INPUT()//初始化
	{
		memset(this,0,sizeof(_GETNOWNEWS_INPUT)); 
	}
}GETNOWNEWS_INPUT ,*PGETNOWNEWS_INPUT;

typedef struct _GETNOWNEWS_ONPUT{

	int  newsID;					  //新闻ID判断是否有更新
	char newsTitle[64];               //新闻标题
	char newsBuff[256];               //新闻内容
	char urlBuff[MAX_PATH];           //链接地址

	_GETNOWNEWS_ONPUT()//初始化
	{
		memset(this,0,sizeof(_GETNOWNEWS_ONPUT)); 
	}

}GETNOWNEWS_ONPUT ,*PGETNOWNEWS_ONPUT;
///////////////////王崧百 获取新闻信息 2012.12.29///////////////
#pragma pack()


#ifdef PROTOCOL_CLIENT

//////////////////////////////////////////////////////////////////////////
//Client interfaces

//////////////////////////////////////////////////////////////////////////
// Structures used in ExecuteSQL(...)

typedef struct _SQL_INPUT {

	char* pszSQL;						//	SQL string
	int nDestDB;						//	Destination database No.
	int nCommTimeout;					//  Communication (between client and server) timeout (in second)	0: wait infinitely
	int nDBQueryTimeout;				//	Database query (server) timeout (in second)						0: wait infinitely

} SQL_INPUT, *PSQL_INPUT;

typedef struct _SQL_OUTPUT {
	int nErrorCode;						// Error code
	int nErrorMsgBufSize;				// Error message buffer size
	char* pszErrorMsg;					// Error message
	BOOL bIsDataset;					// Indicate the result is dataset
	int nRowsOfDataset;					// Rows of dataset
	int nColsOfDataset;					// Cols of dataSet
	int nDatasetBufSize;				// Dataset string buffer size
	char* pszDataset;					// Dataset string

} SQL_OUTPUT, *PSQL_OUTPUT;

/////////////////////////////////////////////////////////////诚诚 2012-3-13 	

//获取用户项目文件信息函数入参结构体
typedef struct _GETPROFILEINFO_INPUT {
	char *pszUserName;   //用户名
} GETPROFILEINFO_INPUT, *PGETPROFILEINFO_INPUT;

//获取用户项目文件信息函数发包结构体
typedef struct _GETPROFILEINFO_BODY {
	char pszUserName[MAX_USER_NAME];				//用户名
	char szToken[MAX_TOKEN];						//动态密匙（访问令牌）
} GETPROFILEINFO_BODY, *PGETPROFILEINFO_BODY;

//获取用户项目文件信息函数回包结构体
typedef struct _GETPROFILEINFO_OUTPUT {
	int nErrorCode;					//服务器返回信息
	char pszProFileInfo[MAX_PROFILENAMEINFO];	//用户所有项目文件信息；长度为：1024字节
}GETPROFILEINFO_OUTPUT, *PGETPROFILEINFO_OUTPUT;

//寻址函数入参结构体
typedef struct _FINDSERVER_INPUT {
	char *pszUserName;   //用户名
	int nServerType;	//寻址服务器类型
} FINDSERVER_INPUT, *PFINDSERVER_INPUT;
//寻址函数发包结构体
typedef struct _FINDSERVER_BODY {
	char pszUserName[MAX_USER_NAME]; //用户名
	char pszkey[MAX_KEY];			//动态密匙
	int nServerType;				//寻址服务器类型
} FINDSERVER_BODY, *PFINDSERVER_BODY;

//寻址函数回包结构体
typedef struct _FINDSERVER_OUTPUT {
	int nErrorCode;					//服务器返回信息
	char pszServerIPAddress[MAX_IP_ADDRESS];		//返回寻址服务器IP地址
	int nPort;						//返回寻址服务器端口
}FINDSERVER_OUTPUT, *PFINDSERVER_OUTPUT;

//下载上传项目文件函数入参结构体
typedef struct _UPDOWNPROFILE_INPUT {
	char *pszFileName;   //上传或者下载的文件名
	char *pszFilePath;	//本地文件路径
	//int nFileLen;		//文件长度
} UPDOWNPROFILE_INPUT, *PUPDOWNPROFILE_INPUT;
//下载上传项目文件函数函数发包结构体
typedef struct _UPDOWNPROFILE_BODY {
	char pszFilePath[MAX_PATH];		//服务器文件路径
	char szToken[MAX_TOKEN];		//动态密匙
	int nFileLen;					//文件长度
} UPDOWNPROFILE_BODY, *PUPDOWNPROFILE_BODY;
//下载上传项目文件函数函数回包结构体
typedef struct _UPDOWNPROFILE_OUTPUT {
	int nErrorCode;					//返回服务器信息
	int nFileLen;					//文件长度
} UPDOWNPROFILE_OUTPUT, *PUPDOWNPROFILE_OUTPUT;


//获取用户信息函数入参结构体
typedef struct _USERINFO_INPUT {
	char *pszUserName;				//用户名

} USERINFO_INPUT, *PUSERINFO_INPUT;
//获取用户信息函数函数发包结构体
typedef struct _USERINFO_BODY {

	char pszUserName[MAX_USER_NAME];				//用户名
	char szToken[MAX_TOKEN];						//动态密匙（访问令牌）

} USERINFO_BODY, *PUSERINFO_BODY;
//获取用户信息函数函数回包结构体
typedef struct _USERINFO_OUTPUT {
	char ntype;					//用户类别 (普通,VIP)
	char nviplevel;				//VIP等级
	double nValue;					//积分
	double nAccountbalance;		//账户余额
	int Servertype;				//一个16进制数表示了,用户购买了什么服务,例（注册码：00000001，在线更新：00000010）
	int nUID;
} USERINFO_OUTPUT, *PUSERINFO_OUTPUT;

/////////////////////////////////////////////////////////////诚诚 2012-3-13 

//////////////////////////////////////////////////////////////////////////
// Exported functions
extern "C" {
	PROTOCOL_API BOOL _stdcall Login(__in LOGIN_INPUT loginInput, __inout LOGIN_OUTPUT &loginOutput);
	PROTOCOL_API BOOL _stdcall Logout(__in int nTimeout = LOGOUT_TIMEOUT_DEFAULT);			// In seconds
	BOOL _stdcall ExecuteSQL(__in SQL_INPUT &sqlInput, __out SQL_OUTPUT &sqlOutput);
	typedef void (_stdcall* LPFN_ExecuteTaskCallback)(int task, char* msg);			//TODO: redefine later
	void _stdcall RegisterCallback(__in LPFN_ExecuteTaskCallback cbExecTask);		//TODO: redefine later
	//王崧百 获取脚本剩余时间 2013.1.5
	PROTOCOL_API BOOL _stdcall GetScrpitTime(__in _GETSCRIPT_INPUT inputData,__inout _GETSCRIPT_OUTPUT &iutputData,int nTimeout);
	//add end

	//添加导出MD5加密方法  诚诚-2012-3-16
	PROTOCOL_API BOOL _stdcall TCMD5String(__in const char *str,__out char *md5str);
	//获取服务器用户信息
	PROTOCOL_API BOOL _stdcall GetUserInfo(__in USERINFO_INPUT&userInfoinput,__inout USERINFO_OUTPUT&userInfooutput);
	//添加导出MD5加密方法  诚诚-2012-3-16

	///////////文件下载///////////////////////////////
	PROTOCOL_API int _stdcall DownScript(__in DOWNSCRIPTINFO_INPUT&downInfoInput,__inout _SCRIPTRINFO_OUTPUT&rInfo);
    int  DownScriptFile(DOWNSCRIPTINFO_OUTPUT&downInpu,char *savepath,int scriptID);
	///////////文件下载///////////////////////////////

	//王崧百 获取新闻信息 2012.12.29
    PROTOCOL_API int _stdcall GetNowNews(__in GETNOWNEWS_INPUT&inPutinfo,__inout GETNOWNEWS_ONPUT&outInfo);
	//王崧百 获取新闻信息 2012.12.29

	//王崧百 防止重复登陆,心跳验证包 2013.1.4
	PROTOCOL_API bool _stdcall HeartBeat(__in int nTimeout,__inout _HEART_BEAT_ACK_BODY &handOutput);
}

#else

//////////////////////////////////////////////////////////////////////////
// Server interfaces

#ifdef __cplusplus

// COM-Like abstract interface.
// This interface doesn't require __declspec(dllexport/dllimport) specifier.
// Method calls are dispatched via virtual table.
// Any C++ compiler can use it.
// Instances are obtained via factory function.
struct IProtocol {

	// Parameters
	// Destination [in]
	//		A pointer to the starting address of the encoded block's block of packet memory
	// cchDest
	// Source [in]
	//		A pointer to the starting address of the block of packet memory to copy
	// 
	virtual void AESEncrypt(__out char* dest, __in const char* src, __in const size_t buf_len, __in const char* key, __in const size_t key_len) = 0;
	virtual void AESDecrypt(__out char* dest, __in const char* src, __in const size_t buf_len, __in const char* key, __in const size_t key_len) = 0;
	virtual void MD5() = 0;			// TODO: redefine later
	virtual void Random() = 0;		// TODO: redefine later
	virtual void Release() = 0;
};

// Handle type. In C++ language the interface type is used.
typedef IProtocol* PROTHANDLE;

#else   // C

// Handle type. In C language there are no classes, so empty struct is used
// in order to ensure type safety.
typedef struct tagPROTHANDLE {} * PROTHANDLE;

#endif // __cplusplus

////////////////////////////////////////////////////////////////////////////////
// 
#ifdef __cplusplus
#   define EXTERN_C     extern "C"
#else
#   define EXTERN_C
#endif // __cplusplus

// Factory function that creates instances of the protocol object.
EXTERN_C
PROTOCOL_API
PROTHANDLE
WINAPI
GetProtocolObject(void);

#ifndef __cplusplus

////////////////////////////////////////////////////////////////////////////////
// Classic C interface.
// These functions can be used with any compiler that knows how to call
// an exported C routine.

EXTERN_C
PROTOCOL_API
void
APIENTRY
ProtocolRelease(
	__in PROTHANDLE hProtocol);

#endif  // #ifndef __cplusplus

#endif // PROTOCOL_CLIENT

#endif	// PROTOCOL_H__
