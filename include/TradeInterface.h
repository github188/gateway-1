#ifndef __TRADE_INTERFACE_H__
#define __TRADE_INTERFACE_H__


#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include "TradeCommon.h"
#include "jthread.h"
#include "jmutex.h"
#include "Client.h"
#include "TradeStruct.h"
#include "TradeEventCode.h"
#include "TradeDefine.h"
#include "TSysBulletin.h"
#include "common/tradeclient.h"

using namespace std;
using namespace TradeAPI;

namespace TradeAPI
{
namespace business
{
	class HeartBeatEvent;
	class SilverQuotePushEvent;
	class GoldQuotePushEvent;
	class LoginEvent;
	class TokenLoginEvent;
	class UserCommodityInfoEvent;

	class MarketStatusQueryEvent;
	class UserCommodityFeeGroupEvent;
	class ClientConfigQueryEvent;
	class QuoteBeginOrStopEvent;
	class QuoteSubscribeEvent;

	class OpenPositionEvent;
	class ClosePositionEvent;
	class LimitOpenPositionEvent;
	class LimitClosePositionEvent;
	class LimitRevokeEvent;
	class CustomerCloseManyEvent;

	class AccountQueryEvent;
	class HoldPositionOrderQueryEvent;
	class LimitOrderQueryEvent;
	class ClosePositionQueryEvent;
	class SysBulletinEvent;

	class MoneyInOutEvent;
	class MoneyQueryEvent;
	class CustomerBankInfoEvent;
	class DisconnectEvent;
	class DepositeQueryEvent;

	class TradeManager;
	class BusinessMessage;

}
}

class SDKDLL_API TradeInterface:  public JThread, public TradeAPI::common::TradeClient
{
public:
	TradeInterface();
	~TradeInterface();

public:

///****************************************
/// @brief 用于初始化
/// @param  strIP		:	IP地址
/// @param  nPort		: 端口号
/// @param  strLogPath		:	日志路径
/// @return 返回处理结果状态 
/// - 0 : 初始化成功
/// - !0 : 失败
///****************************************
	int  Init(string strIP, int nPort, string strLogPath);
	
	
///****************************************
/// @brief  释放资源 
/// @param  
/// @return 返回处理结果状态
///  - 0 : 成功
///  - !0 : 失败 
///****************************************
	int  Release();


///****************************************
/// @brief  将Client类对象注册到接口对象里
/// @param  *p	:	Client对象指针
/// @return 
///****************************************
	void RegisterClient(Client* p);
	
	
///****************************************
/// @brief  设置应用交易码(也即授权码)和校验密码。 
/// @param  strAppCod		:  应用交易码
/// @param  Password		:  校验密码
/// @return   
///*****************************************
	void SetAppMessage(string strAppCode, string Password);

///****************************************
/// @brief  设置会员ID。 
/// @param  vendorid		:  会员ID
/// @return   
///*****************************************
	void setVendorID(string vendorid);

///****************************************
/// @brief  用于登录。 
/// @param  strLoginAccount		:	客户账户名
/// @param  strPwd						:	密码
/// @return 返回处理结果状态
///  -  0 : 成功 
///  - !0 : 失败
///*****************************************
	int  doLogin(string strLoginAccount, string strPwd);

///****************************************
/// @brief  用于登录。 
/// @param  strAccessToken		:	客户接入凭证
/// @param  strClientID			:	客户端ID
/// @return 返回处理结果状态
///  -  0 : 成功 
///  - !0 : 失败
///*****************************************
	int doTokenLogin(string strAccessToken, string strClientID);


///****************************************
/// @brief  市价单建仓 
/// @param  mParam		:	OpenMarketOrderParam 结构体
/// @return 
///  -  0 : 成功 
///  - !0 : 失败 
///*****************************************
	int  doOpenMarketOrder(OpenMarketOrderParam& mParam);

	
///****************************************
/// @brief 市价单平仓 
/// @param mParam			:	CloseMarketOrderParam 结构体
/// @return 
///  -  0 : 成功 
///  - !0 : 失败 
///*****************************************	
	int  doCloseMarketOrder(CloseMarketOrderParam& mParam);


///****************************************
/// @brief  限价单建仓 
/// @param  mParam		:	OpenLimitOrderParam 结构体
/// @return  
///  -  0 : 成功 
///  - !0 : 失败 
///****************************************
	int  doOpenLimitOrder(OpenLimitOrderParam& mParam);


///****************************************
/// @brief 限价单平仓
/// @param mParam			:	CloseLimitOrderParam 结构体
/// @return 
///  -  0 : 成功 
///  - !0 : 失败 
///*****************************************
	int  doLimitClosePosition(CloseLimitOrderParam& mParam);


///****************************************
/// @brief 限价单撤销 
/// @param mParam		:	LimitRevokeParam 结构体
/// @return 
///  -  0 : 成功 
///  - !0 : 失败 
///****************************************
	int  doLimitRevoke(LimitRevokeParam& mParam);


///****************************************
/// @brief 批量平仓 
/// @param mParam				:	CloseMarketOrderManyParam 结构体
/// @return 
///  -  0 : 成功 
///  - !0 : 失败 
///****************************************
	int  doCloseMarketOrderMany(CloseMarketOrderManyParam& mParam);


///****************************************
/// @brief 出入金 
/// @param mParam				:	MoneyInOutParam 结构体
/// @return 
///  -  0 : 成功 
///  - !0 : 失败 
///****************************************
	int	 doMoneyInOut(MoneyInOutParam& Param);


///****************************************
/// @brief 查询银行资金 
/// @param
/// @return 
///  -  0 : 成功 
///  - !0 : 失败 
///****************************************
	int  doMoneyQuery();

///****************************************
// @brief 心跳接口
// @return
//  -  0: 成功
//  - !0: 失败
///****************************************
    int  doHeartBeat();

