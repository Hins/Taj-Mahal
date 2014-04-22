/*
 * Log.h -- encapsulate Log class for recording log;
 * Author: Hins
 * Create date: 09/13/2010
 * Last modified date: 04/22/2014
 */

#ifndef LOG_H
#define LOG_H

#include "Config.h"

namespace HMM{
    using namespace std;
    class Log{
        public:
            Log();
            ~Log();
            //LogWrite -- write log function;
            void LogWrite(string s);
            //LogRead -- read log function;
            void LogRead(string s);
        private:
            //_f -- file pointer;
            FILE * _f;
            char _buf[MAX_SIZE];
            //_path -- default log file path, stored in config.h
            static const char* _path;
    };
}

#endif
