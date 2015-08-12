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
	ERROR_OA_USERNAMEORPSW_ERROR        = ERROR_OA_BASE+26,		// �û������������
	//add end
	ERROR_OA_LISTEN_THREAD_CANCELLED	= ERROR_OA_BASE+25,		// Listen thread was cancelled.


	// ... 
	ERROR_PROTO_RANGE_TO				= 6000,					// Error code of protocol module range from 5000 to 6000
	// Do not beyond this!
	ERROR_OA_BASE_END					= ERROR_OA_BASE+ERROR_PROTO_RANGE_TO,	

	ERROR_OA_SUCCESS					= 0x20000000 + 0x0FFFFFFF,	

	////�ϳ� 2012-2-13 ��֤ע���������Ϣ
	ERROR_CHECKCODE_SUCCESS				=8000,//ͨ����֤
	ERROR_INVALID_USER					=8100,//��Ч�û�(�û�VIP���ڻ����û�����VIP)
	ERROR_INVALID_KEY					=8200,//��ЧKEY
	ERROR_EXPIRED_KEY					=8300,//����KEY
	ERROR_FREEZE_KEY					=8400,//����KEY
	ERROR_INVALID_MACHINECODE           =8500,//KEY�ͻ����벻ƥ��
	////�ϳ� 2012-2-13 ��֤ע���������Ϣ
	////�ϳ� 2012-2-15 ��������Ϣ
	ERROR_UNBUNDLING_SUCCESS			=9000,//���ɹ�
	ERROR_INVALID_UNBUNDLING			=9100,//���ʧ��
	////�ϳ� 2012-2-15 ��������Ϣ
	////�ϳ� 2012-3-10 �����´�����Ϣ
	ERROR_CHECKUPDATA_SUCCESS			=9500,//ͨ��������
	ERROR_CHECKUPDATA_UNSUCCESS			=9600, //������ʧ��
	////�ϳ� 2012-3-10 �����´�����Ϣ

	////�ϳ� 2012-3-13 �����´�����Ϣ
	ERROR_FINDSERVER_SUCCESS			=9700,//Ѱַ�ɹ�
	ERROR_FINDSERVER_UNSUCCESS			=9800,//Ѱַʧ��

	////�ϳ� 2012-3-14 �ϴ�����ɾ����Ŀ�ļ�������Ϣ
	ERROR_DOWNPROFILE_SUCCESS			=9900,//��ȡ������������Ϣ�ɹ�
	ERROR_DOWNPROFILE_UNSUCCESS,			  //��ȡ������������Ϣʧ��

	ERROR_UPPROFILE_SUCCESS,				//��ȡ�������ϴ���Ϣ�ɹ�
	ERROR_UPPROFILE_UNSUCCESS,				//��ȡ�������ϴ���Ϣʧ��

	ERROR_DELETEFILEPRO_SUCCESS,			//��ȡ������ɾ����Ϣ�ɹ�
	ERROR_DELETEFILEPRO_UNSUCCESS,			//��ȡ������ɾ����Ϣʧ��
	////�ϳ� 2012-3-14 �ϴ�����ɾ����Ŀ�ļ�������Ϣ
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
	//match-2012.2.11-��ע����ĳ���
	MAX_REGCODE					= 64,          //ע���볤��
	MAX_PRONAME                 = 32,
	MAX_ERROR_MSG_CLIENT		= 260,
	MAX_DATASET_LEN				= 1000*1024,
	MAX_REGMACHINE_CODE			= 64,				//�ϳ� 2012-2-11 �����볤��
	MAX_PROFILENAMEINFO			= 1024,			//�ϳ� 2012-3-24 �û�������Ŀ��Ϣ����
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


	CMD_USERINFO_REQUEST =0x50,				//�ϳ� 2012-3-17 ��ȡ�û���Ϣ
	CMD_USERINFO_ACK,					    //�ϳ� 2012-3-17 ��ȡ�û���Ϣ

	CMD_FINDSERVER_REQUEST,					//�ϳ� 2012-3-13 ������Ѱַ
	CMD_FINDSERVER_ACK,						//�ϳ� 2012-3-13 ������Ѱַ

	CMD_CHECKCODE_REQUEST,                  //�ϳ� 2012-2-13 ��֤ע���루������֤��������
	CMD_CHECKCODE_ACK,					    //�ϳ� 2012-2-13 ��֤ע���루������֤��������
	CMD_UNBUNDLING_REQUEST,					//�ϳ� 2012-2-13 �������루������֤��������
	CMD_UNBUNDLING_ACK,	                    //�ϳ� 2012-2-13 �������루������֤��������

	CMD_GETINFOPROFILE_REQUEST,				//�ϳ� 2012-2-24 ��ȡ�û���Ŀ�ļ���Ϣ��������֤��������
	CMD_GETINFOPROFILE_ACK,					//�ϳ� 2012-2-24 ��ȡ�û���Ŀ�ļ���Ϣ��������֤��������

	CMD_DOWNPROFILE_REQUEST,				//�ϳ� 2012-3-13 ������Ŀ�ļ������Ӹ��·�������
	CMD_DOWNPROFILE_ACK,					//�ϳ� 2012-3-13 ������Ŀ�ļ������Ӹ��·�������

	CMD_UPPROFILE_REQUEST,					//�ϳ� 2012-3-13 �ϴ���Ŀ�ļ������Ӹ��·�������
	CMD_UPPROFILE_ACK,						//�ϳ� 2012-3-13 �ϴ���Ŀ�ļ������Ӹ��·�������

	CMD_DELETEFILEPRO_REQUEST,				//�ϳ� 2012-3-13 ɾ����Ŀ�ļ������Ӹ��·�������
	CMD_DELETEUPFILEPRO_ACK,				//�ϳ� 2012-3-13 ɾ����Ŀ�ļ������Ӹ��·�������

	CMD_CHECKUPDATA_REQUEST,				//�ϳ� 2012-3-10 ������
	CMD_CHECKUPDATA_ACK,					//�ϳ� 2012-3-10 ������

	CMD_UPDATAEXE_REQUEST,					//�ϳ� 2012-3-12 ����EXE�����Ӹ��·�������
	CMD_UPDATAEXE_ACK,						//�ϳ� 2012-3-12 ����EXE�����Ӹ��·�������

	/////�ļ�����///////////////
    MSG_DOWNSCRIPT_REQ=139, 
    MSG_DOWNSCRIPT_RSP,  
    MSG_DOWNSCRIPTINFO_REQ,
	MSG_DOWNSCRIPTINFO_RSP,
	////�ļ�����////////////////

	//���°� ��ȡ������Ϣ 2012.12.29
	MSG_GETNOWNEWS_REQ,
	MSG_GETNOWNEWS_RSP,
	//���°� ��ȡ������Ϣ 2012.12.29

	//���°� ������,��ֹ�ظ���½���࿪ 2013.1.5
	MSG_HEARTBEAT_REQ,
	MSG_HEARTBEAT_RSP,
	MSG_HEARTBEATSPECIAL_RSP,   //���⴦��
	//���°� ������,��ֹ�ظ���½���࿪ 2013.1.5

	MSG_GENERAL_ERR_RSP			= 0x58+60		// return from server for general error which is not specified for above process 
};

