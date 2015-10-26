#pragma once

#include "Thread.h"

class CDispatchThread : public CThread
{
public:
	CDispatchThread():m_bRun(true) {};

	~CDispatchThread() {};

	bool Init();

	void Run();

	void Stop() { m_bRun = false; };

private:
	bool m_bRun;
};
