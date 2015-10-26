#pragma once
#include "Thread.h"
#include <yt/util/queue.h>
#include <string>
#include "TradeClient.h"

using namespace yt;
using namespace std;


class CClientThread : public CThread
{
public:
	CClientThread():m_bRun(true),m_bExit(false),uid(0){};

	~CClientThread(){};

	bool Init(int u);

	void Run();

	void Stop() { m_bRun = false; };

	void Push(string sreq);

	bool IsExit(){ return m_bExit; };

private:
	bool m_bRun;

	bool m_bExit;

	int uid;

	yt::SyncQueue<std::string> q;

public:
	TradeClient  m_tc;	
};
