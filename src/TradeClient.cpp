

// TradeClient.cpp : 定义控制台应用程序的入口点。
//

#include "protocol.h"
#include <yt/util/queue.h>
#include <string>
#include <arpa/inet.h>
#include <stdlib.h>
#include "constant.h"
#include "TradeClient.h"
#include "bits.h"
//add by liuchengzhu 20150527
#include "serverconf.h"
//end add by liuchengzhu 20150527
//add by liuchengzhu 20150602
#include "CRedis.h"
//end add by liuchengzhu 20150602
//add by liuchengzhu 20150708
#include <sstream>
//end add by liuchengzhu 20150708

//add by wangzhen
#include <yt/util/mutex.h>
#include <yt/util/lockguard.h>
//end
using namespace yt;
using namespace std;

//add by wangzhen
yt::ThreadMutex			 ssl_mutex;
//end

extern yt::SyncQueue<std::string>   res_q;
//add by liuchengzhu 20150303
extern yt::SyncQueue<std::string>   exc_q;
//end add by liuchengzhu 20150303

//add by liuchengzhu 20150602

int TradeClient::requestOrderConfigData(vector<order_config> &vOrderConfig, bool &isWriteCache)
{
	int rettmp;
	vector<CommodityInfo> vCommodity;
	rettmp = GetCommodityInfo(vCommodity);
	if (MSG_STATUS_UPDATED != rettmp)
		return rettmp;

	unsigned int commodityNum = vCommodity.size();

	vOrderConfig.resize(commodityNum);

	for(unsigned int i = 0; i < commodityNum; ++i)
	{
		vOrderConfig[i].CommodityID = vCommodity[i].CommodityID;

		rettmp = GetOpenMarketOrderConf(vOrderConfig[i].CommodityID, vOrderConfig[i].OpenMarket);
		if(0 != rettmp) {
			rettmp = GetOpenMarketOrderConf(vOrderConfig[i].CommodityID, vOrderConfig[i].OpenMarket);
			if (0 != rettmp) {
				isWriteCache = false;
				AC_ERROR("GetOpenMarketOrderConf failed! CommodityID is %d, retcode is %d", vOrderConfig[i].CommodityID, rettmp);
				continue;
			}
		}

		rettmp = GetCloseMarketOrderConf(vOrderConfig[i].CommodityID, vOrderConfig[i].CloseMarket);
		if(0 != rettmp) {
			rettmp = GetCloseMarketOrderConf(vOrderConfig[i].CommodityID, vOrderConfig[i].CloseMarket);
			if (0 != rettmp) {
				isWriteCache = false;
				AC_ERROR("GetCloseMarketOrderConf failed! CommodityID is %d, retcode is %d", vOrderConfig[i].CommodityID, rettmp);
				continue;
			}
		}

		rettmp = GetOpenLimitOrderConf(vOrderConfig[i].CommodityID, vOrderConfig[i].OpenLimit);
		if(0 != rettmp) {
			rettmp = GetOpenLimitOrderConf(vOrderConfig[i].CommodityID, vOrderConfig[i].OpenLimit);
			if (0 != rettmp) {
				isWriteCache = false;
				AC_ERROR("GetOpenLimitOrderConf failed! CommodityID is %d, retcode is %d", vOrderConfig[i].CommodityID, rettmp);
				continue;
			}
		}

		rettmp = GetLimitClosePositionConf(vOrderConfig[i].CommodityID, vOrderConfig[i].CloseLimit);
		if(0 != rettmp) {
			rettmp = GetLimitClosePositionConf(vOrderConfig[i].CommodityID, vOrderConfig[i].CloseLimit);
			if (0 != rettmp) {
				isWriteCache = false;
				AC_ERROR("GetLimitClosePositionConf failed! CommodityID is %d, retcode is %d", vOrderConfig[i].CommodityID, rettmp);
				continue;
			}
		}
	}
	return 0;
}

bool TradeClient::serializeOrderConfigData(const vector<order_config> &vOrderConfig, string &configJsonStr)
{
	json_object *configObj = json_object_new_object();
	if(NULL == configObj || is_error(configObj)) {
		AC_ERROR("json_object_new_object failed!");
		return false;
	}
	JsonObjGuard configBodyObjGuard(configObj);

	json_object *configArray = json_object_new_array();
	if(NULL == configArray || is_error(configArray)) {
		AC_ERROR("json_object_new_array failed!");
		return false;
	}
	JsonObjGuard configArrayGuard(configArray);

	for (unsigned int i = 0; i < vOrderConfig.size(); ++i)
	{
		json_object *tempObject = json_object_new_object();

		char sID[256]={0};
		sprintf(sID,"%d",vOrderConfig[i].CommodityID);
		json_object_object_add(tempObject,"ID",json_object_new_string(sID));

		json_object *tempOpenMarketOrderConfObj = json_object_new_object();
		json_object_object_add(tempOpenMarketOrderConfObj, "maxqty",			json_object_new_double(vOrderConfig[i].OpenMarket.MaxQuantity));
		json_object_object_add(tempOpenMarketOrderConfObj, "minqty",			json_object_new_double(vOrderConfig[i].OpenMarket.MinQuantity));
		json_object_object_add(tempOpenMarketOrderConfObj, "maxpt",				json_object_new_double(vOrderConfig[i].OpenMarket.MaxTradeRange));
		json_object_object_add(tempOpenMarketOrderConfObj, "minpt",				json_object_new_double(vOrderConfig[i].OpenMarket.MinTradeRange));
		json_object_object_add(tempOpenMarketOrderConfObj, "dpt",				json_object_new_double(vOrderConfig[i].OpenMarket.DefaultTradeRange));
		json_object_object_add(tempOpenMarketOrderConfObj, "AgreeUnit",			json_object_new_double(vOrderConfig[i].OpenMarket.AgreeUnit));
		json_object_object_add(tempOpenMarketOrderConfObj, "CommodityMode",		json_object_new_int   (vOrderConfig[i].OpenMarket.CommodityMode));
		json_object_object_add(tempOpenMarketOrderConfObj, "WeightStep",		json_object_new_double(vOrderConfig[i].OpenMarket.WeightStep));
		json_object_object_add(tempOpenMarketOrderConfObj, "WeightRadio",		json_object_new_double(vOrderConfig[i].OpenMarket.WeightRadio));
		json_object_object_add(tempOpenMarketOrderConfObj, "MinTotalWeight",	json_object_new_double(vOrderConfig[i].OpenMarket.MinTotalWeight));
		json_object_object_add(tempOpenMarketOrderConfObj, "MaxTotalWeight",	json_object_new_double(vOrderConfig[i].OpenMarket.MaxTotalWeight));
		//add by liuchengzhu 20150707
		json_object_object_add(tempOpenMarketOrderConfObj,"DepositeRate",       json_object_new_double(vOrderConfig[i].OpenMarket.DepositeRate));
		//end add by liuchengzhu 20150707
		json_object_object_add(tempOpenMarketOrderConfObj,"SpecificationRate",       json_object_new_double(vOrderConfig[i].OpenMarket.SpecificationRate));

		json_object_object_add(tempObject,					"OpenMarket",		tempOpenMarketOrderConfObj);

		json_object *tempCloseMarketOrderConfObj = json_object_new_object();
		json_object_object_add(tempCloseMarketOrderConfObj, "maxqty",			json_object_new_double(vOrderConfig[i].CloseMarket.MaxQuantity));
		json_object_object_add(tempCloseMarketOrderConfObj, "minqty",			json_object_new_double(vOrderConfig[i].CloseMarket.MinQuantity));
		json_object_object_add(tempCloseMarketOrderConfObj, "maxpt",			json_object_new_double(vOrderConfig[i].CloseMarket.MaxTradeRange));
		json_object_object_add(tempCloseMarketOrderConfObj, "minpt",			json_object_new_double(vOrderConfig[i].CloseMarket.MinTradeRange));
		json_object_object_add(tempCloseMarketOrderConfObj, "dpt",				json_object_new_double(vOrderConfig[i].CloseMarket.DefaultTradeRange));
		json_object_object_add(tempCloseMarketOrderConfObj, "AgreeUnit",		json_object_new_double(vOrderConfig[i].CloseMarket.AgreeUnit));
		json_object_object_add(tempCloseMarketOrderConfObj, "CommodityMode",	json_object_new_int   (vOrderConfig[i].CloseMarket.CommodityMode));
		json_object_object_add(tempCloseMarketOrderConfObj, "WeightStep",		json_object_new_double(vOrderConfig[i].CloseMarket.WeightStep));
		json_object_object_add(tempCloseMarketOrderConfObj, "WeightRadio",		json_object_new_double(vOrderConfig[i].CloseMarket.WeightRadio));
		json_object_object_add(tempCloseMarketOrderConfObj, "MinTotalWeight",	json_object_new_double(vOrderConfig[i].CloseMarket.MinTotalWeight));
		json_object_object_add(tempCloseMarketOrderConfObj, "MaxTotalWeight",	json_object_new_double(vOrderConfig[i].CloseMarket.MaxTotalWeight));
		json_object_object_add(tempObject,					"CloseMarket",		tempCloseMarketOrderConfObj);

		json_object *tempOpenLimitOrderConfObj = json_object_new_object();
		json_object_object_add(tempOpenLimitOrderConfObj, "maxqty",				json_object_new_double(vOrderConfig[i].OpenLimit.MaxQuantity));
		json_object_object_add(tempOpenLimitOrderConfObj, "minqty",				json_object_new_double(vOrderConfig[i].OpenLimit.MinQuantity));
		json_object_object_add(tempOpenLimitOrderConfObj, "LimitSpread",		json_object_new_double(vOrderConfig[i].OpenLimit.LimitSpread));
		json_object_object_add(tempOpenLimitOrderConfObj, "FixSpread",			json_object_new_double(vOrderConfig[i].OpenLimit.FixSpread));
		json_object_object_add(tempOpenLimitOrderConfObj, "TPSpread",			json_object_new_double(vOrderConfig[i].OpenLimit.TPSpread));
		json_object_object_add(tempOpenLimitOrderConfObj, "SLSpread",			json_object_new_double(vOrderConfig[i].OpenLimit.SLSpread));
		json_object_object_add(tempOpenLimitOrderConfObj, "MinPriceUnit",		json_object_new_double(vOrderConfig[i].OpenLimit.MinPriceUnit));
		json_object_object_add(tempOpenLimitOrderConfObj, "AgreeUnit",			json_object_new_double(vOrderConfig[i].OpenLimit.AgreeUnit));
		json_object_object_add(tempOpenLimitOrderConfObj, "CommodityMode",		json_object_new_int   (vOrderConfig[i].OpenLimit.CommodityMode));
		json_object_object_add(tempOpenLimitOrderConfObj, "WeightStep",			json_object_new_double(vOrderConfig[i].OpenLimit.WeightStep));
		json_object_object_add(tempOpenLimitOrderConfObj, "WeightRadio",		json_object_new_double(vOrderConfig[i].OpenLimit.WeightRadio));
		json_object_object_add(tempOpenLimitOrderConfObj, "MinTotalWeight",		json_object_new_double(vOrderConfig[i].OpenLimit.MinTotalWeight));
		json_object_object_add(tempOpenLimitOrderConfObj, "MaxTotalWeight",		json_object_new_double(vOrderConfig[i].OpenLimit.MaxTotalWeight));
		//add by liuchengzhu 20150707
		json_object_object_add(tempOpenLimitOrderConfObj, "DepositeRate",       json_object_new_double(vOrderConfig[i].OpenLimit.DepositeRate));
		//end add by liuchengzhu 20150707
		json_object_object_add(tempOpenLimitOrderConfObj,"SpecificationRate",       json_object_new_double(vOrderConfig[i].OpenLimit.SpecificationRate));
		json_object_object_add(tempObject,				  "OpenLimit",			tempOpenLimitOrderConfObj);

		json_object *tempCloseLimitOrderConfObj = json_object_new_object();
		json_object_object_add(tempCloseLimitOrderConfObj, "FixSpread",			json_object_new_double(vOrderConfig[i].CloseLimit.FixedSpread));
		json_object_object_add(tempCloseLimitOrderConfObj, "TPSpread",			json_object_new_double(vOrderConfig[i].CloseLimit.TPSpread));
		json_object_object_add(tempCloseLimitOrderConfObj, "SLSpread",			json_object_new_double(vOrderConfig[i].CloseLimit.SLSpread));
		json_object_object_add(tempCloseLimitOrderConfObj, "MinPriceUnit",		json_object_new_double(vOrderConfig[i].CloseLimit.MinPriceUnit));
		json_object_object_add(tempCloseLimitOrderConfObj, "AgreeUnit",			json_object_new_double(vOrderConfig[i].CloseLimit.AgreeUnit));
		json_object_object_add(tempCloseLimitOrderConfObj, "CommodityMode",		json_object_new_int   (vOrderConfig[i].CloseLimit.CommodityMode));
		json_object_object_add(tempCloseLimitOrderConfObj, "WeightRadio",		json_object_new_double(vOrderConfig[i].CloseLimit.WeightRadio));		
		json_object_object_add(tempObject,				   "CloseLimit",		tempCloseLimitOrderConfObj);

		json_object_array_add(configArray, tempObject);
	}
	json_object_object_add(configObj, "body", configArray);

	configJsonStr = json_object_to_json_string(configObj);
	AC_INFO("----------------------begin commodity configuration information------------------\n");
	AC_INFO("%s\n", configJsonStr.c_str());
	AC_INFO("----------------------end commodity configuration information------------------\n");
	return true;
}

