#ifndef __TMONEYQUERY_EVENT_H__
#define __TMONEYQUERY_EVENT_H__

#include "TradeCommon.h"
#include "TradeEvent.h"
#include "TradeStruct.h"

class SDKDLL_API TMoneyQueryEvent: public TradeEvent
{
public:
	TMoneyQueryEvent();
	~TMoneyQueryEvent();

public:
	void setResult(ProcessResult result);
	ProcessResult  getResult() const;

	void setBourseMoneyInfo(BourseMoneyInfo bourseMoneyInfo);
	BourseMoneyInfo  getBourseMoneyInfo() const;

private:
	ProcessResult   m_result;
	BourseMoneyInfo	m_bourseMoneyInfo;
};


#endif //__TMONEYQUERY_EVENT_H__