/////////////////////////////////////////////////////////////�ϳ� 2012-3-13 
// Ѱַ����������
enum FINDSERVER_TYPE {
	FIND_REGCODESERVER					=0x10,
	FIND_UPDOWNFILESERVER,
	FIND_UPDATASERVER
};
/////////////////////////////////////////////////////////////�ϳ� 2012-3-13 

//���°� ����������ֵ 2013.1.5
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
	ERR_DEC_ERROR,                        //����ʱ��������
	//add end
	//match = ERR_APP_BASE + 3000

	ERR_BUSINESS_BASE   = ERR_APP_BASE + 3000,
	ERR_DB_ILLEGAL_MAC  =ERR_BUSINESS_BASE +1,           //�Ƿ���������ַ
	ERR_DB_ILLEGAL_USER,                                 //�Ƿ����û���  
	ERR_DB_ERROR_PROJNAME,                               //ע������֤����Ŀ���ʹ���
	ERR_DB_ILLEGAL_REG,                                  //�Ƿ���ע����
	ERR_DB_ERRREGTOUSER,                                 //ע������û�����Ӧ
	ERR_DB_OVERDUE_REG,                                  //���ڵ�ע����
	ERR_DB_BINDED_MAC,                                   //�������Ѱ�
	ERR_DB_FREEZING_REGCODE,                             //�����ע����
	ERR_DB_DELETE_REGCODE,                               //��ɾ����ע����
	ERR_DB_UNBIND_FAULT,                                 //���ʧ��
	ERR_SERVER_TYPE,                                     //����ķ���������
	ERR_FORMAT_XML,                                      //CDataSetFormat::format�������쳣
	ERR_SQL_EXCUTE,                                      //CSqlCmd::Excute�������쳣
	ERR_DB_INTICOM_FAULT,                                //��ʼ��COM���ʧ��
	ERR_DB_CREATECON_FAULT,                              //����_ConnectionPtr���Ӷ���
	ERR_DB_NOT_CREATECMD_FAULT,                          //����_CommandPtr����ʧ��
	ERR_DB_NODEFINED_ERROR,                              //DB����δ����Ĵ���
	ERR_DB_GETPWD_FAULT,                                 //��ȡ����ʧ��

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

