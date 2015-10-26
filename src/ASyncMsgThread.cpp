#include <yt/log/log.h>
#include <yt/util/queue.h>
#include <string>
#include "ASyncMsgThread.h"
#include "serverconf.h"

using namespace yt;
using namespace std;

extern yt::SyncQueue<std::string>   req_q;

#define MAX_COMMBUF 8116

bool CASyncMsgThread::Init()
{
	m_pCommSvr = new(nothrow) CRmqSvr(ServerConf::Instance()->m_ASyncConnInfo.m_SvrIp, 
		ServerConf::Instance()->m_ASyncConnInfo.m_port, 
		ServerConf::Instance()->m_ASyncConnInfo.m_ASynFuncNo);
	if (NULL == m_pCommSvr)
	{
		AC_ERROR("new CRmqSvr error!");
		return false;
	}
	if (!m_pCommSvr->Connect())
		return false;

	return true;
}

void CASyncMsgThread::Run()
{
	unsigned char cachebuf[MAX_COMMBUF]; //缓冲区
	int iDataLen = 0;

	while(m_bRun)
	{
		iDataLen = m_pCommSvr->ReadMessage((char*)cachebuf, MAX_COMMBUF, ServerConf::Instance()->m_ASyncConnInfo.m_ReadTimeOut);
		if (iDataLen <= 0)
		{
			continue;
		}
		AC_INFO("Recv %d",iDataLen);
		//放入请求队列
		string sreq;
		sreq.append((const char *)cachebuf,iDataLen);
		req_q.Push(sreq);
	}
}


