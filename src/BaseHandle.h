#pragma once

#include "json.h"
#include <string>

using namespace std;


class CBaseHandle
{
public:
	CBaseHandle(void);
	~CBaseHandle(void);

	//json½âÎö½Ó¿Ú
	void GetJsonField(json_object*, const char* filedname, int& f);
	void GetJsonField(json_object*, const char* filedname, unsigned int& f);
	void GetJsonField(json_object*, const char* filedname, double& f);
	void GetJsonField(json_object*, const char* filedname, string& f);
	void GetJsonField(json_object*, const char* filedname, unsigned char& f);

	int uid;

};

class JsonObjGuard
{
public:
	JsonObjGuard(json_object *obj) : m_obj(obj){}
	~JsonObjGuard(){
		if(NULL != m_obj)
			json_object_put(m_obj);
	}
private:
	json_object *m_obj;
};
