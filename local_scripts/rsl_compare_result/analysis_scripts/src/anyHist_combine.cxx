#include <iostream>
#include <TFile.h>
#include <TH2.h>
#include <TH1.h>
#include <TList.h>

//Instruction:
//Only change input_path, input_name 

int anyHist_combine(){

    //Open initial root files----------------------------------------
    //Open the first ROOT file---
    TFile *file1 = new TFile("./root_data/rsl50_1000num_e67_hist.root", "READ");
    if (!file1 || file1->IsZombie()) {
        std::cerr << "Error: Unable to open file1.root" << std::endl;
        return 1;
    }
    //Open the second root file---
    TFile *file2 = new TFile("./root_data/rsl50_1000num_e67_hist.root", "READ");
    

    //Get access to histos inside root files-----------------
    TH2F *top1 = dynamic_cast<TH2F*>(file1->Get("generator/CRTTop"));
    TH2F *bot1 = dynamic_cast<TH2F*>(file1->Get("generator/CRTBot"));
    TH1F *energy1 = dynamic_cast<TH1F*>(file1->Get("generator/TH1Energy"));
    TH1F *cross1 = dynamic_cast<TH1F*>(file1->Get("TrajCut/CRTTrajectoryCut/CrossingMuon"));

    TH2F *top2 = dynamic_cast<TH2F*>(file1->Get("generator/CRTTop"));
    TH2F *bot2 = dynamic_cast<TH2F*>(file1->Get("generator/CRTBot"));
    TH1F *energy2 = dynamic_cast<TH1F*>(file1->Get("generator/TH1Energy"));
    TH1F *cross2 = dynamic_cast<TH1F*>(file1->Get("TrajCut/CRTTrajectoryCut/CrossingMuon")); 
    
   
    //Create new hist to store cathode 8 XAs-----------------------
    TH2F *top_combine = new TH2F(*top1);
    top_combine->Merge(top1);
    top_combine->Merge(top2);


/*    hist_cathode8XA->SetName("summedCathode8XA");
    hist_cathode8XA->SetTitle("Response of Cathode 8 XAs");
    hist_cathode8XA->GetXaxis()->SetTitle("Distance [cm]");
	hist_cathode8XA->GetYaxis()->SetTitle("#photon / event");
    hist_cathode8XA->SetMarkerStyle(21);
    hist_cathode8XA->SetMarkerSize(1.0);
*/


    
   //Create NEW root file-------------------
    TFile *outputFile = new TFile("./results/Combine_crt_energy.root", "RECREATE");
    if (!outputFile || outputFile->IsZombie()) {
        std::cerr << "Error: Unable to create combined_file.root" << std::endl;
        return 1;
    }

    // Write the combined histogram to the new ROOT file----------
    top_combine->Write();


    delete file1;
    delete file2;  
    delete outputFile;

    return 0;
}
