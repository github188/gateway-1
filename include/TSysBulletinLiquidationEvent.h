#ifndef __TSYSBULLETIN_LIQUIDATION_EVENT_H__
#define __TSYSBULLETIN_LIQUIDATION_EVENT_H__

#include "TradeCommon.h"
#include "TradeEvent.h"
#include "TradeStruct.h"

class SDKDLL_API TSysBulletinLiquidationEvent: public TradeEvent
{
public:
	TSysBulletinLiquidationEvent();
	~TSysBulletinLiquidationEvent();

public:
	void setSysBulletinLiquidation(SysBulletinInfo info);
	SysBulletinInfo  getSysBulletinLiquidation() const;
private:
	SysBulletinInfo m_info;
};


#endif //__TSYSBULLETIN_LIQUIDATION_EVENT_H__
