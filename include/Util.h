/*
 * Util.h -- used to encapsulate common functions
 * Author: Hins
 * Create date: 09/13/2010
 * Last modified date: 04/22/2014
 */

#ifndef LIB_H
#define LIB_H

#include "Config.h"

namespace HMM{
    enum CommandType {CTHelp=1,CTTrain,CTSplit,CTEstimate};
    /*
      * split function -- used to split a string into elements, which will be stored in
      *                   a vector container;
      */
    void split(VecStr& vs,const string& src,const string delimit,const char* null_subst="");
    /*
     * FormatLog function -- format log string with time stamp;
     */
    string FormatLog(const string log);

    CommandType ParseCommand(int argv,char *argv[]);
}

#endif