/////////////////////////////////////////////////////////////�ϳ� 2012-2-10  ��֤�뷢�����հ��ṹ��
//��֤�������շ������ذ��ṹ��
typedef struct _CHECK_DATA_OUTPUT{
	int nDatatime;					//ʣ�൹��ʱ�䣨�֣�
	int nErrorCode;					//ע���������Ϣ
}CHECK_DATA_OUTPUT ,*PCHECK_DATA_OUTPUT;

//��֤������νṹ��
typedef struct _CHECK_CODE_INPUT{
	char *pszUserName;									//�û���			
	char *pszKeyCode;									//ע����
	char *pszServerIPAddress;							//������IP��ַ
	char *pszMachineCode;								//��������Ϣ(include MAC, HDD SN, etc. each one connect with '#')
}CHECK_CODE_INPUT ,*PCHECK_CODE_INPUT;

//��֤�����������ݽṹ��
typedef struct _CHECK_CODE_BODY {
	char szUserName[MAX_USER_NAME];						//�û��� (AES����)
	char RegCode[MAX_REGCODE];							//ע����(AES����)
	char szMachineCode[MAX_REGMACHINE_CODE];			//��������Ϣ(include MAC, HDD��MD5����)
	char szToken[MAX_TOKEN];							//��̬�ܳ�
} CHECK_CODE_BODY, *PCHECK_CODE_BODY;

//2012-2-13 �������뺯�����շ������ذ��ṹ��
typedef struct _UNBUNDLING_OUTPUT{
	int UnbundlingCode;									//���շ������ذ������Ϣ
	//int nErrorCode;										//������Ϣ							
}UNBUNDLING_OUTPUT ,*PUNBUNDLING_OUTPUT;

//2012-2-13 �������뺯����νṹ��
typedef struct _UNBUNDLING_INPUT{
	char *pszUserName;									//�û���			
	char *pszKeyCode;									//ע����
	char *pszServerIPAddress;							//������IP��ַ
	char *pszMachineCode;								//��������Ϣ(include MAC, HDD SN, etc. each one connect with '#')
}UNBUNDLING_INPUT ,*PUNBUNDLING_INPUT;

//2012-2-13 �������뺯���������ݽṹ��
typedef struct _UNBUNDLING_BODY {
	char szUserName[MAX_USER_NAME];						//�û��� (AES����)
	char RegCode[MAX_REGCODE];							//ע����(AES����)
	char szMachineCode[MAX_REGMACHINE_CODE];			//��������Ϣ(include MAC, HDD��MD5����)
	char szToken[MAX_TOKEN];							//��̬�ܳ�
} UNBUNDLING_BODY, *PUNBUNDLING_BODY;
/////////////////////////////////////////////////////////////�ϳ� 2012-2-10  ��֤�뷢�����հ��ṹ��

