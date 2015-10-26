// EMsg.h: interface for the EMsg class.

#if !defined(AFX_EMSG_H_)

#define AFX_EMSG_H_

#if _MSC_VER > 1000

#pragma once

#endif // _MSC_VER > 1000

#include "PackB.h"

#include "PackHandle.h"


#ifdef PACKC_EXPORTS

#define EMSG_API __declspec(dllexport)

#else

#ifdef _WINDOWS

#define EMSG_API __declspec(dllimport)

#else

#define EMSG_API 

#endif

#endif

//设置位图的结束标志
#define END_FIELDMARK  -1

class EMSG_API CEMsg 

{

private:

   CPackHandle phandle;

public:

	explicit CEMsg(const char *szPackDefFile="emsg.dat");  // 缺省装载"emsg.dat"
	virtual ~CEMsg();

	bool IsValidField(ST_PACKHEAD *phead, char *fieldname);

	bool IsValidField(ST_PACKHEAD *phead, int fidx);
	
	void HostByteOrder(void *data, unsigned int datalength);
	
	int GetPackLength();
	
	bool IsRawDataPack(const ST_PACKHEAD *phead);
	
	static void rev_data(void *data,unsigned int datalen);
	
	static char * LRTrim(char *str);
	
	static unsigned short xmk_crc16(const void *ptr, int count);
	
	bool AddField(void *prec, const char *fieldname, double dvalue, ST_PACKHEAD *phead=NULL);
	
	bool AddField(void *prec, const char *fieldname, int ivalue, ST_PACKHEAD *phead=NULL);
	
	bool AddField(void *prec, const char *fieldname, const char *data, ST_PACKHEAD *phead=NULL);
	
	bool AddField(void *prec, int fidx, double dvalue, ST_PACKHEAD *phead=NULL);

	bool AddField(void *prec, int fidx, int ivalue, ST_PACKHEAD *phead=NULL);
	
	bool AddField(void *prec, int fidx, const char *data, ST_PACKHEAD *phead=NULL);
   
	bool CheckValidField(const ST_PACKHEAD *phead, int fidx);
	
	int FindFieldIndex(const char *fieldname);
	
	char * GetPrintFieldValue(const void *prec, int fidx, char *pfv, int precision=-1);

	double GetFieldValueD(const void *prec, const char *fieldname);
	
	double GetFieldValueD(const void *prec, int fidx);
   
	int GetFieldValueI(const void *prec, const char *fieldname);
	
	int GetFieldValueI(const void *prec, int fidx);
	
	int GetFieldValue(const void *prec, const char *fieldname, char *value);
	
	int GetFieldValue(const void *prec, int fidx, char *value);
	
	const ST_PDATALIST * GetPackDef(int &ilastvfidx);

	int GetValidFieldCount(const ST_PACKHEAD *phead);

	const ST_PDATALIST * GetValidFieldInfo(const ST_PACKHEAD *phead,int vfnumber);
	
	void OverwritePack(ST_PACKHEAD *dhead, void *dpack, const ST_PACKHEAD *shead,const void *spack);

	int ClearParmBit(ST_PACKHEAD *phead,int fidx);
	
	int SetParmBit(ST_PACKHEAD *phead,int fidx);
	
	int EmulateEncodeLength(const ST_PACKHEAD *phead);
	
	int DecodeRow(const ST_PACKHEAD *phead, const void *ebuf, int eblen, void *pdata);
	
	int DecodeHead(const void *buf, int blen, ST_PACKHEAD *pHead);
	
	int EncodeRow(const ST_PACKHEAD *phead, const void *pdata, void *ebuf, int ebsize);
	
	int EncodeHead(const ST_PACKHEAD *pHead, void *buf);
	
	void ResetPackHead(ST_PACKHEAD *phead, const ST_PACKHEAD *refphead=NULL);
	
	int Initialize(const char *szPackDefFile="emsg.dat");

	int SetPackHead(ST_PACKHEAD *head,int recCount,int retCode,...);

};

#undef EMSG_API



#endif

