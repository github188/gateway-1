#pragma once


//RMQ环境初始化类
class CRMQInit
{
public:
	CRMQInit();
	~CRMQInit();
};

//客户端方式

class CRmqCli
{
public:
	
	CRmqCli(const char *ip, int port);
	
	~CRmqCli();
	
	inline bool IsConnected() const { return m_Connected; }
	
	bool Connect();
	
	void Disconnect();
	
	int ReadMessage(char *buf, int size, int timeout=-1);
	
	int SendMessage(char *buf, int size, int iBranchNo, int iFuncNo);
	
private:
	char m_RmqIP[64];			//RMQ主机地址
	int  m_RmqPort;			    //RMQ服务端口
	int  m_Handle;				//RMQ连接句柄
	bool m_Connected;			//是否已连接
};