#pragma once
#include "mysqlConnection.h"
#include "mysqlDataResult.h"


using namespace std;

typedef unsigned long long ulonglong;
typedef long longlong;

class MYSQLOperate
{
public:
	MYSQLOperate();

	MYSQLOperate(MYSQLConnection *sql_conn, string& sql_str);

	virtual ~MYSQLOperate();

	//void setCommandText(string sql_str,int cmdType);

	void setConnection(MYSQLConnection* sql_conn);

	bool execute(string& qstr);

	bool execute();

	bool exec_storedProcs(const string& sp_name,string parameter);
	
	/*return the row number of result*/
	ulonglong  StoreResult(DataResult *dataResult);

	ulonglong affected_rows(MYSQL* mysql);
	
private:
	
	MYSQLConnection *m_conn;
	MYSQL *m_mysql;	
	std::string m_cmdString;
	DataResult *m_dataResult;

	
};

class MysqlRes
{
public:
	MysqlRes(MYSQL_RES * res);
	~MysqlRes();
private:
	MYSQL_RES* m_res;
};



