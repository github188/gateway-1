#ifndef __TDISCONNECT_EVENT_H__ 
#define __TDISCONNECT_EVENT_H__

#include "TradeCommon.h"
#include "TradeEvent.h"
#include "TradeStruct.h"


class SDKDLL_API TDisConnectEvent: public TradeEvent
{
public:
	TDisConnectEvent();
	~TDisConnectEvent();
public:
	void setResult(ProcessResult result);
	ProcessResult getResult()const;
private:
	ProcessResult m_Result;

};

#endif //__TDISCONNECT_EVENT_H__



