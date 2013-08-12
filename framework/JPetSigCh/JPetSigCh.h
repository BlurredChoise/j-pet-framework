#ifndef _JPETSIGCH_H_
#define _JPETSIGCH_H_

#include <vector>
#include <map>

#include "../JPetPM/JPetPM.h"
#include "../JPetBarrelSlot/JPetBarrelSlot.h"
#include "../JPetScin/JPetScin.h"
#include "../JPetTRB/JPetTRB.h"

using namespace std;

class JPetSigCh {
public:
	JPetSigCh();
	JPetSigCh(const JPetSigCh& obj);
	JPetSigCh(float EdgeTime, float FallEdgeTime = 0);
	
	enum EdgeType { kRising, kFalling };
	
	float getTime(EdgeType type, int ch_no = 0) const { return fChannels[ch_no].find(type)->second; }
	void addCh(float edge_time, float fall_edge_time);
	void setPM(JPetPM* pm) { fPM = pm; }
	//const JPetPM& getPM(){ return *fPM; }
private:
	typedef map < EdgeType, float > SingleCh;
	typedef vector < SingleCh > ChSet;
	float fAmpl;
	 
	ChSet fChannels;
	bool fIsSlow;
	JPetPM* fPM;
	JPetTRB* fTRB;
	JPetScin* fScin;
	JPetBarrelSlot* fBarrelSlot;
};

#endif
