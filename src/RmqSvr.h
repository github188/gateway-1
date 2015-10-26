#pragma once

#include "rmqlib.h"

//服务端方式

class CRmqSvr
{
public:
	
	CRmqSvr(const char *ip, const int port, const int iSvrNo);
	
	~CRmqSvr();
	
	inline bool IsConnected() const { return m_Connected; }
	
	bool Connect();
	
	void Disconnect();
	
	int ReadMessage(char *buf, int size, int timeout=-1, TAG_RMQHANDLE *pRmqHnd = NULL);
	
	int SendMessage(char* databuf/*8000*/, int datasize, TAG_RMQHANDLE *pRmqHnd = NULL);
	
private:
	char m_RmqIP[64];			//RMQ主机地址
	int  m_RmqPort;			    //RMQ服务端口
	int  m_Handle;				//RMQ连接句柄
	bool m_Connected;			//是否已连接

	int  m_SvrNo;
	TAG_RMQHANDLE m_RmqHnd;
};