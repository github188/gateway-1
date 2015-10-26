#ifndef __TOPENLIMITORDER_EVENT_H__ 
#define __TOPENLIMITORDER_EVENT_H__

#include "TradeCommon.h"
#include "TradeEvent.h"
#include "TradeStruct.h"


class SDKDLL_API TOpenLimitOrderEvent: public TradeEvent
{
public:
	TOpenLimitOrderEvent();
	~TOpenLimitOrderEvent();
public:
	void setResult(ProcessResult result);
	ProcessResult getResult()const;

	void setLimitOrderID(long long LimitOrderID);
	long long getLimitOrderID()const;
private:
	ProcessResult m_Result;
	long long m_LimitOrderID;
};

#endif //__TOPENLIMITORDER_EVENT_H__

