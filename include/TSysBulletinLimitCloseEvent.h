#ifndef __TSYSBULLETIN_LIMITCLOSE_EVENT_H__
#define __TSYSBULLETIN_LIMITCLOSE_EVENT_H__

#include "TradeCommon.h"
#include "TradeEvent.h"
#include "TradeStruct.h"

class SDKDLL_API TSysBulletinLimitCloseEvent: public TradeEvent
{
public:
	TSysBulletinLimitCloseEvent();
	~TSysBulletinLimitCloseEvent();

public:
	void setSysBulletinLimitClose(SysBulletinInfo info);
	SysBulletinInfo  getSysBulletinLimitClose() const;
private:
	SysBulletinInfo m_info;
};


#endif //__TSYSBULLETIN_LIMITCLOSE_EVENT_H__
