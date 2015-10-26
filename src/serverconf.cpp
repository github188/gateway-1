#include "serverconf.h"
#include "yt/xml/tinyxml.h"
#include "yt/log/log.h"

using namespace std;
using namespace yt;

int ServerConf::Read(const char* filename)
{
	TiXmlDocument doc(filename);
	bool ret = doc.LoadFile();
	if (!ret)
	{
		AC_ERROR("load file %s error!",filename);
		return -1;
	}
	m_cfgfilename = filename;
	TiXmlElement *root = doc.FirstChildElement("config");
	if(!root)
	{
		AC_ERROR("config not exists!");
		return -1;
	}
	
	if(-1 == Readbasiccfg(root))
		return -1;
	if(-1 == ReadTimePara(root))
		return -1;
	if(-1 == ReadRedisPara(root))
		return -1;
	if(-1 == ReadBizPara(root))
		return -1;
	if(-1 == ReadExchangeConnInfo(root))
		return -1;
	if(-1 == ReadMonitorConnInfo(root))
		return -1;
	if(-1 == ReadASyncConnInfo(root))
		return -1;
	if(-1 == ReadOrderRouteInfo(root))
		return -1;
	if(-1 == ReadQueryRouteInfo(root))
		return -1;

	return 0;
}

void ServerConf::stringsplit(const char *str,const char seps[],vector<string>& stringvector)
{
	string strtemp(str);
	char *token;
	char *mybuf = NULL;
	token = strtok_r(&strtemp[0],seps,&mybuf);
	while(token != NULL)
	{
		stringvector.push_back(token);
		token = strtok_r(NULL,seps,&mybuf);
	}
}

int ServerConf::Readbasiccfg(TiXmlElement *root)
{
	TiXmlElement *servercfg = root->FirstChildElement("basic");	
	if(!servercfg)
	{
		AC_ERROR("basic not exists!");
		return -1;	
	}
	const char* str = servercfg->Attribute("groupno", &m_groupno);	
	if(!str)	{		
		AC_ERROR("read groupno error");		
		return -1;	
	}	
	AC_INFO("groupno = %d",m_groupno);

	str = servercfg->Attribute("logdir");
	if(!str)	
	{		
		AC_ERROR("read logdir error");		
		return -1;	
	}	
	strncpy(m_logdir,str,sizeof(m_logdir));		
	AC_INFO("logdir = %s",m_logdir);

	str = servercfg->Attribute("logprefix");	
	if(!str)	
	{		
		AC_ERROR("read logprefix error");		
		return -1;	
	}	
	strncpy(m_logprefix,str,sizeof(m_logprefix));	
	AC_INFO("logprefix = %s",m_logprefix);	

	str = servercfg->Attribute("queryhost");	
	if(!str)	{		
		AC_ERROR("read queryhost error");		
		return -1;	
	}	
	strncpy(m_queryhost,str,sizeof(m_queryhost));
	AC_INFO("queryhost = %s",m_queryhost);	

	str = servercfg->Attribute("queryport", &m_queryport);	
	if(!str)	{		
		AC_ERROR("read queryport error");		
		return -1;	
	}	
	AC_INFO("queryport = %d",m_queryport);	

	int runlog;	
	str = servercfg->Attribute("runlog",&runlog);	
	if(str)	
	{		
		AC_INFO("runlog = %d",runlog);		
		if(runlog == 1)
			WriteRunLog();	
	}

	return 0;
}

int ServerConf::ReadTimePara(TiXmlElement *root)
{
	TiXmlElement *servercfg = root->FirstChildElement("timepara");
	if(!servercfg)		
	{
		AC_ERROR("timepara not exists!");
		return -1;	
	}
	const char* str=servercfg->Attribute("sleeptime", &m_TimePara.sleeptime);	
	if(!str)
	{
		AC_ERROR("read <timepara> sleeptime error");
		return -1;	
	}
	AC_INFO("<timepara> sleeptime = %d",m_TimePara.sleeptime);

	str=servercfg->Attribute("stoptime", &m_TimePara.stoptime);	
	if(!str)
	{
		AC_ERROR("read <timepara> stoptime error");
		return -1;	
	}
	AC_INFO("<timepara> stoptime = %d",m_TimePara.stoptime);

	str=servercfg->Attribute("outtime", &m_TimePara.outtime);	
	if(!str)
	{
		AC_ERROR("read <timepara> outtime error");
		return -1;	
	}
	AC_INFO("<timepara> outtime = %d",m_TimePara.outtime);

	return 0;
}

int ServerConf::ReadRedisPara(TiXmlElement *root)
{
	TiXmlElement *servercfg = root->FirstChildElement("redis");
	if(!servercfg)		
	{
		AC_ERROR("redis not exists!");
		return -1;	
	}

	const char* str = servercfg->Attribute("commip");		
	if(!str)
	{
		AC_ERROR("read <redis> commip error");
		return -1;
	}
	strncpy(m_RedisPara.m_SvrIp, str, sizeof(m_RedisPara.m_SvrIp));		
	AC_INFO("<redis> commip = %s", m_RedisPara.m_SvrIp);

	str = servercfg->Attribute("commport", &m_RedisPara.m_port);	
	if(!str)
	{
		AC_ERROR("read <redis> commport error");
		return -1;	
	}
	AC_INFO("<redis> port = %d",m_RedisPara.m_port);

	str = servercfg->Attribute("expiretime", &m_RedisPara.m_expiretime);	
	if(!str)
	{
		AC_ERROR("read <redis> expiretime error");
		return -1;	
	}
	AC_INFO("<redis> expiretime = %d",m_RedisPara.m_expiretime);



	return 0;
}

