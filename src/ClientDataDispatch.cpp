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
	//������ʼ��һ��
	static bool bRunFlag = false;
	if (bRunFlag)
		return true;

	//������һ��
	//RMQ������ʼ��
	static CRMQInit rmqinit;
	
	//Ӧ����Ϣ�̳߳�ʼ��
	m_pResMsgThread = new CResMsgThread();        // �����߷�Ӧ����Ϣ
	if(!m_pResMsgThread->Init())
	{
		AC_ERROR("m_pResMsgThread->Init() error");
		return false;
	}
	m_pResMsgThread->Create();

	//add by liuchengzhu 20150228
	//�쳣��Ϣ�̳߳�ʼ��
	m_pExcMsgThread = new CExcMsgThread();
	if(!m_pExcMsgThread->Init())
	{
		AC_ERROR("m_pExcMsgThread->Init() error");
		return false;
	}
	m_pExcMsgThread->Create();
	//end add by liuchengzhu 20150228

	//������Ϣ�����̳߳�ʼ��
	m_pASyncMsgThread = new CASyncMsgThread();
	if (!m_pASyncMsgThread->Init())              //�����߶�
	{
		AC_ERROR("m_pASyncMsgThread->Init() error!");
		return false;
	}
	m_pASyncMsgThread->Create();

	//������Ϣ�̳߳�ʼ��
	m_pDispatchThread = new CDispatchThread();
	if(!m_pDispatchThread->Init())
	{
		AC_ERROR("m_pDispatchThread->Init() error!");
	}
	m_pDispatchThread->Create();


	//������Ϣ�̳߳�ʼ��
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