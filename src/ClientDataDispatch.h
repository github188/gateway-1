#pragma once

#include "ASyncMsgThread.h"
#include "ResMsgThread.h"
//add by liuchengzhu 20150228
#include "ExcMsgThread.h"
//end add by liuchengzhu 20150228
#include "DispatchThread.h"
#include "HeartThread.h"

using namespace std;

class CASyncMsgThread;

class ClientDataDispatch
{
private:

	ClientDataDispatch() {};
	virtual ~ClientDataDispatch() {};

	CASyncMsgThread* m_pASyncMsgThread;
	CResMsgThread*   m_pResMsgThread;

	//add by liuchengzhu 20150228
	CExcMsgThread*   m_pExcMsgThread;
	//end add by liuchengzhu 20150228

	CDispatchThread* m_pDispatchThread;

	CHeartThread*    m_pHeartThread;

public:

	static ClientDataDispatch* Instance()
	{
		static ClientDataDispatch h_s;
		return &h_s;
	}

	bool Init();

	void Join();

	void Stop();
};