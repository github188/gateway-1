#pragma once
#include <string>
#include <vector>
using namespace std;
struct FieldDefineInfo
{
	int fieldindex;
	int fieldtype;
	string fieldname;
};
class DataSet
{
	public:
		DataSet():m_rownum(0),m_fieldnum(0){}
		virtual	~DataSet(){}
		virtual inline void onNum(int rownum,int fieldnum)
		{
			m_rownum = rownum;
			m_fieldnum = fieldnum;
		};
		virtual bool onData(const char* data, unsigned long length,const int rowindex,const int fieldindex) = 0;
		virtual inline bool onFieldDef(const int fieldindex,const char* fieldname,const int fieldtype)
		{
			FieldDefineInfo temp;
			temp.fieldindex = fieldindex;
			temp.fieldtype = fieldtype;
			temp.fieldname = fieldname;
			m_FieldDefineInfo.push_back(temp);
			return true;
		};
		virtual void Start(const int /*rescnt*/)
		{
		}
		virtual void Stop()
		{
		}
		
		int m_rownum;
		int m_fieldnum;

		vector<FieldDefineInfo> m_FieldDefineInfo;
};