bool TradeClient::getOrderConfigInfo(string &retString)
{
	CRedis redis;
	//add by liuchengzhu 20150602
	//modify by liuchengzhu 20150603
	//redis.Init("/tmp/redis.sock", 6379);
	redis.Init(ServerConf::Instance()->m_RedisPara.m_SvrIp, ServerConf::Instance()->m_RedisPara.m_port);
	//end modify by liuchengzhu 20150603
	if(-1 == redis.ConnectDb()){
		AC_ERROR("connect to redis Srv error ip:%s,port:%d\n",ServerConf::Instance()->m_RedisPara.m_SvrIp,ServerConf::Instance()->m_RedisPara.m_port);
		return -1;
	}
	//end add by liuchengzhu 20150602
	// 	首先到缓存中查询是否有该数据
	// 	如果没有，从交易所查询数据，然后打包成json格式存在缓存中
	// 	否则直接从缓存中取数据
	string data;
	string retdata;
	data = "orderConfig";
	if(redis.Get(data, retdata) == REDIS_ERR || retdata == "" ) {
		vector<string>  pack;
		//add by liuchengzhu 20150708
		vector<string>  pack_expire;
		//end add by liuchengzhu 20150708
// 		string configkey = "orderConfig";
		// 		retdata = requestAndSerializeOrderConfigData(tradeClient);	
		vector<order_config> vOrderConfig;
		bool isWriteCache = true;
		if(requestOrderConfigData(vOrderConfig, isWriteCache) != 0)
			return false;
		if(serializeOrderConfigData(vOrderConfig, retdata) == false)
			return false;
		if(isWriteCache) {
			pack.push_back("set");
	// 		pack.push_back(configkey);
			pack.push_back("orderConfig");
			pack.push_back(retdata);
			int retcode = redis.SendCommad(pack);
			AC_INFO("writing config info to redis. retcode is %d\n", retcode);	

			//add by liuchengzhu 20150708
			int expiretime=ServerConf::Instance()->m_RedisPara.m_expiretime;
			stringstream ss;
			ss<<expiretime;
			string sexpiretime=ss.str();
			
			pack_expire.push_back("expire");
			pack_expire.push_back("orderConfig");
			pack_expire.push_back(sexpiretime);

			retcode = redis.SendCommad(pack_expire);
			AC_INFO("writing expire info to redis. retcode is %d\n", retcode);	
			//end add by liuchengzhu 20150708
		} else {
			AC_ERROR("RequestOrderConfigData error! Not writing config info to redis!\n");
		}
	} 

	// 	将得到的数据加上retcode和username等信息打包，并返回
	json_object *tempOrderConfigBodyObj = json_tokener_parse(retdata.c_str());
	JsonObjGuard tempOrderConfigBodyObjGuard(tempOrderConfigBodyObj);
	if(tempOrderConfigBodyObj == NULL || is_error(tempOrderConfigBodyObj) || (json_object_get_type(tempOrderConfigBodyObj) != json_type_object)) {		
		AC_ERROR("parse tempOrderConfigBodyObj failed!");
		return false;		
	}
	json_object *configBodyDataObj = json_object_object_get(tempOrderConfigBodyObj, "body");
	if(configBodyDataObj == NULL || is_error(configBodyDataObj)) {		
		AC_ERROR("get body object failed!");
		return false;	
	}
	json_object * retObj = json_object_new_object();
	if(NULL == retObj || is_error(retObj)) {
		AC_ERROR("json_object_new_object failed!");
		return false;
	}
	JsonObjGuard retObjGuard(retObj);
	json_object_object_add(retObj, "ret", json_object_new_int(0));
	json_object_object_add(retObj,"msg",json_object_new_string(""));
	json_object_object_add(retObj, "uname", json_object_new_string(LoginAcc.c_str()));
	json_object_object_add(retObj, "body", configBodyDataObj);

	retString = json_object_to_json_string(retObj);

	// 	json_object_put(tempOrderConfigBodyObj);
	// 	json_object_put(retObj);

	AC_INFO("----------------------begin returned info------------------\n");
	AC_INFO("%s\n", retString.c_str());
	AC_INFO("----------------------end returned info------------------\n");

	return true;
}

//end add by liuchengzhu 20150602

#include<iconv.h>
int utf8togb2312(char *sourcebuf,size_t sourcelen,char *destbuf,size_t destlen) 
{   
	iconv_t cd;   

	if( (cd = iconv_open("gb2312","utf-8")) ==0 )     
		return -1;  

	memset(destbuf,0,destlen);   

	char **source = &sourcebuf;   

	char **dest = &destbuf;
	if((size_t)-1 == iconv(cd,source,&sourcelen,dest,&destlen))     

		return -1;   

	iconv_close(cd);   

	return 0;   

}

TradeClient::TradeClient()
{
	//add by liuchengzhu 20150312
	tRequest=0;
	//end add by liuchengzhu 20150312

	//add by liuchengzhu 20150324
	bNeedStop=false;
	//end add by liuchengzhu 20150324
}

TradeClient::~TradeClient()
{

}

int TradeClient::TimeToMSecond(const long long second,long &msecond)
{
	time_t sec = second;
	if(sec < TS_1970_TO_1900)
	{
		msecond=0;
		return 1;
	}
	sec -= TS_1970_TO_1900;
	msecond=sec*1000;
	return 0;
}

int TradeClient::TimeToStr(const long long second, char* dest, int len)
{
	if(dest == NULL || len < 20)
		return -1;

	memset(dest, 0, len);

    time_t sec = second;
	if(sec < TS_1970_TO_1900)
	{
#ifdef WIN32
		_snprintf(dest, len, "1900-01-01 00:00:00");
#else
		snprintf(dest, len, "1900-01-01 00:00:00");
#endif
		return 1;
	}
    sec -= TS_1970_TO_1900;

#ifdef WIN32
	struct tm result;
    localtime_s(&result , &sec);
    _snprintf(dest, len, "%04d-%02d-%02d %02d:%02d:%02d", result.tm_year + 1900, result.tm_mon + 1,
            result.tm_mday, result.tm_hour, result.tm_min, result.tm_sec);
#else
	struct tm *result;
	result = localtime(&sec);
    strftime(dest, len, "%Y-%m-%d %H:%M:%S", result);
#endif

	return 0;
}

int TradeClient::Init()
{
	//add by wangzhen 20150909
	ssl_mutex.Acquire();
	LockGuard lg(&ssl_mutex);
	//end
	m_Interface = new TradeInterface;

	int ret = InitNetwork();
	return ret;
}

int TradeClient::InitNetwork()
{
	//add by liuchengzhu 20150527
	string sIP=ServerConf::Instance()->m_ExchangeConnInfo.m_SvrIp;
	int nPort=ServerConf::Instance()->m_ExchangeConnInfo.m_port;
	//end add by liuchengzhu 20150517

	int ret = m_Interface->Init(sIP,nPort,"./");
	if(0 != ret){
		AC_ERROR("[TE] init network fail !\n");
		return ret;
	}
	m_Interface->RegisterClient(this);
	m_Interface->SetAppMessage("test1","123456");
	m_Interface->setVendorID("874987497234");

	return ret;
}

bool TradeClient::Login(std::string ploginacc, std::string ppwd)
{
	// to get the LoginAccount & pwd from the InputText
	string loginacc = ploginacc;
	string pwd = ppwd;
	LoginAcc = ploginacc;

	int ret = -1;
	ret = m_Interface->doLogin(loginacc,pwd);
	
	string msg;
	if(0 == ret){
		AC_INFO("[T] login succeed !\n");
		msg="login succeed!";
		return true;
	}else{
		AC_ERROR("[TE] TradeClient::Login >> login fail! ret:%d\n",ret);
		msg="login fail!";
	}
	return false;
	/*
	HandleRes(TID_LOGIN,ret,msg);

	return ret;*/
}

void TradeClient::DisConnect()
{
	string json_rep="{}";
	HandleRes(TID_DISCONNECT,json_rep);
}

int TradeClient::Release()
{
	/*
	if(m_Interface)
	{
		delete m_Interface;
	}
	*/
	// add by wangzhen 20150909
	ssl_mutex.Acquire();
	LockGuard lg(&ssl_mutex);
	//end
	int ret = -1;
	if(m_Interface)
	{
		ret = m_Interface->Release();
		if(ret == -1)
			return ret;
		delete m_Interface;
		m_Interface = NULL;
		//gStatus = -99;
	}
	return ret;
}

