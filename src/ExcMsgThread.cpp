#include <yt/log/log.h>
#include <yt/util/queue.h>
#include <string>
#include <arpa/inet.h>
#include "protocol.h"
#include "ExcMsgThread.h"
#include "serverconf.h"
#include "constant.h"

using namespace yt;
using namespace std;

extern yt::SyncQueue<std::string>   exc_q;
extern MonitorInfo		monitorinfo;


bool CExcMsgThread::Init()
{
	m_pCommCli=new(nothrow)CRmqCli(ServerConf::Instance()->m_MonitorConnInfo.m_SvrIp, 
		ServerConf::Instance()->m_MonitorConnInfo.m_port);

	if (NULL == m_pCommCli)
	{
		AC_ERROR("new CRmqCli error!");
		return false;
	}
	if (!m_pCommCli->Connect())
		return false;

	return true;
}

void CExcMsgThread::Run()
{
	while(m_bRun)
	{
		string sres;
		int ret=exc_q.Pop(sres,true);
		if(0==ret)
		{
			//add by liuchengzhu 20140108
			//退出等异常消息
			if(sres.length()<sizeof(NtPkgHead))
				break;
			//end add by liuchengzhu 20140108

			m_pCommCli->SendMessage((char *)sres.c_str(),sres.size(),ServerConf::Instance()->m_QueryRoute.BranchNo,ServerConf::Instance()->m_QueryRoute.FuncNo);

			NtPkgHead *phead=(NtPkgHead *)sres.c_str();
			unsigned short wCmd=ntohs(phead->wCmd);

			//add by liuchengzhu 20150311
			if(wCmd>=TID_OPENMARKETORDER&&wCmd<=TID_LIMITREVOKE)
			{
				monitorinfo.RespDelegExcepCount++;
			}
			else if(wCmd>=TID_GET_ACCOUNTINFO&&wCmd<=TID_GET_HOLDPOSITIONTOTALBYID)
			{
				monitorinfo.RespQueryExcepCount++;
			}
			//end add by liuchengzhu 20150311
		}
	}
}
