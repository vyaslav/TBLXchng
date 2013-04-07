#ifndef _TX_ORACOMMON_H
#define _TX_ORACOMMON_H


#include <string>
#include <occi.h>
#include <exception>
#include <conio.h>
#include "my_log.h"

using namespace std;
using namespace oracle::occi;

#define DEF_ORA_BEGIN(sql) \
	try{\
	Environment *env = Environment::createEnvironment();\
	{Connection *conn = NULL;\
		try{\
			conn = env->createConnection(conn_settings.username, conn_settings.password, conn_settings.connString);\
			Statement *stmt = conn->createStatement(sql);

#define DEF_ORA_END	\
			conn->terminateStatement(stmt);\
			env->terminateConnection(conn);\
		}catch (SQLException ex){\
			MYLOG_ERROR(ex.what())\
			if(conn == NULL){\
				Environment::terminateEnvironment(env);\
				MYLOG_LOG("Can't open connection. Exiting...")\
				_getch();exit(1);\
			}\
		}\
	}Environment::terminateEnvironment(env);\
	}catch (exception &e){\
		MYLOG_ERROR("Can't open enviroment "<<e.what());\
		_getch();exit(1);\
	}

//Settings
struct Conn_settings{
	string username;
	string password;
	string connString;
	void load(const string &filename);
};



#endif