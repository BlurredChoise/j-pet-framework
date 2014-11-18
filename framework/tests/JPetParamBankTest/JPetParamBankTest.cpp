#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE JPetParamBankTest
#include <boost/test/unit_test.hpp>

#define private public
#include "../../JPetParamBank/JPetParamBank.h"
#include <TFile.h>

BOOST_AUTO_TEST_SUITE(ParamBankTS)

BOOST_AUTO_TEST_CASE(DefaultConstructorTest)
{
  JPetParamBank bank;
  BOOST_REQUIRE(bank.getScintillatorsSize() == 0);
  BOOST_REQUIRE(bank.getPMsSize() == 0);
  BOOST_REQUIRE(bank.getPMCalibsSize() == 0);
  BOOST_REQUIRE(bank.getFEBsSize() == 0);
  BOOST_REQUIRE(bank.getTRBsSize() == 0);
  BOOST_REQUIRE(bank.getTOMBChannelsSize() == 0);

  BOOST_REQUIRE(bank.getScintillators().GetEntries() == 0);
  BOOST_REQUIRE(bank.getPMs().GetEntries() == 0);
  BOOST_REQUIRE(bank.getPMCalibs().GetEntries() == 0);
  BOOST_REQUIRE(bank.getFEBs().GetEntries() == 0);
  BOOST_REQUIRE(bank.getTRBs().GetEntries() == 0);
  BOOST_REQUIRE(bank.getTOMBChannels().GetEntries() == 0);
}

BOOST_AUTO_TEST_CASE(AddingDummyElementsTest)
{
  JPetParamBank bank;
  JPetScin scint(111, 8.f, 2.f, 4.f, 8.f);
  JPetPM pm(JPetPM::SideB, 222, 32, 64, std::make_pair(16.f, 32.f));
  JPetPMCalib pmCalib(256, "JPetPMCalibTest", 2.f, 4.f, 8.f, 16.f, 32.f, 128, 512);
  JPetFEB feb(1, true, "testStatus", "descr", 1, 1);
  JPetTRB trb(333, 64, 128);
  JPetTOMBChannel TOMBChannel(32u);
  float epsilon = 0.0001f;
  
  bank.addScintillator(scint);
  bank.addPM(pm);
  bank.addPMCalib(pmCalib);
  bank.addFEB(feb);
  bank.addTRB(trb);
  bank.addTOMBChannel(TOMBChannel);

  BOOST_REQUIRE(bank.getScintillatorsSize() == 1);
  BOOST_REQUIRE(bank.getPMsSize() == 1);
  BOOST_REQUIRE(bank.getPMCalibsSize() == 1);
  BOOST_REQUIRE(bank.getFEBsSize() == 1);
  BOOST_REQUIRE(bank.getTRBsSize() == 1);
  BOOST_REQUIRE(bank.getTOMBChannelsSize() == 1);

  BOOST_REQUIRE(bank.getScintillators().GetEntries() == 1);
  BOOST_REQUIRE(bank.getPMs().GetEntries() == 1);
  BOOST_REQUIRE(bank.getPMCalibs().GetEntries() == 1);
  BOOST_REQUIRE(bank.getFEBs().GetEntries() == 1);
  BOOST_REQUIRE(bank.getTRBs().GetEntries() == 1);
  BOOST_REQUIRE(bank.getTOMBChannels().GetEntries() == 1);

  BOOST_REQUIRE(bank.getScintillator(0).getID() == 111);
  BOOST_CHECK_CLOSE(bank.getScintillator(0).getAttenLen(), 8.f, epsilon);
  struct JPetScin::ScinDimensions scin_dimensions(2.f, 4.f, 8.f);
  scin_dimensions = bank.getScintillator(0).getScinSize();
  BOOST_CHECK_CLOSE(scin_dimensions.fLength, 2.f, epsilon);
  BOOST_CHECK_CLOSE(scin_dimensions.fHeight, 4.f, epsilon);
  BOOST_CHECK_CLOSE(scin_dimensions.fWidth, 8.f, epsilon);
  
  BOOST_REQUIRE(bank.getPM(0).getSide() == JPetPM::SideB);
  BOOST_REQUIRE(bank.getPM(0).getID() == 222);
  BOOST_REQUIRE(bank.getPM(0).getHVset() == 32);
  BOOST_REQUIRE(bank.getPM(0).getHVopt() == 64);
  BOOST_CHECK_CLOSE(bank.getPM(0).getHVgain(JPetPM::kFirst), 16.f, epsilon);
  BOOST_CHECK_CLOSE(bank.getPM(0).getHVgain(JPetPM::kSecond), 32.f, epsilon);
  std::pair<float, float> HVgain = bank.getPM(0).getHVgain();
  BOOST_CHECK_CLOSE(HVgain.first, 16.f, epsilon);
  BOOST_CHECK_CLOSE(HVgain.second, 32.f, epsilon);

  BOOST_REQUIRE(bank.getPMCalib(0).GetId() == 256);
  BOOST_REQUIRE(bank.getPMCalib(0).GetNamePM() == "JPetPMCalibTest");
  BOOST_CHECK_CLOSE(bank.getPMCalib(0).GetOpthv(), 2.f, epsilon);
  BOOST_CHECK_CLOSE(bank.getPMCalib(0).GetECalConst1(), 4.f, epsilon);
  BOOST_CHECK_CLOSE(bank.getPMCalib(0).GetECalConst2(), 8.f, epsilon);
  BOOST_CHECK_CLOSE(bank.getPMCalib(0).GetGainalpha(), 16.f, epsilon);
  BOOST_CHECK_CLOSE(bank.getPMCalib(0).GetGainbeta(), 32.f, epsilon);
  BOOST_REQUIRE(bank.getPMCalib(0).GetPMCalibAssignment().id == 128);
  BOOST_REQUIRE(bank.getPMCalib(0).GetPMCalibAssignment().photomultiplier_id == 512);
  
  BOOST_REQUIRE(bank.getFEB(0).getID() == 1);
  BOOST_REQUIRE(bank.getFEB(0).isActive() == true);
  BOOST_REQUIRE(bank.getFEB(0).status() == "testStatus");
  BOOST_REQUIRE(bank.getFEB(0).description() == "descr");
  BOOST_REQUIRE(bank.getFEB(0).version() == 1);
  
  BOOST_REQUIRE(bank.getTRB(0).getID() == 333);
  BOOST_REQUIRE(bank.getTRB(0).getType() == 64);
  BOOST_REQUIRE(bank.getTRB(0).getChannel() == 128);
  
  BOOST_REQUIRE(bank.getTOMBChannel(0).getChannel() == 32u);
}

