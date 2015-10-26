#ifndef __THEARTBEAT_EVENT_H__ 
#define __THEARTBEAT_EVENT_H__

#include "TradeCommon.h"
#include "TradeEvent.h"
#include "TradeStruct.h"


class SDKDLL_API THeartBeatEvent: public TradeEvent
{
public:
	THeartBeatEvent();
	~THeartBeatEvent();
public:
	void setResult(ProcessResult result);
	ProcessResult getResult()const;
private:
	ProcessResult m_Result;

};

#endif //__THEARTBEAT_EVENT_H__


