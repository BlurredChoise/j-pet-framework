#ifndef Writer_h
#define Writer_h

#include "TFile.h"
#include "TTree.h"
#include "GateHit.h"

class Writer
{
 public:
 Writer() {}
 void init();
 void write(GateHit gh );
 void close();
 void test();

 TFile* p_file = nullptr;
 TTree* p_tree = nullptr;
 GateHit* p_gate_hit = nullptr;
};

#endif
