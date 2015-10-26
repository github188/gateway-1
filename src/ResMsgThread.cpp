#include <yt/log/log.h>
#include <yt/util/queue.h>
#include <string>
#include "protocol.h"
#include "ResMsgThread.h"
#include "serverconf.h"

using namespace yt;
using namespace std;

extern yt::SyncQueue<std::string>   res_q;

bool CResMsgThread::Init()
{
	m_pCommCli=new(nothrow)CRmqCli(ServerConf::Instance()->m_ASyncConnInfo.m_SvrIp, 
		ServerConf::Instance()->m_ASyncConnInfo.m_port);

	if (NULL == m_pCommCli)
	{
		AC_ERROR("new CRmqCli error!");
		return false;
	}
	if (!m_pCommCli->Connect())
		return false;

	return true;
}

void CResMsgThread::Run()
{
	while(m_bRun)
	{
		string sres;
		int ret=res_q.Pop(sres,true);
		if(0==ret)
		{
			//add by liuchengzhu 20140108
			//退出等异常消息
			if(sres.length()<sizeof(NtPkgHead))
				break;
			//end add by liuchengzhu 20140108

			m_pCommCli->SendMessage((char *)sres.c_str(),sres.size(),ServerConf::Instance()->m_OrderRoute.BranchNo,ServerConf::Instance()->m_OrderRoute.FuncNo);

		}
	}
}