  //------------------------------------------------//   
	//               Get Message Interface             //
	//------------------------------------------------//   
	  
///****************************************
/// @brief  获取账户信息，给到account；返回处理结果状态。 
/// @param  account		:	AccountInfo的实例
/// @return 
/// - MSG_STATUS_UPDATED: 成功
/// - !MSG_STATUS_UPDATED: 失败
///****************************************
	int  getAccountInfo(AccountInfo &account);
	

///****************************************
/// @brief  获取商品信息，给到vCommodity；返回处理结果状态。 
/// @param  vCommodity 	:	CommodityInfo的实例
/// @return 
/// - MSG_STATUS_UPDATED: 成功
/// - !MSG_STATUS_UPDATED: 失败
///*****************************************
	int  getCommodity(vector<CommodityInfo> &vCommodity);
	
	
///****************************************
/// @brief  获取持仓单信息，给到vHoldPosition；返回处理结果状态 
/// @param  vHoldPosition		:	 HoldPositionInfo的实例
/// @return 
/// - MSG_STATUS_UPDATED: 成功
/// - !MSG_STATUS_UPDATED: 失败
///*****************************************	
	int  getPositionOrder(vector<HoldPositionInfo>& vHoldPosition);


///****************************************
/// @brief  获取限价单信息，给到vLimitOrder；返回处理结果状态
/// @param  vLimitOrder			:	LimitOrderInfo的实例
/// @return 
/// - MSG_STATUS_UPDATED: 成功
/// - !MSG_STATUS_UPDATED: 失败
///****************************************
	int  getLimitOrder(vector<LimitOrderInfo>& vLimitOrder);


///****************************************
/// @brief  获取平仓信息，给到vClosePosition；返回处理结果状态 
/// @param  vClosePosition		: ClosePositionInfo的实例
/// @return 
/// - MSG_STATUS_UPDATED: 成功
/// - !MSG_STATUS_UPDATED: 失败
///****************************************
	int  getClosePosition(vector<ClosePositionInfo>& vClosePosition);


///****************************************
/// @brief  获取持仓单总量，给到vTotal；返回处理结果状态
/// @param  vTotal		:	HoldPositionTotalInfo的实例
/// @return 
/// - MSG_STATUS_UPDATED: 成功
/// - !MSG_STATUS_UPDATED: 失败
///****************************************
	int  getHoldPositionTotal(vector<HoldPositionTotalInfo>& vTotal);


///****************************************
/// @brief  获取市场开休市状态 
/// @param  nMarketStatus
/// @return 
/// - MSG_STATUS_UPDATED: 成功
/// - !MSG_STATUS_UPDATED: 失败
///*****************************************
	int  getMarketStatus(int& nMarketStatus);


