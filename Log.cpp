/*
 * Log.cpp
 * Author: Hins
 * Create date: 09/13/2010
 * Last modified date: 04/22/2014
 */

#include "./include/Log.h"

namespace HMM{
    using namespace std;

/*
  * Log() -- Log class construction function;
  */
Log::Log()
{
    if((this->_f=fopen(this->_path,"a")) == NULL)
        printf("Error: log file open failed!\n");
}

/*
  * ~Log() -- Log class destructor function;
  */
Log::~Log()
{
    fclose(this->_f);
}

/*
  * LogWrite -- write an info into log file;
  * @param s -- log string;
  */
void Log::LogWrite(string s)
{
    if(fputs(s.c_str(),this->_f) == EOF)
        //_log.LogWrite(FormatLog(WRITE_LOG_ERROR));
        fflush(_f);
}

/*
  * LogRead -- read info from log file;
  * @param s -- store log file content;
  */
void Log::LogRead(string s)
{
    if(fread(this->_buf,MAX_SIZE,1,this->_f) == EOF)
    {
        printf("Error: log file read failed!\n");
        return;
    }
    s = this->_buf;
}
//_path : default log file path;
const char* Log::_path = "../log/log";
}