int ServerConf::ReadBizPara(TiXmlElement *root)
{
	TiXmlElement *servercfg = root->FirstChildElement("bizpara");
	if(!servercfg)		
	{
		AC_ERROR("bizpara not exists!");
		return -1;	
	}

	const char* str = servercfg->Attribute("memberno");		
	if(!str)
	{
		AC_ERROR("read <bizpara> memberno error");
		return -1;
	}
	m_BizPara.memberno=str;		
	AC_INFO("<bizpara> memberno = %s", m_BizPara.memberno.c_str());


	return 0;
}

int ServerConf::ReadExchangeConnInfo(TiXmlElement *root)
{
	TiXmlElement *servercfg = root->FirstChildElement("exchange");
	if(!servercfg)		
	{
		AC_ERROR("exchange not exists!");
		return -1;	
	}

	const char* str = servercfg->Attribute("commip");		
	if(!str)
	{
		AC_ERROR("read <exchange> commip error");
		return -1;
	}
	strncpy(m_ExchangeConnInfo.m_SvrIp, str, sizeof(m_ExchangeConnInfo.m_SvrIp));		
	AC_INFO("<exchange> commip = %s", m_ExchangeConnInfo.m_SvrIp);

	str = servercfg->Attribute("commport", &m_ExchangeConnInfo.m_port);	
	if(!str)
	{
		AC_ERROR("read <exchange> commport error");
		return -1;	
	}
	AC_INFO("<exchange> port = %d",m_ExchangeConnInfo.m_port);

	return 0;
}

int ServerConf::ReadMonitorConnInfo(TiXmlElement *root)
{
	TiXmlElement *servercfg = root->FirstChildElement("monitor");
	if(!servercfg)		
	{
		AC_ERROR("monitor not exists!");
		return -1;	
	}

	const char* str = servercfg->Attribute("commip");		
	if(!str)
	{
		AC_ERROR("read <monitor> commip error");
		return -1;
	}
	strncpy(m_MonitorConnInfo.m_SvrIp, str, sizeof(m_MonitorConnInfo.m_SvrIp));		
	AC_INFO("<monitor> commip = %s", m_MonitorConnInfo.m_SvrIp);

	str = servercfg->Attribute("commport", &m_MonitorConnInfo.m_port);	
	if(!str)
	{
		AC_ERROR("read <monitor> commport error");
		return -1;	
	}
	AC_INFO("<monitor> port = %d",m_MonitorConnInfo.m_port);

	return 0;
}

int ServerConf::ReadASyncConnInfo(TiXmlElement *root)
{
	TiXmlElement *servercfg = root->FirstChildElement("asynconn");
	if(!servercfg)		
	{
		AC_ERROR("synconn not exists!");
		return -1;	
	}

	const char* str = servercfg->Attribute("commip");		
	if(!str)
	{
		AC_ERROR("read <asynconn> commip error");
		return -1;
	}
	strncpy(m_ASyncConnInfo.m_SvrIp, str, sizeof(m_ASyncConnInfo.m_SvrIp));		
	AC_INFO("<asynconn> commip = %s", m_ASyncConnInfo.m_SvrIp);

	str = servercfg->Attribute("commport", &m_ASyncConnInfo.m_port);	
	if(!str)
	{
		AC_ERROR("read <asynconn> commport error");
		return -1;	
	}
	AC_INFO("<asynconn> port = %d",m_ASyncConnInfo.m_port);

	str = servercfg->Attribute("readtimeout", &m_ASyncConnInfo.m_ReadTimeOut);	
	if(!str)
	{
		AC_ERROR("read <asynconn> readtimeout error");
		return -1;	
	}
	AC_INFO("<asynconn> readtimeout = %d", m_ASyncConnInfo.m_ReadTimeOut);

	str = servercfg->Attribute("asynfuncno", &m_ASyncConnInfo.m_ASynFuncNo);	
	if(!str)
	{
		AC_ERROR("read <asynconn> asynfuncno error");
		return -1;	
	}
	AC_INFO("<asynconn> asynfuncno = %d", m_ASyncConnInfo.m_ASynFuncNo);

	return 0;
}

int ServerConf::ReadOrderRouteInfo(TiXmlElement *root)
{
	TiXmlElement *servercfg = root->FirstChildElement("orderroute");
	if(!servercfg)		
	{
		AC_ERROR("orderroute not exists!");
		return -1;	
	}

	const char* str = servercfg->Attribute("funcno", &m_OrderRoute.FuncNo);		
	if(!str)
	{
		AC_ERROR("read <orderroute> funcno error");
		return -1;
	}
	AC_INFO("<orderroute> funcno = %d", m_OrderRoute.FuncNo);

	str = servercfg->Attribute("branchno", &m_OrderRoute.BranchNo);		
	if(!str)
	{
		AC_ERROR("read <orderroute> branchno error");
		return -1;
	}
	AC_INFO("<orderroute> branchno = %d", m_OrderRoute.BranchNo);

	return 0;

}

int ServerConf::ReadQueryRouteInfo(TiXmlElement *root)
{
	TiXmlElement *servercfg = root->FirstChildElement("queryroute");
	if(!servercfg)		
	{
		AC_ERROR("queryroute not exists!");
		return -1;	
	}

	const char* str = servercfg->Attribute("funcno", &m_QueryRoute.FuncNo);		
	if(!str)
	{
		AC_ERROR("read <queryroute> funcno error");
		return -1;
	}
	AC_INFO("<queryroute> funcno = %d", m_QueryRoute.FuncNo);

	str = servercfg->Attribute("branchno", &m_QueryRoute.BranchNo);		
	if(!str)
	{
		AC_ERROR("read <queryroute> branchno error");
		return -1;
	}
	AC_INFO("<queryroute> branchno = %d", m_QueryRoute.BranchNo);

	return 0;
}
