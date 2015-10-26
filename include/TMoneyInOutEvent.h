#ifndef __TMONEYINOUT_EVENT_H__
#define __TMONEYINOUT_EVENT_H__

#include "TradeCommon.h"
#include "TradeStruct.h"
#include "TradeEvent.h"


class SDKDLL_API TMoneyInOutEvent: public TradeEvent
{
public:
	TMoneyInOutEvent();
	~TMoneyInOutEvent();

public:
	void setResult(ProcessResult result);
	ProcessResult  getResult() const;
private:
	ProcessResult m_Result;
};


#endif //__TMONEYINOUT_EVENT_H__
