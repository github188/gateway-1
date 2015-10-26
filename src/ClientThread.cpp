#include <yt/log/log.h>
#include <yt/util/queue.h>
#include <string>
#include "ClientThread.h"

bool CClientThread::Init(int u)
{
	uid=u;

	m_tc.uid=uid;

	int ret=0;

	ret=m_tc.Init();

	if(0!=ret)
	{
		AC_ERROR("Init fail. TODO Release!\n");
		m_tc.Release();
		return false;
	}

	/*
	//temp test
	ret=m_tc.Login("003098765432104","123456");

	if(0!=ret)
	{
		AC_ERROR("Login fail. TODO Release!\n");
		m_tc.Release();
		return false;
	}

	m_tc.OnHeartBeat();
	*/

	return true;
}

void CClientThread::Run()
{
	while(m_bRun)
	{
		string sreq;
		
		int ret=q.Pop(sreq,true);
		if(0==ret)
		{
			m_tc.HandleReq(sreq);
		}
	}
	//AC_ERROR("CClientThread exit uid=%d",uid);
	m_tc.Release();

	m_bExit=true;
}

void CClientThread::Push(string sreq)
{
	q.Push(sreq);
}
