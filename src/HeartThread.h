#pragma once
#include "Thread.h"

class CHeartThread : public CThread
{
public:
	CHeartThread():m_bRun(true) {};

	~CHeartThread(){};

	bool Init();

	void Run();

	void Stop() { m_bRun = false; };

	void SendMonitor(long tcur);

	void SendStartUp();

private:
	bool m_bRun;

	int  m_sleeptime;
	int  m_stoptime;
	int  m_outtime;
};
