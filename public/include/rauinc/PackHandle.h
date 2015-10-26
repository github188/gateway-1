// PackHandle.h: interface for the CPackHandle class.

//

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKHANDLE_H__5FABD1A8_F02E_485D_9CA8_7825CB168A2C__INCLUDED_)

#define AFX_PACKHANDLE_H__5FABD1A8_F02E_485D_9CA8_7825CB168A2C__INCLUDED_

#if _MSC_VER > 1000

#pragma once

#endif // _MSC_VER > 1000

#include "PackB.h"



#ifdef PACKC_EXPORTS

#define EMSG_API __declspec(dllexport)

#else

#ifdef _WINDOWS

#define EMSG_API __declspec(dllimport)

#else

#define EMSG_API 

#endif

#endif

class EMSG_API CPackHandle  
{

public:

   bool bReverse;                   

   unsigned char iMode;             
   unsigned char iVersionID;        

   char version[20];                

   int  iLastPField;                

   ST_PDATALIST pdlist[PARMBITS];   
   int  nPackLength;

public:

	CPackHandle();

	virtual ~CPackHandle();

	void HostByteOrder(void *data, unsigned int datalength);

	int GetPackLength();

	bool CheckValidField(const unsigned char *parmbits, int fidx);

	const ST_PDATALIST * GetPackDef(int &ilastvfidx);

	void OverwritePack(unsigned char *dparmbits, void *dpack,const unsigned char *sparmbits,const void *spack);

	int GetValidFieldCount(const unsigned char *parmbits);

	const ST_PDATALIST * GetValidFieldInfo(const unsigned char *parmbits,int vfnumber);

	void ResetParmBits(unsigned char *parmbits);

	static unsigned short xmk_crc16(const void *ptr, int count);

	int SetPBIndex(unsigned char *parmbits,int fidx,bool bToSet=true);

	char * GetPrintFieldValue(const void *prec, int fidx, char *pfv, int precision=-1);

	double GetFieldValueD(const void *prec, const char *fieldname);

	int GetFieldValueI(const void *prec, const char *fieldname);

	int GetFieldValue(const void *prec, const char *fieldname, char *value);

	double GetFieldValueD(const void *prec, int fidx);

	int GetFieldValueI(const void *prec, int fidx);

	int GetFieldValue(const void *prec, int fidx, char *value);
	
	bool AddField(void *prec, const char *fieldname, double dvalue, unsigned char *parmbits=NULL);
	
	bool AddField(void *prec, const char *fieldname, int ivalue, unsigned char *parmbits=NULL);
	
	bool AddField(void *prec, const char *fieldname, const char *data, unsigned char *parmbits=NULL);
	
	bool AddField(void *prec, int fidx, double dvalue, unsigned char *parmbits=NULL);
	
	bool AddField(void *prec, int fidx, int ivalue, unsigned char *parmbits=NULL);
	
	bool AddField(void *prec, int fidx, const char *data, unsigned char *parmbits=NULL);

	int EmulateEncodeLength(const unsigned char *parmbits);

	int ClearParmBitByFIndex(unsigned char *parmbits, int fidx);

	int ClearParmBit(unsigned char *parmbits, int fpb);

	int SetParmBitByFIndex(unsigned char *parmbits,int fidx);

	int SetParmBit(unsigned char *parmbits,int fparmbit);

	int FindFieldIndex(const char *fieldname);

	int DecodeRow(const unsigned char *parmbits, const void *ebuf, int eblen, void *prec);

	int EncodeRow(const unsigned char *parmbits, const void *prec, void *ebuf,int ebsize);

	int DecodeHead(const void *buf, int blen, ST_PACKHEAD *pHead);

	int EncodeHead(const ST_PACKHEAD *pHead, void *buf);

	int P_Index(int parmbit);

	int Initialize(const char *szPackDefFile="pack.dat");

	static void rev_data(void *data,unsigned int datalen);

	static void Hex2Buffer(unsigned char *buf,int len,char *hex);

	static char * LRTrim(char *str);

private:

	char * GetPrintFieldValue(const void *prec, const ST_PDATALIST *pfd, char *pfv,int precision);
	
	int GetFieldValue(const void *prec, const ST_PDATALIST *pfd, char *value);

	bool SetFieldValue(void *prec, int fidx, char *data);
	
	bool SetFieldValue(void *prec, const ST_PDATALIST *pfd, double dvalue);

	bool SetFieldValue(void *prec, const ST_PDATALIST *pfd, int ivalue);

	bool SetFieldValue(void *prec, const ST_PDATALIST *pfd, char *data);
	
	int GetEncodeFieldLength(const ST_PDATALIST *pfd);

	int DecodeSingleField(const ST_PDATALIST *pfd, const void *pbuf, void *pdata);

	int EncodeSingleField(const ST_PDATALIST *pfd, const void *pdata, void *pbuf);
};

#undef EMSG_API

#endif // !defined(AFX_PACKHANDLE_H__5FABD1A8_F02E_485D_9CA8_7825CB168A2C__INCLUDED_)

