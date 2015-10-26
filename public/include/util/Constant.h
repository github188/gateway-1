#pragma once

//[前置机]
const unsigned int TID_FRONT_BASE =                           101010000; //1分布式 01 前置机
const unsigned int TID_FRONT_NOTIFY =                         101010001; //1分布式 01 前置机
const unsigned int TID_FRONT_NOTIFY_PCONN =                   101010002; //1分布式 01 前置机


//[QUERY]
const unsigned int TID_QUERY_BASE =                           102000000; //1分布式 02 QUERY
const unsigned int TID_QUERY_CPUINFO =                        102010001; //1分布式 02 QUERY
const unsigned int TID_QUERY_PROINFO =                        102010002; //1分布式 02 QUERY

//[IMPORT]
const unsigned int TID_IMPORT_BASE =                          103000000;
const unsigned int TID_IMPORT_MCINFO =                        103010001; //1分布式 03 IMPORT

//[NOTIFY]
const unsigned int TID_NOTIFY_MCINFO =                        104010001; //1分布式 04 IMP

//[LDBLOAD]
const unsigned int TID_LDBLOAD_BASE =                         105000000;
const unsigned int TID_LDBLOAD_MCINFO =                       105010001; //1分布式 04 IMP

#define MAX_STREAM_SIZE 4000

#ifdef WIN32
#define snprintf _snprintf
#else
#define snprintf snprintf
#endif