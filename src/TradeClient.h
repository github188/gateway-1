#pragma once

#include <time.h>
#include <map>

#include "TradeInterface.h"
#include "AppLog.h"

#include "TQuoteEvent.h"
#include "TLoginEvent.h"
#include "TOpenMarketOrderEvent.h"
#include "TCloseMarketOrderEvent.h"
#include "TOpenLimitOrderEvent.h"
#include "TLimitClosePositionEvent.h"
#include "TRevokeLimitOrderEvent.h"
#include "TSysBulletin.h"
#include "TSysBulletinLimitCloseEvent.h"
#include "TSysBulletinLiquidationEvent.h"
#include "TDisConnectEvent.h"
#include "TMoneyInOutEvent.h"
#include "TMoneyQueryEvent.h"

#include "BaseHandle.h"

typedef struct _ORDER_CONFIG
{
	int CommodityID;
	OpenMarketOrderConf OpenMarket;
	CloseMarketOrderConf CloseMarket;
	OpenLimitOrderConf OpenLimit;
	LimitClosePositionConf CloseLimit;
}order_config;

#define Event_Ret_Success 99999
#define Event_Ret_DROPUSER 4006
#define Event_Ret_LOGACCERR 4001
#define Order_Ret_Success 0


#define TS_1970_TO_1900  2208988800

class TradeClient: public Client,public CBaseHandle
{
public:
	TradeClient();
	~TradeClient();

public:
	int HandleQueryRes(unsigned short cmd,string& strData);

	int HandleRes(unsigned short cmd,string& strData);

	int HandleRes(unsigned short cmd,int ret,string& msg);

	int HandleReq(string sreq);

	int Init();
	int Release();

	int Login(std::string ploginacc, std::string ppwd);

	void DisConnect();//通知前置断开连接

	int GetAccountInfo();  // 账户信息

	//add by liuchengzhu 20150603
	int requestOrderConfigData(vector<order_config> &vOrderConfig, bool &isWriteCache);

	bool serializeOrderConfigData(const vector<order_config> &vOrderConfig, string &configJsonStr);

	bool getOrderConfigInfo(string &retString);
	//end add by liuchengzhu 20150603

	//add by liuchengzhu 20150602
	void GetOrderConfig();//获取下单配置信息

	int GetCommodityInfo(vector<CommodityInfo> &vCommodity);	//获取商品信息

	int GetOpenMarketOrderConf(const int CommodityID,  OpenMarketOrderConf &pConf);	//获取市价建仓配置信息

	int GetOpenLimitOrderConf(const int CommodityID, OpenLimitOrderConf &pConf);	//获取限价建仓配置信息

	int GetCloseMarketOrderConf(const int CommodityID, CloseMarketOrderConf &pConf);	//获取市价平仓配置信息

	int GetLimitClosePositionConf(const int CommodityID, LimitClosePositionConf &pConf);	//获取限价平仓配置信息
	//end add by liuchengzhu 20150602

	int OnOpenMarketOrder(OpenMarketOrderParam &Order);  // 市价单建仓

	//add by liuchengzhu 20141212

	int OnCloseMarketOrder(CloseMarketOrderParam& CloseOrder);//市价单平仓
	

	int  OnOpenLimitOrder(OpenLimitOrderParam& LimitOrder);//限价单建仓
	//end add by liuchengzhu 20141212

	//add by liuchengzhu 20141216

	int OnLimitClosePosition(CloseLimitOrderParam& LimitClosePosition);//限价单平仓
	//end add by liuchengzhu 20141216
	
	//add by liuchengzhu 20141218
	int OnLimitRevoke(LimitRevokeParam& LimitRevoke);//限价单撤销
	int OnCloseMarketOrderMany(CloseMarketOrderManyParam& CloseMarketOrderMany);//批量平仓
	int OnMoneyInOut(MoneyInOutParam& MoneyInOut);//出入金
	//end add by liuchengzhu 20141218
	
	//add by liuchengzhu 20141219
	int OnMoneyQuery();//查询银行资金
	//end add by liuchengzhu 20141219

	int OnHeartBeat(); // 心跳

	//add by liuchengzhu 20141211
	//SDK接口已经废弃
	//int GetMoneyQueryInfo();//获取银行资金信息

	int GetCommodity();//获取商品信息

	int  GetPositionOrder();//获取持仓单信息

	int  GetLimitOrder();//获取限价单信息
	//end add by liuchengzhu 20141211
	
	//add by liuchengzhu 20141212
	int  GetClosePosition();//获取平仓信息

	int GetHoldPositionTotal();//获取持仓单总量
	//end add by liuchengzhu 20141212

	//add by liuchengzhu 20150123
	int GetMarketStatus();//获取市场开休市状态 

	int GetHoldPositionByID(string sID);//根据持仓单ID，获取持仓单信息

	int GetLimitOrderByID(string sID);//根据限价单ID，获取限价单信息

	int GetClosePositionByID(string sID);//根据平仓单ID，获取平仓信息

	int GetHoldPositionTotalByCommodityID(int ID,int dir);//根据商品ID，获取持仓汇总信息

	//end add by liuchengzhu 20150123


private:
	int InitNetwork();

	void procTradeEvent(const TradeEvent *pEvent);

	int TimeToStr(const long long second, char* dest, int len);

	int TimeToMSecond(const long long second,long &msecond);
public:
	//add by liuchengzhu 20150312
	time_t  tRequest;
	//end add by liuchengzhu 20150312

	//add by liuchengzhu 20150324
	bool bNeedStop;
	//end add by liuchengzhu 20150324

private:
	TradeInterface				*m_Interface;
	
	RealTimeQuote 				m_quote;
	//vector<CommodityInfo>		m_vecCommodity;
	//BourseMoneyInfo			m_BourseMoneyInfo;

	string				LoginAcc;
};


