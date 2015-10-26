#include"CRedis.h"
#include<stdlib.h>
#include<stdio.h>
#define DEBUG_RUNTIME_MSGOUT printf


CRedis::CRedis()
{
	Is_connected = false;
	client = NULL;
}

CRedis::~CRedis(){
	if(Is_connected && client != NULL)
		redisFree(client);
}

int CRedis::Init(char *ip,int port,int timeout)
{
	m_ip = ip;
	m_port = port;
	m_timeout = timeout;
	return REDIS_OK;
}
int CRedis::ConnectDb()
{
	//modify by liuchengzhu 20150603
	//client = redisConnectUnix(m_ip.c_str());
	client = redisConnect(m_ip.c_str(),m_port);
	//end modify by liuchengzhu 20150603
	if(client == NULL)
		return REDIS_ERR;

	if (client->err) 
	 {
		DisconnectDb();
		DEBUG_RUNTIME_MSGOUT("connect to redis error");
        return REDIS_ERR;
     }

	 Is_connected= true;
	 struct timeval timeout = {m_timeout,0}; 
	 redisSetTimeout(client,timeout);
	 return REDIS_OK;
}

bool CRedis::IsConnected()
{
	return Is_connected;
}

int CRedis::SendCommad(std::vector<std::string> & data)
{
	std::vector<size_t> alllen;
	std::vector<char*> allpos;
	for(size_t i=0;data.size()>i;i++)
	{
		alllen.push_back(data[i].length());
		allpos.push_back((char*)data[i].data());
	}
	redisAppendCommandArgv(client,data.size(),(const char**)(&allpos[0]),(const size_t*)&alllen[0]);
	void *_reply = NULL;
	if (redisGetReply(client,&_reply) != REDIS_OK) //发送数据并获取执行结果
	{
		char erbuf[1024];
		sprintf(erbuf,"redis err info:%s",client->errstr);
		DEBUG_RUNTIME_MSGOUT(erbuf);
		if(_reply)
			freeReplyObject(_reply);
		DisconnectDb();
		return REDIS_ERR;
	}
	freeReplyObject(_reply);
	return REDIS_OK;//成功
}
int CRedis::RecvResult(std::string& command1,std::string &retdata)
{
	redisReply* reply = NULL;
	reply= (redisReply*)redisCommand(client,command1.c_str());
	if(reply == NULL)
	{
		char erbuf[1024];
		sprintf(erbuf,"redis err info:%s",client->errstr);
		DEBUG_RUNTIME_MSGOUT(erbuf);
		DisconnectDb();
		return REDIS_ERR;
	}
	if(reply->type == REDIS_REPLY_STATUS /*&& strcasecmp(reply->str,"OK") == 0*/)
	{
		retdata.append(reply->str,reply->len);
		DEBUG_RUNTIME_MSGOUT("REDIS_REPLY_STATUS  ");
	}
	else if(reply->type == REDIS_REPLY_STRING)
	{
		retdata.append(reply->str,reply->len);
	}
	else if(reply->type == REDIS_REPLY_ARRAY)//list set zset 返回结果集处
	{
	}
	else if(reply->type == REDIS_REPLY_INTEGER)
	{
		char data[256];
		sprintf(data,"%lld",reply->integer);
		DEBUG_RUNTIME_MSGOUT("REDIS_REPLY_INTEGER  ");
	}
	else if(reply->type == REDIS_REPLY_ERROR)
	{
		DEBUG_RUNTIME_MSGOUT("REDIS_REPLY_ERROR    ");
		freeReplyObject(reply);
		return REDIS_ERR;
	}
	freeReplyObject(reply);
	return REDIS_OK;
}
int CRedis::RecvResult(std::string& command1,std::vector<std::string> &retdata)
{
	redisReply* reply = NULL;
	reply= (redisReply*)redisCommand(client,command1.c_str());
	if(reply == NULL)
	{
		//可能出现问题,重新连接
		 DisconnectDb();
		 DEBUG_RUNTIME_MSGOUT("redisCommand mgs: error");
		 return REDIS_ERR;
	}
	if(reply->type == REDIS_REPLY_STATUS /*&& strcasecmp(reply->str,"OK") == 0*/){
	}
	else if(reply->type == REDIS_REPLY_STRING)
	{
		DEBUG_RUNTIME_MSGOUT("REDIS_REPLY_STRING");
	}
	else if(reply->type == REDIS_REPLY_ARRAY)//list set zset 返回结果集处
	{
		DEBUG_RUNTIME_MSGOUT("REDIS_REPLY_ARRAY");;
		retdata.resize(reply->elements);
		for(size_t i= 0; reply->elements>i;i++)
		{
			redisReply* childReply = reply->element[i];
			if (childReply->type == REDIS_REPLY_STRING)
			{
					retdata[i].append(childReply->str,childReply->len);
			}
		}
	}
	else if(reply->type == REDIS_REPLY_INTEGER)
	{
		char data[256];
		sprintf(data,"%lld",reply->integer);
		DEBUG_RUNTIME_MSGOUT("REDIS_REPLY_INTEGER");
	}
	else if(reply->type == REDIS_REPLY_ERROR)
	{
		DEBUG_RUNTIME_MSGOUT("REDIS_REPLY_ERROR");
		freeReplyObject(reply);
		return REDIS_ERR;
	}
	freeReplyObject(reply);
	return REDIS_OK;
}
int CRedis::DisconnectDb()
{
	Is_connected = false;
	if(client)
	{
		redisFree(client);
		client = NULL;
	}
	return REDIS_OK;
}
int CRedis::Add(std::vector<std::string> &data)
{
	DEBUG_RUNTIME_MSGOUT("insert data to redis!\n");
	return SendCommad(data);
}
int CRedis::Remove(std::string& /*data*/)
{
	return REDIS_OK;
}
int CRedis::Get(string data ,std::string & retdata)
{
	std::string adddata = "get ";
	adddata+=data;
	return RecvResult(adddata,retdata);
}
int CRedis::Get(string data ,std::vector<std::string> & retdata)
{
	std::string adddata = "lrange ";
	adddata+=data;
	data += "0 -1";
	return RecvResult(adddata,retdata);
}
int CRedis::Modify(std::string& /*data*/)
{
	return REDIS_OK;
}