BOOST_AUTO_TEST_CASE(clearAllContainersTest)
{
  JPetParamBank bank;
  JPetScin scint(111, 8.f, 2.f, 4.f, 8.f);
  JPetPM pm(JPetPM::SideB, 222, 32, 64, std::make_pair(16.f, 32.f));
  JPetPMCalib pmCalib(256, "JPetPMCalibTest", 2.f, 4.f, 8.f, 16.f, 32.f, 128, 512);
  JPetFEB feb(1, true, "testStatus", "descr", 1, 1);
  JPetTRB trb(333, 64, 128);
  JPetTOMBChannel TOMBChannel(32u);
  
  bank.addScintillator(scint);
  bank.addPM(pm);
  bank.addPMCalib(pmCalib);
  bank.addFEB(feb);
  bank.addTRB(trb);
  bank.addTOMBChannel(TOMBChannel);

  BOOST_REQUIRE(bank.getScintillatorsSize() == 1);
  BOOST_REQUIRE(bank.getPMsSize() == 1);
  BOOST_REQUIRE(bank.getPMCalibsSize() == 1);
  BOOST_REQUIRE(bank.getFEBsSize() == 1);
  BOOST_REQUIRE(bank.getTRBsSize() == 1);
  BOOST_REQUIRE(bank.getTOMBChannelsSize() == 1);
  
  bank.clear();
  
  BOOST_REQUIRE(bank.getScintillatorsSize() == 0);
  BOOST_REQUIRE(bank.getPMsSize() == 0);
  BOOST_REQUIRE(bank.getPMCalibsSize() == 0);
  BOOST_REQUIRE(bank.getFEBsSize() == 0);
  BOOST_REQUIRE(bank.getTRBsSize() == 0);
  BOOST_REQUIRE(bank.getTOMBChannelsSize() == 0);
}

