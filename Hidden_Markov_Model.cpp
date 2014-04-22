/*
 * Hidden_Markov_Model.cpp -- hidden markov model implementation file
 * Author: Hins
 * Create date: 07/27/2010
 * Last modified date: 04/22/2014
 */

 #include "./include/Hidden_Markov_Model.h"

namespace HMM{
    using namespace std;

/*
  * _GetFileStatus -- determine whether corpuses have existed;
  * @return -- true: existed, false: haven't existed;
  */
bool HMM::_GetFileStatus()
{
    if(!access(INITFILE.c_str(),0) && !access(O2HFILE.c_str(),0) && !access(TRFFILE.c_str(),0) && !access(WORDFILE.c_str(),0) && !access(POSFILE.c_str(),0))
    {
        _log.LogWrite(FormatLog(GET_FILE_OK));
        return true;
    }
    _log.LogWrite(FormatLog(GET_FILE_ERROR));
    return false;
}

/*
  * _Compute -- compute init, o2h and tfr matrix;
  * @return -- 0: normal, ERROR: error;
  */
int HMM::_Compute()
{
    return _ComputeInit() || _ComputeO2h() || _ComputeTfr();
}

/*
  * _WriteFile() -- write corpus data into file;
  * @return -- 0: normal, ERROR: error;
  */
int HMM::_WriteFile()
{
    return _WriteInitFile() || _WriteO2hFile() || _WriteTfrFile() || _WriteWordFile() || _WritePosFile(); 
}

/*
  * _Parse() -- parse corpus data into raw matrix;
  * @return -- 0: normal, ERROR: error;
  */
int HMM::_Parse()
{
    try
    {
        FILE* f;
        if((f=fopen(CORPUS_PATH.c_str(),"r")) == NULL)
        {
            _log.LogWrite(FormatLog(OPEN_CORPUS_FAIL));
            return ERROR;
        }
        unsigned int ipos = 0, ivalue = 0;
        _init.clear();
        _o2h.clear();
        _tfr.clear();
        VecStr vs;
        while(fgets(_buf,MAX_SIZE,f) != NULL)
        {
            _s = _buf;
            split(vs,_s," ");
            if(vs.size() <= 0)
                continue;
            _preposnode = "";
            for(VecStrItr itr=vs.begin();itr!=vs.end();itr++)
            {
                if((_index=(*itr).find("/",0))!=string::npos)
                {
                    _value = (*itr).substr(0,_index);
                    _posnode = (*itr).substr(_index+1,(*itr).length()-1);
                    if(!_pos[_posnode])
                        _pos[_posnode] = 1;
                    else
                        _pos[_posnode]++;
                    if(!_word[_value])
                        _word[_value] = 1;
                    else
                        _word[_value]++;
                    HMMNode n(_value,_posnode);
                    _o2h[n]++;
                    if(!strcmp(_preposnode.c_str(),""))
                    {
                        _preposnode = _posnode;
                        _init[_posnode]++;
                    }
                    else
                    {
                        HMMNode n(_preposnode,_posnode);
                        _tfr[n]++;
                    }
                }
            }
        }
        fclose(f);
        _log.LogWrite(FormatLog(PARSE_CORPUS_OVER));
        return 0;
    }
    catch(exception &e)
    {
        _log.LogWrite(FormatLog(e.what()));
        return ERROR;
    }
}

/*
  * HMM -- HMM class constructor function;
  */
HMM::HMM()
{
    _log.LogWrite(FormatLog(START_PARSE));
    _bestsplit = -1;
    if(_GetFileStatus())
    {
        #ifdef TEST
        printf("File existed...\n");
        #endif
        if(_ReadFile())
            _log.LogWrite(FormatLog(READ_FILE_FAIL));
            return;
    }
    
    #ifdef TEST
    printf("Begin parse...\n");
    #endif
    if(_Parse())
    {
        _log.LogWrite(FormatLog(PARSE_CORPUS_FAIL));
        return;
    }
    #ifdef TEST
    printf("Parse over...\n");
    #endif

    #ifdef TEST
    printf("Begin compute...\n");
    #endif
    if(_Compute())
    {
        _log.LogWrite(FormatLog(COMPUTE_MATRIX_FAIL));
        return;
    }
    _log.LogWrite(FormatLog(COMPUTE_MATRIX_OK));
    #ifdef TEST
    printf("Compute over...\n");
    #endif
};

/*
  * ~HMM -- HMM class deconstructor function;
  */
HMM::~HMM()
{
    if(!_GetFileStatus())
    {
        #ifdef TEST
        printf("Begin write file...\n");
        #endif
        if(_WriteFile())
        {
            _log.LogWrite(FormatLog(WRITE_FILE_FAIL));
            return;
        }
        _log.LogWrite(FormatLog(WRITE_FILE_OK));
        #ifdef TEST
        printf("Write file over...\n");
        #endif
    }

    _sentence.clear();
    _pos.clear();
    _word.clear();
    _log.LogWrite(FormatLog(PARSE_OVER));
    _log.LogWrite("\n");
};

/*
  * _WriteInitFile -- write init matrix into file;
  * @return -- 0: normal, ERROR: error;
  */
int HMM::_WriteInitFile()
{
    try
    {
        #ifdef TEST
        printf("Begin write init file\n");
        #endif
        FILE* f;
        if((f=fopen(INITFILE.c_str(),"w")) == NULL)
        {
            _log.LogWrite(FormatLog(OPEN_INIT_FAIL));
            return ERROR;
        }
        for(InitMapItr itr=_init.begin();itr!=_init.end();itr++)
        {
            sprintf(_buf,"%s %f\n",(itr->first).c_str(),itr->second);
            fputs(_buf,f);
        }
        fclose(f);
        #ifdef TEST
        printf("End write init file\n");
        #endif
        return 0;
    }
    catch(exception &e)
    {
        _log.LogWrite(FormatLog(e.what()));
        return ERROR;
    }
}

/*
  * _WriteO2hFile -- write o2h matrix into file;
  * @return -- 0: normal, ERROR: error;
  */
int HMM::_WriteO2hFile()
{
    try
    {
        #ifdef TEST
        printf("Begin write o2h file\n");
        #endif
        FILE* f;
        if((f=fopen(O2HFILE.c_str(),"w")) == NULL)
        {
            _log.LogWrite(FormatLog(OPEN_O2H_FAIL));
            return ERROR;
        }
        for(HMMMapItr itr=_o2h.begin();itr!=_o2h.end();itr++)
        {
            sprintf(_buf,"%s %s %f\n",(itr->first).first.c_str(),(itr->first).second.c_str(),itr->second);
            fputs(_buf,f);
        }
        fclose(f);
        #ifdef TEST
        printf("End write o2h file\n");
        #endif
        return 0;
    }
    catch(exception &e)
    {
        _log.LogWrite(FormatLog(e.what()));
        return ERROR;
    }
}

/*
  * _WriteTrfFile -- write trf matrix into file;
  * @return -- 0: normal, ERROR: error;
  */
int HMM::_WriteTfrFile()
{
    try
    {
        #ifdef TEST
        printf("Begin write tfr file\n");
        #endif
        FILE* f;
        if((f=fopen(TRFFILE.c_str(),"w")) == NULL)
        {
            _log.LogWrite(FormatLog(OPEN_TRF_FAIL));
            return ERROR;
        }
        for(HMMMapItr itr=_tfr.begin();itr!=_tfr.end();itr++)
        {
            sprintf(_buf,"%s %s %f\n",(itr->first).first.c_str(),(itr->first).second.c_str(),itr->second);
            fputs(_buf,f);
        }
        fclose(f);
        #ifdef TEST
        #endif
        return 0;
    }
    catch(exception &e)
    {
        _log.LogWrite(FormatLog(e.what()));
        return ERROR;
    }
}

/*
  * _WriteWordFile -- write word matrix into file;
  * @return -- 0: normal, ERROR: error;
  */
int HMM::_WriteWordFile()
{
    try
    {
        #ifdef TEST
        printf("Begin write word file\n");
        #endif
        FILE* f;
        if((f=fopen(WORDFILE.c_str(),"w")) == NULL)
        {
            _log.LogWrite(FormatLog(OPEN_WORD_FAIL));
            return ERROR;
        }
        for(MapStrIntItr itr=_word.begin();itr!=_word.end();itr++)
        {
            sprintf(_buf,"%s %d\n",(itr->first).c_str(),itr->second);
            fputs(_buf,f);
        }
        fclose(f);
        #ifdef TEST
        printf("End write word file\n");
        #endif
        return 0;
    }
    catch(exception &e)
    {
        _log.LogWrite(FormatLog(e.what()));
        return ERROR;
    }
}

/*
  * _WritePosFile -- write pos matrix into file;
  * @return -- 0: normal, ERROR: error;
  */
int HMM::_WritePosFile()
{
    try
    {
        #ifdef TEST
        printf("Begin write pos file\n");
        #endif
        FILE* f;
        if((f=fopen(POSFILE.c_str(),"w")) == NULL)
        {
            _log.LogWrite(FormatLog(OPEN_POS_FAIL));
            return ERROR;
        }
        for(MapStrIntItr itr=_pos.begin();itr!=_pos.end();itr++)
        {
            sprintf(_buf,"%s %d\n",(itr->first).c_str(),itr->second);
            fputs(_buf,f);
        }
        fclose(f);
        #ifdef TEST
        printf("End write pos file\n");
        #endif
        return 0;
    }
    catch(exception &e)
    {
        _log.LogWrite(FormatLog(e.what()));
        return ERROR;
    }
}

/*
  * _ComputeInit -- compute init matrix;
  * @return -- 0: normal, ERROR: error;
  */
int HMM::_ComputeInit()
{
    try
    {
        float sum = 0;
        for(InitMapItr itr=_init.begin();itr!=_init.end();itr++)
            sum += itr->second;
        if(sum)
            for(InitMapItr itr=_init.begin();itr!=_init.end();itr++)
                _init[itr->first] /= sum;
        return 0;
    }
    catch(exception &e)
    {
        _log.LogWrite(FormatLog(e.what()));
        return ERROR;
    }
}

/*
  * _ComputeO2h -- compute o2h matrix;
  * @return -- 0: normal, ERROR: error;
  */
int HMM::_ComputeO2h()
{
    try
    {
        for(HMMMapItr itr=_o2h.begin();itr!=_o2h.end();itr++)
        {
            //HMMNode n(itr.first,itr.second);
            _o2h[itr->first] /= _word[(itr->first).first];
        }
        return 0;
    }
    catch(exception &e)
    {
        _log.LogWrite(FormatLog(e.what()));
        return ERROR;
    }
}

/*
  * _ComputeTfr -- compute tfr matrix;
  * @return -- 0: normal, ERROR: error;
  */
int HMM::_ComputeTfr()
{
    try
    {
        for(HMMMapItr itr=_tfr.begin();itr!=_tfr.end();itr++)
        {
            HMMNode n(itr->first);
            _tfr[n] /= _pos[(itr->first).first];
        }
        return 0;
    }
    catch(exception &e)
    {
        _log.LogWrite(FormatLog(e.what()));
        return ERROR;
    }
}

/*
  * _ReadInitFile -- read init matrix data from file;
  * @return -- 0: normal, ERROR: error;
  */
int HMM::_ReadInitFile()
{
    try
    {
        _init.clear();
        FILE* f;
        if((f=fopen(INITFILE.c_str(),"r")) == NULL)
        {
            _log.LogWrite(FormatLog(OPEN_INIT_FAIL));
            return ERROR;
        }
        VecStr vs;
        string s;
        while(fgets(_buf,MAX_SIZE,f) != NULL)
        {
            s = _buf;
            split(VecStr & vs, const string & src, const string delimit, const char * null_subst)(vs,s," ");
            if(vs.size() == 1)
                _init[" "] = atof(vs[0].c_str());
            else
                _init[vs[0]] = atof(vs[1].c_str());
        }
        fclose(f);
        return 0;
    }
    catch(exception &e)
    {
        _log.LogWrite(FormatLog(e.what()));
        return ERROR;
    }
}

/*
  * _ReadO2hFile -- read o2h matrix data from file;
  * @return -- 0: normal, ERROR: error;
  */
int HMM::_ReadO2hFile()
{
    try
    {
        _o2h.clear();
        FILE* f;
        if((f=fopen(O2HFILE.c_str(),"r")) == NULL)
        {
            _log.LogWrite(FormatLog(OPEN_O2H_FAIL));
            return ERROR;
        }
        VecStr vs;
        string s;
        while(fgets(_buf,MAX_SIZE,f) != NULL)
        {
            s = _buf;
            split(vs,s," ");
            if(vs.size() == 2)
            {
                HMMNode n(vs[0],"");
                _o2h[n] = atof(vs[1].c_str());
            }
            else
            {
                HMMNode n(vs[0],vs[1]);
                _o2h[n] = atof(vs[2].c_str());
            }
        }
        fclose(f);
        return 0;
    }
    catch(exception &e)
    {
        _log.LogWrite(FormatLog(e.what()));
        return ERROR;
    }
}

/*
  * _ReadTfrFile -- read tfr matrix data from file;
  * @return -- 0: normal, ERROR: error;
  */
int HMM::_ReadTfrFile()
{
    try
    {
        _tfr.clear();
        FILE* f;
        if((f=fopen(TRFFILE.c_str(),"r")) == NULL)
        {
            _log.LogWrite(FormatLog(OPEN_TRF_FAIL));
            return ERROR;
        }
        VecStr vs;
        string s;
        while(fgets(_buf,MAX_SIZE,f) != NULL)
        {
            s = _buf;
            split(vs,s," ");
            if(vs.size() == 2)
            {
                HMMNode n("",vs[0]);
                _tfr[n] = atof(vs[1].c_str());
            }
            else
            {
                HMMNode n(vs[0],vs[1]);
                _tfr[n] = atof(vs[2].c_str());
            }
        }
        fclose(f);
        return 0;
    }
    catch(exception &e)
    {
        _log.LogWrite(FormatLog(e.what()));
        return ERROR;
    }
}

/*
  * _ReadWordFile -- read word matrix data from file;
  * @return -- 0: normal, ERROR: error;
  */
int HMM::_ReadWordFile()
{
    try
    {
        _word.clear();
        FILE* f;
        if((f=fopen(WORDFILE.c_str(),"r")) == NULL)
        {
            _log.LogWrite(FormatLog(OPEN_WORD_FAIL));
            return ERROR;
        }
        VecStr vs;
        string s;
        while(fgets(_buf,MAX_SIZE,f) != NULL)
        {
            s = _buf;
            split(vs,s," ");
            _word[vs[0]] = atoi(vs[1].c_str());
        }
        fclose(f);
        return 0;
    }
    catch(exception &e)
    {
        _log.LogWrite(FormatLog(e.what()));
        return ERROR;
    }
}

/*
  * _ReadPosFile -- read pos matrix data from file;
  * @return -- 0: normal, ERROR: error;
  */
int HMM::_ReadPosFile()
{
    try
    {
        _pos.clear();
        FILE* f;
        if((f=fopen(POSFILE.c_str(),"r")) == NULL)
        {
            _log.LogWrite(FormatLog(OPEN_POS_FAIL));
            return ERROR;
        }
        VecStr vs;
        string s;
        while(fgets(_buf,MAX_SIZE,f) != NULL)
        {
            s = _buf;
            split(vs,s," ");
            _pos[vs[0]] = atoi(vs[1].c_str());
        }
        fclose(f);
        return 0;
    }
    catch(exception &e)
    {
        _log.LogWrite(FormatLog(e.what()));
        return ERROR;
    }
}

/*
  * _ReadFile -- read matrix data from files;
  * @return -- 0: normal, ERROR: error;
  */
int HMM::_ReadFile()
{
    return _ReadInitFile() || _ReadO2hFile() || _ReadTfrFile() || _ReadWordFile() || _ReadPosFile();
}

/*
  * _Estimate -- Model 1, calculate the summarization propobility for observation sequence;
  * @param v -- sentence split result;
  * @return -- split probability;
  */
float HMM::_Estimate(VecStr v)
{
    InitMap frd_array;
    for(MapStrIntItr itr=_pos.begin();itr!=_pos.end();itr++)
    {
        HMMNode node(v[0],itr->first);
        if(!_o2h[node]) frd_array[itr->first] = 0;
        else frd_array[itr->first] = _init[itr->first];
    }
    unsigned int len = v.size();
    float temp = 0;
    float sum = 0;
    for(unsigned int i=1;i<len;i++)
    {
            for(MapStrIntItr itr=_pos.begin();itr!=_pos.end();itr++)
            {
                if(frd_array[itr->first])
                {
                    sum = 0;
                    for(MapStrIntItr itr2=_pos.begin();itr2!=_pos.end();itr2++)
                    {
                        temp = frd_array[itr->first];
                        HMMNode node1(itr->first,itr2->first);
                        HMMNode node2(v[i],itr2->first);
                        temp *= _tfr[node1]*_o2h[node2];
                        sum += temp;	
                    }
                    frd_array[itr->first] = sum;
                }
            }
        }
        sum = 0;
        for(InitMapItr itr=frd_array.begin();itr!=frd_array.end();itr++)
            sum += itr->second;
        return sum;
}

/*
  * _Sequence -- Model 2, Viterby algorithm;
  * @param v -- sentence split result;
  * @return -- split maximum probability;
  */
float HMM::_Sequence(VecStr v)
{
    InitMap frd_array;
    for(MapStrIntItr itr=_pos.begin();itr!=_pos.end();itr++)
    {
        HMMNode node(v[0],itr->first);
        if(!_o2h[node]) frd_array[itr->first] = 0;
        else frd_array[itr->first] = _init[itr->first];
    }
    unsigned int len = v.size();
    float temp = 0;
    float tempmax = -1;
    for(unsigned int i=1;i<len;i++)
    {
        tempmax = -1;
        for(MapStrIntItr itr=_pos.begin();itr!=_pos.end();itr++)
        {
            if(frd_array[itr->first])
            {
                for(MapStrIntItr itr2=_pos.begin();itr2!=_pos.end();itr2++)
                {
                    temp = frd_array[itr->first];
                    HMMNode node1(itr->first,itr2->first);
                    HMMNode node2(v[i],itr2->first);
                    temp *= _tfr[node1]*_o2h[node2];
                    if(temp > tempmax)
                        tempmax = temp;
                }
                frd_array[itr->first] = tempmax;
            }
        }
    }
    temp = -1;
    for(InitMapItr itr=frd_array.begin();itr!=frd_array.end();itr++)
        if(temp < itr->second)
            temp = itr->second;
    return temp;
}

/*
  * PrintHelp -- print help information;
  */
void HMM::PrintHelp()
{
    printf(
                "Usage: PST [options] [sentence]\n"

                "sentence -- sentence need to be splitted\n"

                "options -- command parameters\n"
                "-t -- train system corpus\n"
                "      Format: -t\n"
                "-s -- split a sentence\n"
                "      Format: -s sentence\n"
                "-e -- estimate a sentence\n"
                "      Format: -e sentence\n"
                "-h -- get help information\n"
                "      Format: -h\n"
            );
    exit(1);
}

/*
  * SplitWord -- split sentence main function;
  * @param s -- sentence;
  * @return -- 0: normal, ERROR: error;
  */
int HMM::SplitWord(string s)
{
    _bestsplit = -1;
    VecStr vs;
    vs.clear();
    _log.LogWrite(FormatLog(START_SPLIT_SEN));
    if(_DFS4SplitForward(s,0,vs))
        return ERROR;
    _log.LogWrite(FormatLog(SPLIT_SEN_OK));
    return 0;
}

/*
  * _DFS4SplitForward -- split senetece's recursive function;
  * @param src -- sentence;
  * @param site -- current split site;
  * @param cur -- current split result;
  * @return -- 0: normal, ERROR: error;
  */
int HMM::_DFS4SplitForward(string src,unsigned int site, VecStr cur)
{
    try
    {
        if(site >= src.size())
        {
            _log.LogWrite(FormatLog(FIND_RESULT));
            string s = "";
            for(VecStrItr itr = cur.begin();itr!=cur.end();itr++)
                s += (*itr) + " ";
            _log.LogWrite(s+"\n");
            float tmp = _Estimate(cur);
            if(tmp > _bestsplit)
            {
                _bestsplit = tmp;
                _result.clear();
                _result = cur;
            }
            return 0;
        }
        for(unsigned int i=WORDMAXLEN*2;i>0;i--,i--)
        {
            if(site+i<=src.size() && _word[src.substr(site,i)])
            {
                cur.push_back(src.substr(site,i));
                _DFS4SplitForward(src,site+i,cur);
            }
        }
        return 0;
    }
    catch(exception& e)
    {
        _log.LogWrite(FormatLog(e.what()));
        return ERROR;
    }
}

/*
  * EMestimate -- EM algorithm to estimate HMM model parameters;
  * @param path -- corpus path
  * @return -- 0: normal, ERROR: error;
  */
int HMM::EMestimate(const string path)
{
    return 0;
}
}

#endif
