#pragma once
#include "mysql.h"
#include<iostream>
#include<vector>
#include<string>

#include "OptionalExceptions.h"



typedef unsigned long long ulonglong;
typedef long longlong;

typedef struct FieldDefineInfo_
{
	int field_index;
	int field_type;
	string field_name;
}FieldDefineInfo;



class DataResult
{
public:
	DataResult();

	virtual ~DataResult();

	int FeildType( int feildIndex);

	string FeildData( int feildIndex);

	string FeildName( int  feildIndex);

	bool FetchNext();	
    
	int GetFeildNum();


	bool Package_feildDefinfo(MYSQL_FIELD* feild,int numFeild);

	bool Package_data(MYSQL_ROW res,unsigned long *length, ulonglong rowCount, ulonglong row_idex);
	
private:

	std::vector<string> m_dataRow;
	std::vector<vector<string> >  m_dataRes;
	std::vector<FieldDefineInfo> m_fieldDef;
	
	int feild_num;
	unsigned long rows_num;
	unsigned long row_count;
	
	MYSQL_ROW m_row;

};