  //------------------------------------------------// 
  //              Get Message By ID                 //
  //------------------------------------------------//  
	
///****************************************
/// @brief  根据持仓单ID，获取持仓单信息，给到pHoldPosition；返回处理结果状态
/// @param  nHoldPositionID		:	持仓单ID
/// @param  pHoldPosition			:	HoldPositionInfo类的实例
/// @return 
/// - MSG_STATUS_UPDATED: 成功
/// - !MSG_STATUS_UPDATED: 失败
///****************************************
	int  getHoldPositionByID(long long nHoldPositionID, HoldPositionInfo& pHoldPosition);


///****************************************
/// @brief  根据限价单ID，获取限价单信息，给到pLimitOrder；返回处理结果状态 
/// @param  nLimitOrderID		:	限价单ID
/// @param  pLimitOrder			:	LimitOrderInfo类的实例
/// @return 
/// - MSG_STATUS_UPDATED: 成功
/// - !MSG_STATUS_UPDATED: 失败
///*****************************************
	int  getLimitOrderByID(long long nLimitOrderID,  LimitOrderInfo& pLimitOrder);


///****************************************
/// @brief  根据平仓单ID，获取平仓信息，给到pClosePosition；返回处理结果状态
/// @param  nClosePositionID		:	平仓单ID
/// @param  pClosePosition			:	ClosePositionInfo类的实例
/// @return 
/// - MSG_STATUS_UPDATED: 成功
/// - !MSG_STATUS_UPDATED: 失败
///*****************************************
	int  getClosePositionByID(long long nClosePositionID, ClosePositionInfo& pClosePosition);


///****************************************
/// @brief  根据商品ID，获取持仓汇总信息，给到pTotal；返回处理结果状态
/// @param  nCommodityID			:	商品ID
/// @param  nOpenDirector			:	建仓方向
/// @param  pTotal						:	HoldPositionTotalInfo类的实例
/// @return 
/// - MSG_STATUS_UPDATED: 成功
/// - !MSG_STATUS_UPDATED: 失败
///*****************************************
	int  getHoldPositionTotalByCommodityID(int nCommodityID, int nOpenDirector, HoldPositionTotalInfo& pTotal);


///****************************************
/// @brief  根据商品ID，获取实时行情，给到quote；返回处理结果状态
/// @param  CommoditID		:	商品ID
/// @param  quote					:	RealTimeQuote类的实例
/// @return 
/// - MSG_STATUS_UPDATED: 成功
/// - !MSG_STATUS_UPDATED: 失败
///*****************************************
	int  getCommodityQuote(int CommoditID, RealTimeQuote& quote);

