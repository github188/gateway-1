#pragma once

#include "Thread.h"
#include "RmqClient.h"

class CResMsgThread : public CThread
{
public:
	CResMsgThread():m_bRun(true),m_pCommCli(NULL) {};

	~CResMsgThread(){};

	bool Init();

	void Run();

	void Stop() { m_bRun = false; };

private:
	bool m_bRun;

	CRmqCli* m_pCommCli;
};
