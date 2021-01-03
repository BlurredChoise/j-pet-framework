#include "Reader.h"
#include <iostream>

void Reader::init()
{
 p_file = new TFile("output.mcGate.root","READ");
 p_tree = dynamic_cast<TTree*>(p_file->Get("tree"));
 entries = p_tree->GetEntries();

 p_tree->SetBranchAddress("PDGEncoding", &pdg, &b_pdg);
 p_tree->SetBranchAddress("trackID",&track_id,&b_track_id);
 p_tree->SetBranchAddress("parentID",&parent_id,&b_parent_id);
 p_tree->SetBranchAddress("time",&time,&b_time);
 p_tree->SetBranchAddress("eventID",&event_id,&b_event_id);
 p_tree->SetBranchAddress("posX",&posx,&b_posx);
 p_tree->SetBranchAddress("posY",&posy,&b_posy);
 p_tree->SetBranchAddress("posZ",&posz,&b_posz);
 p_tree->SetBranchAddress("edep",&edep,&b_edep);
 p_tree->SetBranchAddress("processName",process_name,&b_process_name);
 p_tree->SetBranchAddress("sourcePosX",&sourcex,&b_sourcex);
 p_tree->SetBranchAddress("sourcePosY",&sourcey,&b_sourcey);
 p_tree->SetBranchAddress("sourcePosZ",&sourcez,&b_sourcez);
 p_tree->SetBranchAddress("volumeID_1",&volID,&b_volID);
}

bool Reader::read()
{
 if ( entry_index < entries )
  p_tree->GetEntry(entry_index);
 else
  return false;
 ++entry_index;
 return true;
}

GateHit* Reader::get()
{
 bool is_ok = parent_id == 0;
 is_ok = is_ok && pdg == 22;
 is_ok = is_ok && std::string(process_name) == "compt";
 if( !is_ok )
  return nullptr;
 ++counter;
 gate_hit.track_id = track_id;
 gate_hit.event_id = event_id;
 gate_hit.edep = edep;
 gate_hit.time = time;
 gate_hit.posx = posx;
 gate_hit.posy = posy;
 gate_hit.posz = posz;
 gate_hit.sourcex = sourcex;
 gate_hit.sourcey = sourcey;
 gate_hit.sourcez = sourcez;
 gate_hit.sci_id = volID + 1;
 return &gate_hit;
}

void Reader::close()
{
 p_file->Close();
 delete p_file;
 std::cout << counter << std::endl;
 //delete p_tree;
}
