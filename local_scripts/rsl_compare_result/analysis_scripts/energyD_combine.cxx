#include <iostream>
#include <TFile.h>
#include <TH2.h>
#include <TGraph.h>

//Instruction:
//Only change input_path, input_name 

//Purpose: combine energy related distributions of two different Combine_rslxx_1000numx_e67_hist.root, the product of ./command.sh

int energyD_combine(){

    //Input & output root files===========================
    //Open the first ROOT file---
    TFile *file0 = new TFile("/Users/shuaixiangzhang/Work/current/FNAL_Work2024/rsl_analyses/v4_analysis/abs_explore/results/tmp/opCh00_abs10_1000num_e67_hist.root", "READ");
    if (!file0 || file0->IsZombie()) {
        std::cerr << "Error: Unable to open file1.root" << std::endl;
        return 1;
    }
    //Open the second root file---
    TFile *file1 = new TFile("/Users/shuaixiangzhang/Work/current/FNAL_Work2024/rsl_analyses/v4_analysis/abs_explore/results/tmp/opCh00_abs10_1000num2_e67_hist.root", "READ");
    //Open the third root file---
    TFile *file2 = new TFile("/Users/shuaixiangzhang/Work/current/FNAL_Work2024/rsl_analyses/v4_analysis/abs_explore/results/tmp/opCh00_abs10_1000num2_e67_hist.root", "READ");

    //Output location:------------
    string output_path = "/Users/shuaixiangzhang/Work/current/FNAL_Work2024/rsl_analyses/v4_analysis/abs_explore/results/e67_3000events_crtCut/";
    string output_name = "abs10_energyDistribution.root";
    //===================================================


    //Get access to histos inside root files--------------------------------------
    TGraph *graph0 = dynamic_cast<TGraph*>(file0->Get("Graph"));
    TGraph *graph1 = dynamic_cast<TGraph*>(file1->Get("Graph"));
    TGraph *graph2 = dynamic_cast<TGraph*>(file2->Get("Graph"));

    TH1F *before0 = dynamic_cast<TH1F*>(file0->Get("hist_energyB"));
    TH1F *after0 = dynamic_cast<TH1F*>(file0->Get("hist_energyA"));    
    TH1F *before1 = dynamic_cast<TH1F*>(file1->Get("hist_energyB"));
    TH1F *after1 = dynamic_cast<TH1F*>(file1->Get("hist_energyA"));  
    TH1F *before2 = dynamic_cast<TH1F*>(file2->Get("hist_energyB"));
    TH1F *after2 = dynamic_cast<TH1F*>(file2->Get("hist_energyA"));   


   //Create NEW root file--------------------------------------------------------
    TFile *outputFile = new TFile(TString(output_path)+TString(output_name), "RECREATE");
    if (!outputFile || outputFile->IsZombie()) {
        std::cerr << "Error: Unable to create combined_file.root" << std::endl;
        return 1;
    }


   
    //Create new TGraph * TH1F to copy data from file0 & file1-------------------
    TGraph *graphC = new TGraph(graph0->GetN() + graph1->GetN() + graph2->GetN());
    for(int i = 0; i < graph0->GetN(); ++i){
        double x, y;
        graph0->GetPoint(i, x, y);
        graphC->SetPoint(i, x, y);
    }
    for(int i = 0; i < graph1->GetN(); ++i) {
        double x, y;
        graph1->GetPoint(i, x, y);
        graphC->SetPoint(graph0->GetN()+i, x, y);
    }
    for(int i = 0; i < graph2->GetN(); ++i) {
        double x, y;
        graph2->GetPoint(i, x, y);
        graphC->SetPoint(graph1->GetN()+i, x, y);
    }


    //prevent drawing a line from the origin to the data points
    graphC->SetLineWidth(0);

    graphC->SetName("NumEnergy");
    graphC->SetTitle("Total Photon Num vs Muon Energy");
    graphC->GetXaxis()->SetTitle("Muon Energy [GeV]");
    graphC->GetYaxis()->SetTitle("(Total Photon Num) / Event");
    graphC->Draw("AP");

    graphC->Write();

    //Energy Distributions w/o crt cut------------------------------
    TH1F *beforeC = new TH1F(*before0);
    beforeC->Add(before1);
    beforeC->Add(before2);
    beforeC->SetName("noCut");

    TH1F *afterC = new TH1F(*after0);
    afterC->Add(after1);
    afterC->Add(after2);
    afterC->SetName("withCut");

    TCanvas *canvas = new TCanvas("crtCut", "Energy Distribution", 800, 600);
    beforeC->SetLineColor(kBlue);
    beforeC->SetLineWidth(3);
    beforeC->GetXaxis()->SetTitle("Energy [GeV]");
    beforeC->GetYaxis()->SetTitle("#Events");
    beforeC->Draw();
    afterC->SetLineColor(kRed);
    afterC->SetLineWidth(3);
    afterC->Draw("same");

    TLegend *legend = new TLegend(0.6, 0.7, 0.9, 0.9);
    legend->AddEntry(beforeC, Form("Total events (%.0f entries)", beforeC->GetEntries()), "l");
    legend->AddEntry(afterC, Form("After CRT cut: (%.0f entries)", afterC->GetEntries()), "l");
    legend->Draw();

    beforeC->Write();
    afterC->Write();
    canvas->Write();
 


    //close files-------------
    file0->Close();
    file1->Close();  
    outputFile->Close();

    return 0;
}
