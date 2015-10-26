#ifndef __TRADE_EVENT_H__
#define __TRADE_EVENT_H__

#include "TradeCommon.h"

class SDKDLL_API TradeEvent
{
public:
	TradeEvent(){}
    virtual ~TradeEvent(){}

public:
    int  GetType() const;
	void SetType(int type);

private:
    TradeEvent(const TradeEvent&);
    TradeEvent& operator=(const TradeEvent&);

protected:
    int  m_Type;

};


inline int TradeEvent::GetType() const
{
    return m_Type;
}

inline void TradeEvent::SetType(int type)
{
	m_Type = type;
}
#endif //__TRADE_EVENT_H__


