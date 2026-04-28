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
    cout<<"Xa info: "<<XA_posX[2]*2<<endl;

    //Test the distance calculation---
    Point3D p = {1.0, 2.0, 3.0};
    Point3D s = {0.0, 0.0, 0.0};
    Point3D e   = {6.0, 7.0, 8.0};

    double d = distance_point_line(p, s, e);
    cout<<"Distance between Point ("<<p.x<<","<<p.y<<","<<p.z<<")";
    cout<<" and Line segment ("<<s.x<<","<<s.y<<","<<s.z<<")&";
    cout<<"("<<e.x<<","<<e.y<<","<<e.z<<")";
    cout<<" is: "<<d<<endl;





    //extract information from .root file---------------------------------
    string file_suffix = "CRT_300num_hist";
    string file_name = "./root_data/" + file_suffix + ".root";
    cout<<"Root file : "<<file_name<<endl;

    TH1F* XA_counts[16];//16 XArapucas---
    TH2F* CRT_XA_response = new TH2F("CRT_XA", "CRT_XA", 300, 0, 300, 400, 0, 80000);

    for(Int_t i=0; i<16; ++i){
        XA_counts[i] = new TH1F("h_PhotonAtXA " + TString(to_string(i)), "h_PhotonAtXA " + TString(to_string(i)), 500, 0, 10000); 
    }

    //Read in ROOT file and get OpDets TTree---
    TFile *f = new TFile(file_name.c_str());
    TTree *OpDetsCRT = (TTree*)f->Get("generator/CRTResponse");
    TTree *OpDetsXA = (TTree*)f->Get("XAresponse/OpDets");

    //Declare data names---
    Double_t CRTTop_posX, CRTTop_posZ, CRTBot_posX, CRTBot_posZ;
    Int_t OpChannel, CountDetected;
    Double_t distance;//distance between point and line---
    Point3D XA_center = {0.0, 0.0, 0.0};
    Point3D CRT_start = {0.0, 0.0, 0.0};
    Point3D CRT_end = {0.0, 0.0, 0.0};    
    Double_t CRTTop_posY = 450.0;
    Double_t CRTBot_posY = -450.0;

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

        for(Int_t j=i*16; j<(i+1)*16; ++j){
            OpDetsXA->GetEntry(j);
            XA_counts[(Int_t)OpChannel]->Fill(CountDetected);
            //Used to test, 20231008---
            if(OpChannel>3 && OpChannel<12){
                cout<<"------Entry "<<j<<" Channel "<<(Int_t)OpChannel;
                cout<<" counts: "<<CountDetected<<endl;
                cout<<"XA position: ("<<XA_posX[(Int_t)OpChannel]<<", ";
                cout<<XA_posY[(Int_t)OpChannel]<<", "<<XA_posZ[(Int_t)OpChannel]<<")"<<endl;

                XA_center.x = XA_posX[(Int_t)OpChannel];
                XA_center.y = XA_posY[(Int_t)OpChannel];
                XA_center.z = XA_posZ[(Int_t)OpChannel];
                CRT_start.x = CRTTop_posX;
                CRT_start.y = CRTTop_posY;
                CRT_start.z = CRTTop_posZ;
                CRT_end.x   = CRTBot_posX; 
                CRT_end.y   = CRTBot_posY;
                CRT_end.z   = CRTBot_posZ;
                distance = distance_point_line(XA_center, CRT_start, CRT_end);
                cout<<"Distance: "<<distance<<endl; 

                CRT_XA_response->Fill(distance, CountDetected);
            }

        }
    }


/*    for(Int_t i = 0; i<num_entries; i++){
	      OpDetsXA->GetEntry(i);
	      XA_counts[(Int_t)OpChannel]->Fill(CountDetected);

        //Used to test, 20231008---
        if(OpChannel>3 && OpChannel<12)
            cout<<"Entry "<<i<<" Channel "<<(Int_t)OpChannel<<" has counts: "<<CountDetected<<endl;

    }
*/

    //Draw histograms & put them in a ROOT file---
    TFile *f_out = new TFile("./analyses_results/Analysis_"+TString(file_suffix)+".root", "RECREATE"); 
    f_out->cd();//This is VERY Important!!!---

    for(Int_t i=0; i<16; i++){
	    XA_counts[i]->SetTitle("Photons reaching XArapuca #" + TString(to_string(i)));
	    XA_counts[i]->GetXaxis()->SetTitle("Number of Photons");
	    XA_counts[i]->GetXaxis()->CenterTitle(true);
	    XA_counts[i]->Write();
    }
   
    CRT_XA_response->SetTitle("Photon counts vs distance to cosmic muon track");
	CRT_XA_response->GetXaxis()->SetTitle("Distance [cm]");
	CRT_XA_response->GetYaxis()->SetTitle("Counts");
    CRT_XA_response->SetMarkerStyle(21);
    CRT_XA_response->SetMarkerSize(1.0);
    CRT_XA_response->Write();

    f_out->Write();
    f_out->Close();




}