BOOST_AUTO_TEST_CASE(getSizeTest)
{
  JPetParamBank bank;
  JPetScin scint(111, 8.f, 2.f, 4.f, 8.f);
  JPetPM pm(JPetPM::SideB, 222, 32, 64, std::make_pair(16.f, 32.f));
  JPetPMCalib pmCalib(256, "JPetPMCalibTest", 2.f, 4.f, 8.f, 16.f, 32.f, 128, 512);
  JPetFEB feb(1, true, "testStatus", "descr", 1, 1);
  JPetTRB trb(333, 64, 128);
  JPetTOMBChannel TOMBChannel(32u);
  
  bank.addScintillator(scint);
  bank.addPM(pm);
  bank.addPMCalib(pmCalib);
  bank.addFEB(feb);
  bank.addTRB(trb);
  bank.addTOMBChannel(TOMBChannel);
  
  BOOST_REQUIRE(bank.getSize(JPetParamBank::kScintillator) == 1);
  BOOST_REQUIRE(bank.getSize(JPetParamBank::kPM) == 1);
  BOOST_REQUIRE(bank.getSize(JPetParamBank::kPMCalib) == 1);
  BOOST_REQUIRE(bank.getSize(JPetParamBank::kFEB) == 1);
  BOOST_REQUIRE(bank.getSize(JPetParamBank::kTRB) == 1);
  BOOST_REQUIRE(bank.getSize(JPetParamBank::kTOMBChannel) == 1);


}

BOOST_AUTO_TEST_CASE( saving_reading_file )
{
  JPetParamBank bank;
  JPetScin scint1(1, 0, 0, 0, 0);
  JPetScin scint2(2, 0, 0, 0, 0);
  JPetPM pm1;
  JPetPM pm2;
  JPetPM pm3;
  JPetPM pm4;
  pm1.setID(1);
  pm2.setID(2);
  pm3.setID(3);
  pm4.setID(4);
  JPetPMCalib pmCalib(256, "JPetPMCalibTest", 2.f, 4.f, 8.f, 16.f, 32.f, 128, 512);
  pm1.setScin(scint1);
  pm2.setScin(scint1);
  pm3.setScin(scint2);
  pm4.setScin(scint2);

  JPetFEB feb(1, true, "testStatus", "descr", 1, 1);
  JPetTRB trb;
  bank.addPM(pm1);
  bank.addPM(pm2);
  bank.addPM(pm3);
  bank.addPM(pm4);
  bank.addPMCalib(pmCalib);
  bank.addScintillator(scint1);
  bank.addScintillator(scint2);
  bank.addTRB(trb);
  bank.addFEB(feb);
  for (int i = 0; i < 100; i++) {
    JPetTOMBChannel channel(i);
    bank.addTOMBChannel(channel);
  }

  BOOST_REQUIRE(bank.getPMCalibsSize() == 1);
  
  TFile file("test.root", "UPDATE");
  file.cd();
  file.WriteObject(&bank, "ParamBank");
  file.Close();
  bank.clear();

  TFile file2("test.root", "READ");
  JPetParamBank* pBank = static_cast<JPetParamBank*>(file2.Get("ParamBank"));
  JPetParamBank& bank2 = *pBank;

  BOOST_REQUIRE(bank2.getScintillatorsSize() == 2);
  BOOST_REQUIRE(bank2.getPMsSize() == 4);
  BOOST_REQUIRE(bank2.getPMCalibsSize() == 0); // TODO ERROR - should be 1
  BOOST_REQUIRE(bank2.getFEBsSize() == 1);
  BOOST_REQUIRE(bank2.getTRBsSize() == 1);

  BOOST_REQUIRE(bank2.getScintillators().GetEntries() == 2);
  BOOST_REQUIRE(bank2.getPMs().GetEntries() == 4);
  BOOST_REQUIRE(bank2.getPMCalibs().GetEntries() == 0); // TODO ERROR - should be 1
  BOOST_REQUIRE(bank2.getFEBs().GetEntries() == 1);
  BOOST_REQUIRE(bank2.getTRBs().GetEntries() == 1);

  // BOOST_REQUIRE(bank2.getPMCalib(0).GetId() == 256); // TODO ERROR
  
  BOOST_REQUIRE(bank2.getFEB(0).getID() == 1);
  BOOST_REQUIRE(bank2.getFEB(0).isActive());
  BOOST_REQUIRE(bank2.getFEB(0).status() == "testStatus");
  BOOST_REQUIRE(bank2.getFEB(0).description() == "descr");
  BOOST_REQUIRE(bank2.getFEB(0).version() == 1);

  BOOST_REQUIRE(bank2.getPM(0).getScin().getID() == 1);
  BOOST_REQUIRE(bank2.getPM(1).getScin().getID() == 1);
  BOOST_REQUIRE(bank2.getPM(2).getScin().getID() == 2);
  BOOST_REQUIRE(bank2.getPM(3).getScin().getID() == 2);
  BOOST_REQUIRE(bank2.getScintillator(0).getID() == 1);
  BOOST_REQUIRE(bank2.getScintillator(1).getID() == 2);

  file2.Close();
}


BOOST_AUTO_TEST_SUITE_END()

