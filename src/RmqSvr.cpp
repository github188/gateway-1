#include <yt/log/log.h>
#include <string.h>
#include "rmqlib.h"
#include "rmqerr.h"
#include "RmqSvr.h"


CRmqSvr::CRmqSvr(const char *ip, const int port, const int iSvrNo):m_RmqPort(port), m_Handle(0), m_Connected(false),m_SvrNo(iSvrNo)
{
	strncpy(m_RmqIP, ip, sizeof(m_RmqIP));
}

CRmqSvr::~CRmqSvr()
{
	Disconnect();
}

void CRmqSvr::Disconnect()
{
	if (true == m_Connected)
	{
		m_Connected = false;
		::RmqClose(m_Handle);
	}
}

bool CRmqSvr::Connect()
{
	int  iCode = 0;
	char acErrMsg[300];

	if (!m_Connected)
	{
		m_Handle = RmqConnect(m_RmqIP, m_RmqPort, &iCode, acErrMsg);
		if (m_Handle > 0)
		{
			if (RmqAddFunction(m_SvrNo, &iCode, acErrMsg, m_Handle))
			{
				m_Connected = true;
				AC_INFO("Rmq服务端注册功能号:%d 成功!\n", m_SvrNo);
			}
			else
			{
				AC_ERROR("Rmq服务端注册功能号失败!:%d,%s\n", iCode, acErrMsg);
			}
		}
		else
		{
			AC_ERROR("Rmq服务端连接失败!:%d,%s\n", iCode, acErrMsg);
		}
	}

	return m_Connected;
}

int CRmqSvr::ReadMessage(char *cachebuf/*8192*/, int cachesize, int timeout, TAG_RMQHANDLE *pRmqHnd)
{
	int  iCode = 0;
	char acErrMsg[300];
	int datalen = 0;

	//内部自动重连一次
	if (false == Connect())
		return -1;

	if (FALSE == ::RmqReadMessage(cachebuf, cachesize, &datalen, m_Handle, &m_RmqHnd, &iCode, acErrMsg, timeout))
	{
		switch (iCode)
		{
		case ERROR_READ_TIMEOUT:
			datalen = 0;
			return datalen;
		default:
			datalen = -1;
			Disconnect();
			AC_ERROR("Rmq服务端读数据失败:%d,%s\n", iCode, acErrMsg);
		}
	}
	else
	{
		if (pRmqHnd)
		{
			*pRmqHnd = m_RmqHnd;
		}
	}

	return datalen;
}

int CRmqSvr::SendMessage(char* databuf/*8000*/, int datasize, TAG_RMQHANDLE *pRmqHnd)
{
	int  iCode = 0;
	char acErrMsg[300];
	TAG_RMQHANDLE *pHnd;

	if (false == Connect())
		return -1;

	if (pRmqHnd)
	{
		pHnd = pRmqHnd;
	}
	else
	{
		pHnd = &m_RmqHnd;
	}

	if (FALSE == ::RmqSendMessage(databuf, datasize, &iCode, acErrMsg, pHnd))
	{
		if (ERROR_SOCKET == iCode)
		{
			Disconnect();
		}
		AC_ERROR("Rmq服务端send应答数据失败:%d,%s\n", iCode, acErrMsg);
		return -1;
	}

	return datasize;
}