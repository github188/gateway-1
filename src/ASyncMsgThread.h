#pragma once

#include "Thread.h"
#include "RmqSvr.h"

class CASyncMsgThread : public CThread
{
public:
	CASyncMsgThread():m_bRun(true),m_pCommSvr(NULL) {};

	~CASyncMsgThread() {};

	bool Init();

	void Run();

	void Stop() { m_bRun = false; };

private:
	bool m_bRun;
	CRmqSvr* m_pCommSvr;

};