#pragma once
#include "mysql.h"
#include "OptionalExceptions.h"

class MYSQLConnection :public OptionalExceptions
{
public:
	MYSQLConnection();
	virtual ~MYSQLConnection();

	bool connect( const char *host, const char *user, const char *passwd, const char *db, unsigned int port);
	bool is_connected() const { return m_conn_flag; }
	void* get_connHandle(){return (void*) mysql;}
	void disconnect();
	bool select_db(const std::string& db);
	
public:

	unsigned int m_current_ThreadId;
	unsigned int m_last_ThreadId;

private: 
	void connect_init();
	

	MYSQL *mysql;
	bool m_conn_flag;

};

