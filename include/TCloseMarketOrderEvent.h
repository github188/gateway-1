#ifndef __TCLOSEMARKETORDER_EVENT_H__ 
#define __TCLOSEMARKETORDER_EVENT_H__

#include "TradeCommon.h"
#include "TradeEvent.h"
#include "TradeStruct.h"

class SDKDLL_API TCloseMarketOrderEvent: public TradeEvent
{
public:
	TCloseMarketOrderEvent();
	~TCloseMarketOrderEvent();
public:
	void setResult(ProcessResult result);
	ProcessResult getResult()const;

	void setHoldPositionID(long long nHoldPositionID);
	long long getHoldPositionID() const;
private:
	ProcessResult m_Result;
	long long     m_HoldPositionID;
};

#endif //__TCLOSEMARKETORDER_EVENT_H__


