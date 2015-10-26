#ifndef __TINITIALIZE_EVENT_H__ 
#define __TINITIALIZE_EVENT_H__

#include "TradeCommon.h"
#include "TradeEvent.h"
#include "TradeStruct.h"


class SDKDLL_API TInitializeEvent: public TradeEvent
{
public:
	TInitializeEvent();
	~TInitializeEvent();
public:
	void setResult(ProcessResult result);
	ProcessResult getResult()const;
private:
	ProcessResult m_Result;

};

#endif //__TINITIALIZE_EVENT_H__


