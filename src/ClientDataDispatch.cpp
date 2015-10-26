#include <yt/log/log.h>
#include "ClientDataDispatch.h"
#include "RmqClient.h"
#include "serverconf.h"
#include "ClientThread.h"
#include <queue>
extern map<int,CClientThread *>  map_cs;
extern std::queue <CClientThread*>   ClientThread_q;

bool ClientDataDispatch::Init()
{
	//启动初始化一次
	static bool bRunFlag = false;
	if (bRunFlag)
		return true;

	//初版设一组
	//RMQ环境初始化
	static CRMQInit rmqinit;
	
	//应答消息线程初始化
	m_pResMsgThread = new CResMsgThread();        // 向总线发应答消息
	if(!m_pResMsgThread->Init())
	{
		AC_ERROR("m_pResMsgThread->Init() error");
		return false;
	}
	m_pResMsgThread->Create();

	//add by liuchengzhu 20150228
	//异常消息线程初始化
	m_pExcMsgThread = new CExcMsgThread();
	if(!m_pExcMsgThread->Init())
	{
		AC_ERROR("m_pExcMsgThread->Init() error");
		return false;
	}
	m_pExcMsgThread->Create();
	//end add by liuchengzhu 20150228

	//请求消息接收线程初始化
	m_pASyncMsgThread = new CASyncMsgThread();
	if (!m_pASyncMsgThread->Init())              //从总线读
	{
		AC_ERROR("m_pASyncMsgThread->Init() error!");
		return false;
	}
	m_pASyncMsgThread->Create();

	//分派消息线程初始化
	m_pDispatchThread = new CDispatchThread();
	if(!m_pDispatchThread->Init())
	{
		AC_ERROR("m_pDispatchThread->Init() error!");
	}
	m_pDispatchThread->Create();


	//心跳消息线程初始化
	m_pHeartThread = new CHeartThread();
	if(!m_pHeartThread->Init())
	{
		AC_ERROR("m_pHeartThread->Init() error!");
	}
	m_pHeartThread->Create();

	 
	bRunFlag = true;
	return true;
}

void ClientDataDispatch::Join()
{
	map<int,CClientThread *>::iterator it;
	for(it=map_cs.begin();it!=map_cs.end();it++)
	{
		it->second->Join();
	}
	//add by wangzhen
	CClientThread *pc=NULL;
	while (!ClientThread_q.empty())
	{
		pc = ClientThread_q.front();
		ClientThread_q.pop();
		pc->Join();
	}
	pc = NULL;
	//end

	m_pASyncMsgThread->Join();
	m_pResMsgThread->Join();
	//add by liuchengzhu 20150228
	m_pExcMsgThread->Join();
	//end add by liuchengzhu 20150228
	m_pDispatchThread->Join();
	//add by liuchengzhu 20150326
	m_pHeartThread->Join();
	//end add by liuchengzhu 20150326
}

void ClientDataDispatch::Stop()
{
	map<int,CClientThread *>::iterator it;
	for(it=map_cs.begin();it!=map_cs.end();it++)
	{
		it->second->Stop();
	}

	m_pASyncMsgThread->Stop();
	m_pResMsgThread->Stop();
	//add by liuchengzhu 20150228
	m_pExcMsgThread->Stop();
	//end add by liuchengzhu 20150228
	m_pDispatchThread->Stop();
	//add by liuchengzhu 20150326
	m_pHeartThread->Stop();
	//end add by liuchengzhu 20150326
}