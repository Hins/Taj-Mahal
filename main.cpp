/*
 * main.cpp -- main test program
 * Author: Hins
 * Create date: 07/27/2010
 * Last modified date: 04/22/2014
 */

#include "./include/Hidden_Markov_Model.h"

using namespace std;

namespace HMM
{
    using namespace std;
    int test(int argc, char *argv[])
    {
        #ifdef TEST
        printf("Begin generate object...\n");
        #endif
        HMM* instance = new HMM();
        #ifdef TEST
        printf("Generate object over...\n");
        #endif
        CommandType On = static_cast<CommandType>(ParseCommand(argc,argv));
        switch(On)
        {
            case CTHelp:
                instance->PrintHelp();
                break;
            case CTTrain:
                #ifdef TEST
                printf("train data over\n");
                #endif
                break;
            case CTSplit:
            {
                #ifdef TEST
                printf("split start\n");
                #endif
                if(instance->SplitWord(argv[2]))
                    return ERROR;
                #ifdef TEST
                printf("split sentence over\n");
                #endif
                break;
            }
            case CTEstimate:
                if(instance->SplitWord(argv[2]))
                    return ERROR;
                printf("%f\n",instance->GetBestResult());
                break;
            default:
                instance->PrintHelp();
                break;
        }
        delete instance;
        return 0;
    }
}

int main(int argc, char *argv[])
{
	return HMM::test(argc,argv);
}