  //------------------------------------------------//
  //              Get Configure Message             //
  //------------------------------------------------//
  
///****************************************
/// @brief  根据商品ID，获取其市价建仓的配置信息，给到pConf；返回处理结果状态
/// @param  CommodityID		:	商品ID
/// @param  pConf					:	OpenMarketOrderConf类的实例
/// @return 
///  -  0 : 成功
///  - !0 : 失败 
///*****************************************
	int  getOpenMarketOrderConf(int CommodityID,  OpenMarketOrderConf& pConf);


///****************************************
/// @brief  根据商品ID，获取其限价建仓的配置信息，给到pConf；返回处理结果状态
/// @param  CommodityID			:	商品ID
/// @param  pConf						:	OpenLimitOrderConf类的实例
/// @return 
///  -  0 : 成功
///  - !0 : 失败 
///****************************************
	int  getOpenLimitOrderConf(int CommodityID,   OpenLimitOrderConf& pConf);


///****************************************
/// @brief  根据商品ID，获取其市价平仓的配置信息，给到pConf；返回处理结果状态
/// @param  CommodityID			:	商品ID
/// @param  pConf						:	CloseMarketOrderConf类的实例
/// @return 
///  -  0 : 成功
///  - !0 : 失败 
///****************************************
	int  getCloseMarketOrderConf(int CommodityID, CloseMarketOrderConf& pConf);


///****************************************
/// @brief  根据商品ID，获取限价平仓的配置信息，给到pConf；返回处理结果状态 
/// @param  CommodityID			:	商品ID
/// @param  pConf						:	LimitClosePositionConf类的实例
/// @return 
///  -  0 : 成功
///  - !0 : 失败 
///*****************************************
	int  getLimitClosePositionConf(int CommodityID,  LimitClosePositionConf& pConf);


///****************************************
/// @brief  根据客户端的配置类型，获取客户端的配置信息，给到ParamValue；返回处理结果 
/// @param  ParamType			:	客户端配置类型
/// @param  ParamValue		:	客户端配置标识
/// @return 
/// - MSG_STATUS_UPDATED: 成功
/// - !MSG_STATUS_UPDATED: 失败
///****************************************
	int  getClientConfig(int ParamType, double& ParamValue);

	void  loginOutNotice(int ret);

private:
	int  StartService();
	void StopService();
	void *Thread();

private:
	void processEvent(const TradeAPI::common::Event* pEvent);
	int  prcocessDispatch();

	int  InitMessage();
	int	 doReqSid(TradeAPI::common::Event *pEvent);
	int  procReqSid(const std::string strSID);

	int  QuoteBeginOrStop();
	int  QuoteSubscribe();

	int  queryAccountInfo();
	int  queryUserCommodityInfo();
	int  queryHoldPosition();
	int  queryLimitOrder();
	int  queryClosePosition();
	int  queryClientConfig();
	int  queryUserCommodityFeeGroup();
	int  queryMarketStatus();
	int  queryUserDepositeGroup();

	int	 queryCustomerBankInfo();

	void updateCommodityQuote(RealTimeQuote* pQuote);
	void processData();

	int  checkInitMessage();
	

	int  processHeartbeatEvent(const TradeAPI::business::HeartBeatEvent* pEvent);
	int  processQuoteEvent(const TradeAPI::business::SilverQuotePushEvent* pEvent);
	int  processQuoteEvent(const TradeAPI::business::GoldQuotePushEvent* pEvent);
	int  processLoginEvent(const TradeAPI::business::LoginEvent* pEvent);
	int  processTokenLoginEvent(const TradeAPI::business::TokenLoginEvent* pEvent);
	int  processUserCommodityInfoEvent(const TradeAPI::business::UserCommodityInfoEvent* pEvent);
	int  processMarketStatusEvent(const TradeAPI::business::MarketStatusQueryEvent* pEvent);
	int  processUserCommodityFeeGroupEvent(const TradeAPI::business::UserCommodityFeeGroupEvent* pEvent);
	int  processClientConfigQueryEvent(const TradeAPI::business::ClientConfigQueryEvent* pEvent);
	int  processQuoteBeginOrStopEvent(const TradeAPI::business::QuoteBeginOrStopEvent* pEvent);
	int  processQuoteSubscribe(const TradeAPI::business::QuoteSubscribeEvent* pEvent);
	int  processUserDepositeGroupEvent(const TradeAPI::business::DepositeQueryEvent* pEvent);

	int  processOpenPositionEvent(const TradeAPI::business::OpenPositionEvent* pEvent);
	int  processClosePositionEvent(const TradeAPI::business::ClosePositionEvent* pEvent);
	int  processLimitOpenPositionEvent(const TradeAPI::business::LimitOpenPositionEvent* pEvent);
	int  processLimitClosePositionEvent(const TradeAPI::business::LimitClosePositionEvent* pEvent);
	int  processLimitRevokeEvent(const TradeAPI::business::LimitRevokeEvent* pEvent);
	int  processCustomerCloseManyEvent(const TradeAPI::business::CustomerCloseManyEvent* pEvent);

