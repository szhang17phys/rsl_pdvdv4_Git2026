/**
 * @file: CRTGenPdvd_module.cc
 * @brief: Producer generating Monte Carlo truth record in LArSoft format to simulate protodunevd CRT Trigger
 * @author Shuaixiang (Shu) Zhang
 * @Date: Sep 15, 2023
 */

//Based on Jose Soto's CRTGen_module.cc in dualphase folder.

/**
 * @class evgen::CRTGenPdvd
 *  This module assumes muons cross both CRT pannels. One muon will be generated per event.
 *  Only X-drift (the real senario of protoDUNE_VD) case is considered.
 *  This module could read in root files containing CRT hitting information from real data (Mode=1); It could also do pure simulation, i.e. specify the properties of muon by the user (Mode=0).
 */

// Commented by Shu [20260417]:
// The module was previously inserted at duneprototypes GitHub repository
// under ./duneprototypes/duneprototypes/Protodune/vd/  


#include <string>
#include <fstream>

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/Event.h"
#include "fhiclcpp/ParameterSet.h"
#include "cetlib_except/exception.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

#include "TLorentzVector.h"
#include "TVector3.h"

#include "larcore/Geometry/Geometry.h"
#include "larcoreobj/SummaryData/RunData.h"
#include "nusimdata/SimulationBase/MCTruth.h"
#include "nusimdata/SimulationBase/MCParticle.h"
#include "CLHEP/Random/RandFlat.h"
#include "art_root_io/TFileDirectory.h"
#include "art_root_io/TFileService.h"

#include"TH1D.h"
#include"TH2D.h"
#include"TFile.h"
#include"TTree.h"
namespace evgen {
  class CRTGenPdvd;
}

class evgen::CRTGenPdvd : public art::EDProducer {
public:
  explicit CRTGenPdvd(fhicl::ParameterSet const & p);

  void produce(art::Event & e)        override;
  void beginJob()               		  override;
  void beginRun(art::Run & run) 		  override;

private:
  short int driftcoordinate=0;
  std::vector<double> CRT_TOP_center;//initialization values, cm---
  std::vector<double> CRT_BOT_center;
  double CRTSizeX;//size of the CRT in x,y and z corrected to the actual geometry---
  double CRTSizeY;//initialization values, cm---
  double CRTSizeZ;

  TH2D CRTTop, CRTBot;
  TH1D EnergyDistribution;
  TH1D CRTTopX, CRTTopZ, CRTBotX, CRTBotZ;

  double fCRTTop_posX, fCRTTop_posZ, fCRTBot_posX, fCRTBot_posZ;


  int fmode;
  int fEnergyDistributionMode;
  std::pair<float,float> fEnergyRange;
  std::string    fInputFileNameCRT;//Name of text file containing events to simulate---
  std::string    fInputFileNameEnergy;

  TH2F *fTH2CRTTop;
  TH2F *fTH2CRTBot;
  TH1F *fTH1Energy;
  TH1F *fTH1CRTTopX;
  TH1F *fTH1CRTTopZ;
  TH1F *fTH1CRTBotX;
  TH1F *fTH1CRTBotZ;

  //Added by Shu, 20231017---
  TTree * fTheCRTTree;

  //We might have some muons that are pointing out of the CRT bottom,
  //but they end up reaching CRT bottom due to the scattering, 
  //to take those into account, we include a buffer---
  double BufferLengthOnCRTBottom; //buffer in cm---
};