/////////////////////////////////////////////////////////////�ϳ� 2012-3-10  �����·������հ��ṹ��
//2012-3-10 �����º������շ������ذ��ṹ��
typedef struct _CHECKUPDATA_OUTPUT{							//���շ������ذ���������Ϣ
	int nErrorCode;										//������Ϣ
	char *pszServerIPAddress;							//������IP��ַ
	int nPort;                                            //�������˿�
}CHECKUPDATA_OUTPUT ,*PCHECKUPDATA_OUTPUT;

//2012-3-10 �����º�����νṹ��
typedef struct _CHECKUPDATA_INPUT{
	char *pszUserName;									//�û���			
	char *pszKeyCode;									//ע����
	char *pszServerIPAddress;							//������IP��ַ
	char *pszMachineCode;								//��������Ϣ(include MAC, HDD SN, etc. each one connect with '#')
}CHECKUPDATA_INPUT ,*PCHECKUPDATA_INPUT;

//2012-3-10 �����º����������ݽṹ��
typedef struct _CHECKUPDATA_BODY {
	char szUserName[MAX_USER_NAME];						//�û��� (AES����)
	char RegCode[MAX_REGCODE];							//ע����(AES����)
	char szMachineCode[MAX_REGMACHINE_CODE];			//��������Ϣ(include MAC, HDD��MD5����)
	char szToken[MAX_TOKEN];							//��̬�ܳ�
} CHECKUPDATA_BODY, *PCHECKUPDATA_BODY;

//2012-3-10 �����º������շ������ذ��ṹ��
typedef struct _UPDATAEXE_OUTPUT{							//���շ������ذ���������Ϣ
	int nErrorCode;										//������Ϣ
	int nLastFileLen;									//�ϵ������ļ�����
}UPDATAEXE_OUTPUT ,*PUPDATAEXE_OUTPUT;

//2012-3-12 �����£����͸��·�����������νṹ��
typedef struct _UPDATAEXE_INPUT {
	char *szToken;										//��̬�ܳף��������ƣ�
	int nLastFileLen;									//�ϵ������ļ�����
} UPDATAEXE_INPUT, *PUPDATAEXE_INPUT;
//2012-3-12 �����£����͸��·������ķ������ݽṹ��
typedef struct _UPDATAEXE_BODY {
	char szToken[MAX_TOKEN];							//��̬�ܳף��������ƣ�
	int LastFileLen;									//�ϵ������ļ�����
} UPDATAEXE_BODY, *PUPDATAEXE_BODY;

/////////////////////////////////////////////////////////////�ϳ� 2012-3-10  �����·������հ��ṹ��


////////////////////////�ļ�����////////////////////////////////////////
typedef struct _DOWNSCRIPTINFO_INPUT{

	char username[MAX_USER_NAME];                   //�û������20������
	char userpwd[MAX_PASSWORD];                     //�û������32λ
	char tokenTemp[MAX_TOKEN]; 					    //Ȩ��ƥ���ִ�
	float scriptVersion;							//��Դ�ļ��汾
	char savePath[MAX_PATH];					    //���ر���·��
	int scriptID;								    //�ű�IDΨһ��ʾ��
	int fileClinetsize;								//���ذ汾��С
	int userType;								    //�û�����

	_DOWNSCRIPTINFO_INPUT()//��ʼ��
	{
		memset(this,0,sizeof(_DOWNSCRIPTINFO_INPUT)); 
	}

}DOWNSCRIPTINFO_INPUT ,*PDOWNSCRIPTINFO_INPUT;