void TradeClient::procTradeEvent(const TradeEvent *pEvent)
{
	string strData;
	json_object *RspObj = json_object_new_object();
	if(NULL == RspObj || is_error(RspObj))
	{
		AC_ERROR("%s", "json_object_new_object failed!");
		return ;
	}
	JsonObjGuard RspObjGuard(RspObj);

	char msg[MAX_MESSAGE_LEN];


	switch(pEvent->GetType())
	{
		case EVENT_CODE_LOGIN:
		{
			const TLoginEvent *p  = (const TLoginEvent*)(pEvent);
			ProcessResult result = p->getLoginResult();
			if(Event_Ret_Success != result.RetCode)
			{
				if(Event_Ret_DROPUSER == result.RetCode)
				{
					AC_INFO("[TE] ACC[%s] uid[%d] You Have Logon At Other Place! Ret:%d, Msg:%s\n",LoginAcc.c_str(),uid,result.RetCode, result.Message);
				}
				else if(Event_Ret_LOGACCERR == result.RetCode)
				{
					AC_INFO("[TE] ACC[%s] uid[%d] LOGINACCOUNT Not Exist! Ret:%d, Msg:%s\n",LoginAcc.c_str(),uid,result.RetCode, result.Message);
				}
				AC_INFO("[TE] ACC[%s] uid[%d] Login Fail! Ret:%d, Msg:%s\n",LoginAcc.c_str(),uid,result.RetCode, result.Message);
			}
			else
			{
				AC_INFO("[T] ACC[%s] uid[%d] Login success! Ret:%d, Msg:%s\n",LoginAcc.c_str(),uid,result.RetCode, result.Message);
			}
			
			//utf8togb2312(result.Message,strlen(result.Message),msg,MAX_MESSAGE_LEN);
			memcpy(msg,result.Message,MAX_MESSAGE_LEN);

			json_object_object_add(RspObj, "ret", json_object_new_int(result.RetCode));
			json_object_object_add(RspObj, "msg", json_object_new_string(msg));
			//add by liuchengzhu 20150518
			json_object_object_add(RspObj,"uname",json_object_new_string(LoginAcc.c_str()));
			//end add by liuchengzhu 20150518


			strData=json_object_to_json_string(RspObj);
			
			if(Event_Ret_Success != result.RetCode)
			{
				HandleRes(TID_EVENT_DISCONNECT-1,strData);
				HandleRes(TID_EVENT_DISCONNECT,strData);
			}
			else
			{
				HandleRes(TID_EVENT_DISCONNECT-1,strData);
			}
			

			break;
		}
		case EVENT_CODE_QUOTE:
		{
			const TQuoteEvent *p = (const TQuoteEvent*)(pEvent);
			RealTimeQuote quote =  p->getQuote();
			//printf("[T] ACC[%s] QUOTE: %d,%f,%f,%f,%f,%lld\n",LoginAcc.c_str(),quote.CommodityID,quote.BuyPrice,quote.SellPrice,quote.HighPrice,quote.LowPrice,quote.QuoteTime);
			if(quote.CommodityID == 100006010)
				m_quote = quote;

			//  or to update the values of price_view by  quote.CommodityID
			break;
		}
		case EVENT_CODE_OPENMARKETORDER:
		{
			AC_INFO("get OPENMARKETORDER retSignal !\n");

			const TOpenMarketOrderEvent *e=NULL;
			e = (const TOpenMarketOrderEvent *)pEvent;

			char cHPID[64];
			snprintf(cHPID,sizeof(cHPID),"%lld",e->getHoldPositionID());

			//utf8togb2312(e->getResult().Message,strlen(e->getResult().Message),msg,MAX_MESSAGE_LEN);
			memcpy(msg,e->getResult().Message,MAX_MESSAGE_LEN);

			json_object_object_add(RspObj, "ret", json_object_new_int(e->getResult().RetCode));
			json_object_object_add(RspObj, "msg", json_object_new_string(msg));
			//add by liuchengzhu 20150518
			json_object_object_add(RspObj,"uname",json_object_new_string(LoginAcc.c_str()));
			//end add by liuchengzhu 20150518


			json_object_object_add(RspObj,"HPID",json_object_new_string(cHPID));

			strData=json_object_to_json_string(RspObj);

			HandleRes(TID_EVENT_OPENMARKETORDER,strData);


			AC_INFO("get OPENMARKETORDER retSignal end !\n");


			break;
		}
		case EVENT_CODE_CLOSEMARKETORDER:
			{
				AC_INFO("get CLOSEMARKETORDER retSignal !\n");

				const TCloseMarketOrderEvent *e=NULL;
				e = (const TCloseMarketOrderEvent *)pEvent;

				char cHPID[64];
				snprintf(cHPID,sizeof(cHPID),"%lld",e->getHoldPositionID());

				//utf8togb2312(e->getResult().Message,strlen(e->getResult().Message),msg,MAX_MESSAGE_LEN);
				memcpy(msg,e->getResult().Message,MAX_MESSAGE_LEN);

				json_object_object_add(RspObj, "ret", json_object_new_int(e->getResult().RetCode));
				json_object_object_add(RspObj, "msg", json_object_new_string(msg));
				//add by liuchengzhu 20150518
				json_object_object_add(RspObj,"uname",json_object_new_string(LoginAcc.c_str()));
				//end add by liuchengzhu 20150518


				json_object_object_add(RspObj,"HPID",json_object_new_string(cHPID));

				strData=json_object_to_json_string(RspObj);

				HandleRes(TID_EVENT_CLOSEMARKETORDER,strData);

				AC_INFO("get CLOSEMARKETORDER retSignal end !\n");


				break;
			}
		case EVENT_CODE_OPENLIMITORDER:
			{
				AC_INFO("get OPENLIMITORDER retSignal !\n");

				const TOpenLimitOrderEvent *e=NULL;
				e = (const TOpenLimitOrderEvent *)pEvent;

				char cLOID[64];
				snprintf(cLOID,sizeof(cLOID),"%lld",e->getLimitOrderID());

				//utf8togb2312(e->getResult().Message,strlen(e->getResult().Message),msg,MAX_MESSAGE_LEN);
				memcpy(msg,e->getResult().Message,MAX_MESSAGE_LEN);

				json_object_object_add(RspObj, "ret", json_object_new_int(e->getResult().RetCode));
				json_object_object_add(RspObj, "msg", json_object_new_string(msg));
				//add by liuchengzhu 20150518
				json_object_object_add(RspObj,"uname",json_object_new_string(LoginAcc.c_str()));
				//end add by liuchengzhu 20150518


				json_object_object_add(RspObj,"LimitOrderID",json_object_new_string(cLOID));

				strData=json_object_to_json_string(RspObj);

				HandleRes(TID_EVENT_OPENLIMITORDER,strData);


				AC_INFO("get OPENLIMITORDER retSignal end !\n");


				break;
			}
		case EVENT_CODE_LIMITCLOSEPOSITION:
			{
				AC_INFO("get LIMITCLOSEPOSITION retSignal !\n");

				const TLimitClosePositionEvent *e=NULL;
				e = (const TLimitClosePositionEvent *)pEvent;

				char cHPID[64];
				snprintf(cHPID,sizeof(cHPID),"%lld",e->getHoldPositionID());

				//utf8togb2312(e->getResult().Message,strlen(e->getResult().Message),msg,MAX_MESSAGE_LEN);
				memcpy(msg,e->getResult().Message,MAX_MESSAGE_LEN);

				json_object_object_add(RspObj, "ret", json_object_new_int(e->getResult().RetCode));
				json_object_object_add(RspObj, "msg", json_object_new_string(msg));
				//add by liuchengzhu 20150518
				json_object_object_add(RspObj,"uname",json_object_new_string(LoginAcc.c_str()));
				//end add by liuchengzhu 20150518


				json_object_object_add(RspObj,"HPID",json_object_new_string(cHPID));

				strData=json_object_to_json_string(RspObj);

				HandleRes(IID_EVENT_LIMITCLOSEPOSITION,strData);


				AC_INFO("get LIMITCLOSEPOSITION retSignal end !\n");
				break;
			}
		case EVENT_CODE_LIMITREVOKE:
			{
				AC_INFO("get LIMITREVOKE retSignal !\n");

				const TRevokeLimitOrderEvent *e=NULL;
				e = (const TRevokeLimitOrderEvent *)pEvent;

				char cLOID[64];
				snprintf(cLOID,sizeof(cLOID),"%lld",e->getLimitOrderID());

				//utf8togb2312(e->getResult().Message,strlen(e->getResult().Message),msg,MAX_MESSAGE_LEN);
				memcpy(msg,e->getResult().Message,MAX_MESSAGE_LEN);

				json_object_object_add(RspObj, "ret", json_object_new_int(e->getResult().RetCode));
				json_object_object_add(RspObj, "msg", json_object_new_string(msg));
				//add by liuchengzhu 20150518
				json_object_object_add(RspObj,"uname",json_object_new_string(LoginAcc.c_str()));
				//end add by liuchengzhu 20150518


				json_object_object_add(RspObj,"LimitOrderID",json_object_new_string(cLOID));

				strData=json_object_to_json_string(RspObj);

				HandleRes(TID_EVENT_LIMITREVOKE,strData);

				AC_INFO("get LIMITREVOKE retSignal end !\n");
				break;
			}


		case EVENT_CODE_DISCONNECT:
		{
			AC_INFO("get DISCONNECT retSignal !\n");

			const TDisConnectEvent *e = (const TDisConnectEvent*)(pEvent);

			if(Event_Ret_Success != e->getResult().RetCode)
			{
			    AC_INFO("[TE] ACC[%s] DISCONNECT ERROR! result code = %d, msg = %s\n",LoginAcc.c_str(),e->getResult().RetCode, e->getResult().Message);
			}
			else
			{
				AC_INFO("[TE] ACC[%s] DISCONNECT SUCCESS! result code = %d, msg = %s\n",LoginAcc.c_str(),e->getResult().RetCode, e->getResult().Message);
			}

			//utf8togb2312(e->getResult().Message,strlen(e->getResult().Message),msg,MAX_MESSAGE_LEN);
			memcpy(msg,e->getResult().Message,MAX_MESSAGE_LEN);

			json_object_object_add(RspObj, "ret", json_object_new_int(e->getResult().RetCode));
			json_object_object_add(RspObj, "msg", json_object_new_string(msg));
			//add by liuchengzhu 20150518
			json_object_object_add(RspObj,"uname",json_object_new_string(LoginAcc.c_str()));
			//end add by liuchengzhu 20150518


			strData=json_object_to_json_string(RspObj);

			HandleRes(TID_EVENT_DISCONNECT-1,strData);
			HandleRes(TID_EVENT_DISCONNECT,strData);

			AC_INFO("EVENT_CODE_DISCONNECT %s",strData.c_str());
			AC_INFO("EVENT_CODE_DISCONNECT uid=%d",uid);

			break;
		}
		case EVENT_CODE_SYSBULLETIN:
		{
			AC_INFO("get SYSBULLETIN!\n");
			
			const TSysBulletinEvent *p = (const TSysBulletinEvent*)(pEvent);
			SysBulletinInfo mSysBulletinInfo = p->getSysBulletin();

			AC_INFO("[T] LoginID:%d, BulletinID:%lld, TradeMode:%d, BulletinType:%d, BulletinPriorty:%d\n",mSysBulletinInfo.LoginID,\
				mSysBulletinInfo.BulletinID, mSysBulletinInfo.TradeMode, mSysBulletinInfo.BulletinType, mSysBulletinInfo.BulletinPriorty);
			AC_INFO("BulletinHeader:%s, BulletinContent:%s\n",mSysBulletinInfo.BulletinHeader, mSysBulletinInfo.BulletinContent);
			AC_INFO("PublisherName:%s, BulletinMethod:%d\n",mSysBulletinInfo.PublisherName, mSysBulletinInfo.BulletinMethod);

			break;
		}
		case EVENT_CODE_MONEYINOUT:
		{
			AC_INFO("get MONEYINOUT retSignal !\n");

			const TMoneyInOutEvent *e=NULL;
			e = (const TMoneyInOutEvent *)pEvent;
			if(Event_Ret_Success != e->getResult().RetCode)
			{
				//modify by liuchengzhu 20141211
				//printf("[TE] OPENMARKETORDER ERROR! result code = %d, msg = %s\n",e->getResult().RetCode, e->getResult().Message);
				AC_INFO("\n[TE] MONEYINOUT ERROR! \n");
				AC_INFO(" result code = %d,\n",e->getResult().RetCode);

				char msg[MAX_MESSAGE_LEN];
				//utf8togb2312(e->getResult().Message,strlen(e->getResult().Message),msg,MAX_MESSAGE_LEN);
				memcpy(msg,e->getResult().Message,MAX_MESSAGE_LEN);

				//printf(" msg = %s\n", e->getResult().Message);
				AC_INFO(" msg = %s\n", msg);
				//end modify by liuchengzhu 20141211
			}
			else
			{
				//add by liuchengzhu 20141211
				AC_INFO("[T]MONEYINOUT SUCESS!\n");
				//end add by liuchengzhu 20141211

			}
			AC_INFO("get MONEYINOUT retSignal end !\n");
			break;
		}
		case EVENT_CODE_MONEYQUERY:
		{
			AC_INFO("get MONEYQUERY retSignal !\n");

			const TMoneyQueryEvent *e=NULL;
			e = (const TMoneyQueryEvent *)pEvent;


			//utf8togb2312(e->getResult().Message,strlen(e->getResult().Message),msg,MAX_MESSAGE_LEN);
			memcpy(msg,e->getResult().Message,MAX_MESSAGE_LEN);

			json_object_object_add(RspObj, "ret", json_object_new_int(e->getResult().RetCode));
			json_object_object_add(RspObj, "msg", json_object_new_string(msg));
			//add by liuchengzhu 20150518
			json_object_object_add(RspObj,"uname",json_object_new_string(LoginAcc.c_str()));
			//end add by liuchengzhu 20150518


			if(Event_Ret_Success == e->getResult().RetCode)
			{
				//add by liuchengzhu 20141219
				BourseMoneyInfo moneyInfo=e->getBourseMoneyInfo();
				AC_INFO("[T]GetMoneyQueryInfo Amount=%f\tAmountAvailable=%f\tAmountFetchable=%f\n",moneyInfo.Amount,moneyInfo.AmountAvailable,moneyInfo.AmountFetchable);
				//end add by liuchengzhu 20141219

				json_object_object_add(RspObj,"Amount",json_object_new_double(moneyInfo.Amount));
				json_object_object_add(RspObj,"Available",json_object_new_double(moneyInfo.AmountAvailable));
				json_object_object_add(RspObj,"Fetchable",json_object_new_double(moneyInfo.AmountFetchable));

			}
			strData=json_object_to_json_string(RspObj);

			HandleRes(TID_MONEY_QUERY,strData);

			AC_INFO("get MONEYQUERY retSignal end !\n");
			break;

		}
		case EVENT_CODE_HEARTBEAT:
		{
			//printf("[T] ACC[%s] Keep HeartBeat!!\n",LoginAcc.c_str());
			break;
		}
		default:
		{
			AC_INFO("[TE] get unknow RETEVENT:%d\n", pEvent->GetType());
			break;
		}
	}

}