//------------------------------------------------------------------------------
evgen::CRTGenPdvd::CRTGenPdvd(fhicl::ParameterSet const & p)
  : EDProducer{p}
  , CRT_TOP_center{p.get<std::vector<double>>("CRT_TOP_center", {320, 450, 200})}//cm---
  , CRT_BOT_center{p.get<std::vector<double>>("CRT_BOT_center", {120, -450, 200})}
  , CRTSizeX{p.get<double>("CRTSizeX", 120)}//cm---
  , CRTSizeY{p.get<double>("CRTSizeY", 0)}
  , CRTSizeZ{p.get<double>("CRTSizeZ", 120)}
  , fmode{p.get<int>("Mode", 0)} //0 for uniform distribution on CRT geoometry, 1 to get the distribution from TH2D---
  , fEnergyDistributionMode{p.get<int>("EnergyDistribution", 0)} // 0 for uniform distribution on CRT geoometry, 1...---
  , fEnergyRange{p.get<std::pair<float,float>>("EnergyRange", std::make_pair(3,7))}//Energy range, GeV---
  , fInputFileNameCRT{p.get<std::string>("InputFileNameCRT", "CRT_RawInputs.root")}
  , fInputFileNameEnergy{p.get<std::string>("InputFileNameEnergy", "MuonEnergy_v3.root")}
  , BufferLengthOnCRTBottom{p.get<double>("BufferLengthOnCRTBottom", 1.0)}//cm---
{
  produces< std::vector<simb::MCTruth>   >();
  produces< sumdata::RunData, art::InRun >();
}

//------------------------------------------------------------------------------
void evgen::CRTGenPdvd::beginJob()
{
  if(fmode==0)
  {
    std::cout<<" fmode=0 - Sending muons uniformly distributed on the CRT panels." << std::endl;
  }else if(fmode==1)
  {
    std::cout<<" fmode=1 - Sending muons following the distribution contained on file: " << fInputFileNameCRT << std::endl;
  }
  else{ throw cet::exception("CRTGenPdvd") << "unknown fmode value " << fmode << " \n"; }  

  if(fEnergyDistributionMode==0)
  {
    std::cout<<"fEnergyDistributionMode=0 - Muons will follow a uniform distribution between the values: " << fEnergyRange.first<< " " << fEnergyRange.second <<" GeV"<< std::endl;
  }else if (fEnergyDistributionMode==1)
  {
    std::cout<<" fEnergyDistributionMode=1 - Sending muons following the distribution contained on file: " << fInputFileNameEnergy <<std::endl;
  }
  else{ throw cet::exception("CRTGenPdvd") << "unknown fEnergyDistributionMode value " << fEnergyDistributionMode << " \n"; }  


  if(fmode==1)
  {
    TFile *fInputFileCRT = new TFile(fInputFileNameCRT.c_str(),"READ");
    // check that the file is a good one
    if(!fInputFileCRT->IsOpen())
      throw cet::exception("CRTGenPdvd")<<"input text file "<< fInputFileNameCRT << " cannot be read.\n";

    TH2D *h = (TH2D*)fInputFileCRT->Get("CRTTop");
    if (!h) 
      throw cet::exception("CRTGenPdvd")<<"TH2D named CRTTop not found in "<< fInputFileNameCRT<< ".\n";
    CRTTop = *h;

    h = (TH2D*)fInputFileCRT->Get("CRTBot");
    if(!h) 
      throw cet::exception("CRTGenPdvd")<<"TH2D named CRTBot not found in "<< fInputFileNameCRT<< ".\n";
    CRTBot = *h;
    fInputFileCRT->Close();

  }

  if(fEnergyDistributionMode==1)
  {
    TFile *fInputFileEnergy = new TFile(fInputFileNameEnergy.c_str(),"READ");
    // check that the file is a good one---
    if(!fInputFileEnergy->IsOpen())
      throw cet::exception("CRTGenPdvd") <<"input root file "<<fInputFileNameEnergy<< " cannot be read.\n";

    TH1D *h = (TH1D*)fInputFileEnergy->Get("EnergyDistribution");
    if(!h) 
      throw cet::exception("CRTGenPdvd") << "TH1D named EnergyDistribution not found in "<< fInputFileNameEnergy<< ".\n";
    EnergyDistribution = *h;
    fInputFileEnergy->Close();
  }

  art::ServiceHandle<art::TFileService> tfs;

  fTH2CRTTop = tfs->make<TH2F>("CRTTop","Start position at CRT Top",50,0,0,50,0,0);
  fTH2CRTBot = tfs->make<TH2F>("CRTBot","Expected end track position at CRT Bot",50,0,0,50,0,0);
  fTH1Energy = tfs->make<TH1F>("TH1Energy","Muon energy GeV",1000,0,0);
  fTH1CRTTopX = tfs->make<TH1F>("CRTTopX","Start X pos at CRT Top",50,0,0);
  fTH1CRTTopZ = tfs->make<TH1F>("CRTTopZ","Start Z pos at CRT Top",50,0,0);
  fTH1CRTBotX = tfs->make<TH1F>("CRTBotX","End X pos at CRT Bot",50,0,0);
  fTH1CRTBotZ = tfs->make<TH1F>("CRTBotZ","End Z pos at CRT Bot",50,0,0);

  //Added by Shu, 20231017---
  fTheCRTTree = tfs->make<TTree>("CRTResponse", "CRTResponse");
  fTheCRTTree->Branch("CRTTop_posX", &fCRTTop_posX, "CRTTop_posX/D");
  fTheCRTTree->Branch("CRTTop_posZ", &fCRTTop_posZ, "CRTTop_posZ/D");
  fTheCRTTree->Branch("CRTBot_posX", &fCRTBot_posX, "CRTBot_posX/D");
  fTheCRTTree->Branch("CRTBot_posZ", &fCRTBot_posZ, "CRTBot_posZ/D");

}

