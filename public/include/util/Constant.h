#pragma once

//[ǰ�û�]
const unsigned int TID_FRONT_BASE =                           101010000; //1�ֲ�ʽ 01 ǰ�û�
const unsigned int TID_FRONT_NOTIFY =                         101010001; //1�ֲ�ʽ 01 ǰ�û�
const unsigned int TID_FRONT_NOTIFY_PCONN =                   101010002; //1�ֲ�ʽ 01 ǰ�û�


//[QUERY]
const unsigned int TID_QUERY_BASE =                           102000000; //1�ֲ�ʽ 02 QUERY
const unsigned int TID_QUERY_CPUINFO =                        102010001; //1�ֲ�ʽ 02 QUERY
const unsigned int TID_QUERY_PROINFO =                        102010002; //1�ֲ�ʽ 02 QUERY

//[IMPORT]
const unsigned int TID_IMPORT_BASE =                          103000000;
const unsigned int TID_IMPORT_MCINFO =                        103010001; //1�ֲ�ʽ 03 IMPORT

//[NOTIFY]
const unsigned int TID_NOTIFY_MCINFO =                        104010001; //1�ֲ�ʽ 04 IMP

//[LDBLOAD]
const unsigned int TID_LDBLOAD_BASE =                         105000000;
const unsigned int TID_LDBLOAD_MCINFO =                       105010001; //1�ֲ�ʽ 04 IMP

#define MAX_STREAM_SIZE 4000

#ifdef WIN32
#define snprintf _snprintf
#else
#define snprintf snprintf
#endif