//add by liuchengzhu 20150602
void TradeClient::GetOrderConfig()
{
	string strData;
	getOrderConfigInfo(strData);
	HandleQueryRes(TID_GETORDERCONFIG,strData);
}
//end add by liuchengzhu 20150602

//获取账户信息
int TradeClient::GetAccountInfo()  
{
	AccountInfo account;

	int ret = m_Interface->getAccountInfo(account);

	string msg;

	if(MSG_STATUS_UPDATED != ret)
		msg="GetAccountInfo fail!";
	else
		msg="GetAccountInfo success!";

	string strData;
	json_object *RspObj = json_object_new_object();
	if(NULL == RspObj || is_error(RspObj))
	{
		AC_ERROR("%s", "json_object_new_object failed!");
		return -1;
	}
	JsonObjGuard RspObjGuard(RspObj);

	json_object_object_add(RspObj,"ret",json_object_new_int(ret));
	json_object_object_add(RspObj,"msg",json_object_new_string(msg.c_str()));
	//add by liuchengzhu 20150518
	json_object_object_add(RspObj,"uname",json_object_new_string(LoginAcc.c_str()));
	//end add by liuchengzhu 20150518


	json_object *tmpObj = json_object_new_object();

	if(MSG_STATUS_UPDATED == ret){
		json_object_object_add(tmpObj,"Acc",json_object_new_string(account.Account));
		json_object_object_add(tmpObj,"LoginAcc",json_object_new_string(account.LoginAccount));
		json_object_object_add(tmpObj,"Name",json_object_new_string(account.CustomerName));
		json_object_object_add(tmpObj,"NAVPrice",json_object_new_double(account.NAVPrice));
		json_object_object_add(tmpObj,"Amount",json_object_new_double(account.Amount));
		json_object_object_add(tmpObj,"OProfit",json_object_new_double(account.OpenProfit));
		json_object_object_add(tmpObj,"Exchange",json_object_new_double(account.ExchangeReserve));
		json_object_object_add(tmpObj,"Performance",json_object_new_double(account.PerformanceReserve));
		json_object_object_add(tmpObj,"Frozen",json_object_new_double(account.FrozenReserve));
		json_object_object_add(tmpObj,"RiskRate",json_object_new_double(account.RiskRate));
	}

	json_object_object_add(RspObj,"body",tmpObj);

	strData=json_object_to_json_string(RspObj);

	HandleQueryRes(TID_GET_ACCOUNTINFO,strData);

	return ret;
}

//add by liuchengzhu 20150602
int TradeClient::GetCommodityInfo(vector<CommodityInfo> &vCommodity)
{
	int ret = m_Interface->getCommodity(vCommodity);

	if(MSG_STATUS_UPDATED != ret){
		AC_INFO("[TE] GetCommoditiyInfo fail! ret: %d\n", ret);
		return ret;
	}

	int commodityNum = vCommodity.size();
	for(int i = 0; i < commodityNum; ++i) {
		CommodityInfo commodity = vCommodity[i];
		AC_INFO("[T] ID=%d\tName=%s\tRight=%lld\tAgreeUnit=%f\tCurrency=%d\n \
			   MinQCU=%f\tMinPriceUnit=%f\tFixedSpread=%f\tBuyPrice=%f\tSellPrice=%f\n \
			   HighPrice=%f\tLowPrice=%f\tQuoteTime=%lld\tClass=%d\tClassName=%s\n \
			   Mode=%d\tIsDisplay=%d\tGroupId=%d\tGroupName=%s\tWeightStep=%f\tWeightRadio=%f\n", \
			   commodity.CommodityID, commodity.CommodityName, commodity.CommodityRight, /*commodity.TradeRight,*/ commodity.AgreeUnit, commodity.Currency, \
			   commodity.MinQuoteChangeUnit, commodity.MinPriceUnit, commodity.FixedSpread, commodity.BuyPrice, commodity.SellPrice, \
			   commodity.HighPrice, commodity.LowPrice, commodity.QuoteTime, commodity.CommodityClass, commodity.CommodityClassName, \
			   commodity.CommodityMode, commodity.IsDisplay, commodity.CommodityGroupID, commodity.CommodityGroupName, commodity.WeightStep, commodity.WeightRadio);
	}
	return ret;
}

int TradeClient::GetOpenMarketOrderConf(const int CommodityID,  OpenMarketOrderConf &pConf)
{
	int ret = m_Interface->getOpenMarketOrderConf(CommodityID,pConf);
	if(0 != ret)
		return ret;
	AC_INFO("*********** OpenMarketOrderConf ************\n");
	AC_INFO("CommodityID: %d, MinQuantity: %6.3f, MaxQuantity: %6.3f, MinTotalWeight: %6.3f, MaxTotalWeight: %6.3f\n",pConf.CommodityID, pConf.MinQuantity, pConf.MaxQuantity, pConf.MinTotalWeight, pConf.MaxTotalWeight);
	AC_INFO("DefaultTradeRange: %6.3f, MinTradeRange: %6.3f, MaxTradeRange: %6.3f\n",pConf.DefaultTradeRange, pConf.MinTradeRange, pConf.MaxTradeRange);
	AC_INFO("AgreeUnit: %6.3f, CommodityMode: %s, WeightStep: %6.4f, WeightRadio: %6.3f\n",pConf.AgreeUnit, pConf.CommodityMode==NEW_COMMODITY?"新商品":"旧商品", pConf.WeightStep, pConf.WeightRadio);
	return ret;
}

int TradeClient::GetOpenLimitOrderConf(const int CommodityID, OpenLimitOrderConf &pConf)
{
	int ret = m_Interface->getOpenLimitOrderConf(CommodityID, pConf);
	if(0 != ret)
		return ret;
	AC_INFO("*********** OpenLimitOrderConf ************\n");
	AC_INFO("CommodityID: %d, MinQuantity: %6.3f, MaxQuantity: %6.3f, MinTotalWeight: %6.3f, MaxTotalWeight: %6.3f\n",pConf.CommodityID, pConf.MinQuantity, pConf.MaxQuantity, pConf.MinTotalWeight, pConf.MaxTotalWeight);
	AC_INFO("LimitSpread: %6.3f, FixSpread: %6.3f, TPSpread: %6.3f, SLSpread: %6.3f\n",pConf.LimitSpread, pConf.FixSpread, pConf.TPSpread, pConf.MinPriceUnit);
	AC_INFO("MinPriceUnit: %6.3f, AgreeUnit: %6.3f, CommodityMode: %s, WeightStep: %6.4f, WeightRadio: %6.3f\n",pConf.MinPriceUnit, pConf.AgreeUnit, pConf.CommodityMode==NEW_COMMODITY?"新商品":"旧商品", pConf.WeightStep, pConf.WeightRadio);
	return ret;

}

int TradeClient::GetCloseMarketOrderConf(const int CommodityID, CloseMarketOrderConf &pConf)
{
	int ret = m_Interface->getCloseMarketOrderConf(CommodityID, pConf);
	if(0 != ret)
		return ret;
	AC_INFO("*********** CloseMarketOrderConf ************\n");
	AC_INFO("CommodityID: %d, MinQuantity: %6.3f, MaxQuantity: %6.3f, MinTotalWeight: %6.3f, MaxTotalWeight: %6.3f\n",pConf.CommodityID, pConf.MinQuantity, pConf.MaxQuantity, pConf.MinTotalWeight, pConf.MaxTotalWeight);
	AC_INFO("DefaultTradeRange: %6.3f, MinTradeRange: %6.3f, MaxTradeRange: %6.3f\n",pConf.DefaultTradeRange, pConf.MinTradeRange, pConf.MaxTradeRange);
	AC_INFO("AgreeUnit: %6.3f, CommodityMode: %s, WeightStep: %6.4f, WeightRadio: %6.3f\n",pConf.AgreeUnit, pConf.CommodityMode==NEW_COMMODITY?"新商品":"旧商品", pConf.WeightStep, pConf.WeightRadio);
	return ret;
}

int TradeClient::GetLimitClosePositionConf(const int CommodityID, LimitClosePositionConf &pConf)
{
	int ret = m_Interface->getLimitClosePositionConf(CommodityID, pConf);
	if(0 != ret)
		return ret;
	AC_INFO("*********** LimitClosePositionConf ************\n");
	AC_INFO("CommodityID: %d\n",pConf.CommodityID);
	AC_INFO("FixSpread: %6.3f, TPSpread: %6.3f, SLSpread: %6.3f\n", pConf.FixedSpread, pConf.TPSpread, pConf.MinPriceUnit);
	AC_INFO("MinPriceUnit: %6.3f, AgreeUnit: %6.3f, CommodityMode: %s, WeightRadio: %6.3f\n",pConf.MinPriceUnit, pConf.AgreeUnit, pConf.CommodityMode==NEW_COMMODITY?"新商品":"旧商品", pConf.WeightRadio);
	return ret;
}
//end add by liuchengzhu 20150602


//市价单开仓
int TradeClient::OnOpenMarketOrder(OpenMarketOrderParam &Order)
{
	//Order.dbPrice = m_quote.BuyPrice;
	//add by liuchengzhu 20141217
	AC_INFO("[T] Order.dbPrice=%f\n",Order.dbPrice);
	//end add by liuchengzhu 20141217

	int ret = m_Interface->doOpenMarketOrder(Order);

	string msg;
	if(0!=ret)
		msg="OnOpenMarketOrder fail!";
	else
		msg="OnOpenMarketOrder success!";

	HandleRes(TID_OPENMARKETORDER,ret,msg);
		
	return ret;
}

//市价单平仓 
int  TradeClient::OnCloseMarketOrder(CloseMarketOrderParam& CloseOrder)
{
	//CloseOrder.dbPrice = m_quote.SellPrice;
	//add by liuchengzhu 20141217
	AC_INFO("[T] Order.dbPrice=%f\n",CloseOrder.dbPrice);
	//end add by liuchengzhu 20141217

	int ret = m_Interface->doCloseMarketOrder(CloseOrder);
	string msg;

	if(Order_Ret_Success != ret)
	{
		msg="OnCloseMarketOrder fail!";
	}
	else
	{
		msg="OnCloseMarketOrder success!";
	}
	HandleRes(TID_CLOSEMARKETORDER,ret,msg);
	return ret;
}


//限价单建仓
int  TradeClient::OnOpenLimitOrder(OpenLimitOrderParam& LimitOrder)
{
	int ret = m_Interface->doOpenLimitOrder(LimitOrder);
	string msg;

	if(Order_Ret_Success != ret)
	{
		msg="OnOpenLimitOrder fail!";
	}
	else
	{
		msg="OnOpenLimitOrder success!";
	}
	HandleRes(TID_OPENLIMITORDER,ret,msg);

	return ret;
}

//add by liuchengzhu 20141216

//限价单平仓
int TradeClient::OnLimitClosePosition(CloseLimitOrderParam& LimitClosePosition)
{
	int ret = m_Interface->doLimitClosePosition(LimitClosePosition);
	string msg;

	if(Order_Ret_Success != ret)
	{
		msg="OnLimitClosePosition fail!";
	}
	else
	{
		msg="OnLimitClosePosition success!";
	}
	HandleRes(TID_LIMITCLOSEPOSITION,ret,msg);

	return ret;

}
//end add by liuchengzhu 20141216

//add by liuchengzhu 20141218
//限价单撤销
int TradeClient::OnLimitRevoke(LimitRevokeParam& LimitRevoke)
{
	int ret = m_Interface->doLimitRevoke(LimitRevoke);
	string msg;

	if(Order_Ret_Success != ret)
	{
		msg="OnLimitRevoke fail!";
	}
	else
	{
		msg="OnLimitRevoke success!";
	}
	HandleRes(TID_LIMITREVOKE,ret,msg);

	return ret;
}

//批量平仓
int TradeClient::OnCloseMarketOrderMany(CloseMarketOrderManyParam& CloseMarketOrderMany)
{
	int ret = m_Interface->doCloseMarketOrderMany(CloseMarketOrderMany);
	string msg;
	if(Order_Ret_Success != ret)
	{
		msg="OnCloseMarketOrderMany fail!";
	}
	else
	{
		msg="OnCloseMarketOrderMany success!";
	}
	HandleRes(TID_CLOSEMARKETORDERMANY,ret,msg);

	return ret;
}

