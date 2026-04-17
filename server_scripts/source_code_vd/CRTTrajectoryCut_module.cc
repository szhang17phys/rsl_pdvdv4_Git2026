/**
  This is initially developed by Jose Soto, to select cosmic muons
  going through both crt panels, based on the previous protoDUNE-DP (y-drift)
  geometry.
  Later, Shu (Shuaixiang Zhang) slightly modified this script, to make it 
  fitting the geomtry of protoDUNE-VD (x-drift).

  Jan 17, 2024---

*/


#include <functional>   // std::greater

//ROOT includes
#include "TH1I.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "TMath.h"
#include "TGeoMatrix.h" // TGeoHMatrix

// LArSoft libraries

//LArSoft includes
#include "larcore/Geometry/Geometry.h"

#include "larcorealg/Geometry/LocalTransformationGeo.h"
#include "larcorealg/Geometry/WireGeo.h"

#include "larcoreobj/SimpleTypesAndConstants/RawTypes.h"
#include "larcoreobj/SimpleTypesAndConstants/geo_vectors.h"

#include "lardataobj/RawData/RawDigit.h"
#include "lardataobj/RawData/raw.h"
#include "lardataobj/RecoBase/Hit.h"
#include "lardataobj/RecoBase/OpHit.h"
#include "lardataobj/RecoBase/Wire.h"
#include "lardataobj/Simulation/sim.h"
#include "lardataobj/Simulation/SimChannel.h"
#include "lardataobj/Simulation/SupernovaTruth.h"

#include "lardata/DetectorInfoServices/DetectorPropertiesService.h"
// #include "lardata/DetectorInfoServices/DetectorClocksService.h"

#include "larsim/MCCheater/BackTrackerService.h"
#include "larsim/MCCheater/ParticleInventoryService.h"
#include "larsim/MCCheater/PhotonBackTrackerService.h"

//ART includes
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "art_root_io/TFileDirectory.h"
#include "art_root_io/TFileService.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "canvas/Utilities/InputTag.h"
#include "canvas/Persistency/Common/FindMany.h"
#include "canvas/Persistency/Common/FindManyP.h"
#include "canvas/Persistency/Common/FindOneP.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"


class CRTTrajectoryCut : public art::EDAnalyzer {

public:
  explicit CRTTrajectoryCut(fhicl::ParameterSet const & p);

  CRTTrajectoryCut(CRTTrajectoryCut const &) = delete;
  CRTTrajectoryCut(CRTTrajectoryCut &&) = delete;
  CRTTrajectoryCut & operator = (CRTTrajectoryCut const &) = delete;
  CRTTrajectoryCut & operator = (CRTTrajectoryCut &&) = delete;

  void analyze(art::Event const & evt) override;
  void reconfigure(fhicl::ParameterSet const & p);
  void beginJob() override;
  void endJob() override;

private:

  std::string fGEANTLabel, fSIGNLabel;
  TTree* fAnaTree;
  int Run;
  int SubRun;
  int Event;
  bool CUT;
  float Energy;
  float _EntryPoint[3];
  float _ExitPoint[3];

  art::ServiceHandle<geo::Geometry> geo;
  art::ServiceHandle<cheat::BackTrackerService> bt_serv;
  art::ServiceHandle<cheat::ParticleInventoryService> pi_serv;
  art::ServiceHandle<cheat::PhotonBackTrackerService> pbt_serv;
  int counter, NEvents;

};

CRTTrajectoryCut::CRTTrajectoryCut(fhicl::ParameterSet const & p):EDAnalyzer(p)
{
  this->reconfigure(p);
}


void CRTTrajectoryCut::reconfigure(fhicl::ParameterSet const & p)
{
  fGEANTLabel = p.get<std::string> ("GEANT4Label"  );
  fSIGNLabel = p.get<std::string> ("fSIGNLabel"  );
  std::cout << "Reconfigured " << this->processName() << std::endl;
}




void CRTTrajectoryCut::beginJob()
{
  art::ServiceHandle<art::TFileService> tfs;

  fAnaTree = tfs->make<TTree>("CRTTrajectoryCut","CRT muon MC truth data");
  fAnaTree->Branch("Run"       , &Run       , "Run/I"       );
  fAnaTree->Branch("SubRun"    , &SubRun    , "SubRun/I"    );
  fAnaTree->Branch("Event"     , &Event     , "Event/I"     );
  fAnaTree->Branch("Energy"     , &Energy     , "Energy/F"     );
  fAnaTree->Branch("CrossingMuon"     , &CUT     , "CrossingMuon/O"     );
  fAnaTree->Branch("CRT_EntryPoint"   , _EntryPoint       	, "CRT_EntryPoint[3]/F");
  fAnaTree->Branch("CRT_ExitPoint"    , _ExitPoint		, "CRT_ExitPoint[3]/F"  );
  counter=0; NEvents=0;
}


