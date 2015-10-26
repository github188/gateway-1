#ifndef __TSYSBULLETIN_EVENT_H__
#define __TSYSBULLETIN_EVENT_H__

#include "TradeCommon.h"
#include "TradeEvent.h"
#include "TradeStruct.h"

class SDKDLL_API TSysBulletinEvent: public TradeEvent
{
public:
	TSysBulletinEvent();
	~TSysBulletinEvent();

public:
	void setSysBulletin(SysBulletinInfo info);
	SysBulletinInfo  getSysBulletin() const;
private:
	SysBulletinInfo m_info;
};


#endif //__TSYSBULLETIN_EVENT_H__