//出入金
int TradeClient::OnMoneyInOut(MoneyInOutParam& MoneyInOut)
{
	int ret = m_Interface->doMoneyInOut(MoneyInOut);
	if(Order_Ret_Success != ret)
	{
		AC_INFO("[TE] OnMoneyInOut fail! ret: %d\n",ret);
		return ret;
	}
	AC_INFO("[T]OnMoneyInOut sucess!\n");
	return ret;
}


//end add by liuchengzhu 20141218

//add by liuchengzhu 20141219
//查询银行资金
int TradeClient::OnMoneyQuery()
{
	int ret =m_Interface->doMoneyQuery();
	if(Order_Ret_Success != ret)
	{
		AC_INFO("[TE] OnMoneyQuery fail! ret: %d\n",ret);
		return ret;
	}
	AC_INFO("[T]OnMoneyQuery sucess!\n");
	return ret;
}
//end add by liuchengzhu 20141219


int TradeClient::OnHeartBeat()
{
	m_Interface->doHeartBeat();	
	return 0;
}
//add by liuchengzhu 20141210
/*
//SDK接口已经废弃
int TradeClient::GetMoneyQueryInfo()
{
	BourseMoneyInfo moneyInfo;

	int ret=MSG_STATUS_UPDATED;//m_Interface->getMoneyQueryInfo(moneyInfo);

	if(MSG_STATUS_UPDATED != ret){
		printf("[TE] GetMoneyQueryInfo fail! ret: %d\n",ret);
		return ret;
	}

	printf("[T]GetMoneyQueryInfo Amount=%f\tAmountAvailable=%f\tAmountFetchable=%f\n",moneyInfo.Amount,moneyInfo.AmountAvailable,moneyInfo.AmountFetchable);

	return ret;
}
*/
//end add by liuchengzhu 20141210
//add by liuchengzhu 20141211
int TradeClient::GetCommodity()
{
	vector<CommodityInfo> vCommodity;

	int ret=m_Interface->getCommodity(vCommodity);


	string msg;

	if(MSG_STATUS_UPDATED != ret)
		msg="GetCommodity fail!";
	else
		msg="GetCommodity success!";

	string strData;
	json_object *RspObj = json_object_new_object();
	if(NULL == RspObj || is_error(RspObj))
	{
		AC_ERROR("%s", "json_object_new_object failed!");
		return -1;
	}
	JsonObjGuard RspObjGuard(RspObj);

	json_object_object_add(RspObj,"ret",json_object_new_int(ret));
	json_object_object_add(RspObj,"msg",json_object_new_string(msg.c_str()));
	//add by liuchengzhu 20150518
	json_object_object_add(RspObj,"uname",json_object_new_string(LoginAcc.c_str()));
	//end add by liuchengzhu 20150518

	json_object *RsArrary = json_object_new_array();


	for(unsigned int i=0;i<vCommodity.size();i++)
	{
		CommodityInfo commodity=vCommodity[i];
		AC_INFO("GetCommodity CommodityID:%d\tCommodityName:%s\tFixedSpread:%f\n",commodity.CommodityID,commodity.CommodityName,commodity.FixedSpread);

		json_object *tmpObj = json_object_new_object();

		//modify by liuchengzhu 20150520
		char sID[256]={0};
		sprintf(sID,"%d",commodity.CommodityID);
		json_object_object_add(tmpObj,"ID",json_object_new_string(sID));
		//end modify by liuchengzhu 20150520

		json_object_object_add(tmpObj,"Name",json_object_new_string(commodity.CommodityName));

		char cRight[64];
		snprintf(cRight,sizeof(cRight),"%lld",commodity.CommodityRight);
		json_object_object_add(tmpObj,"Right",json_object_new_string(cRight));

		char cTradeRight[64];
		snprintf(cTradeRight,sizeof(cTradeRight),"%lld",commodity.TradeRight);
		json_object_object_add(tmpObj,"Right",json_object_new_string(cTradeRight));

		json_object_object_add(tmpObj,"AgreeUnit",json_object_new_double(commodity.AgreeUnit));
		json_object_object_add(tmpObj,"Currency",json_object_new_int(commodity.Currency));
		json_object_object_add(tmpObj,"MinQCU",json_object_new_double(commodity.MinQuoteChangeUnit));
		json_object_object_add(tmpObj,"MinPriceUnit",json_object_new_double(commodity.MinPriceUnit));
		json_object_object_add(tmpObj,"FixedSpread",json_object_new_double(commodity.FixedSpread));
		json_object_object_add(tmpObj,"BuyPrice",json_object_new_double(commodity.BuyPrice));
		json_object_object_add(tmpObj,"SellPrice",json_object_new_double(commodity.SellPrice));
		json_object_object_add(tmpObj,"HighPrice",json_object_new_double(commodity.HighPrice));
		json_object_object_add(tmpObj,"LowPrice",json_object_new_double(commodity.LowPrice));
		
		long lQuoteTime=0;
		TimeToMSecond(commodity.QuoteTime,lQuoteTime);
		json_object_object_add(tmpObj,"QuoteTime",json_object_new_int64(lQuoteTime));

		//add by liuchengzhu 20150525
		json_object_object_add(tmpObj,"Class",json_object_new_int(commodity.CommodityClass));
		json_object_object_add(tmpObj,"ClassName",json_object_new_string(commodity.CommodityClassName));
		json_object_object_add(tmpObj,"Mode",json_object_new_int(commodity.CommodityMode));
		json_object_object_add(tmpObj,"IsDisplay",json_object_new_int(commodity.IsDisplay));
		json_object_object_add(tmpObj,"GroupID",json_object_new_int(commodity.CommodityGroupID));
		json_object_object_add(tmpObj,"GroupName",json_object_new_string(commodity.CommodityGroupName));
		json_object_object_add(tmpObj,"WeightStep",json_object_new_double(commodity.WeightStep));
		json_object_object_add(tmpObj,"WeightRadio",json_object_new_double(commodity.WeightRadio));
		//end add by liuchengzhu 20150525

		//add by liuchengzhu 20150707 商品增添了 规格系数,  用于粤锭银等商品
		json_object_object_add(tmpObj,"SpecificationRate",json_object_new_double(commodity.SpecificationRate));
		//end add by liuchengzhu 20150707

		json_object_array_add(RsArrary,tmpObj);
	}
	json_object_object_add(RspObj,"body",RsArrary);

	strData=json_object_to_json_string(RspObj);

	HandleQueryRes(TID_GET_COMMODITY,strData);
	
	return ret;
}

//获取持仓单信息
int  TradeClient::GetPositionOrder()
{
	AC_INFO("GetPositionOrder begin\n");

	vector<HoldPositionInfo> vHoldPosition;
	
	int ret=m_Interface->getPositionOrder(vHoldPosition);
	
	string msg;

	if(MSG_STATUS_UPDATED != ret)
		msg="GetPositionOrder fail!";
	else
		msg="GetPositionOrder success!";

	string strData;
	json_object *RspObj = json_object_new_object();
	if(NULL == RspObj || is_error(RspObj))
	{
		AC_ERROR("%s", "json_object_new_object failed!");
		return -1;
	}
	JsonObjGuard RspObjGuard(RspObj);

	json_object_object_add(RspObj,"ret",json_object_new_int(ret));
	json_object_object_add(RspObj,"msg",json_object_new_string(msg.c_str()));
	//add by liuchengzhu 20150518
	json_object_object_add(RspObj,"uname",json_object_new_string(LoginAcc.c_str()));
	//end add by liuchengzhu 20150518

	json_object *RsArrary = json_object_new_array();


	for(unsigned int i=0;i<vHoldPosition.size();i++)
	{
		HoldPositionInfo holdposition=vHoldPosition[i];

		AC_INFO("GetPositionOrder CommodityID:%d\tHoldPositionID:%lld\tHoldPositionPrice:%f\tQuantity=%d\n",\
			holdposition.CommodityID,holdposition.HoldPositionID,holdposition.HoldPositionPrice,holdposition.Quantity);
		AC_INFO("ClosePrice:%f\tSLPrice:%f\tTPPrice:%f\tSLLimitOrderID:%lld\tTPLimitOrderID:%lld\n",\
			holdposition.ClosePrice,holdposition.SLPrice,holdposition.TPPrice,holdposition.SLLimitOrderID,holdposition.TPLimitOrderID);

		json_object *tmpObj = json_object_new_object();
		
		char cHPID[64];
		snprintf(cHPID,sizeof(cHPID),"%lld",holdposition.HoldPositionID);
		json_object_object_add(tmpObj,"HPID",json_object_new_string(cHPID));
		
		//modify by liuchengzhu 20150520
		char sID[256]={0};
		sprintf(sID,"%d",holdposition.CommodityID);
		json_object_object_add(tmpObj,"ID",json_object_new_string(sID));
		//end modify by liuchengzhu 20150520
		json_object_object_add(tmpObj,"Name",json_object_new_string(holdposition.CommodityName));
		json_object_object_add(tmpObj,"OType",json_object_new_int(holdposition.OpenType));
		json_object_object_add(tmpObj,"dir",json_object_new_int(holdposition.OpenDirector));
		json_object_object_add(tmpObj,"qty",json_object_new_int(holdposition.Quantity));
		json_object_object_add(tmpObj,"OPrice",json_object_new_double(holdposition.OpenPrice));
		json_object_object_add(tmpObj,"HPPrice",json_object_new_double(holdposition.HoldPositionPrice));
		json_object_object_add(tmpObj,"CPrice",json_object_new_double(holdposition.ClosePrice));

		char cSLID[64];
		snprintf(cSLID,sizeof(cSLID),"%lld",holdposition.SLLimitOrderID);
		json_object_object_add(tmpObj,"SLLimitOrderID",json_object_new_string(cSLID));

		json_object_object_add(tmpObj,"SLPrice",json_object_new_double(holdposition.SLPrice));

		char cTPID[64];
		snprintf(cTPID,sizeof(cTPID),"%lld",holdposition.TPLimitOrderID);
		json_object_object_add(tmpObj,"TPLimitOrderID",json_object_new_string(cTPID));

		json_object_object_add(tmpObj,"TPPrice",json_object_new_double(holdposition.TPPrice));
		json_object_object_add(tmpObj,"OProfit",json_object_new_double(holdposition.OpenProfit));
		json_object_object_add(tmpObj,"CAmount",json_object_new_double(holdposition.CommissionAmount));

		long lDate=0;
		TimeToMSecond(holdposition.OpenDate,lDate);
		json_object_object_add(tmpObj,"ODate",json_object_new_int64(lDate));

		json_object_object_add(tmpObj,"AgreeMargin",json_object_new_double(holdposition.AgreeMargin));
		json_object_object_add(tmpObj,"Freezemargin",json_object_new_double(holdposition.Freezemargin));
		json_object_object_add(tmpObj,"OverdueFindFund",json_object_new_double(holdposition.OverdueFindFund));
		//add by liuchengzhu 20150521
		json_object_object_add(tmpObj,"weight",json_object_new_double(holdposition.TotalWeight));
		//end add by liuchengzhu 20150521
		
		json_object_array_add(RsArrary,tmpObj);
	}
	json_object_object_add(RspObj,"body",RsArrary);

	strData=json_object_to_json_string(RspObj);

	HandleQueryRes(TID_GET_POSITIONORDER,strData);

	AC_INFO("GetPositionOrder end\n");

	return ret;
}

