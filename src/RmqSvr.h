#pragma once

#include "rmqlib.h"

//����˷�ʽ

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
	char m_RmqIP[64];			//RMQ������ַ
	int  m_RmqPort;			    //RMQ����˿�
	int  m_Handle;				//RMQ���Ӿ��
	bool m_Connected;			//�Ƿ�������

	int  m_SvrNo;
	TAG_RMQHANDLE m_RmqHnd;
};