#include <iostream>
#include <TFile.h>
#include <TH2.h>

//Instruction:
//Only change input_path, input_name 

int hist_combine(){
    //Input & output root files===========================

    string rslxx ="corsika_rsl150_abs20";

    //Open the first ROOT file---
    TFile *file0 = new TFile("/Users/shuaixiangzhang/Work/current/FNAL_Work2024/rsl_analyses/new_20250127/rsl_compare_result/results/tmp/Combine_"+TString(rslxx)+"_hist.root", "READ");
    if (!file0 || file0->IsZombie()) {
        std::cerr << "Error: Unable to open file1.root" << std::endl;
        return 1;
    }
    //Open the second & third root file---
    TFile *file1 = new TFile("/Users/shuaixiangzhang/Work/current/FNAL_Work2024/rsl_analyses/new_20250127/rsl_compare_result/results/tmp/Combine_"+TString(rslxx)+"_2_hist.root", "READ");
    TFile *file2 = new TFile("/Users/shuaixiangzhang/Work/current/FNAL_Work2024/rsl_analyses/new_20250127/rsl_compare_result/results/tmp/Combine_"+TString(rslxx)+"_3_hist.root", "READ");



    //Output location:------------
    string output_path = "/Users/shuaixiangzhang/Work/current/FNAL_Work2024/rsl_analyses/new_20250127/rsl_compare_result/results/combined_3k/";

    string output_name = rslxx + "_3000num_crtCut.root";
    //===================================================





    //Get access to histos inside root files----------------------------
    TH2F *hist10 = dynamic_cast<TH2F*>(file0->Get("cathode8XA"));
    TH2F *hist11 = dynamic_cast<TH2F*>(file1->Get("cathode8XA"));  
    TH2F *hist12 = dynamic_cast<TH2F*>(file2->Get("cathode8XA")); 


    TH2F *hist20 = dynamic_cast<TH2F*>(file0->Get("membrane1"));
    TH2F *hist21 = dynamic_cast<TH2F*>(file1->Get("membrane1"));    
    TH2F *hist22 = dynamic_cast<TH2F*>(file2->Get("membrane1"));  


    TH2F *hist30 = dynamic_cast<TH2F*>(file0->Get("membrane2"));
    TH2F *hist31 = dynamic_cast<TH2F*>(file1->Get("membrane2"));    
    TH2F *hist32 = dynamic_cast<TH2F*>(file2->Get("membrane2"));     


    TH2F *hist40 = dynamic_cast<TH2F*>(file0->Get("pmt1"));
    TH2F *hist41 = dynamic_cast<TH2F*>(file1->Get("pmt1"));    
    TH2F *hist42 = dynamic_cast<TH2F*>(file2->Get("pmt1"));  


    TH2F *hist50 = dynamic_cast<TH2F*>(file0->Get("pmt2"));
    TH2F *hist51 = dynamic_cast<TH2F*>(file1->Get("pmt2"));    
    TH2F *hist52 = dynamic_cast<TH2F*>(file2->Get("pmt2"));  


   
    //Create new hist to store cathode 8 XAs-----------------------
    TH2F *hist_cathode8XA = new TH2F(*hist10);
    hist_cathode8XA->Add(hist11);
    hist_cathode8XA->Add(hist12);

    hist_cathode8XA->SetName("summedCathode8XA");
    hist_cathode8XA->SetTitle("Response of Cathode 8 XAs");
    hist_cathode8XA->GetXaxis()->SetTitle("Distance [cm]");
	hist_cathode8XA->GetYaxis()->SetTitle("#photon / event");
    hist_cathode8XA->SetMarkerStyle(21);
    hist_cathode8XA->SetMarkerSize(1.0);


    //Create new hist to store opch 00, 02, 17, 23---------------
    TH2F *hist_m1 = new TH2F(*hist20);
    hist_m1->Add(hist21);
    hist_m1->Add(hist22);

    hist_m1->SetName("summedM1");
    hist_m1->SetTitle("Response of OpCh 00, 02, 17 & 23");
    hist_m1->GetXaxis()->SetTitle("Distance [cm]");
	hist_m1->GetYaxis()->SetTitle("#photon / event");
    hist_m1->SetMarkerStyle(21);
    hist_m1->SetMarkerSize(1.0);

    //Create new hist to store opch 01, 03, 16, 22---------------
    TH2F *hist_m2 = new TH2F(*hist30);
    hist_m2->Add(hist31);
    hist_m2->Add(hist32);

    hist_m2->SetName("summedM2");
    hist_m2->SetTitle("Response of OpCh 01, 03, 16 & 22");
    hist_m2->GetXaxis()->SetTitle("Distance [cm]");
	hist_m2->GetYaxis()->SetTitle("#photon / event");
    hist_m2->SetMarkerStyle(21);
    hist_m2->SetMarkerSize(1.0);


    //Create new hist to store opch 31, 33---------------
    TH2F *hist_p1 = new TH2F(*hist40);
    hist_p1->Add(hist41);
    hist_p1->Add(hist42);

    hist_p1->SetName("summedPMT1");
    hist_p1->SetTitle("Response of OpCh 31 & 33");
    hist_p1->GetXaxis()->SetTitle("Distance [cm]");
	hist_p1->GetYaxis()->SetTitle("#photon / event");
    hist_p1->SetMarkerStyle(21);
    hist_p1->SetMarkerSize(1.0);


    //Create new hist to store opch 12~15, 24~29, 30, 32, 34~39--------
    TH2F *hist_p2 = new TH2F(*hist50);
    hist_p2->Add(hist51);
    hist_p2->Add(hist52);

    hist_p2->SetName("summedPMT2");
    hist_p2->SetTitle("Response of OpCh 12~15, 24~29, 30, 32, 34~39");
    hist_p2->GetXaxis()->SetTitle("Distance [cm]");
	hist_p2->GetYaxis()->SetTitle("#photon / event");
    hist_p2->SetMarkerStyle(21);
    hist_p2->SetMarkerSize(1.0);



   //Create NEW root file--------------------------------------------------
    TFile *outputFile = new TFile(TString(output_path)+TString(output_name), "RECREATE");
    if (!outputFile || outputFile->IsZombie()) {
        std::cerr << "Error: Unable to create combined_file.root" << std::endl;
        return 1;
    }

    // Write the combined histogram to the new ROOT file----------
    hist_cathode8XA->Write();
    hist_m1->Write();
    hist_m2->Write();
    hist_p1->Write();
    hist_p2->Write();


    delete file0;
    delete file1;
    delete file2;   
    delete outputFile;

    return 0;
}
