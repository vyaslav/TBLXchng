#ifndef _TX_MY_LOG_H
#define _TX_MY_LOG_H

#include <iostream>

//debug
#define MYLOG_LOG(logstr) cout<<logstr<<std::endl;
#define MYLOG_ERROR(logstr) cout<<"ERROR: "<<logstr<<std::endl;

#endif