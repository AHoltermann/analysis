#include "towerid.h"

#include <fun4all/Fun4AllReturnCodes.h>

#include <phool/PHCompositeNode.h>


//Fun4All
#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllHistoManager.h>
#include <phool/PHCompositeNode.h>
#include <phool/getClass.h>
#include <phool/phool.h>
#include <ffaobjects/EventHeader.h>

//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>

//Tower stuff
#include <calobase/TowerInfoContainer.h>
#include <calobase/TowerInfo.h>
#include <calobase/TowerInfoDefs.h>

//________________________________
towerid::towerid(const std::string &name,float adccut_sg, float adccut_k, float sigmas, int nevents, float SG_f, float Kur_f, float region_f): 
SubsysReco(name)
  ,T(nullptr)
  ,Outfile(name)
  ,adccut_sg(adccut_sg)
  ,adccut_k(adccut_k)
  ,sigmas(sigmas)
  ,nevents(nevents)
  ,SG_f(SG_f)
  ,Kur_f(Kur_f)
  ,region_f(region_f)
{
  std::cout << "towerid::towerid(const std::string &name) Calling ctor" << std::endl;
}
//__________________________________
towerid::~towerid()
{
  std::cout << "towerid::~towerid() Calling dtor" << std::endl;
}
//_____________________________
int towerid::Init(PHCompositeNode *topNode)
{

  out = new TFile(Outfile.c_str(),"RECREATE");

  T = new TTree("T","T");
  T -> Branch("hot_channels",&m_hot_channels);

  fchannels = new TFile("channels.root");
  channels = (TTree*)fchannels->Get("myTree");
  channels->SetBranchAddress("fiber_type",&fiber_type);

  std::cout << "towerid::Init(PHCompositeNode *topNode) Initializing" << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;

}
//_____________________________
int towerid::InitRun(PHCompositeNode *topNode)
{
  std::cout << "towerid::InitRun(PHCompositeNode *topNode) Initializing for Run XXX" << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}
//_____________________________
int towerid::process_event(PHCompositeNode *topNode)
{
TowerInfoContainer *emcTowerContainer;
emcTowerContainer =  findNode::getClass<TowerInfoContainer>(topNode,"TOWERS_CEMC");
 if(!emcTowerContainer)
    {
      std::cout << PHWHERE << "towerid::process_event Could not find node TOWERS_CEMC"  << std::endl;
      return Fun4AllReturnCodes::ABORTEVENT;
    }

 bool goodevent = false;
 int tower_range = emcTowerContainer->size();
        for(int j = 0; j < tower_range; j++){
	
	//	unsigned int towerkey = emcTowerContainer->encode_key(j);
       // unsigned int ieta = TowerInfoDefs::getCaloTowerEtaBin(towerkey);
	//	unsigned int iphi = TowerInfoDefs::getCaloTowerPhiBin(towerkey);

	    double energy = emcTowerContainer -> get_tower_at_channel(j) -> get_energy();
		channels->GetEntry(j);
		if((fiber_type ==0) && (energy > adccut_sg)){
		towerF[j]++;
		goodevent = true;
                sectorF[j/384]++;
                ibF[j/64]++;
		}
		else if ((fiber_type ==1) &&( energy > adccut_k)){
		towerF[j]++;
		sectorF[j/384]++;
		ibF[j/64]++;
		}
				
		//std::cout << energy << std::endl;
		towerE[j]+=energy;
		sectorE[j/384]+=energy;
		ibE[j/64]+=energy;
		
			
            /*if(energy > adccut){
                towerF[j]++;
            }*/
	}
	if(goodevent == true){goodevents++;}
  return Fun4AllReturnCodes::EVENT_OK;
}
//__________________________
int towerid::ResetEvent(PHCompositeNode *topNode)
{
  return Fun4AllReturnCodes::EVENT_OK;
}


