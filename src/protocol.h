#pragma once

typedef struct {
	unsigned char  bStartFlag; //协议包起始标志
	unsigned char  bVer; //版本号
	unsigned char  bEncryptFlag; //加密标志(如果不加密,则为0,其值请参照nt_crypt.h中的枚举值)
	unsigned char  bFrag; //是否有包分片
	unsigned short  wLen; //总包长
	unsigned short  wCmd; //命令号
	unsigned short  wSeq; //包的序列号,业务使用,转发不再关心
	unsigned short  wCrc; //Crc16校验码
	unsigned int dwSID; //会话ID,前端和后端分别使用不同的SID
	//unsigned char  sReserved[4]; //保留字段
	unsigned short  wTotal;//有包分片时，分片总数
	unsigned short  wCurSeq;//有包分片时，分片序号
} NtPkgHead; //协议包头
typedef struct {
	NtPkgHead stNtPkgHead;
	unsigned char   sBody[0];
} NtPkg; //协议包

typedef struct {
	int Groupno;		//组号
	char Ip[32];		//IP地址
	int Port;			//Port
	char AppName[32];	//应用程序名称
	int RequestOrder;	//下单数
	int  RequestQuery;	//查询数
	int  RespDelegExcepCount;	//委托异常总数
	int  RespQueryExcepCount;	//查询异常总数
	int ThreadCount;			//工作线程数
	int HeartKeepCount;			//心跳维持工作线程数
	long StartTime;				//启动时间
	//add by liuchengzhu 20150322
	int Funcno;					//服务功能号
	//end add by liuchengzhu 20150322

} MonitorInfo;  //业务监控信息