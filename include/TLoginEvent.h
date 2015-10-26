#ifndef __TLOGIN_EVENT_H__
#define __TLOGIN_EVENT_H__

#include "TradeCommon.h"
#include "TradeStruct.h"
#include "TradeEvent.h"


class SDKDLL_API TLoginEvent: public TradeEvent
{
public:
	TLoginEvent();
	~TLoginEvent();

public:
	void setLoginResult(ProcessResult result);
	ProcessResult  getLoginResult() const;
private:
	ProcessResult m_Result;
};


#endif //__TLOGIN_EVENT_H__
