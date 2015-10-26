#pragma once

#include "Thread.h"
#include "RmqClient.h"

class CExcMsgThread : public CThread
{
public:
	CExcMsgThread():m_bRun(true),m_pCommCli(NULL) {};

	~CExcMsgThread(){};

	bool Init();

	void Run();

	void Stop() { m_bRun = false; };

private:
	bool m_bRun;

	CRmqCli* m_pCommCli;
};
