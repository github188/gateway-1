#ifndef __TQUOTE_EVENT_H__
#define __TQUOTE_EVENT_H__

#include "TradeCommon.h"
#include "TradeEvent.h"
#include "TradeStruct.h"

class SDKDLL_API TQuoteEvent: public TradeEvent
{
public:
	TQuoteEvent();
	~TQuoteEvent();

public:
	void setQuote(RealTimeQuote quote);
	RealTimeQuote  getQuote() const;
private:
	RealTimeQuote m_quote;
};


#endif //__TQUOTE_EVENT_H__