typedef struct _DOWNSCRIPTINFO_OUTPUT{

	char serverIP[MAX_IP_ADDRESS];					//������IP
	int type;                                       //0:�޸��� 1:�и���
	char randKey[MAX_KEY];                          //������֤key
	float newestVersion;							//������Դ�ļ��汾
	char savePath[MAX_PATH];						//��������Դ�ļ�����·��
	int fileLenth;								    //�������ļ���С
	int fileClinetsize;								//���ذ汾��С
	float ptRunVersion;                             //ƽ̨�汾
	int scriptTime;                                 //�ű�ʣ��ʱ��,0:�ű�����,������:ʣ��ʱ��,����:��ѽű�
	int useType;                                    //�ű�ʹ������,0:����,1:����ʹ��
	int resultValue;                                //�洢����ִ�н������ֵ
// 	char scriptName[MAX_SCRIPT_NAME];				//�ű���
// 	char scriptDescription[MAX_SCRIPT_DESCRIPTION];	//�ű����
// 	char shopName[MAX_SHOP_NAME];					//������

	_DOWNSCRIPTINFO_OUTPUT()//��ʼ��
	{
		memset(this,0,sizeof(_DOWNSCRIPTINFO_OUTPUT)); 
	}

}DOWNSCRIPTINFO_OUTPUT ,*PDOWNSCRIPTINFO_OUTPUT;

typedef struct _DOWNSCRIPT_INPUT{

	char serverIP[MAX_IP_ADDRESS];					//������IP
	int type;                                       //0:�޸��� 1:�и���
	char randKey[MAX_KEY];                          //������֤key
	float newestVersion;							//������Դ�ļ��汾
	char savePath[MAX_PATH];						//��������Դ�ļ�����·��
	int fileLenth;								    //�������ļ���С
	int fileClinetsize;								//���ذ汾��С

	_DOWNSCRIPT_INPUT()//��ʼ��
	{
		memset(this,0,sizeof(_DOWNSCRIPT_INPUT)); 
	}

}DOWNSCRIPT_INPUT ,*PDOWNSCRIPT_INPUT;


typedef struct _DOWNSCRIPT_OUTPUT{

	int port;								        //�������˿�
	int resultValue;                                //�洢����ִ�н������ֵ

	_DOWNSCRIPT_OUTPUT()//��ʼ��
	{
		memset(this,0,sizeof(_DOWNSCRIPT_OUTPUT)); 
	}

}DOWNSCRIPT_OUTPUT ,*PDOWNSCRIPT_OUTPUT;

//������Ϣ
typedef struct _SCRIPTRINFO_OUTPUT{

	float newestVersion;							//������Դ�ļ��汾
	int scriptTime;                                 //�ű�ʣ��ʱ��,0:�ű�����,������:ʣ��ʱ��,����:��ѽű�
	int useType;                                    //�ű�ʹ������,0:����,1:����ʹ��
	int resultValue;                                //�洢����ִ�н������ֵ

	_SCRIPTRINFO_OUTPUT()//��ʼ��
	{
		memset(this,0,sizeof(_SCRIPTRINFO_OUTPUT)); 
	}

}SCRIPTRINFO_OUTPUT ,*PSCRIPTRINFO_OUTPUT;

/////////////////////////�ļ�����//////////////////////////////////////////


#pragma pack(1)
///////////////////���°� �������ű�ʣ��ʱ��� 2012.11.10///////////////
typedef struct _GETSCRIPT_INPUT{

	char username[MAX_USER_NAME];                   //�û������20������
	char userpwd[MAX_PASSWORD];                     //�û������32λ
	int  scriptID;								    //�ű�IDΨһ��ʾ��

	_GETSCRIPT_INPUT()//��ʼ��
	{
		memset(this,0,sizeof(_GETSCRIPT_INPUT)); 
	}

}GETSCRIPT_INPUT ,*PGETSCRIPT_INPUT;

typedef struct _GETSCRIPT_OUTPUT{

	int scriptTime;                                 //�ű�ʣ��ʱ��,0:�ű�����,������:ʣ��ʱ��,����:��ѽű�
	int resultValue;                                //�洢����ִ�н������ֵ

	_GETSCRIPT_OUTPUT()//��ʼ��
	{
		memset(this,0,sizeof(_GETSCRIPT_OUTPUT)); 
	}

}GETSCRIPT_OUTPUT ,*PGETSCRIPT_OUTPUT;
///////////////////���°� �������ű�ʣ��ʱ��� 2012.11.10///////////////