//获取限价单信息
int  TradeClient::GetLimitOrder()
{
	AC_INFO("GetLimitOrder begin\n");

	vector<LimitOrderInfo> vLimitOrder;

	int ret=m_Interface->getLimitOrder(vLimitOrder);

	string msg;

	if(MSG_STATUS_UPDATED != ret)
		msg="GetLimitOrder fail!";
	else
		msg="GetLimitOrder success!";

	string strData;
	json_object *RspObj = json_object_new_object();
	if(NULL == RspObj || is_error(RspObj))
	{
		AC_ERROR("%s", "json_object_new_object failed!");
		return -1;
	}
	JsonObjGuard RspObjGuard(RspObj);

	json_object_object_add(RspObj,"ret",json_object_new_int(ret));
	json_object_object_add(RspObj,"msg",json_object_new_string(msg.c_str()));
	//add by liuchengzhu 20150518
	json_object_object_add(RspObj,"uname",json_object_new_string(LoginAcc.c_str()));
	//end add by liuchengzhu 20150518

	json_object *RsArrary = json_object_new_array();

	for(unsigned int i=0;i<vLimitOrder.size();i++)
	{
		LimitOrderInfo limitorder=vLimitOrder[i];
		AC_INFO("GetLimitOrder LimitOrderID:%lld\tCommodityID:%d\tCreateDate:%lld\tOrderPrice:%f\tSLPrice:%f\tTPPrice:%f\n",\
			limitorder.LimitOrderID,limitorder.CommodityID,limitorder.CreateDate,limitorder.OrderPrice,limitorder.SLPrice,limitorder.TPPrice);

		json_object *tmpObj = json_object_new_object();

		char cOID[64];
		snprintf(cOID,sizeof(cOID),"%lld",limitorder.LimitOrderID);
		json_object_object_add(tmpObj,"OrderID",json_object_new_string(cOID));
		
		//modify by liuchengzhu 20150520
		char sID[256]={0};
		sprintf(sID,"%d",limitorder.CommodityID);
		json_object_object_add(tmpObj,"ID",json_object_new_string(sID));
		//end modify by liuchengzhu 20150520

		json_object_object_add(tmpObj,"Name",json_object_new_string(limitorder.CommodityName));
		json_object_object_add(tmpObj,"LimitType",json_object_new_int(limitorder.LimitType));
		json_object_object_add(tmpObj,"OrderType",json_object_new_int(limitorder.OrderType));
		json_object_object_add(tmpObj,"dir",json_object_new_int(limitorder.OpenDirector));
		json_object_object_add(tmpObj,"OrderPrice",json_object_new_double(limitorder.OrderPrice));
		json_object_object_add(tmpObj,"SLPrice",json_object_new_double(limitorder.SLPrice));
		json_object_object_add(tmpObj,"TPPrice",json_object_new_double(limitorder.TPPrice));
		json_object_object_add(tmpObj,"qty",json_object_new_int(limitorder.OpenQuantity));

		long lDate=0;
		TimeToMSecond(limitorder.CreateDate,lDate);
		json_object_object_add(tmpObj,"CreateDate",json_object_new_int64(lDate));

		char cEType[64];
		snprintf(cEType,sizeof(cEType),"%lld",limitorder.ExpireType);
		json_object_object_add(tmpObj,"EType",json_object_new_string(cEType));

		long lDate2=0;
		TimeToMSecond(limitorder.UpdateDate,lDate2);
		json_object_object_add(tmpObj,"UpdateDate",json_object_new_int64(lDate2));

		json_object_object_add(tmpObj,"FreeszMargin",json_object_new_double(limitorder.FreeszMargin));
		//add by liuchengzhu 20150521
		json_object_object_add(tmpObj,"weight",json_object_new_double(limitorder.TotalWeight));
		//end add by liuchengzhu 20150521
		
		json_object_array_add(RsArrary,tmpObj);

	}
	json_object_object_add(RspObj,"body",RsArrary);

	strData=json_object_to_json_string(RspObj);

	HandleQueryRes(TID_GET_LIMITORDER,strData);

	AC_INFO("GetLimitOrder end\n");

	return ret;
}
//end add by liuchengzhu 20141211

//add by liuchengzhu 20141212
//获取平仓信息
int  TradeClient::GetClosePosition()
{
	AC_INFO("GetClosePosition begin\n");

	vector<ClosePositionInfo> vClosePosition;

	int ret=m_Interface->getClosePosition(vClosePosition);

	string msg;

	if(MSG_STATUS_UPDATED != ret)
		msg="GetClosePosition fail!";
	else
		msg="GetClosePosition success!";

	string strData;
	json_object *RspObj = json_object_new_object();
	if(NULL == RspObj || is_error(RspObj))
	{
		AC_ERROR("%s", "json_object_new_object failed!");
		return -1;
	}
	JsonObjGuard RspObjGuard(RspObj);

	json_object_object_add(RspObj,"ret",json_object_new_int(ret));
	json_object_object_add(RspObj,"msg",json_object_new_string(msg.c_str()));
	//add by liuchengzhu 20150518
	json_object_object_add(RspObj,"uname",json_object_new_string(LoginAcc.c_str()));
	//end add by liuchengzhu 20150518

	json_object *RsArrary = json_object_new_array();


	for(unsigned int i=0;i<vClosePosition.size();i++)
	{
		ClosePositionInfo closeposition=vClosePosition[i];
		AC_INFO("[T] GetClosePosition ClosePositionID=%lld\tCommodityID=%d\tClosePrice=%f\tOpenPositionID=%lld\n",\
			closeposition.ClosePositionID,closeposition.CommodityID,closeposition.ClosePrice,closeposition.OpenPositionID);

		json_object *tmpObj = json_object_new_object();

		char cCPID[64];
		snprintf(cCPID,sizeof(cCPID),"%lld",closeposition.ClosePositionID);
		json_object_object_add(tmpObj,"CPID",json_object_new_string(cCPID));

		//modify by liuchengzhu 20150520
		char sID[256]={0};
		sprintf(sID,"%d",closeposition.CommodityID);
		json_object_object_add(tmpObj,"ID",json_object_new_string(sID));
		//end modify by liuchengzhu 20150520
		json_object_object_add(tmpObj,"Name",json_object_new_string(closeposition.CommodityName));
		json_object_object_add(tmpObj,"Cdir",json_object_new_int(closeposition.CloseDirector));
		json_object_object_add(tmpObj,"OPrice",json_object_new_double(closeposition.OpenPrice));
		json_object_object_add(tmpObj,"HPrice",json_object_new_double(closeposition.HoldPrice));
		json_object_object_add(tmpObj,"CPrice",json_object_new_double(closeposition.ClosePrice));
		json_object_object_add(tmpObj,"qty",json_object_new_int(closeposition.Quantity));

		char cOPID[64];
		snprintf(cOPID,sizeof(cOPID),"%lld",closeposition.OpenPositionID);
		json_object_object_add(tmpObj,"OPID",json_object_new_string(cOPID));
		
		json_object_object_add(tmpObj,"CAmount",json_object_new_double(closeposition.CommissionAmount));

		long lODate=0;
		TimeToMSecond(closeposition.OpenDate,lODate);
		json_object_object_add(tmpObj,"ODate",json_object_new_int64(lODate));

		long lCDate=0;
		TimeToMSecond(closeposition.CloseDate,lCDate);
		json_object_object_add(tmpObj,"CDate",json_object_new_int64(lCDate));

		json_object_object_add(tmpObj,"MemberID",json_object_new_int(closeposition.MemberID));
		json_object_object_add(tmpObj,"OType",json_object_new_int(closeposition.OpenType));
		json_object_object_add(tmpObj,"CType",json_object_new_int(closeposition.CloseType));
		//add by liuchengzhu 20150521
		json_object_object_add(tmpObj,"weight",json_object_new_double(closeposition.TotalWeight));
		//end add by liuchengzhu 20150521


		json_object_array_add(RsArrary,tmpObj);

	}
	json_object_object_add(RspObj,"body",RsArrary);

	strData=json_object_to_json_string(RspObj);

	HandleQueryRes(TID_GET_CLOSEPOSITION,strData);

	AC_INFO("GetClosePosition end\n");
	
	return ret;
}

//获取持仓单总量
int TradeClient::GetHoldPositionTotal()
{
	AC_INFO("GetHoldPositionTotal begin\n");
	vector<HoldPositionTotalInfo> vTotal;

	int ret=m_Interface->getHoldPositionTotal(vTotal);

	string msg;

	if(MSG_STATUS_UPDATED != ret)
		msg="GetHoldPositionTotal fail!";
	else
		msg="GetHoldPositionTotal success!";

	string strData;
	json_object *RspObj = json_object_new_object();
	if(NULL == RspObj || is_error(RspObj))
	{
		AC_ERROR("%s", "json_object_new_object failed!");
		return -1;
	}
	JsonObjGuard RspObjGuard(RspObj);

	json_object_object_add(RspObj,"ret",json_object_new_int(ret));
	json_object_object_add(RspObj,"msg",json_object_new_string(msg.c_str()));
	//add by liuchengzhu 20150518
	json_object_object_add(RspObj,"uname",json_object_new_string(LoginAcc.c_str()));
	//end add by liuchengzhu 20150518

	json_object *RsArrary = json_object_new_array();


	for(unsigned int i=0;i<vTotal.size();i++)
	{
		HoldPositionTotalInfo hpt=vTotal[i];
		AC_INFO("[T]GetHoldPositionTotal CommodityID=%d\tOpenDirector=%d\tHoldPriceTotal=%f\tOpenPriceTotal=%f\n",hpt.CommodityID,hpt.OpenDirector,hpt.HoldPriceTotal,hpt.OpenPriceTotal);

		json_object *tmpObj = json_object_new_object();

		//modify by liuchengzhu 20150520
		char sID[256]={0};
		sprintf(sID,"%d",hpt.CommodityID);
		json_object_object_add(tmpObj,"ID",json_object_new_string(sID));
		//end modify by liuchengzhu 20150520
		json_object_object_add(tmpObj,"Name",json_object_new_string(hpt.CommodityName));
		json_object_object_add(tmpObj,"dir",json_object_new_int(hpt.OpenDirector));
		json_object_object_add(tmpObj,"qty",json_object_new_int(hpt.Quantity));
		json_object_object_add(tmpObj,"OPTotal",json_object_new_double(hpt.OpenPriceTotal));
		json_object_object_add(tmpObj,"AvgOP",json_object_new_double(hpt.AvgOpenPrice));
		json_object_object_add(tmpObj,"HPTotal",json_object_new_double(hpt.HoldPriceTotal));
		json_object_object_add(tmpObj,"AvgHP",json_object_new_double(hpt.AvgHoldPrice));
		json_object_object_add(tmpObj,"CP",json_object_new_double(hpt.ClosePrice));
		json_object_object_add(tmpObj,"OProfit",json_object_new_double(hpt.OpenProfit));
		//add by liuchengzhu 20150521
		json_object_object_add(tmpObj,"weight",json_object_new_double(hpt.TotalWeight));
		//end add by liuchengzhu 20150521

		json_object_array_add(RsArrary,tmpObj);

	}
	json_object_object_add(RspObj,"body",RsArrary);

	strData=json_object_to_json_string(RspObj);

	HandleQueryRes(TID_GET_HOLDPOSITIONTOTAL,strData);



	AC_INFO("GetHoldPositionTotal end\n");

	return ret;
}
//end add by liuchengzhu 20141212

