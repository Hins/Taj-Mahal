/*
 * Hidden_Markov_Model.h -- Hidden markov model class
 * Author: Hins
 * Create date: 07/27/2010
 * Last modified date: 04/22/2014
 */

#ifndef HIDDEN_MARKOV_MODEL_H
#define HIDDEN_MARKOV_MODEL_H

#include "Util.h"
#include "Log.h"
#include "Config.h"

namespace HMM{

    using namespace std;
    class HMM
    {
        private:
            char _buf[MAX_SIZE];

            //Init vector;
            InitMap _init;

            //Observation to hidden status' probability matrix;
            HMM_Map _o2h;

            //status transferation matrix;
            HMM_Map _tfr;

            VecStr _sentence;
            VecStrItr _sitr;
            VecStr _result;

            string _s;
            string _value;
            string _preposnode;
            string _posnode;

            long _index;
            MapStrInt _pos;
            MapStrInt _word;

            Log _log;
            float _bestsplit;

            bool _GetFileStatus();
            int _Parse();

            //Scaffold test program;
            int _WriteInitFile();
            int _WriteO2hFile();
            int _WriteTfrFile();
            int _WriteWordFile();
            int _WritePosFile();
            int _WriteFile();

            //Vector and matrix generation program;
            int _ComputeInit();
            int _ComputeO2h();
            int _ComputeTfr();
            int _Compute();

            //Read matrix from file;
            int _ReadInitFile();
            int _ReadO2hFile();
            int _ReadTfrFile();
            int _ReadWordFile();
            int _ReadPosFile();
            int _ReadFile();

            int _DFS4SplitForward(string src,unsigned int site, VecStr cur);

            float _Estimate(VecStr v);
            float _Sequence(VecStr v);
    public:	
        HMM(){};
        ~HMM{};
        int SplitWord(string s);
        void PrintHelp();
        float GetBestResult() const {return _bestsplit;};
        int EMestimate(const string path=CORPUS_PATH);
    };
}

#endif
