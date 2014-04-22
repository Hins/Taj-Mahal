/*
 * Util.cpp
 * Author: Hins
 * Create date: 09/13/2010
 * Last modified date: 04/22/2014
 */

#include "./include/Util.h"

namespace HMM{
    using namespace std;
    
/*
  * split -- split a sentence into some parts by split symbol;
  * @param vs -- store split result;
  * @param src -- sentence which will be splitted;
  * @param delimit -- split symbol;
  * @param null_substr -- default as "";
  */
void split(VecStr& vs,const string& src,const string delimit,const char* null_subst)
{
    try
    {       
        vs.clear();
        if(src.empty() || delimit.empty())
        {
            printf("Error: split parameters are wrong\n");
            return; 
        } 
        int deli_len = delimit.size();
        long index = string::npos, last_search_position = 0;
        while( (index=src.find(delimit,last_search_position))!=string::npos )
        {
            if(index==last_search_position)
            {
            }
            else if(strcmp(src.substr(last_search_position, index-last_search_position).c_str(),""))
            {
                string slash = src.substr(last_search_position, index-last_search_position);
                vs.push_back(slash);
            }
            last_search_position = index + deli_len;
        }
        string last_one = src.substr(last_search_position);
        vs.push_back(last_one.empty()? null_subst:last_one );
    }
    catch(exception& e)
    {
        printf("Split error: %s\n",e.what());
    }
}

/*
  * FormateLog -- format log string with time stamp;
  * @param log -- description string for log;
  * @return -- reorganized string with time information;
  */
string FormatLog(const string log)
{
    try
    {
        time_t now;
        struct tm *t;
        time(&now);
        t = localtime(&now);
        string s = asctime(t);
        return "["+s.substr(0,s.size()-1)+"] : "+log+"\n";
    }
    catch(exception& e)
    {
        printf("%s\n",e.what());
        return "";
    }
}

CommandType ParseCommand(int argc,char *argv[])
{
    if(argc == 2 && !strcmp(argv[1],"-h"))
        return CTHelp;
    if(argc == 2 && !strcmp(argv[1],"-t"))
        return CTTrain;
    if(argc == 3 && !strcmp(argv[1],"-s"))
        return CTSplit;
    if(argc == 3 && !strcmp(argv[1],"-e"))
        return CTEstimate;
    return CTHelp;
}
}
