#include <iostream>
#include "./lib/XArapucaInfo.h"
#include "./lib/distance.h"

#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <string.h>
#include <stdio.h>
#include <vector>

using namespace std;

void cathodeXA() {
    //Test if XArapuca works---
    cout<<"Opch info: "<<Opch_posX[2]*2<<endl;

    //Test the distance calculation---
    Point3D p = {1.0, 1.0, 0.0};
    Point3D s = {0.0, 2.0, 1.0};
    Point3D e   = {0.0, 0.0, 1.0};

    double d = distance_point_line(p, s, e);
    cout<<"\nSimple test-----------------"<<endl;
    cout<<"Distance between Point ("<<p.x<<","<<p.y<<","<<p.z<<")";
    cout<<" and Line segment ("<<s.x<<","<<s.y<<","<<s.z<<")&";
    cout<<"("<<e.x<<","<<e.y<<","<<e.z<<")";
    cout<<" is: "<<d<<endl;
    cout<<"-----------------------------\n"<<endl;




    //extract information from .root file of CRT+graphModule LArSoft simulation------------


    //change file name each time-----------------------
    string file_suffix = "rsl99_500num_hist";
    //-------------------------------------------------


    string file_name = "./root_data/" + file_suffix + ".root";
    cout<<"Root file : "<<file_name<<endl;

    TH1F* Opch_counts[40];//40 XArapucas---
    TH2F* CRT_XA_response = new TH2F("CRT_Opch", "CRT_Opch", 100, 0, 300, 100, 0, 200000);

    for(Int_t i=0; i<40; ++i){
        Opch_counts[i] = new TH1F("h_PhotonAtOpch " + TString(to_string(i)), "h_PhotonAtOpch " + TString(to_string(i)), 1500, 0, 300000); 
    }

    //Read in ROOT file and get OpDets TTree---
    TFile *f = new TFile(file_name.c_str());
    TTree *OpDetsCRT = (TTree*)f->Get("generator/CRTResponse");
    TTree *OpDetsXA = (TTree*)f->Get("XAresponse/OpDets");

    //Declare data names---
    Double_t CRTTop_posX, CRTTop_posZ, CRTBot_posX, CRTBot_posZ;
    Int_t OpChannel, CountDetected;
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

    Int_t num_entriesCRT = (Int_t)OpDetsCRT->GetEntries();
    Int_t num_entries = (Int_t)OpDetsXA->GetEntries();

    for(Int_t i=0; i<num_entriesCRT; ++i){
        OpDetsCRT->GetEntry(i);
        cout<<"\n"<<i<<", CRT Top: ("<<CRTTop_posX<<", "<<CRTTop_posY<<", ";
        cout<<CRTTop_posZ<<");   CRT Bot: ("<<CRTBot_posX<<", ";
        cout<<CRTBot_posY<<", "<<CRTBot_posZ<<")"<<endl;        

        for(Int_t j=i*40; j<(i+1)*40; ++j){
            OpDetsXA->GetEntry(j);
            Opch_counts[(Int_t)OpChannel]->Fill(CountDetected);

            //Only cathode XA are used here------------
            if(OpChannel>10 && OpChannel<12){
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


/*    for(Int_t i = 0; i<num_entries; i++){
	      OpDetsXA->GetEntry(i);
	      Opch_counts[(Int_t)OpChannel]->Fill(CountDetected);

        //Used to test, 20231008---
        if(OpChannel>3 && OpChannel<12)
            cout<<"Entry "<<i<<" Channel "<<(Int_t)OpChannel<<" has counts: "<<CountDetected<<endl;

    }
*/

    //Draw histograms & put them in a ROOT file---
    //-------------------------------------------------------
    TFile *f_out = new TFile("./results/rsl99_500num/opCh11_"+TString(file_suffix)+".root", "RECREATE"); 
    f_out->cd();//This is VERY Important!!!---

    for(Int_t i=0; i<40; i++){
	    Opch_counts[i]->SetTitle("Photons reaching Opch #" + TString(to_string(i)));
	    Opch_counts[i]->GetXaxis()->SetTitle("# of Photons");
	    Opch_counts[i]->GetXaxis()->CenterTitle(true);
	    Opch_counts[i]->Write();
    }
   
    //CRT_XA_response->SetTitle("Photon counts vs distance to cosmic muon track");
    CRT_XA_response->SetTitle("OpCh11");
	CRT_XA_response->GetXaxis()->SetTitle("Distance [cm]");
	CRT_XA_response->GetYaxis()->SetTitle("#photon / event");
    CRT_XA_response->SetMarkerStyle(21);
    CRT_XA_response->SetMarkerSize(1.0);
    CRT_XA_response->Write();
    //-------------------------------------------------------

    f_out->Write();
    f_out->Close();




}
