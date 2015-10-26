#ifndef _KSSHMTABLE_

#define _KSSHMTABLE_



#ifndef SMT_IN_ONE

#define SMT_IN_ONE

#endif



typedef void * CURSORHANDLE;        // 多记录查询时候的游标句柄



typedef void * SMTABLEHANDLE;       // 共享内存表的句柄



#ifdef WIN32

typedef __int64 Int64;

#else

typedef long long Int64;

#endif





#ifdef KSSHMT_EXPORTS

#define KSSHMT_API __declspec(dllexport)

#else

#ifdef _WINDOWS

#define KSSHMT_API __declspec(dllimport)

#else

#define KSSHMT_API 

#endif

#endif





enum SHTCMP_TYPES       // 数据比较的时候的比较符

{

   SHTCT_L,

   SHTCT_LE,

   SHTCT_E,

   SHTCT_GE,

   SHTCT_G

};



enum SHT_KEYTYPES       // 索引关键字的类型，如果为纯内存数据或字符串比较，则用>0的长度值

{

   SHTKT_INT32 = -1,

   SHTKT_INT64 = -2,

   SHTKT_FLOAT = -3,

   SHTKT_DOUBLE = -4,

   SHTKT_UINT32 = -5

};



#define SHTABLE_KEYID   28





typedef  struct {

   int  m_tableid;      // 表ID

   int  m_idxcnt;       // 本数据表内包含多少个索引

	int  m_reccnt;       // 实际数量

	int  m_maxrecs;      // 可容纳数量

	int  m_recsize;      // 记录大小 (原始记录长度)

   int  m_unitsize;     // 记录对齐后的长度(字节数)

   int  m_firstfree;    // 第一个空闲Record的编号

   int  m_firstdata;    // 第一个有效数据块号

   int  m_lastdata;     // 最后一个有效数据块号

} sH_TABLEHEAD;



#ifdef  __cplusplus

extern "C" {

#else

#ifndef true

#define __true__

typedef unsigned char bool;

#define true  1

#define false 0

#endif

#endif

   

#ifdef SMT_IN_ONE   // 如果采用一个系统中的共享内存表只用一个共享内存块，则需要如下函数：

typedef struct  

{

   unsigned int maxrecs;

   unsigned int recsize;

   int   idxcnt;

} T_TABLEINFO;



bool INITSMT(const char *SMTName, int tablecnt, T_TABLEINFO *tableinfo);



void EXITSMT();

void REMOVESMT(const char *SMTName);   // 将共享内存表从系统中彻底清除


KSSHMT_API SMTABLEHANDLE SMTCreate(int tableid, unsigned int maxrecs, unsigned int recsize, int idxcnt, char *eMsg);

KSSHMT_API SMTABLEHANDLE SMTOpen(int tableid, char *eMsg);


#else


KSSHMT_API SMTABLEHANDLE SMTCreate(const char *tablename, int tableid, unsigned int maxrecs, unsigned int recsize, int idxcnt, char *eMsg);


KSSHMT_API SMTABLEHANDLE SMTOpen(const char *tablename, char *eMsg);

#endif


KSSHMT_API unsigned int EmulateSizeOfSMT(unsigned int maxrecs, unsigned int recsize, int idxcnt);


KSSHMT_API bool SMTCreateIndex(SMTABLEHANDLE SMTHandle,int idxid, unsigned int keyoffset, int keytype_len, int uniquekey, char *eMsg);


KSSHMT_API void SMTClose(SMTABLEHANDLE SMTHandle);


KSSHMT_API int  SMTInsert(SMTABLEHANDLE SMTHandle, void *recdata, char *eMsg);


KSSHMT_API bool SMTUpdate(SMTABLEHANDLE SMTHandle, int dataid,void *recdata, char *eMsg);

KSSHMT_API bool SMTDelete(SMTABLEHANDLE SMTHandle, int dataid, char *eMsg);

KSSHMT_API bool SMTGetRecord(SMTABLEHANDLE SMTHandle, int dataid,void *recdata, char *eMsg);

KSSHMT_API int SMTGetFirstDataID(SMTABLEHANDLE SMTHandle, char *eMsg);

KSSHMT_API int SMTGetNextDataID(SMTABLEHANDLE SMTHandle, int cur_dataid, char *eMsg);

KSSHMT_API int SMTGetRecordCount(SMTABLEHANDLE SMTHandle, char *eMsg);

KSSHMT_API int SMTIndexSeek(SMTABLEHANDLE SMTHandle, int idxid, void *key, int cmp_oper, char *eMsg);

KSSHMT_API int SMTSelectFirst(SMTABLEHANDLE SMTHandle, void *cmpdata, int cmp_oper,int cd_offset,int type_len, char *eMsg);

KSSHMT_API bool SMTIsOpen(SMTABLEHANDLE SMTHandle, char *eMsg);

KSSHMT_API bool SMTIsValidRecord(SMTABLEHANDLE SMTHandle, int dataid, char *eMsg);

KSSHMT_API bool SMTLock(SMTABLEHANDLE SMTHandle, bool bWait, char *eMsg);

KSSHMT_API bool SMTUnlock(SMTABLEHANDLE SMTHandle, char *eMsg);

KSSHMT_API CURSORHANDLE SMTCursorIndexOpen(SMTABLEHANDLE SMTHandle, int idxid, void *key, int cmp_oper, char *eMsg);

KSSHMT_API CURSORHANDLE SMTCursorOpen(SMTABLEHANDLE SMTHandle, void *cmpdata, int cmp_oper, int cd_offset, int cd_type_len, char *eMsg);

KSSHMT_API bool SMTCursorDestroy(CURSORHANDLE csrhandle, char *eMsg);

KSSHMT_API int SMTCursorSize(CURSORHANDLE csrhandle, char *eMsg);

KSSHMT_API bool SMTCursorFetch(SMTABLEHANDLE SMTHandle, CURSORHANDLE csrhandle, int &dataid, void *pData, char *eMsg);


KSSHMT_API bool SMTCursorPrev(SMTABLEHANDLE SMTHandle, CURSORHANDLE csrhandle, char *eMsg);

KSSHMT_API bool SMTCursorNext(SMTABLEHANDLE SMTHandle, CURSORHANDLE csrhandle, char *eMsg);

KSSHMT_API bool SMTGetIndexInfo(SMTABLEHANDLE SMTHandle, int idxid, int *k_offset, int *k_type_len, int *uniquekey, char *eMsg);

KSSHMT_API bool SMTGetIndexKeyData(SMTABLEHANDLE SMTHandle,int idxid, void *recdata, void *keybuf, int *type_len, char *eMsg);

KSSHMT_API int SMTCompData(SMTABLEHANDLE SMTHandle, int type_len, void *data0, void *data1);


KSSHMT_API bool SMTGetTableInfo(SMTABLEHANDLE SMTHandle, sH_TABLEHEAD *ptableinfo, char *eMsg);


KSSHMT_API bool SMTRemove(const char *tablename);


KSSHMT_API void SMTClear(SMTABLEHANDLE SMTHandle);



#ifdef  __cplusplus

}

#else



#ifdef __true__

#undef true

#undef false

#undef __true__

#endif



#endif



#undef KSSHMT_API

   

#endif