//------------------------------------------------------------------------------
void evgen::CRTGenPdvd::beginRun(art::Run& run)
{
  art::ServiceHandle<geo::Geometry const> geo;
  run.put(std::make_unique<sumdata::RunData>(geo->DetectorName()), art::fullRun());
  //driftcoordinate = geo->TPC(geo::TPCID{0, 0}).DetectDriftDirection();
  //Modified by Shu, 20230914---
  //+1: +x; +2: +y; +3: +z; -1: -x; -2: -y; -3: -z; 0: other(or algorithm failed)---
  driftcoordinate = 1;

  if(driftcoordinate==1){
	    std::cout<<" drift coordinate: "<<driftcoordinate<< " (X direction)"<<std::endl;
  }
  else{ throw cet::exception("CRTGenPdvd")<<"unknown drift coordinate: "<< driftcoordinate<<"\n"; }

 }

//------------------------------------------------------------------------------
void evgen::CRTGenPdvd::produce(art::Event & e)
{

  std::unique_ptr<std::vector<simb::MCTruth>> truthcol(new std::vector<simb::MCTruth>);
  simb::MCTruth truth;

  // declare the variables for reading in the event record---
  int 	 pdg = 13;
  double energy;

  //The case of reading input root file(real data)---
  if(fEnergyDistributionMode==1) 
    energy = EnergyDistribution.GetRandom();
  else 
    energy = CLHEP::RandFlat::shoot(fEnergyRange.first,fEnergyRange.second);//uniform distribution; pure simulation---

  double mass = 0.1056583745;//muon mass in GeV---
  double xPosition, yPosition, zPosition, xPositionEnd, yPositionEnd, zPositionEnd;

  if(fmode==1)//The case of reading input root file(real data)---
  {
    CRTTop.GetRandom2(zPosition, xPosition);//CRT panel is in z-x plane---
    CRTBot.GetRandom2(zPositionEnd, xPositionEnd);
    yPosition = CRT_TOP_center[1];
    yPositionEnd = CRT_BOT_center[1];

    fTH2CRTTop->Fill(zPosition, xPosition);
    fTH2CRTBot->Fill(zPositionEnd, xPositionEnd);
  }
  else//Case of uniformly generating positions at CRTTop and CRTBot---
  {
    xPosition = CLHEP::RandFlat::shoot(CRT_TOP_center[0]-0.5*CRTSizeX, CRT_TOP_center[0]+0.5*CRTSizeX);
    yPosition = CLHEP::RandFlat::shoot(CRT_TOP_center[1]-0.5*CRTSizeY, CRT_TOP_center[1]+0.5*CRTSizeY);
    zPosition = CLHEP::RandFlat::shoot(CRT_TOP_center[2]-0.5*CRTSizeZ, CRT_TOP_center[2]+0.5*CRTSizeZ);

    xPositionEnd = CLHEP::RandFlat::shoot(CRT_BOT_center[0]-0.5*CRTSizeX-BufferLengthOnCRTBottom, CRT_BOT_center[0]+0.5*CRTSizeX+BufferLengthOnCRTBottom);
    yPositionEnd = CLHEP::RandFlat::shoot(CRT_BOT_center[1]-0.5*CRTSizeY, CRT_BOT_center[1]+0.5*CRTSizeY);
    zPositionEnd = CLHEP::RandFlat::shoot(CRT_BOT_center[2]-0.5*CRTSizeZ-BufferLengthOnCRTBottom, CRT_BOT_center[2]+0.5*CRTSizeZ+BufferLengthOnCRTBottom);
    
    fTH2CRTTop->Fill(zPosition, xPosition);
    fTH2CRTBot->Fill(zPositionEnd, xPositionEnd);
    fTH1CRTTopX->Fill(xPosition);
    fTH1CRTTopZ->Fill(zPosition);
    fTH1CRTBotX->Fill(xPositionEnd);
    fTH1CRTBotZ->Fill(zPositionEnd);

    std::cout<<"\n---TopCRT: ("<<xPosition<<", "<<yPosition<<", "<<zPosition<<")"<<std::endl;
    std::cout<<"---BotCRT: ("<<xPositionEnd<<", "<<yPositionEnd<<", "<<zPositionEnd<<")\n"<<std::endl;

    //Added by Shu, 20231017---
    fCRTTop_posX = xPosition;
    fCRTTop_posZ = zPosition;
    fCRTBot_posX = xPositionEnd;
    fCRTBot_posZ = zPositionEnd;

    fTheCRTTree->Fill();

  }

  
  double time	= 0.;
  double totmom = sqrt(pow(energy,2)-pow(mass,2));

  double dx = xPositionEnd-xPosition;
  double dy = yPositionEnd-yPosition;
  double dz = zPositionEnd-zPosition;
  double norm = sqrt(pow(dx,2)+pow(dy,2)+pow(dz,2));
  dx /= norm;
  dy /= norm;
  dz /= norm;

  double xMomentum = dx*totmom;
  double yMomentum = dy*totmom;
  double zMomentum = dz*totmom;

  std::cout<<"\n\n\n"<<std::endl;
  std::cout<<"===Cosmic Muon Information================="<<std::endl;

  mf::LogPrint("CRTGenPdvd")<<"Shooting muon on ("<<xPosition<<", "<<yPosition<<", "<<zPosition<<") to ("<<xPositionEnd<<", "<<yPositionEnd<<", "<<zPositionEnd<<") [cm]"<<std::endl;
  std::cout<<"With momentum: ("<<xMomentum<<", "<<yMomentum;
  std::cout<<", "<<zMomentum<<") [GeV]"<<std::endl;
  std::cout<<"Energy = "<<energy<<" GeV; mass = "<<mass<<std::endl;

  std::cout<<"==========================================="<<std::endl;
  std::cout<<"\n\n\n"<<std::endl;


  TLorentzVector pos(xPosition, yPosition, zPosition, time);
  TLorentzVector mom(xMomentum, yMomentum, zMomentum, energy);

  fTH1Energy->Fill(energy);

  simb::MCParticle part(-1, pdg, "primary");
  part.AddTrajectoryPoint(pos, mom);

  truth.Add(part);

  truthcol->push_back(truth);

  e.put(std::move(truthcol));
}

DEFINE_ART_MODULE(evgen::CRTGenPdvd)
