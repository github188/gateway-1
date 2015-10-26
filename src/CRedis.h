#include<string>
#include<string.h>
#include<iostream>
#include <hiredis.h>
#include<vector>
using namespace std;

/* Error codes */
#define REDIS_OK                0
#define REDIS_ERR               -1

class CRedis
{
	public:
		CRedis();
		~CRedis();
		int Init(char * ip ,int port,int timeout=5 );
		int ConnectDb();
		int SendCommad(std::vector<std::string> & data);
		int RecvResult(std::string&,std::string &data);
		int RecvResult(std::string&,std::vector<std::string> &data);
		int DisconnectDb();
		int Add(std::vector<std::string> & data);
		int Remove(std::string &data);
		int Get(string data ,std::string & retdata);
		int Get(string data ,std::vector<std::string> & retdata);
		int Modify(std::string &data);
		bool IsConnected();
	private:
		redisContext* client;	
		std::string m_ip;
		int m_port;
		int m_timeout;
		bool Is_connected;
};