//add by liuchengzhu 20150123
//获取市场开休市状态 
int TradeClient::GetMarketStatus()
{
	int nMarketStatus=0;

	int ret=m_Interface->getMarketStatus(nMarketStatus);

	string msg;

	if(MSG_STATUS_UPDATED != ret)
		msg="GetMarketStatus fail!";
	else
		msg="GetMarketStatus success!";

	string strData;
	json_object *RspObj = json_object_new_object();
	if(NULL == RspObj || is_error(RspObj))
	{
		AC_ERROR("%s", "json_object_new_object failed!");
		return -1;
	}
	JsonObjGuard RspObjGuard(RspObj);

	json_object_object_add(RspObj,"ret",json_object_new_int(ret));
	json_object_object_add(RspObj,"msg",json_object_new_string(msg.c_str()));
	//add by liuchengzhu 20150518
	json_object_object_add(RspObj,"uname",json_object_new_string(LoginAcc.c_str()));
	//end add by liuchengzhu 20150518

	if(MSG_STATUS_UPDATED==ret)
	{
		json_object_object_add(RspObj,"Status",json_object_new_int(nMarketStatus));
	}


	strData=json_object_to_json_string(RspObj);

	HandleQueryRes(TID_GET_MARKETSTATUS,strData);

	return ret;
}
//根据持仓单ID，获取持仓单信息
int TradeClient::GetHoldPositionByID(string sID)
{
	long long nHoldPositionID=atoll(sID.c_str());
	HoldPositionInfo holdposition;
	
	int ret=m_Interface->getHoldPositionByID(nHoldPositionID,holdposition);

	string msg;

	if(MSG_STATUS_UPDATED != ret)
		msg="GetHoldPositionByID fail!";
	else
		msg="GetHoldPositionByID success!";

	string strData;
	json_object *RspObj = json_object_new_object();
	if(NULL == RspObj || is_error(RspObj))
	{
		AC_ERROR("%s", "json_object_new_object failed!");
		return -1;
	}
	JsonObjGuard RspObjGuard(RspObj);

	json_object_object_add(RspObj,"ret",json_object_new_int(ret));
	json_object_object_add(RspObj,"msg",json_object_new_string(msg.c_str()));
	//add by liuchengzhu 20150518
	json_object_object_add(RspObj,"uname",json_object_new_string(LoginAcc.c_str()));
	//end add by liuchengzhu 20150518

	json_object *tmpObj = json_object_new_object();

	if(MSG_STATUS_UPDATED==ret)
	{
		char cHPID[64];
		snprintf(cHPID,sizeof(cHPID),"%lld",holdposition.HoldPositionID);
		json_object_object_add(tmpObj,"HPID",json_object_new_string(cHPID));

		//modify by liuchengzhu 20150520
		char sID[256]={0};
		sprintf(sID,"%d",holdposition.CommodityID);
		json_object_object_add(tmpObj,"ID",json_object_new_string(sID));
		//end modify by liuchengzhu 20150520
		json_object_object_add(tmpObj,"Name",json_object_new_string(holdposition.CommodityName));
		json_object_object_add(tmpObj,"OType",json_object_new_int(holdposition.OpenType));
		json_object_object_add(tmpObj,"dir",json_object_new_int(holdposition.OpenDirector));
		json_object_object_add(tmpObj,"qty",json_object_new_int(holdposition.Quantity));
		json_object_object_add(tmpObj,"OPrice",json_object_new_double(holdposition.OpenPrice));
		json_object_object_add(tmpObj,"HPPrice",json_object_new_double(holdposition.HoldPositionPrice));
		json_object_object_add(tmpObj,"CPrice",json_object_new_double(holdposition.ClosePrice));

		char cSLID[64];
		snprintf(cSLID,sizeof(cSLID),"%lld",holdposition.SLLimitOrderID);
		json_object_object_add(tmpObj,"SLLimitOrderID",json_object_new_string(cSLID));

		json_object_object_add(tmpObj,"SLPrice",json_object_new_double(holdposition.SLPrice));

		char cTPID[64];
		snprintf(cTPID,sizeof(cTPID),"%lld",holdposition.TPLimitOrderID);
		json_object_object_add(tmpObj,"TPLimitOrderID",json_object_new_string(cTPID));

		json_object_object_add(tmpObj,"TPPrice",json_object_new_double(holdposition.TPPrice));
		json_object_object_add(tmpObj,"OProfit",json_object_new_double(holdposition.OpenProfit));
		json_object_object_add(tmpObj,"CAmount",json_object_new_double(holdposition.CommissionAmount));

		long lDate=0;
		TimeToMSecond(holdposition.OpenDate,lDate);
		json_object_object_add(tmpObj,"ODate",json_object_new_int64(lDate));

		json_object_object_add(tmpObj,"AgreeMargin",json_object_new_double(holdposition.AgreeMargin));
		json_object_object_add(tmpObj,"Freezemargin",json_object_new_double(holdposition.Freezemargin));
		json_object_object_add(tmpObj,"OverdueFindFund",json_object_new_double(holdposition.OverdueFindFund));
		//add by liuchengzhu 20150521
		json_object_object_add(tmpObj,"weight",json_object_new_double(holdposition.TotalWeight));
		//end add by liuchengzhu 20150521
	}

	json_object_object_add(RspObj,"body",tmpObj);

	strData=json_object_to_json_string(RspObj);

	HandleQueryRes(TID_GET_HOLDPOSITIONBYID,strData);

	return ret;		
}
//根据限价单ID，获取限价单信息
int TradeClient::GetLimitOrderByID(string sID)
{
	long long nLimitOrderID=atoll(sID.c_str());
	LimitOrderInfo limitorder;

	int ret=m_Interface->getLimitOrderByID(nLimitOrderID,limitorder);

	string msg;

	if(MSG_STATUS_UPDATED != ret)
		msg="GetLimitOrderByID fail!";
	else
		msg="GetLimitOrderByID success!";

	string strData;
	json_object *RspObj = json_object_new_object();
	if(NULL == RspObj || is_error(RspObj))
	{
		AC_ERROR("%s", "json_object_new_object failed!");
		return -1;
	}
	JsonObjGuard RspObjGuard(RspObj);

	json_object_object_add(RspObj,"ret",json_object_new_int(ret));
	json_object_object_add(RspObj,"msg",json_object_new_string(msg.c_str()));
	//add by liuchengzhu 20150518
	json_object_object_add(RspObj,"uname",json_object_new_string(LoginAcc.c_str()));
	//end add by liuchengzhu 20150518

	json_object *tmpObj = json_object_new_object();

	if(MSG_STATUS_UPDATED==ret)
	{
		char cOID[64];
		snprintf(cOID,sizeof(cOID),"%lld",limitorder.LimitOrderID);
		json_object_object_add(tmpObj,"OrderID",json_object_new_string(cOID));

		//modify by liuchengzhu 20150520
		char sID[256]={0};
		sprintf(sID,"%d",limitorder.CommodityID);
		json_object_object_add(tmpObj,"ID",json_object_new_string(sID));
		//end modify by liuchengzhu 20150520
		json_object_object_add(tmpObj,"Name",json_object_new_string(limitorder.CommodityName));
		json_object_object_add(tmpObj,"LimitType",json_object_new_int(limitorder.LimitType));
		json_object_object_add(tmpObj,"OrderType",json_object_new_int(limitorder.OrderType));
		json_object_object_add(tmpObj,"dir",json_object_new_int(limitorder.OpenDirector));
		json_object_object_add(tmpObj,"OrderPrice",json_object_new_double(limitorder.OrderPrice));
		json_object_object_add(tmpObj,"SLPrice",json_object_new_double(limitorder.SLPrice));
		json_object_object_add(tmpObj,"TPPrice",json_object_new_double(limitorder.TPPrice));
		json_object_object_add(tmpObj,"qty",json_object_new_int(limitorder.OpenQuantity));

		long lDate=0;
		TimeToMSecond(limitorder.CreateDate,lDate);
		json_object_object_add(tmpObj,"CreateDate",json_object_new_int64(lDate));

		char cEType[64];
		snprintf(cEType,sizeof(cEType),"%lld",limitorder.ExpireType);
		json_object_object_add(tmpObj,"EType",json_object_new_string(cEType));

		long lDate2=0;
		TimeToMSecond(limitorder.UpdateDate,lDate2);
		json_object_object_add(tmpObj,"UpdateDate",json_object_new_int64(lDate2));

		json_object_object_add(tmpObj,"FreeszMargin",json_object_new_double(limitorder.FreeszMargin));
		//add by liuchengzhu 20150521
		json_object_object_add(tmpObj,"weight",json_object_new_double(limitorder.TotalWeight));
		//end add by liuchengzhu 20150521
	}

	json_object_object_add(RspObj,"body",tmpObj);

	strData=json_object_to_json_string(RspObj);

	HandleQueryRes(TID_GET_LIMITORDERBYID,strData);
	
	return ret;
}
//根据平仓单ID，获取平仓信息
int TradeClient::GetClosePositionByID(string sID)
{
	long long nClosePositionID=atoll(sID.c_str());
	ClosePositionInfo closeposition;

	int ret=m_Interface->getClosePositionByID(nClosePositionID,closeposition);

	string msg;

	if(MSG_STATUS_UPDATED != ret)
		msg="GetClosePositionByID fail!";
	else
		msg="GetClosePositionByID success!";

	string strData;
	json_object *RspObj = json_object_new_object();
	if(NULL == RspObj || is_error(RspObj))
	{
		AC_ERROR("%s", "json_object_new_object failed!");
		return -1;
	}
	JsonObjGuard RspObjGuard(RspObj);

	json_object_object_add(RspObj,"ret",json_object_new_int(ret));
	json_object_object_add(RspObj,"msg",json_object_new_string(msg.c_str()));
	//add by liuchengzhu 20150518
	json_object_object_add(RspObj,"uname",json_object_new_string(LoginAcc.c_str()));
	//end add by liuchengzhu 20150518

	json_object *tmpObj = json_object_new_object();

	if(MSG_STATUS_UPDATED==ret)
	{
		char cCPID[64];
		snprintf(cCPID,sizeof(cCPID),"%lld",closeposition.ClosePositionID);
		json_object_object_add(tmpObj,"CPID",json_object_new_string(cCPID));

		//modify by liuchengzhu 20150520
		char sID[256]={0};
		sprintf(sID,"%d",closeposition.CommodityID);
		json_object_object_add(tmpObj,"ID",json_object_new_string(sID));
		//end modify by liuchengzhu 20150520
		json_object_object_add(tmpObj,"Name",json_object_new_string(closeposition.CommodityName));
		json_object_object_add(tmpObj,"Cdir",json_object_new_int(closeposition.CloseDirector));
		json_object_object_add(tmpObj,"OPrice",json_object_new_double(closeposition.OpenPrice));
		json_object_object_add(tmpObj,"HPrice",json_object_new_double(closeposition.HoldPrice));
		json_object_object_add(tmpObj,"CPrice",json_object_new_double(closeposition.ClosePrice));
		json_object_object_add(tmpObj,"qty",json_object_new_int(closeposition.Quantity));

		char cOPID[64];
		snprintf(cOPID,sizeof(cOPID),"%lld",closeposition.OpenPositionID);
		json_object_object_add(tmpObj,"OPID",json_object_new_string(cOPID));

		json_object_object_add(tmpObj,"CAmount",json_object_new_double(closeposition.CommissionAmount));

		long lODate=0;
		TimeToMSecond(closeposition.OpenDate,lODate);
		json_object_object_add(tmpObj,"ODate",json_object_new_int64(lODate));

		long lCDate=0;
		TimeToMSecond(closeposition.CloseDate,lCDate);
		json_object_object_add(tmpObj,"CDate",json_object_new_int64(lCDate));

		json_object_object_add(tmpObj,"MemberID",json_object_new_int(closeposition.MemberID));
		json_object_object_add(tmpObj,"OType",json_object_new_int(closeposition.OpenType));
		json_object_object_add(tmpObj,"CType",json_object_new_int(closeposition.CloseType));
		//add by liuchengzhu 20150521
		json_object_object_add(tmpObj,"weight",json_object_new_double(closeposition.TotalWeight));
		//end add by liuchengzhu 20150521
	}

	json_object_object_add(RspObj,"body",tmpObj);
	
	strData=json_object_to_json_string(RspObj);

	HandleQueryRes(TID_GET_CLOSEPOSITIONBYID,strData);

	return ret;
}
//根据商品ID，获取持仓汇总信息
int TradeClient::GetHoldPositionTotalByCommodityID(int ID,int dir)
{
	HoldPositionTotalInfo hpt;

	int ret=m_Interface->getHoldPositionTotalByCommodityID(ID,dir,hpt);

	string msg;

	if(MSG_STATUS_UPDATED != ret)
		msg="GetHoldPositionTotalByCommodityID fail!";
	else
		msg="GetHoldPositionTotalByCommodityID success!";

	string strData;
	json_object *RspObj = json_object_new_object();
	if(NULL == RspObj || is_error(RspObj))
	{
		AC_ERROR("%s", "json_object_new_object failed!");
		return -1;
	}
	JsonObjGuard RspObjGuard(RspObj);

	json_object_object_add(RspObj,"ret",json_object_new_int(ret));
	json_object_object_add(RspObj,"msg",json_object_new_string(msg.c_str()));
	//add by liuchengzhu 20150518
	json_object_object_add(RspObj,"uname",json_object_new_string(LoginAcc.c_str()));
	//end add by liuchengzhu 20150518

	json_object *tmpObj = json_object_new_object();

	if(MSG_STATUS_UPDATED==ret)
	{
		//modify by liuchengzhu 20150520
		char sID[256]={0};
		sprintf(sID,"%d",hpt.CommodityID);
		json_object_object_add(tmpObj,"ID",json_object_new_string(sID));
		//end modify by liuchengzhu 20150520
		json_object_object_add(tmpObj,"Name",json_object_new_string(hpt.CommodityName));
		json_object_object_add(tmpObj,"dir",json_object_new_int(hpt.OpenDirector));
		json_object_object_add(tmpObj,"qty",json_object_new_int(hpt.Quantity));
		json_object_object_add(tmpObj,"OPTotal",json_object_new_double(hpt.OpenPriceTotal));
		json_object_object_add(tmpObj,"AvgOP",json_object_new_double(hpt.AvgOpenPrice));
		json_object_object_add(tmpObj,"HPTotal",json_object_new_double(hpt.HoldPriceTotal));
		json_object_object_add(tmpObj,"AvgHP",json_object_new_double(hpt.AvgHoldPrice));
		json_object_object_add(tmpObj,"CP",json_object_new_double(hpt.ClosePrice));
		json_object_object_add(tmpObj,"OProfit",json_object_new_double(hpt.OpenProfit));
		//add by liuchengzhu 20150521
		json_object_object_add(tmpObj,"weight",json_object_new_double(hpt.TotalWeight));
		//end add by liuchengzhu 20150521
	}

	json_object_object_add(RspObj,"body",tmpObj);

	strData=json_object_to_json_string(RspObj);

	HandleQueryRes(TID_GET_HOLDPOSITIONTOTALBYID,strData);

	return ret;
}

