/**
 *  @copyright Copyright 2020 The J-PET Framework Authors. All rights reserved.
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may find a copy of the License in the LICENCE file.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *  @file JPetGateParser.cpp
 */

#include <JPetAnalysisTools/JPetAnalysisTools.h>
#include <JPetGateParser/JPetGateParser.h>
//#include <JPetGeantParser/JPetGeantParserTools.h>
#include <JPetOptionsTools/JPetOptionsTools.h>
#include <JPetWriter/JPetWriter.h>
#include <JPetTimeWindow/JPetTimeWindow.h>
#include <iostream>

#include <JPetScin/JPetScin.h>
#include <TMath.h>
#include <array>
#include <cmath>
#include <string>
#include <GateHit/GateHit.h>
#include <JPetScin/JPetScin.h>

using namespace jpet_options_tools;

JPetGateParser::JPetGateParser(const char* name) : JPetUserTask(name) {}

JPetGateParser::~JPetGateParser() {}

bool JPetGateParser::init()
{
 std::unique_ptr<JPetGeomMapping> fDetectorMap(new JPetGeomMapping(getParamBank()));
 fOutputEvents = new JPetTimeWindow("JPetHit");
 INFO("[#] JPetGateParser::init()");
 return true;
}

bool JPetGateParser::exec()
{
 //INFO("[#] JPetGateParser::exec()");
 if (auto& gate_hit = dynamic_cast<GateHit* const>(fEvent))
 {
  processGateHit(gate_hit);
  if ( fActivityIndex > abs( fMinTime * fSimulatedActivity * pow( 10, -6 ) ) )
  {
   saveHits();
   fActivityIndex = 0;
  }
  else
  {
   fActivityIndex++;
  }
 }
 else
  return false;
 return true;
}

bool JPetGateParser::terminate()
{
 INFO("[#]  JPetGateParser::terminate()");

 return true;
}

void JPetGateParser::saveHits()
{
   //INFO("[#]  JPetGateParser::saveHits");
  for (const auto& hit : fStoredHits)
  {
    fOutputEvents->add<JPetHit>(hit);
  }

  fStoredHits.clear();
}

void JPetGateParser::saveReconstructedHit(JPetHit recHit)
{
  //INFO("[#]  JPetGateParser::saveReconstructedHit");
  fStoredHits.push_back(recHit);
}

void JPetGateParser::processGateHit(GateHit* gate_hit)
{
 //INFO("[#]  JPetGateParser::processGateHit");
 JPetHit hit;

 std::string s = std::to_string(gate_hit->sci_id);
// INFO("[#]  JPetGateParser::processGateHit " +s);
 JPetScin& scin = getParamBank().getScintillator(gate_hit->sci_id);
 //INFO("[#]  JPetGateParser::processGateHit 1");
 hit.setScintillator(scin);
 hit.setBarrelSlot(scin.getBarrelSlot());

 /// Nonsmeared values
 //INFO("[#]  JPetGateParser::processGateHit 2");
 auto scinID = gate_hit->sci_id;
 auto posZ = gate_hit->posz;
 auto energy = gate_hit->edep * 1000.0;
 auto timeShift = fMinTime;
 auto time = gate_hit->time*1e6 + timeShift;

 ///Smeared values
 //INFO("[#]  JPetGateParser::processGateHit 3");
 hit.setEnergy(fExperimentalParametrizer.addEnergySmearing(scinID, posZ, energy, time));
 // adjust to time window and smear
 hit.setTime(fExperimentalParametrizer.addTimeSmearing(scinID, posZ, energy, time));
 auto radius = getParamBank().getScintillator(scinID).getBarrelSlot().getLayer().getRadius();
 auto theta = TMath::DegToRad() * getParamBank().getScintillator(hit.getScintillator().getID()).getBarrelSlot().getTheta();
 hit.setPosX(radius * std::cos(theta));
 hit.setPosY(radius * std::sin(theta));
 hit.setPosZ(fExperimentalParametrizer.addZHitSmearing(scinID, posZ, energy, time));

 saveReconstructedHit(hit);
}
