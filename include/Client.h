#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "TradeCommon.h"

class TradeEvent;

class SDKDLL_API Client
{
public:
	
	virtual void procTradeEvent(const TradeEvent* pEvent) = 0;
	virtual ~Client(){}
};


#endif //__CLIENT_H__



