#ifndef READROOT_H
#define READROOT_H
//This script is used to read TH1F and TH2F file inside root file---
//Author: Shuaixiang (Shu)---
//Date: Oct 8, 2023---


#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <vector>
#include <TCanvas.h>

//Function used to extract data from 'generator/CRTTop'---
TH2F* copyCRTTop(const char* fileName){

    std::cout<<"\nStart reading root file...\n"<<std::endl;

    TFile* file = TFile::Open(fileName, "READ");

    std::cout<<"\nRead over! \n"<<std::endl;

    if(!file || file->IsZombie()) {
        return nullptr; //Return nullptr on error---
    }

    TH2F* CRTTop = static_cast<TH2F*>(file->Get("generator/CRTTop"));
    if(!CRTTop) {
        file->Close();
        return nullptr; //Return nullptr if "CRTTop" is not found---
    }

    std::cout<<"\nFile Get over! \n"<<std::endl;

    //Create a new TH2F and copy data from CRTTop---
    TH2F* newCRTTop = new TH2F(*CRTTop);

    std::cout<<"\nCreate new TH2F over! \n"<<std::endl;

    file->Close();

    std::cout<<"\nFileClose Get over! \n"<<std::endl;

    return newCRTTop;
}

//Function used to extract data from 'generator/CRTBot'---
TH2F* copyCRTBot(const char* fileName){
    TFile* file = TFile::Open(fileName, "READ");
    if(!file || file->IsZombie()) {
        return nullptr; //Return nullptr on error---
    }

    TH2F* CRTBot = static_cast<TH2F*>(file->Get("generator/CRTBot"));
    if(!CRTBot) {
        file->Close();
        return nullptr; //Return nullptr if "CRTBot" is not found---
    }

    //Create a new TH2F and copy data from CRTBot---
    TH2F* newCRTBot = new TH2F(*CRTBot);

    file->Close();
    return newCRTBot;
}


//Function used to store responses of 40 XArapuca---
std::vector<TH1F*> copyXAs(const char* fileName){
    //Open the ROOT file---
    TFile* file = TFile::Open(fileName, "READ");
    if(!file || file->IsZombie()){
        std::cerr << "Error: Could not open the file." << std::endl;
        return {};
    }

    //Create a vector to store the 40 TH1F histograms---
    std::vector<TH1F*> histograms;//used as output of function---
    TH1F* XA_counts[40];//40 XArapuca in total---
    for(int i=0; i<40; i++) 
        XA_counts[i] = new TH1F("h_PhotonsAtXArapuca" + TString(to_string(i)), "h_PhotonsAtXArapuca" + TString(to_string(i)), 500, 0, 5000); 

    TTree *OpDets = (TTree*)file->Get("XAresponse/OpDets");

    //Declare data names---
    int OpChannel, CountDetected;

    //Set Branch addresses---
    OpDets->SetBranchAddress("OpChannel", &OpChannel);
    OpDets->SetBranchAddress("CountDetected", &CountDetected);

    //Get entries for initial histogram---
    int num_entries = (int)OpDets->GetEntries();
    for(int i=0; i<num_entries; i++){
        OpDets->GetEntry(i);
	    XA_counts[(int)OpChannel]->Fill(CountDetected);
    }

    for(int i=0; i<40; i++){
	    XA_counts[i]->SetTitle("Photons reaching XArapuca #" + TString(to_string(i)));
	    XA_counts[i]->GetXaxis()->SetTitle("# of Photons");
	    XA_counts[i]->GetXaxis()->CenterTitle(true);
	    XA_counts[i]->Write();

        //Add the histogram to the vector---
        histograms.push_back(XA_counts[i]);
    }

    //Close the file---
    file->Close();

    return histograms;

}



void SaveTH2FPlotToImage(TH2F* hist, const char* outputFileName, const char* imageFormat) {
    TCanvas canvas;
    hist->Draw("colz");
    canvas.SaveAs(outputFileName);
    canvas.Close();
}


#endif