//__________________________
int towerid::EndRun(const int runnumber)
{
  Fspec->SetBins(goodevents,0,goodevents);
  Fspec_SG->SetBins(goodevents,0,goodevents);
  Fspec_K->SetBins(goodevents,0,goodevents);  
  Fspec_IB->SetBins(goodevents,0,goodevents);
  Fspec_sector->SetBins(goodevents,0,goodevents);
  
  Espec->SetBins(10*goodevents,0,1000*goodevents);
  Espec_SG->SetBins(10*goodevents,0,1000*goodevents);
  Espec_K->SetBins(10*goodevents,0,1000*goodevents);
  Espec_IB->SetBins(10*goodevents,0,1000*goodevents);
  Espec_sector->SetBins(10*goodevents,0,1000*goodevents); 

 
 for(int i = 0; i < 24576; i++){

	Fspec->Fill(towerF[i]);
	Espec->Fill(towerE[i]);
	channels->GetEntry(i);
	if(fiber_type == 0){
	Fspec_SG->Fill(towerF[i]);
	Espec_SG->Fill(towerE[i]);
	}
	else{
	Fspec_K->Fill(towerF[i]);
	Espec_K->Fill(towerE[i]);
	}
	}
 for(int j = 0; j<384; j++){
	Fspec_IB->Fill(1.0*ibF[j]/64);
	Espec_IB->Fill(1.0*ibE[j]/64);
	}
 for(int k = 0; k<64; k++){
	Fspec_sector->Fill(1.0*sectorF[k]/384);
	Espec_sector->Fill(1.0*sectorE[k]/384);
	}

  Fspec->SetBinContent(1,0);
  Fspec_SG->SetBinContent(1,0);
  Fspec_K->SetBinContent(1,0);
  Fspec_IB->SetBinContent(1,0);
  Fspec_sector->SetBinContent(1,0);
	
	float cutoffFreq_SG;
	float cutoffFreq_K;
	float cutoffFreq;
	
	float cutoffFreq_sector;
	float cutoffFreq_IB;
	
	cutoffFreq_SG = Fspec_SG->GetStdDev()*sigmas + Fspec_SG->GetXaxis()->GetBinCenter(Fspec_SG->GetMaximumBin());
	cutoffFreq_K = Fspec_K->GetStdDev()*sigmas + Fspec_K->GetXaxis()->GetBinCenter(Fspec_K->GetMaximumBin());
	cutoffFreq = Fspec->GetStdDev()*sigmas + Fspec->GetXaxis()->GetBinCenter(Fspec->GetMaximumBin());	
	cutoffFreq_IB = Fspec_IB->GetStdDev()*sigmas + Fspec_IB->GetXaxis()->GetBinCenter(Fspec_IB->GetMaximumBin());
	cutoffFreq_sector = Fspec_sector->GetStdDev()*sigmas + Fspec_sector->GetXaxis()->GetBinCenter(Fspec_sector->GetMaximumBin());

    for(int i = 0; i < 24576; i++){

	channels->GetEntry(i);
	if(fiber_type == 0 && towerF[i] > SG_f*goodevents){
		hottowers[i]++;
	}
	else if(fiber_type == 1 && towerF[i]> Kur_f*goodevents){
		hottowers[i]++;
	}
	else if(towerF[i]==0){
		deadtowers[i]++;
	}
	/*
	if(towerF[i]> cutoffFreq){
	//	std::cout << i << std::endl;
		m_hot_channels = i;
		T->Fill();
	}	*/
	}
    for(int j = 0; j<384; j++){
	if(ibF[j]>region_f*goodevents){
		hot_regions = 1;
		hotIB[j]++;
		for(int j1 = 0; j1<64; j1++ ){
			hottowers[j*64+j1]++;
		}
	}
	}
    
    for(int k = 0; k<64; k++){
	if(sectorF[k]>region_f*goodevents){
		hot_regions = 1;
		hotsectors[k]++;
		for(int k1 = 0; k1 < 384; k1++){
			hottowers[k*384+k1]++;
		}
	}
	}

	std::fill_n(towerF, 24576, 0);
        std::fill_n(towerE, 24576, 0);
        std::fill_n(ibF,384,0);
        std::fill_n(ibE,384,0);
        std::fill_n(sectorF,64,0);
        std::fill_n(sectorE,64,0);

        Fspec->Reset();
        Fspec_SG->Reset();
        Fspec_K->Reset();
        Fspec_IB->Reset();
        Fspec_sector->Reset();

        Espec->Reset();
        Espec_SG->Reset();
        Espec_K->Reset();
        Espec_IB->Reset();
        Espec_sector->Reset();


  std::cout << "towerid::EndRun(const int runnumber) Ending Run for Run " << runnumber << std::endl;
  std::cout << "Saint Gobain Cutoff Frequency: " << cutoffFreq_SG << std::endl;
  std::cout <<  "Kurary Cutoff Frequency: " << cutoffFreq_K << std::endl;
  std::cout << "Overall Tower Cutoff Frequency: " << cutoffFreq << std::endl;
  std::cout << "IB Cutoff hit Frequency: " << cutoffFreq_IB << std::endl;
  std::cout <<  "Sector Cutoff hit Frequency: " << cutoffFreq_sector << std::endl;
  return hot_regions;
}
//____________________________________________________________________________..
int towerid::End(PHCompositeNode *topNode)
{
  std::cout << "towerid::End(PHCompositeNode *topNode) This is the End..." << std::endl;

   for(int i = 0; i<24576; i++){
                if(hottowers[i] >= 0.5){
                        m_hot_channels = i;
                        T->Fill();
                }
        }

  fchannels->Close();
  delete fchannels;
  out -> cd();
  Fspec->Write();
  Fspec_K->Write();
  Fspec_SG->Write();
  Fspec_sector->Write();
  Fspec_IB->Write();
  Espec->Write();
  Espec_SG->Write();
  Espec_K->Write();
  Espec_sector->Write();
  Espec_IB->Write();
  T -> Write();
  out -> Close();
  delete out;

  return Fun4AllReturnCodes::EVENT_OK;
 }

 //____________________________________________________________________________..
 int towerid::Reset(PHCompositeNode *topNode)
   {
 std::cout << "towerid::Reset(PHCompositeNode *topNode) being Reset" << std::endl;
 return Fun4AllReturnCodes::EVENT_OK;
    }
