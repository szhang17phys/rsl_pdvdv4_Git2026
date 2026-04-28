#include <iostream>
#include "../lib/XArapucaInfo.h"
#include "../lib/distance.h"

#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <string.h>
#include <stdio.h>
#include <vector>

//Instruction:
//Only change file_suffix, output_path, opch, opch_string & TH2F template---

using namespace std;

void singleOpCh1() {

    //extract information from .root file of CRT+graphModule LArSoft simulation------------
    //change file name each time-----------------------
    string file_suffix = "rsl50_1000num2_e67_hist";//initial root file name---
    string output_path = "../results/rsl50_1000num2_e67_crtCut/opCh";
    //-------------------------------------------------

    //OpCh we are interested------
    Int_t opch = 0;
    string opch_string = "00";//Must double quotes!!!
    //----------------------------



    string file_name = "../root_data/" + file_suffix + ".root";
    cout<<"Root file : "<<file_name<<endl;

    TH1F* Opch_counts[40];//40 XArapucas---

    //For membrane XA 0, 2, 17, 23---
    //Old: 100, 0, 200000; Current: 100, 0, 5000000
    TH2F* CRT_XA_response = new TH2F("CRT_Opch", "CRT_Opch", 100, 0, 300, 100, 0, 200000);

    //For membrane XA 1, 3, 16, 22---
    //Old: 100, 0, 2000; Current: 100, 0, 50000
    //TH2F* CRT_XA_response = new TH2F("CRT_Opch", "CRT_Opch", 100, 0, 600, 100, 0, 2000);

    //For Cathode XA 4, 5, 6, 7, 8, 9, 10, 11---
    //Old: 100, 0, 200000; Current: 100, 0, 5000000
    //TH2F* CRT_XA_response = new TH2F("CRT_Opch", "CRT_Opch", 100, 0, 300, 100, 0, 200000);

    //For PMTs 31, 33---
    //Old: 100, 0, 20000; Current: 100, 0, 500000
    //TH2F* CRT_XA_response = new TH2F("CRT_Opch", "CRT_Opch", 100, 0, 300, 100, 0, 20000);

    //For all left PMTs, 12~15, 18~21, 24~29, 30, 32, 34~39---
    //Old: 100, 0, 2000; Current: 100, 0, 50000
    //TH2F* CRT_XA_response = new TH2F("CRT_Opch", "CRT_Opch", 100, 0, 600, 100, 0, 2000);        


    for(Int_t i=0; i<40; ++i){
        Opch_counts[i] = new TH1F("h_PhotonAtOpch " + TString(to_string(i)), "h_PhotonAtOpch " + TString(to_string(i)), 2000, 0, 1000000); 
    }

    //Read in ROOT file and get OpDets TTree---
    TFile *f = new TFile(file_name.c_str());
    TTree *OpDetsCRT = (TTree*)f->Get("generator/CRTResponse");
    TTree *CrtCut = (TTree*)f->Get("TrajCut/CRTTrajectoryCut");
    TTree *OpDetsXA = (TTree*)f->Get("XAresponse/OpDets");

    //Declare data names---
    Double_t CRTTop_posX, CRTTop_posZ, CRTBot_posX, CRTBot_posZ;
    Int_t OpChannel, CountDetected;
    Bool_t cross;//Bot CRT Cut---
    Int_t pass = 0;

    Double_t distance;//distance between point and line---
    Point3D Opch_center = {0.0, 0.0, 0.0};//Initialization---
    Point3D CRT_start = {0.0, 0.0, 0.0};
    Point3D CRT_end = {0.0, 0.0, 0.0};    

    Double_t CRTTop_posY = 588.2;//units here is cm---
    Double_t CRTBot_posY = -588.2;

    //Set Branch addresses---
    OpDetsCRT->SetBranchAddress("CRTTop_posX", &CRTTop_posX);
    OpDetsCRT->SetBranchAddress("CRTTop_posZ", &CRTTop_posZ);
    OpDetsCRT->SetBranchAddress("CRTBot_posX", &CRTBot_posX);
    OpDetsCRT->SetBranchAddress("CRTBot_posZ", &CRTBot_posZ);
    OpDetsXA->SetBranchAddress("OpChannel", &OpChannel);
    OpDetsXA->SetBranchAddress("CountDetected", &CountDetected);
    CrtCut->SetBranchAddress("CrossingMuon", &cross);

    Int_t num_entriesCRT = (Int_t)OpDetsCRT->GetEntries();
    Int_t num_entries = (Int_t)OpDetsXA->GetEntries();

    for(Int_t i=0; i<num_entriesCRT; ++i){
        OpDetsCRT->GetEntry(i);
        cout<<"\n"<<i<<", CRT Top: ("<<CRTTop_posX<<", "<<CRTTop_posY<<", ";
        cout<<CRTTop_posZ<<");   CRT Bot: ("<<CRTBot_posX<<", ";
        cout<<CRTBot_posY<<", "<<CRTBot_posZ<<")"<<endl;        

        CrtCut->GetEntry(i);
        if(cross){
            cout<<"\nIt went through bot CRT!\n"<<endl;
            pass++;
        }
        else{ 
            cout<<"\nOUT!\n"<<endl;
            continue; 
        }

        for(Int_t j=i*40; j<(i+1)*40; ++j){
            OpDetsXA->GetEntry(j);
            Opch_counts[(Int_t)OpChannel]->Fill(CountDetected);

            //Only cathode XA are used here------------
            if(OpChannel == opch){
            //-----------------------------------------        
                cout<<"------Entry "<<j<<" Channel "<<(Int_t)OpChannel;
                cout<<" counts: "<<CountDetected<<endl;
                cout<<"Opch position: ("<<Opch_posX[(Int_t)OpChannel]<<", ";
                cout<<Opch_posY[(Int_t)OpChannel]<<", "<<Opch_posZ[(Int_t)OpChannel]<<")"<<endl;

                Opch_center.x = Opch_posX[(Int_t)OpChannel];
                Opch_center.y = Opch_posY[(Int_t)OpChannel];
                Opch_center.z = Opch_posZ[(Int_t)OpChannel];
                CRT_start.x = CRTTop_posX;
                CRT_start.y = CRTTop_posY;
                CRT_start.z = CRTTop_posZ;
                CRT_end.x   = CRTBot_posX; 
                CRT_end.y   = CRTBot_posY;
                CRT_end.z   = CRTBot_posZ;
                distance = distance_point_line(Opch_center, CRT_start, CRT_end);
                cout<<"Distance: "<<distance<<endl; 

                CRT_XA_response->Fill(distance, CountDetected);
            }

        }
    }



    cout<<"\n\n#muon through bot CRT: "<<pass<<endl;
    std::cout << std::fixed << std::setprecision(2) << "Pass Rate: " << (static_cast<double>(pass)/num_entriesCRT)*100 <<"%\n\n"<< std::endl;

    //Draw histograms & put them in a ROOT file---
    //-------------------------------------------------------
    TFile *f_out = new TFile(TString(output_path)+TString(opch_string)+"_"+TString(file_suffix)+".root", "RECREATE"); 
    f_out->cd();//This is VERY Important!!!---

    for(Int_t i=0; i<40; i++){
	    Opch_counts[i]->SetTitle("Photons reaching Opch #" + TString(to_string(i)));
	    Opch_counts[i]->GetXaxis()->SetTitle("# of Photons");
	    Opch_counts[i]->GetXaxis()->CenterTitle(true);
	    Opch_counts[i]->Write();
    }
   
    //CRT_XA_response->SetTitle("Photon counts vs distance to cosmic muon track");
    CRT_XA_response->SetTitle(TString(opch_string));
	CRT_XA_response->GetXaxis()->SetTitle("Distance [cm]");
	CRT_XA_response->GetYaxis()->SetTitle("#photon / event");
    CRT_XA_response->SetMarkerStyle(21);
    CRT_XA_response->SetMarkerSize(1.0);
    CRT_XA_response->Write();
    //-------------------------------------------------------

    f_out->Write();
    f_out->Close();


}