///////////////////���°� ��ȡ������Ϣ 2012.12.29///////////////
typedef struct _GETNOWNEWS_INPUT{
	int  newsID;								   
	//����ID�ж��Ƿ��и���
	_GETNOWNEWS_INPUT()//��ʼ��
	{
		memset(this,0,sizeof(_GETNOWNEWS_INPUT)); 
	}
}GETNOWNEWS_INPUT ,*PGETNOWNEWS_INPUT;

typedef struct _GETNOWNEWS_ONPUT{

	int  newsID;					  //����ID�ж��Ƿ��и���
	char newsTitle[64];               //���ű���
	char newsBuff[256];               //��������
	char urlBuff[MAX_PATH];           //���ӵ�ַ

	_GETNOWNEWS_ONPUT()//��ʼ��
	{
		memset(this,0,sizeof(_GETNOWNEWS_ONPUT)); 
	}

}GETNOWNEWS_ONPUT ,*PGETNOWNEWS_ONPUT;
///////////////////���°� ��ȡ������Ϣ 2012.12.29///////////////
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

/////////////////////////////////////////////////////////////�ϳ� 2012-3-13 	

//��ȡ�û���Ŀ�ļ���Ϣ������νṹ��
typedef struct _GETPROFILEINFO_INPUT {
	char *pszUserName;   //�û���
} GETPROFILEINFO_INPUT, *PGETPROFILEINFO_INPUT;

//��ȡ�û���Ŀ�ļ���Ϣ���������ṹ��
typedef struct _GETPROFILEINFO_BODY {
	char pszUserName[MAX_USER_NAME];				//�û���
	char szToken[MAX_TOKEN];						//��̬�ܳף��������ƣ�
} GETPROFILEINFO_BODY, *PGETPROFILEINFO_BODY;

//��ȡ�û���Ŀ�ļ���Ϣ�����ذ��ṹ��
typedef struct _GETPROFILEINFO_OUTPUT {
	int nErrorCode;					//������������Ϣ
	char pszProFileInfo[MAX_PROFILENAMEINFO];	//�û�������Ŀ�ļ���Ϣ������Ϊ��1024�ֽ�
}GETPROFILEINFO_OUTPUT, *PGETPROFILEINFO_OUTPUT;

//Ѱַ������νṹ��
typedef struct _FINDSERVER_INPUT {
	char *pszUserName;   //�û���
	int nServerType;	//Ѱַ����������
} FINDSERVER_INPUT, *PFINDSERVER_INPUT;
//Ѱַ���������ṹ��
typedef struct _FINDSERVER_BODY {
	char pszUserName[MAX_USER_NAME]; //�û���
	char pszkey[MAX_KEY];			//��̬�ܳ�
	int nServerType;				//Ѱַ����������
} FINDSERVER_BODY, *PFINDSERVER_BODY;

//Ѱַ�����ذ��ṹ��
typedef struct _FINDSERVER_OUTPUT {
	int nErrorCode;					//������������Ϣ
	char pszServerIPAddress[MAX_IP_ADDRESS];		//����Ѱַ������IP��ַ
	int nPort;						//����Ѱַ�������˿�
}FINDSERVER_OUTPUT, *PFINDSERVER_OUTPUT;

//�����ϴ���Ŀ�ļ�������νṹ��
typedef struct _UPDOWNPROFILE_INPUT {
	char *pszFileName;   //�ϴ��������ص��ļ���
	char *pszFilePath;	//�����ļ�·��
	//int nFileLen;		//�ļ�����
} UPDOWNPROFILE_INPUT, *PUPDOWNPROFILE_INPUT;
//�����ϴ���Ŀ�ļ��������������ṹ��
typedef struct _UPDOWNPROFILE_BODY {
	char pszFilePath[MAX_PATH];		//�������ļ�·��
	char szToken[MAX_TOKEN];		//��̬�ܳ�
	int nFileLen;					//�ļ�����
} UPDOWNPROFILE_BODY, *PUPDOWNPROFILE_BODY;
//�����ϴ���Ŀ�ļ����������ذ��ṹ��
typedef struct _UPDOWNPROFILE_OUTPUT {
	int nErrorCode;					//���ط�������Ϣ
	int nFileLen;					//�ļ�����
} UPDOWNPROFILE_OUTPUT, *PUPDOWNPROFILE_OUTPUT;


