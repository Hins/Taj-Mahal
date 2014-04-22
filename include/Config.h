/*
 * Config.h -- HMM model's configuration file, including macro definition and type redifinition
 * Author: Hins
 * Create date: 09/27/2010
 * Last modified date: 04/22/2014
 */

#ifndef CONFIG_H
#define CONFIG_H

#include<iostream>
#include<string>
#include<map>
#include<vector>
#include<algorithm>
#include<exception>
#include<time.h>
#include<stdio.h>

using namespace std;

#define MAX_SIZE 1024
#define WORDMAXLEN 5
#define ERROR -1

#define OPEN_CORPUS_FAIL "open chinese corpus.txt failed!"
#define PARSE_OVER "parse over!"

#define OPEN_INIT_FAIL "open init file fail!"
#define OPEN_O2H_FAIL "open o2h file fail!"
#define OPEN_TRF_FAIL "open tfr file fail!"
#define OPEN_WORD_FAIL "open word file fail!"
#define OPEN_POS_FAIL "open pos file fail!"
#define PARSE_CORPUS_FAIL "parse corpus fail!"
#define COMPUTE_MATRIX_FAIL "compute matrix fail!"
#define WRITE_FILE_FAIL "write file fail!"
#define READ_FILE_FAIL "read from file fail!"
#define GET_FILE_OK "Get file status ok!"
#define GET_FILE_ERROR "Get file status error!"
#define PARSE_CORPUS_OVER "Parse corpus over!"
#define COMPUTE_MATRIX_OK "Compute corpus status OK!"
#define WRITE_FILE_OK "Write file status OK!"
#define START_PARSE "Start parse corpus!"
#define START_SPLIT_SEN "Start splitting sentence!"
#define SPLIT_SEN_OK "Split sentence status OK!"
#define FIND_RESULT "Find a result!"

#define PATH "/data0/home/xiaotong/pub/projects/gsps/src/media/NLP/PST"

const string INITFILE = PATH"/data/init";
const string O2HFILE = PATH"/data/o2h";
const string TRFFILE = PATH"/data/tfr";
const string WORDFILE = PATH"/data/word";
const string POSFILE = PATH"/data/pos";
const string CORPUS_PATH = PATH"/data/chinese corpus.txt";

namespace HMM{
    using namespace std;
    typedef vector<string> VecStr;
    typedef vector<string>::iterator VecStrItr;
    typedef map<string,float> InitMap;
    typedef map<string,float>::iterator InitMapItr;
    typedef map<string,unsigned int> MapStrInt;
    typedef map<string,unsigned int>::iterator MapStrIntItr;

    struct HMM_Node
    {
        string first;
        string second;
        HMM_Node(string f,string s):first(f),second(s){};
        HMM_Node(const HMM_Node &n){first=n.first;second=n.second;};
        bool operator=(const HMM_Node &r) const
        {
            return !strcmp(first.c_str(),(r.first).c_str()) && !strcmp(second.c_str(),(r.second).c_str());
        };
        bool operator<(const HMM_Node &r) const
        {
            if(!strcmp(first.c_str(),(r.first).c_str()))
                return strcmp(second.c_str(),(r.second).c_str()) < 0;
            return strcmp(first.c_str(),(r.first).c_str()) < 0;
        };      
    };
    typedef HMM_Node HMMNode;
    typedef map<HMMNode,float> HMM_Map;
    typedef map<HMMNode,float>::iterator HMMMapItr;
}

#endif
