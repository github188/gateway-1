#include <yt/log/log.h>
#include <stdlib.h>
#include <string.h>
#include "rmqlib.h"
#include "rmqerr.h"
#include "RmqClient.h"

CRMQInit::CRMQInit()
{
	int  iCode = 0;
	char acErrMsg[300];
	
	if (FALSE == ::RmqInit(&iCode,acErrMsg))
	{
		AC_ERROR("RMQ环境初始化失败\n");
		exit(1);
	}
}

CRMQInit::~CRMQInit()
{
	::RmqExit();
}

CRmqCli::CRmqCli(const char *ip, const int port):m_RmqPort(port), m_Handle(0), m_Connected(false)
{
	strncpy(m_RmqIP, ip, sizeof(m_RmqIP));
}

CRmqCli::~CRmqCli()
{
	Disconnect();
}

void CRmqCli::Disconnect()
{
	if (true == m_Connected)
	{
		m_Connected = false;
		::RmqClose(m_Handle);
	}
}

bool CRmqCli::Connect()
{
	int  iCode = 0;
	char acErrMsg[300];

	if (false == m_Connected)//锁隐患？
	{
		m_Handle = ::RmqConnect(m_RmqIP, m_RmqPort, &iCode, acErrMsg);
		if (m_Handle > 0)
		{
			m_Connected = true;
		}
		else
		{
			AC_ERROR("RMQCli 连接RMQ失败! %d:%s\n", iCode, acErrMsg);
		}
	}

	return m_Connected;
}

int CRmqCli::ReadMessage(char *cachebuf/*8192*/, int cachesize, int timeout)
{
	int  iCode = 0;
	char acErrMsg[300];
	int datalen = 0;
	TAG_RMQHANDLE RMQtagHnd;

	//内部自动重连一次
	if (false == Connect())
		return -1;

	if (FALSE == ::RmqReadMessage(cachebuf, cachesize, &datalen, m_Handle, &RMQtagHnd, &iCode, acErrMsg, timeout))
	{
		switch (iCode)
		{
		case ERROR_READ_TIMEOUT:
			datalen = 0;
			return datalen;
		default:
			datalen = -1;
			Disconnect();
			AC_ERROR("RMQCli客户端读数据失败:%d,%s\n", iCode, acErrMsg);
		}
	}

	return datalen;
}

int CRmqCli::SendMessage(char* databuf, int datasize, int iBranchNo, int iFuncNo)
{
	int  iCode = 0;
	char acErrMsg[300];
	int  Prior = 3; //根据实际业务需要调整
	
	//内部自动重连一次
	if (false == Connect())
		return -1;
	
	if (FALSE == ::RmqPostMessage(Prior, iBranchNo, iFuncNo, databuf, datasize, &iCode, acErrMsg, m_Handle))
	{
		Disconnect();
		AC_ERROR("RMQCli 客户端发送数据失败:%d,%s\n", iCode, acErrMsg);
		return -1;
	}

	return datasize;
}