//��ȡ�û���Ϣ������νṹ��
typedef struct _USERINFO_INPUT {
	char *pszUserName;				//�û���

} USERINFO_INPUT, *PUSERINFO_INPUT;
//��ȡ�û���Ϣ�������������ṹ��
typedef struct _USERINFO_BODY {

	char pszUserName[MAX_USER_NAME];				//�û���
	char szToken[MAX_TOKEN];						//��̬�ܳף��������ƣ�

} USERINFO_BODY, *PUSERINFO_BODY;
//��ȡ�û���Ϣ���������ذ��ṹ��
typedef struct _USERINFO_OUTPUT {
	char ntype;					//�û���� (��ͨ,VIP)
	char nviplevel;				//VIP�ȼ�
	double nValue;					//����
	double nAccountbalance;		//�˻����
	int Servertype;				//һ��16��������ʾ��,�û�������ʲô����,����ע���룺00000001�����߸��£�00000010��
	int nUID;
} USERINFO_OUTPUT, *PUSERINFO_OUTPUT;

/////////////////////////////////////////////////////////////�ϳ� 2012-3-13 

//////////////////////////////////////////////////////////////////////////
// Exported functions
extern "C" {
	PROTOCOL_API BOOL _stdcall Login(__in LOGIN_INPUT loginInput, __inout LOGIN_OUTPUT &loginOutput);
	PROTOCOL_API BOOL _stdcall Logout(__in int nTimeout = LOGOUT_TIMEOUT_DEFAULT);			// In seconds
	BOOL _stdcall ExecuteSQL(__in SQL_INPUT &sqlInput, __out SQL_OUTPUT &sqlOutput);
	typedef void (_stdcall* LPFN_ExecuteTaskCallback)(int task, char* msg);			//TODO: redefine later
	void _stdcall RegisterCallback(__in LPFN_ExecuteTaskCallback cbExecTask);		//TODO: redefine later
	//���°� ��ȡ�ű�ʣ��ʱ�� 2013.1.5
	PROTOCOL_API BOOL _stdcall GetScrpitTime(__in _GETSCRIPT_INPUT inputData,__inout _GETSCRIPT_OUTPUT &iutputData,int nTimeout);
	//add end

	//��ӵ���MD5���ܷ���  �ϳ�-2012-3-16
	PROTOCOL_API BOOL _stdcall TCMD5String(__in const char *str,__out char *md5str);
	//��ȡ�������û���Ϣ
	PROTOCOL_API BOOL _stdcall GetUserInfo(__in USERINFO_INPUT&userInfoinput,__inout USERINFO_OUTPUT&userInfooutput);
	//��ӵ���MD5���ܷ���  �ϳ�-2012-3-16

	///////////�ļ�����///////////////////////////////
	PROTOCOL_API int _stdcall DownScript(__in DOWNSCRIPTINFO_INPUT&downInfoInput,__inout _SCRIPTRINFO_OUTPUT&rInfo);
    int  DownScriptFile(DOWNSCRIPTINFO_OUTPUT&downInpu,char *savepath,int scriptID);
	///////////�ļ�����///////////////////////////////

	//���°� ��ȡ������Ϣ 2012.12.29
    PROTOCOL_API int _stdcall GetNowNews(__in GETNOWNEWS_INPUT&inPutinfo,__inout GETNOWNEWS_ONPUT&outInfo);
	//���°� ��ȡ������Ϣ 2012.12.29

	//���°� ��ֹ�ظ���½,������֤�� 2013.1.4
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
