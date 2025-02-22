#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>

#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>

#include <phool/PHRandomSeed.h>
#include <phool/recoConsts.h>

#include <towerid/towerid.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libtowerid.so)
//R__LOAD_LIBRARY(libmakeMBDTrees.so)
#endif
void toweridmacro(const int nEvents = 5000, const char *listFile = "/sphenix/lustre01/sphnxpro/commissioning/DST/run_21981/dsts_fast/DST-00021891-0042.root", const char *inName = "commissioning.root", float adccut_sg = 150, float adccut_k = 150, float sigmas = 4, float SG_f = 0.02, float Kur_f = 0.04, float region_f = 2.0)
{
  Fun4AllServer *se = Fun4AllServer::instance();
  recoConsts *rc = recoConsts::instance();

  towerid *calo = new towerid(inName,adccut_sg,adccut_k,sigmas,nEvents,SG_f,Kur_f);
  se->registerSubsystem(calo);

  Fun4AllInputManager *in = new Fun4AllDstInputManager("DSTcalo");
  in->AddFile(listFile);
  se->registerInputManager(in);
  
  se->run(nEvents);
  
  se->End();
  se->PrintTimer();

  std::cout << "All done!" << std::endl;

  gSystem->Exit(0);
}