//____________________________________________________
void towerid::Print(const std::string &what) const
{
  std::cout << "towerid::Print(const std::string &what) const Printing info for " << what << std::endl;
}

//____________________________________________________
/*
int towerid::Sectormask(PHCompositeNode *topNode)
{
	if(hot_regions == 1){
	//std::cout << "hot regions detected" << std::endl;
	
	TowerInfoContainer *emcTowerContainer;
	emcTowerContainer =  findNode::getClass<TowerInfoContainer>(topNode,"TOWERS_CEMC");
	 if(!emcTowerContainer)
   	 {
     	 std::cout << PHWHERE << "towerid::process_event Could not find node TOWERS_CEMC"  << std::endl;
     	 return Fun4AllReturnCodes::ABORTEVENT;
   	 }
 	int tower_range = emcTowerContainer->size();
        for(int j = 0; j < tower_range; j++){
		
		if(hotsectors[j/384] != 0){continue;}
		if(hotIB[j/64]!= 0){continue;}
		
		double energy = emcTowerContainer -> get_tower_at_channel(j) -> get_energy();
                channels->GetEntry(j);
                
		if((fiber_type ==0) && (energy > adccut_sg)){
                towerF[j]++;
                sectorF[j/384]++;
                ibF[j/64]++;
                }
                else if ((fiber_type ==1) &&( energy > adccut_k)){
                towerF[j]++;
                sectorF[j/384]++;
                ibF[j/64]++;
                }

                //std::cout << energy << std::endl;
                 towerE[j]+=energy;
                 sectorE[j/384]+=energy;
                 ibE[j/64]+=energy;
		}
	}
	//else{std::cout << "no hot regions, continuing...." << std::endl;}	
	return Fun4AllReturnCodes::EVENT_OK;
}
*/
//______________________________________________________