//end add by liuchengzhu 20150123



//add by liuchengzhu 20150104
int  TradeClient::HandleReq(string sreq)
{
	NtPkgHead *phead=(NtPkgHead *)sreq.c_str();
	unsigned short wCmd=ntohs(phead->wCmd);
	
	string strData=sreq.substr(sizeof(NtPkgHead),sreq.length()-sizeof(NtPkgHead));
	AC_INFO("strData:%s",strData.c_str());

	json_object* pReqBody = json_tokener_parse(strData.c_str());

	string sloginacc="";
	string stoken="";

	string sID="";
	int ID;
	int dir;



	switch(wCmd)
	{
	case 0:
		OnHeartBeat();
		break;
	case TID_GETORDERCONFIG:  //获取下单配置
		GetOrderConfig();
		break;
	case TID_OPENMARKETORDER:   //市价建仓
		OpenMarketOrderParam mOrder;
		//modify by liuchengzhu 20150520
		GetJsonField(pReqBody,"ID",sID);
		mOrder.nCommodityID=atoi(sID.c_str());
		//end modify by liuchengzhu 20150520
		GetJsonField(pReqBody,"dir",mOrder.nOpenDirector);
		GetJsonField(pReqBody,"price",mOrder.dbPrice);
		GetJsonField(pReqBody,"qty",mOrder.nQuantity);
		GetJsonField(pReqBody,"pt",mOrder.dbTradeRange);
		//add by liuchengzhu 20150518
		GetJsonField(pReqBody,"weight",mOrder.dbWeight);
		//end add by liuchengzhu 20150518
		
		OnOpenMarketOrder(mOrder);
		break;
	case TID_CLOSEMARKETORDER:    //市价平仓
		CloseMarketOrderParam mCloseOrder;
		GetJsonField(pReqBody,"HPID",sID);
		mCloseOrder.nHoldPositionID=atoll(sID.c_str());
		//modify by liuchengzhu 20150520
		GetJsonField(pReqBody,"ID",sID);
		mCloseOrder.nCommodityID=atoi(sID.c_str());
		//end modify by liuchengzhu 20150520
		GetJsonField(pReqBody,"qty",mCloseOrder.nQuantity);
		GetJsonField(pReqBody,"pt",mCloseOrder.nTradeRange);
		GetJsonField(pReqBody,"price",mCloseOrder.dbPrice);
		//add by liuchengzhu 20150518
		GetJsonField(pReqBody,"weight",mCloseOrder.dbWeight);
		//end add by liuchengzhu 20150518

		OnCloseMarketOrder(mCloseOrder);
		break;
	case TID_CLOSEMARKETORDERMANY:     //市价批量平仓
		CloseMarketOrderManyParam mCloseMarketOrderMany;
		//modify by liuchengzhu 20150520
		GetJsonField(pReqBody,"ID",sID);
		mCloseMarketOrderMany.nCommodityID=atoi(sID.c_str());
		//end modify by liuchengzhu 20150520
		GetJsonField(pReqBody,"qty",mCloseMarketOrderMany.nQuantity);
		GetJsonField(pReqBody,"pt",mCloseMarketOrderMany.nTradeRange);
		GetJsonField(pReqBody,"price",mCloseMarketOrderMany.dbPrice);
		GetJsonField(pReqBody,"dir",mCloseMarketOrderMany.nCloseDirector);
		//add by liuchengzhu 20150518
		GetJsonField(pReqBody,"weight",mCloseMarketOrderMany.dbWeight);
		//end add by liuchengzhu 20150518

		OnCloseMarketOrderMany(mCloseMarketOrderMany);
		break;
	case TID_OPENLIMITORDER:
		OpenLimitOrderParam   mLimitOrder;
		//modify by liuchengzhu 20150520
		GetJsonField(pReqBody,"ID",sID);
		mLimitOrder.nCommodityID=atoi(sID.c_str());
		//end modify by liuchengzhu 20150520
		GetJsonField(pReqBody,"EType",mLimitOrder.nExpireType);
		GetJsonField(pReqBody,"dir",mLimitOrder.nOpenDirector);
		GetJsonField(pReqBody,"qty",mLimitOrder.nQuantity);
		GetJsonField(pReqBody,"price",mLimitOrder.dbOrderPrice);
		GetJsonField(pReqBody,"TPPrice",mLimitOrder.dbTPPrice);
		GetJsonField(pReqBody,"SLPrice",mLimitOrder.dbSLPrice);
		//add by liuchengzhu 20150518
		GetJsonField(pReqBody,"weight",mLimitOrder.dbWeight);
		//end add by liuchengzhu 20150518

		OnOpenLimitOrder(mLimitOrder);
		break;
	case TID_LIMITCLOSEPOSITION:
		CloseLimitOrderParam mLimitClosePosition;
		//modify by liuchengzhu 20150520
		GetJsonField(pReqBody,"ID",sID);
		mLimitClosePosition.nCommodityID=atoi(sID.c_str());
		//end modify by liuchengzhu 20150520
		GetJsonField(pReqBody,"price",mLimitClosePosition.dbClosePrice);
		GetJsonField(pReqBody,"EType",mLimitClosePosition.nExpireType);
		GetJsonField(pReqBody,"HPID",sID);
		mLimitClosePosition.nHoldPositionID=atoll(sID.c_str());
		//comment by liuchengzhu 20150515
		//GetJsonField(pReqBody,"qty",mLimitClosePosition.nQuantity);
		//end comment by liuchengzhu 20150515
		GetJsonField(pReqBody,"SLPrice",mLimitClosePosition.dbSLPrice);
		GetJsonField(pReqBody,"TPPrice",mLimitClosePosition.dbTPPrice);

		OnLimitClosePosition(mLimitClosePosition);
		break;
	case TID_LIMITREVOKE:
		LimitRevokeParam mLimitRevoke;
		GetJsonField(pReqBody,"OrderID",sID);
		mLimitRevoke.nLimitOrderID=atoll(sID.c_str());
		//modify by liuchengzhu 20150520
		GetJsonField(pReqBody,"ID",sID);
		mLimitRevoke.nCommodityID=atoi(sID.c_str());
		//end modify by liuchengzhu 20150520
		GetJsonField(pReqBody,"Type",mLimitRevoke.nLimitType);

		OnLimitRevoke(mLimitRevoke);
		break;

	case TID_GET_ACCOUNTINFO:
		GetAccountInfo();
		break;
    
	case TID_MONEY_QUERY:
		OnMoneyQuery();
		break;
    
	case TID_GET_COMMODITY:
		GetCommodity();
		break;

	case TID_GET_POSITIONORDER:
		GetPositionOrder();
		break;
    
	case TID_GET_LIMITORDER:
		GetLimitOrder();
		break;
	
	case TID_GET_CLOSEPOSITION:
		GetClosePosition();
		break;
	
	case TID_GET_HOLDPOSITIONTOTAL:
		GetHoldPositionTotal();
		break;

	case TID_GET_MARKETSTATUS:
		GetMarketStatus();
		break;

	case TID_GET_HOLDPOSITIONBYID:
		GetJsonField(pReqBody,"HPID",sID);
		GetHoldPositionByID(sID);
		break;

	case TID_GET_LIMITORDERBYID:
		GetJsonField(pReqBody,"OrderID",sID);
		GetLimitOrderByID(sID);
		break;

	case TID_GET_CLOSEPOSITIONBYID:
		GetJsonField(pReqBody,"CPID",sID);
		GetClosePositionByID(sID);
		break;

	case TID_GET_HOLDPOSITIONTOTALBYID:
		GetJsonField(pReqBody,"ID",sID);
		ID=atoi(sID.c_str());
		GetJsonField(pReqBody,"dir",dir);
		GetHoldPositionTotalByCommodityID(ID,dir);
		break;

	case TID_LOGIN:
		GetJsonField(pReqBody,"loginacc",sloginacc);
		GetJsonField(pReqBody,"token",stoken);

		Login(sloginacc,stoken);
		break;
	case TID_DISCONNECT:
		DisConnect();
		break;
	default:
		AC_ERROR("UnKnown wCmd %d",wCmd);
	}

	//记录除心跳外的最新请求时间
	if(wCmd!=0)
	{
		tRequest=time(NULL);
	}

	return 0;
}
//end add by liuchengzhu 20150104

//add by liuchengzhu 20150112
int TradeClient::HandleRes(unsigned short cmd,int ret,string& msg)
{
	string strData;
	json_object *RspObj = json_object_new_object();
	if(NULL == RspObj || is_error(RspObj))
	{
		AC_ERROR("%s", "json_object_new_object failed!");
		return -1;
	}
	JsonObjGuard RspObjGuard(RspObj);

	json_object_object_add(RspObj, "ret", json_object_new_int(ret));

	json_object_object_add(RspObj, "msg", json_object_new_string(msg.c_str()));

	//add by liuchengzhu 20150518
	json_object_object_add(RspObj,"uname",json_object_new_string(LoginAcc.c_str()));
	//end add by liuchengzhu 20150518


	strData=json_object_to_json_string(RspObj);

	string sRes;
	NtPkgHead mHead;
	memset((void *)&mHead,0,sizeof(NtPkgHead));
	mHead.bStartFlag=0xff;
	mHead.dwSID=htonl(uid);
	mHead.wCmd=htons(cmd);
	mHead.wLen=htons(sizeof(NtPkgHead)+strData.length());

	sRes.append((const char *)&mHead,sizeof(NtPkgHead));
	sRes.append(strData.c_str(),strData.length());

	res_q.Push(sRes);

	return 0;
}
//end add by liuchengzhu 20150112

//add by liuchengzhu 20150114
int TradeClient::HandleRes(unsigned short cmd,string& strData)
{
	string sRes;
	NtPkgHead mHead;
	memset((void *)&mHead,0,sizeof(NtPkgHead));
	mHead.bStartFlag=0xff;
	mHead.dwSID=htonl(uid);
	mHead.wCmd=htons(cmd);
	mHead.wLen=htons(sizeof(NtPkgHead)+strData.length());

	sRes.append((const char *)&mHead,sizeof(NtPkgHead));
	sRes.append(strData.c_str(),strData.length());

	res_q.Push(sRes);

	return 0;
}

//end add by liuchengzhu 20150114

//add by liuchengzhu 20150121
int TradeClient::HandleQueryRes(unsigned short cmd,string& strData)
{
	const unsigned int MaxPackSize=8000;
	unsigned int packcount=0;
	packcount=strData.length()/MaxPackSize;
	if((strData.length()%MaxPackSize)!=0)
		packcount=packcount+1;

	string sRes;
	NtPkgHead mHead;
	memset((void *)&mHead,0,sizeof(NtPkgHead));
	mHead.bStartFlag=0xff;
	mHead.dwSID=htonl(uid);
	mHead.wCmd=htons(cmd);
	if(packcount>1)
		mHead.bFrag=1;
	mHead.wTotal=htons(packcount);


	string::iterator itor;
	unsigned int iBufLen = 0;
	int iIndex=0;

	while(!strData.empty())
	{
		itor = strData.begin();

		iBufLen=strData.length();
		if(iBufLen>MaxPackSize)
			iBufLen=MaxPackSize;

		mHead.wLen=htons(sizeof(NtPkgHead)+iBufLen);
		mHead.wCurSeq=htons(iIndex);
		iIndex++;

		sRes.clear();
		sRes.append((const char *)&mHead,sizeof(NtPkgHead));
		sRes.append(strData.c_str(),iBufLen);

		res_q.Push(sRes);

		strData.erase(itor, itor + iBufLen);

	}

	return 0;
}

//end add by liuchengzhu 20150121


