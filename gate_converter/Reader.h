#ifndef Reader_h
#define Reader_h

#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include <string>
#include "GateHit.h"

class Reader
{
 public:
  Reader() {}
  void init();
  bool read();
  GateHit* get();
  void close();

  TFile* p_file = nullptr;
  TTree* p_tree = nullptr;
  int entries = 0;
  int entry_index = 0;

  GateHit gate_hit;

  //Zmienne branchów
  int event_id = -1;
  int track_id = -1;
  int parent_id = -1;
  int pdg = 0;
  int volID = 0;

  float edep = 0.0;
  double time = 0.0;
  float posx = 0.0;
  float posy = 0.0;
  float posz = 0.0;
  float sourcex = 0.0;
  float sourcey = 0.0;
  float sourcez = 0.0;

  Char_t process_name[20];

  unsigned int counter = 0;

  //Branche
  TBranch* b_event_id = nullptr;
  TBranch* b_track_id = nullptr;
  TBranch* b_parent_id = nullptr;
  TBranch* b_pdg = nullptr;

  TBranch* b_edep = nullptr;
  TBranch* b_time = nullptr;
  TBranch* b_posx = nullptr;
  TBranch* b_posy = nullptr;
  TBranch* b_posz = nullptr;
  TBranch* b_sourcex = nullptr;
  TBranch* b_sourcey = nullptr;
  TBranch* b_sourcez = nullptr;
  TBranch* b_process_name = nullptr;
  TBranch* b_volID = nullptr;


};

#endif
