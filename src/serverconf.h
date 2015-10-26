#pragma once
#include <string>
#include <vector>
#include <set>
#include <map>
#include <string.h>
using namespace std;
class TiXmlElement;

struct TimePara
{
	int sleeptime;
	int stoptime;
	int outtime;
};

//add by liuchengzhu 20150708
struct RedisPara
{
	char m_SvrIp[256];
	int m_port;
	int m_expiretime;	
};
//end add by liuchengzhu 20150708

struct BizPara
{
	string memberno;
};

struct ExchangeConnInfo
{
	char m_SvrIp[256];
	int m_port;
};

struct MonitorConnInfo
{
	char m_SvrIp[256];
	int m_port;
};

struct ASyncConnInfo
{
	char m_SvrIp[256];
	int m_port;
	int m_ReadTimeOut;
	int m_ASynFuncNo;
};

struct ReqRouteInfo
{
	int FuncNo;
	int BranchNo;
};

class ServerConf
{
private:
		ServerConf(){}
		~ServerConf(){}
public:
		static ServerConf* Instance()
		{
			static ServerConf sconf;
			return &sconf;
		}
	
		int Read(const char* filename);
		int Readbasiccfg(TiXmlElement *root);
		int ReadTimePara(TiXmlElement *root);
		//add by liuchengzhu 20150707
		int ReadRedisPara(TiXmlElement *root);
		//end add by liuchengzhu 20150707
		int ReadBizPara(TiXmlElement *root);
		int ReadExchangeConnInfo(TiXmlElement *root);
		int ReadMonitorConnInfo(TiXmlElement *root);
		int ReadASyncConnInfo(TiXmlElement *root);
		int ReadOrderRouteInfo(TiXmlElement *root);
		int ReadQueryRouteInfo(TiXmlElement *root);

public:

	void stringsplit(const char *str,const char seps[],vector<string>& stringvector);
	
	//basic
	string m_cfgfilename;
	int  m_groupno;
	char m_logdir[256];
	char m_logprefix[256];
	char m_queryhost[256];
	int m_queryport;

	TimePara m_TimePara;

	//add by liuchengzhu 20150708
	RedisPara m_RedisPara;
	//end add by liuchengzhu 20150708

	BizPara  m_BizPara;

	ExchangeConnInfo m_ExchangeConnInfo;

	MonitorConnInfo  m_MonitorConnInfo;

	ASyncConnInfo m_ASyncConnInfo;

	ReqRouteInfo m_OrderRoute;
	ReqRouteInfo m_QueryRoute;
};

