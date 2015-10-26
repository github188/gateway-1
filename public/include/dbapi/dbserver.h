#pragma once
#include <string>
#include <vector>

using namespace std;

class DataSet;
class DBServerConnect
{
	public:
		DBServerConnect():m_cnnstatus(-2){}
		virtual ~DBServerConnect(){}
		virtual bool Connect(const char* ip, const char* user, const char* passwd, int port = 0,unsigned int connecttimeout = 5,const char* pdbname = NULL,unsigned int readtimeout = 0) = 0;
		virtual void* GetCnn() = 0;
		virtual bool IsConnected() = 0;
		void SetCnnStatus(int iCnnStatus);
		int GetCnnStatus();
		virtual bool Init() = 0;
		virtual void Close() = 0;
	private:
		int	m_cnnstatus;//连接状态，0连接正常，-1连接已经建立，但断开了需要重连 -2连接还没建立，需要进行连接
};
class DBServerOperate
{
	public :
		DBServerOperate():m_iLastErrorCode(0){}
		virtual	~DBServerOperate(){}
		virtual void Init(DBServerConnect* cnn,DataSet* dataset = 0) = 0;
		virtual bool Exec(const string& sql) = 0;
		virtual bool Response(int resultCnt=0x7fffffff, bool bgetfielddef = false) = 0;
		virtual bool GetFromSQL(const char* from, const int fromlength, char* sto , unsigned long& stolen) = 0;
		virtual unsigned int GetLastErrorCode();
		virtual void SetLastErrorCode(unsigned int iLastErrorCode);
		virtual bool RPCProcedure(const string& dbname, const string& rpcname, vector<string>& params)=0;
		void SetLastErrorInfo(const char *errorInfo);
		string GetLastErrorInfo();
		
	private:
		unsigned int m_iLastErrorCode;
		string m_errorInfo;
};
