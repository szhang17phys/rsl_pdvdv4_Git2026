#include <TFile.h>
#include <TH2F.h>
#include <TH1F.h>

void analyze1() {

    //change file name each time-----------------------
    string file_path = "/Users/shuaixiangzhang/Work/current/FNAL_Work2024/rsl_analyses/v4_analysis/results/";
    string file_suffix = "rsl99_2000num_e67_crtCut.root";
    string output_path = "/Users/shuaixiangzhang/Work/current/FNAL_Work2024/rsl_analyses/v4_analysis/results/";
    string output_name = "fit";
    //-------------------------------------------------

    //Choose the slice you want to look at!---
    //Define the X(distance) values where you want to extract data---
    Double_t distances[] = {200, 150, 100, 50, 30};//Add the desired dis(cm) values---
    //-------------------------------------------


    //Open the input root file---
    TFile* inputFile = new TFile(TString(file_path)+TString(file_suffix), "READ");

    //Check if the file is open---
    if(!inputFile || inputFile->IsZombie()){
        std::cerr<<"Error: Cannot open input.root"<<std::endl;
        return;
    }

    //Access the TH2F from the file---
    TH2F* inputTH2F = (TH2F*)inputFile->Get("summedCathode8XA");

    if(!inputTH2F){
        std::cerr<<"Error: Cannot find TH2F in the input file"<< std::endl;
        inputFile->Close();
        return;
    }


    //Create an array of TH1F histograms to store extracted data---
    const int num = sizeof(distances)/sizeof(distances[0]);
    TH1F* hists[num];

    //Draw all TH1Fs in one canvas---
    TCanvas* canvas = new TCanvas("canvas", "All Histograms", 800, 600);
    //Add a legend(optional)---
    TLegend* legend = new TLegend(0.7, 0.7, 0.9, 0.9);

    //Extract values for each distance value and create TH1F histograms---
    for(int i=0; i<num; ++i){
        double x = distances[i];
        int binX = inputTH2F->GetXaxis()->FindBin(x);

        //Create a TH1F for each extracted data---
        hists[i] = new TH1F(Form("hist_%d", i), Form("Distance = %.1f cm", x), inputTH2F->GetNbinsY(), inputTH2F->GetYaxis()->GetXmin(), inputTH2F->GetYaxis()->GetXmax());
        hists[i]->GetXaxis()->SetTitle("Photon Counts");
        hists[i]->GetYaxis()->SetTitle("Event Rate");

        //Fill the TH1F with data from the TH2F---
        for(int binY=1; binY<=inputTH2F->GetNbinsY(); ++binY){
            double y = inputTH2F->GetYaxis()->GetBinCenter(binY);
            double value = inputTH2F->GetBinContent(binX, binY);
            hists[i]->SetBinContent(binY, value);
        }

        //Set the line color for each TH1F(optional)---
        hists[i]->SetLineColor(i + 1);

        //Draw each TH1F on the canvas---
        if(i==0){
            hists[i]->Draw();
        } 
        else{
            hists[i]->Draw("SAME");
        }

        legend->AddEntry(hists[i], Form("Dis = %.1f cm", distances[i]), "l");
    }

    legend->Draw("SAME");



    //Open the output ROOT file for writing---
    TFile* outputFile = new TFile(TString(output_path)+TString(output_name)+"_"+TString(file_suffix), "RECREATE");




    canvas->Write();
    legend->Write();
    for(int i=0; i<num; ++i){
        hists[i]->Write();
    }


    //Close the input and output files---
    inputFile->Close();
    outputFile->Close();

}


