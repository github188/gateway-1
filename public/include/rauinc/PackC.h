#ifndef __PACKC_H_

#define __PACKC_H_



#ifdef PACKC_EXPORTS

#define PACKC_API __declspec(dllexport)

#else

#ifdef _WINDOWS

#define PACKC_API __declspec(dllimport)

#else

#define PACKC_API 

#endif

#endif



#ifdef __cplusplus

extern "C" {

#else

   typedef unsigned char bool;

   #define true 1

   #define false 0

#endif



PACKC_API int LoadPackDefFile(char *szPackDefFile);

PACKC_API void * GetDefaultEMsg();




PACKC_API void Reverse(void *buf,int len);




PACKC_API void Reserve(void *buf,int len);


PACKC_API int DecodeHead(unsigned char *buf,int len,ST_PACKHEAD *pHead,char *eMsg);



PACKC_API int DecodeBufHead(unsigned char *buf,int len,ST_CPACK *pPack,char *eMsg);


PACKC_API int DecodeAPack(char *buf,int len,ST_PACKHEAD *phead,ST_PACK *pack,char *eMsg);


PACKC_API int DecodeBuf(unsigned char *buf,int len,ST_CPACK *pPack,char *eMsg);


PACKC_API int DecodeBufWithArray(unsigned char *buf,int len,ST_CPACK *pPack,ST_PACK *packArray,int *recCount,char *eMsg);



PACKC_API int EmulateEncodeLen(ST_PACKHEAD *phead);



PACKC_API int ModifyHead(ST_PACKHEAD *phead,unsigned char *buf);


PACKC_API int EncodeRow(ST_PACKHEAD *pHead,ST_PACK *pRowPack,unsigned char *buf,int *len,char *eMsg);


PACKC_API int EncodeBuf(ST_CPACK *pPack,unsigned char *buf,int *len,char *eMsg);


PACKC_API int EncodeBufArray(ST_CPACK *pPack,ST_PACK *pArray,unsigned char *buf,int *len,char *eMsg);


PACKC_API int SetParmBit(ST_PACKHEAD *ph,int fidx);


PACKC_API int ClearParmBit(ST_PACKHEAD *ph,int fidx);


PACKC_API int SetErrorPack(ST_CPACK *inPack,ST_CPACK *outPack,char *emsg,int eCode);



PACKC_API int SetErrorPack2(ST_CPACK *outPack,char *emsg,int eCode);


PACKC_API int SetNormalPack(ST_CPACK *inPack,ST_CPACK *outPack,int nextflag,int recCount);


PACKC_API int SetNormalPack2(ST_CPACK *outPack,unsigned int reqtype,int recCount);  // update reqtype (unsigned char ) with (unsigned int) 20050325


PACKC_API int ShowPackHead(char *buf,ST_PACKHEAD *phead);


PACKC_API bool AddFieldValue(ST_CPACK *pack,const char *fieldname,const char *fieldvalue);


PACKC_API bool AddFieldValueD(ST_CPACK *pack,const char *fieldname,const double dfvalue);


PACKC_API bool AddFieldValueI(ST_CPACK *pack,const char *fieldname,const int ifvalue);



PACKC_API int FindFieldIndex(const char *fieldname);



#ifdef __cplusplus

}

#endif



#undef PACKC_API 



#endif