void CRTTrajectoryCut::analyze(art::Event const & evt)
{

  Run    = evt.run();
  SubRun = evt.subRun();
  Event  = evt.event();

  //auto const* geo = lar::providerFrom<geo::Geometry>();
  std::cout << "Event " << Event << std::endl;
  art::Handle< std::vector<simb::MCTruth> > TrueMuon;


  //convert vertical-drift (driftX) geometry (protoDUNE-VD) to driftY:
  //In driftX, Top CRT: (x', y', z') = (384.8, 588.2, 149.6)
  //Bottowm crt: (x', y', z') = (-440.8, -588.2, 149.6) 
  //For driftY, x = -y', y = x', z = z' 
  std::vector<double> CRT_TOP_center_driftX={384.8, 588.2, 149.6};//cm---
  std::vector<double> CRT_BOT_center_driftX={-440.8, -588.2, 149.6}; 
  double CRTHeight=8*14.0; //size in the vertical direction
  double CRTLength=144; //size in Z - Along the scintillator bars.





  if(evt.getByLabel(fSIGNLabel, TrueMuon))
  {
    std::cout << "Found a true part" << std::endl;
    art::FindManyP<simb::MCParticle> MuonAssn(TrueMuon,evt,fGEANTLabel);

    for(size_t i1=0; i1<TrueMuon->size(); ++i1)
    {
      const simb::MCParticle ThisPar = (*MuonAssn.at(i1).at(MuonAssn.at(i1).size()-1));
      std::cout<<"\n"<<std::endl;
      std::cout<<"(x, y, z, PDG, TrackID) = " << ThisPar.Vx() << " " << ThisPar.Vy() << " " << ThisPar.Vz() << " - " << ThisPar.PdgCode() << " " << ThisPar.TrackId() << std::endl;
      std::cout<<"\n"<<std::endl;

      Energy=ThisPar.E();
      int j=0;
      _EntryPoint[0]=ThisPar.Vx();
      _EntryPoint[1]=ThisPar.Vy();
      _EntryPoint[2]=ThisPar.Vz();

      //Find the segment of the track closest to bot crt---
      while(ThisPar.Trajectory().Position(j).Y()>=CRT_BOT_center_driftX[1] && j< static_cast<int>(ThisPar.Trajectory().size()-1) ) {j++;}

      std::cout<<"Information of segment of track close to bot crt: "<<std::endl;
      ThisPar.Trajectory().Position(j-1).Print(); 
      std::cout<<"Segment closing to bot crt: "<<std::endl;
      ThisPar.Trajectory().Position(j).Print(); 

      if(ThisPar.Trajectory().Position(j).Y()<CRT_BOT_center_driftX[1] && ThisPar.Trajectory().Position(j-1).Y()>=CRT_BOT_center_driftX[1])
      {
        double posX = (ThisPar.Trajectory().Position(j).X() -ThisPar.Trajectory().Position(j-1).X())*(CRT_BOT_center_driftX[1]-ThisPar.Trajectory().Position(j-1).Y())/(ThisPar.Trajectory().Position(j).Y() -ThisPar.Trajectory().Position(j-1).Y()) + ThisPar.Trajectory().Position(j-1).X();
        double posZ = (ThisPar.Trajectory().Position(j).Z() -ThisPar.Trajectory().Position(j-1).Z())*(CRT_BOT_center_driftX[1]-ThisPar.Trajectory().Position(j-1).Y())/(ThisPar.Trajectory().Position(j).Y() -ThisPar.Trajectory().Position(j-1).Y()) + ThisPar.Trajectory().Position(j-1).Z();
        
        
        std::cout<<"\n\nSegment on the left of crt (y > y[bot_crt]): "<<std::endl;
        ThisPar.Trajectory().Position(j-1).Print(); 
        std::cout<<"Segment on the right of crt (y < y[bot_crt]): "<<std::endl;
        ThisPar.Trajectory().Position(j).Print(); 
        std::cout << "\nInterpoled: " << std::endl;
        std::cout <<"posX: "<< posX << ", posZ: " << posZ << std::endl;

        _ExitPoint[1]=CRT_BOT_center_driftX[1];
        _ExitPoint[0]=posX;
        _ExitPoint[2]=posZ;

        if(   posX > CRT_BOT_center_driftX[0]-CRTHeight*0.5
           && posX < CRT_BOT_center_driftX[0]+CRTHeight*0.5
           && posZ > CRT_BOT_center_driftX[2]-CRTLength*0.5
           && posZ < CRT_BOT_center_driftX[2]+CRTLength*0.5 )
          {
           std::cout << "\n\n---------- IT CROSSES! ----------" << std::endl; 
           counter++;
           CUT=true;
          }
        else{
            std::cout << "\n\n---------- NOT CROSSES ----------" << std::endl; 
            CUT=false;
            }
      }
      else{
          std::cout << "\n\n---------- IT NEVER GETS EVEN CLOSE ----------\n\n\n" << std::endl; 
          CUT=false;
          }
    }
  }

  fAnaTree->Fill(); NEvents++;
}



void CRTTrajectoryCut::endJob()
{
  std::cout << "\n\n\n====== Conclusion: ======" << std::endl;
  std::cout << "#(Events through both crts): " << counter << "; #(Total Events): " << NEvents << std::endl;
  std::cout << "----------------------------\n\n\n" << std::endl;
}

DEFINE_ART_MODULE(CRTTrajectoryCut)
