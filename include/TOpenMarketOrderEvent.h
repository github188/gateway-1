#ifndef __TOPENMARKETORDER_EVENT_H__ 
#define __TOPENMARKETORDER_EVENT_H__

#include "TradeCommon.h"
#include "TradeEvent.h"
#include "TradeStruct.h"

class SDKDLL_API TOpenMarketOrderEvent: public TradeEvent
{
public:
	TOpenMarketOrderEvent();
	~TOpenMarketOrderEvent();
public:
	void setResult(ProcessResult result);
	ProcessResult getResult()const;

	void setHoldPositionID(long long nHoldPositionID);
	long long getHoldPositionID() const;
private:
	ProcessResult m_Result;
	long long     m_HoldPositionID;
};

#endif //__TOPENMARKETORDER_EVENT_H__