	int  processAccountQueryEvent(const TradeAPI::business::AccountQueryEvent* pEvent);
	int  processHoldPositionOrderQueryEvent(const TradeAPI::business::HoldPositionOrderQueryEvent* pEvent);
	int  processLimitOrderQueryEvent(const TradeAPI::business::LimitOrderQueryEvent* pEvent);
	int  processClosePositionQueryEvent(const TradeAPI::business::ClosePositionQueryEvent* pEvent);

	int  processSysBulletinEvent(const TradeAPI::business::SysBulletinEvent* pEvent);

	int	 processMoneyInOutEvent(const TradeAPI::business::MoneyInOutEvent* pEvent);
	int  processCustomerBankInfoEvent(const TradeAPI::business::CustomerBankInfoEvent* pEvent);
	int  processMoneyQueryEvent(const TradeAPI::business::MoneyQueryEvent* pEvent);

	int  processDisconnectEvent(const TradeAPI::business::DisconnectEvent* pEvent);

	double findFeeItem(int CommodityID, int FeeType, int FeeSubType);
	int  findCommodityIsDisplay(const int CommodityID);
	int  findCommodity(const int CommodityID, CommodityInfo &pCommodityInfo);

	int  checkCommodityRight(const long long mCommodityRight);
	int	 checkOpenMarketOrder(const OpenMarketOrderParam mParam, const int mTotalWeight);
	int  checkCloseMarketOrder(const CloseMarketOrderParam mParam,const int mTotalWeight);
	int  checkCloseMarketOrderMany(const CloseMarketOrderManyParam mParam, const int mTotalWeight);
	int	 checkOpenLimitOrder(const OpenLimitOrderParam mParam, const int mTotalWeight);
	int  checkCloseLimitOrder(const CloseLimitOrderParam mParam, const int mOpenDirector, const double mClosePrice);
	int  checkMoneyInOut(const MoneyInOutParam mParam);
private:
	TradeAPI::business::TradeManager                *m_manager;
	Client                                          *m_Client;

	TradeAPI::business::BusinessMessage             *m_BusinessMessage;

private:
	UserInfo                            m_UserInfo;
	AccountInfo                         m_AccountInfo;
	CustomerBankInfo					m_CustomerBankInfo;

	RealTimeQuote                       m_Quote;
	map<int, CommodityInfo>             *m_mapCommodityInfo;
	map<long long, HoldPositionInfo>    *m_mapHoldPositionInfo;
	map<long long, LimitOrderInfo>      *m_mapLimitOrderInfo;
	map<long long, ClosePositionInfo>   *m_mapClosePositionInfo;
	map<long long, HoldPositionTotalInfo> *m_mapHoldPositionTotal;
	map<int, ClientConfig>              *m_mapClientConfig;
	vector<CommodityFeeGroup>           *m_vecFeeItem;

	map<string, HoldPositionInfo>       *m_mapNewHoldPosition;
	map<string, LimitOrderInfo>         *m_mapNewLimitOrder;
	map<string, LimitRevokeInfo>        *m_mapNewRevokeOrder;
	map<string, long long>              *m_mapNewLimitClosePosition;

	map<string, int>					*m_mapReqSid;
	
	//int m_HeartTimeOut;
	int	m_RetryTimes;
	int m_ResTimeOut;

	int m_MarketStatus;

	int  m_AccountStatus;
	int  m_CommodityStatus;
	int  m_HoldPositionStatus;
	int  m_LimitOrderStatus;
	int  m_ClosePositionStatus;
	int  m_FeeItemStatus;
	int  m_ClientConfigStatus;
	int  m_CustomerBankInfoStatus;

	int  m_InitFlag;

	//about thread
	bool    m_bStop;
	JMutex  m_StopMutex;
	JMutex  m_ReqMutex;

	//login flag;
	int		m_LoginStatus;
	bool	m_bIsLogin; // true--Login  false--UnLogin
	int		m_LifeStage; // 1--born by init()  2--grow by login() 0--dead by release()
	int 	m_bIsConnect;  // true--LoginRes  false--DisCon

	//Session
	int		m_fd;
	int		m_port;

};
#endif //__TRADE_INTERFACE_H__



