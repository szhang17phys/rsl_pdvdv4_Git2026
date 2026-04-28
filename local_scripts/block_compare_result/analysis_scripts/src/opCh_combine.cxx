#include <iostream>
#include <TFile.h>
#include <TH2.h>

//Instruction:
//Only change input_path, input_name 

int opCh_combine(string input_path, string input_name, string output_path){

    //----------------------------------------------------
    //string input_path = "./results/rsl50_1000num2_e67/opCh";//input root file path---
    //string input_name = "_rsl50_1000num2_e67_hist.root";//input root file name---
    //string output_path = "./results/rsl50_1000num2_e67/";
    //----------------------------------------------------

    //Stupid method, TH2F array does not work well---
    //Open the first ROOT file---
    TFile *file00 = new TFile(TString(input_path)+"opCh"+"00"+TString(input_name), "READ");
    if (!file00 || file00->IsZombie()) {
        std::cerr << "Error: Unable to open file1.root" << std::endl;
        return 1;
    }    
    TFile *file01 = new TFile(TString(input_path)+"opCh"+"01"+TString(input_name), "READ");
    TFile *file02 = new TFile(TString(input_path)+"opCh"+"02"+TString(input_name), "READ");
    TFile *file03 = new TFile(TString(input_path)+"opCh"+"03"+TString(input_name), "READ");
    TFile *file04 = new TFile(TString(input_path)+"opCh"+"04"+TString(input_name), "READ");
    TFile *file05 = new TFile(TString(input_path)+"opCh"+"05"+TString(input_name), "READ");
    TFile *file06 = new TFile(TString(input_path)+"opCh"+"06"+TString(input_name), "READ");
    TFile *file07 = new TFile(TString(input_path)+"opCh"+"07"+TString(input_name), "READ");
    TFile *file08 = new TFile(TString(input_path)+"opCh"+"08"+TString(input_name), "READ");
    TFile *file09 = new TFile(TString(input_path)+"opCh"+"09"+TString(input_name), "READ");
    TFile *file10 = new TFile(TString(input_path)+"opCh"+"10"+TString(input_name), "READ");
    TFile *file11 = new TFile(TString(input_path)+"opCh"+"11"+TString(input_name), "READ");
    TFile *file12 = new TFile(TString(input_path)+"opCh"+"12"+TString(input_name), "READ");
    TFile *file13 = new TFile(TString(input_path)+"opCh"+"13"+TString(input_name), "READ");
    TFile *file14 = new TFile(TString(input_path)+"opCh"+"14"+TString(input_name), "READ");
    TFile *file15 = new TFile(TString(input_path)+"opCh"+"15"+TString(input_name), "READ");
    TFile *file16 = new TFile(TString(input_path)+"opCh"+"16"+TString(input_name), "READ");
    TFile *file17 = new TFile(TString(input_path)+"opCh"+"17"+TString(input_name), "READ");
    TFile *file18 = new TFile(TString(input_path)+"opCh"+"18"+TString(input_name), "READ");
    TFile *file19 = new TFile(TString(input_path)+"opCh"+"19"+TString(input_name), "READ");
    TFile *file20 = new TFile(TString(input_path)+"opCh"+"20"+TString(input_name), "READ");
    TFile *file21 = new TFile(TString(input_path)+"opCh"+"21"+TString(input_name), "READ");
    TFile *file22 = new TFile(TString(input_path)+"opCh"+"22"+TString(input_name), "READ");
    TFile *file23 = new TFile(TString(input_path)+"opCh"+"23"+TString(input_name), "READ");
    TFile *file24 = new TFile(TString(input_path)+"opCh"+"24"+TString(input_name), "READ");
    TFile *file25 = new TFile(TString(input_path)+"opCh"+"25"+TString(input_name), "READ");
    TFile *file26 = new TFile(TString(input_path)+"opCh"+"26"+TString(input_name), "READ");
    TFile *file27 = new TFile(TString(input_path)+"opCh"+"27"+TString(input_name), "READ");
    TFile *file28 = new TFile(TString(input_path)+"opCh"+"28"+TString(input_name), "READ");
    TFile *file29 = new TFile(TString(input_path)+"opCh"+"29"+TString(input_name), "READ");
    TFile *file30 = new TFile(TString(input_path)+"opCh"+"30"+TString(input_name), "READ");
    TFile *file31 = new TFile(TString(input_path)+"opCh"+"31"+TString(input_name), "READ");
    TFile *file32 = new TFile(TString(input_path)+"opCh"+"32"+TString(input_name), "READ");
    TFile *file33 = new TFile(TString(input_path)+"opCh"+"33"+TString(input_name), "READ");
    TFile *file34 = new TFile(TString(input_path)+"opCh"+"34"+TString(input_name), "READ");
    TFile *file35 = new TFile(TString(input_path)+"opCh"+"35"+TString(input_name), "READ");
    TFile *file36 = new TFile(TString(input_path)+"opCh"+"36"+TString(input_name), "READ");
    TFile *file37 = new TFile(TString(input_path)+"opCh"+"37"+TString(input_name), "READ");
    TFile *file38 = new TFile(TString(input_path)+"opCh"+"38"+TString(input_name), "READ");
    TFile *file39 = new TFile(TString(input_path)+"opCh"+"39"+TString(input_name), "READ");


    TH2F *hist00 = dynamic_cast<TH2F*>(file00->Get("CRT_Opch"));
    TH2F *hist01 = dynamic_cast<TH2F*>(file01->Get("CRT_Opch"));    
    TH2F *hist02 = dynamic_cast<TH2F*>(file02->Get("CRT_Opch"));
    TH2F *hist03 = dynamic_cast<TH2F*>(file03->Get("CRT_Opch"));
    TH2F *hist04 = dynamic_cast<TH2F*>(file04->Get("CRT_Opch"));
    TH2F *hist05 = dynamic_cast<TH2F*>(file05->Get("CRT_Opch"));
    TH2F *hist06 = dynamic_cast<TH2F*>(file06->Get("CRT_Opch"));
    TH2F *hist07 = dynamic_cast<TH2F*>(file07->Get("CRT_Opch"));
    TH2F *hist08 = dynamic_cast<TH2F*>(file08->Get("CRT_Opch"));
    TH2F *hist09 = dynamic_cast<TH2F*>(file09->Get("CRT_Opch"));
    TH2F *hist10 = dynamic_cast<TH2F*>(file10->Get("CRT_Opch"));
    TH2F *hist11 = dynamic_cast<TH2F*>(file11->Get("CRT_Opch"));
    TH2F *hist12 = dynamic_cast<TH2F*>(file12->Get("CRT_Opch"));
    TH2F *hist13 = dynamic_cast<TH2F*>(file13->Get("CRT_Opch"));
    TH2F *hist14 = dynamic_cast<TH2F*>(file14->Get("CRT_Opch"));
    TH2F *hist15 = dynamic_cast<TH2F*>(file15->Get("CRT_Opch"));
    TH2F *hist16 = dynamic_cast<TH2F*>(file16->Get("CRT_Opch"));
    TH2F *hist17 = dynamic_cast<TH2F*>(file17->Get("CRT_Opch"));
    TH2F *hist18 = dynamic_cast<TH2F*>(file18->Get("CRT_Opch"));
    TH2F *hist19 = dynamic_cast<TH2F*>(file19->Get("CRT_Opch"));
    TH2F *hist20 = dynamic_cast<TH2F*>(file20->Get("CRT_Opch"));
    TH2F *hist21 = dynamic_cast<TH2F*>(file21->Get("CRT_Opch"));
    TH2F *hist22 = dynamic_cast<TH2F*>(file22->Get("CRT_Opch"));
    TH2F *hist23 = dynamic_cast<TH2F*>(file23->Get("CRT_Opch"));
    TH2F *hist24 = dynamic_cast<TH2F*>(file24->Get("CRT_Opch"));
    TH2F *hist25 = dynamic_cast<TH2F*>(file25->Get("CRT_Opch"));
    TH2F *hist26 = dynamic_cast<TH2F*>(file26->Get("CRT_Opch"));
    TH2F *hist27 = dynamic_cast<TH2F*>(file27->Get("CRT_Opch"));
    TH2F *hist28 = dynamic_cast<TH2F*>(file28->Get("CRT_Opch"));
    TH2F *hist29 = dynamic_cast<TH2F*>(file29->Get("CRT_Opch"));
    TH2F *hist30 = dynamic_cast<TH2F*>(file30->Get("CRT_Opch"));
    TH2F *hist31 = dynamic_cast<TH2F*>(file31->Get("CRT_Opch"));
    TH2F *hist32 = dynamic_cast<TH2F*>(file32->Get("CRT_Opch"));
    TH2F *hist33 = dynamic_cast<TH2F*>(file33->Get("CRT_Opch"));
    TH2F *hist34 = dynamic_cast<TH2F*>(file34->Get("CRT_Opch"));
    TH2F *hist35 = dynamic_cast<TH2F*>(file35->Get("CRT_Opch"));
    TH2F *hist36 = dynamic_cast<TH2F*>(file36->Get("CRT_Opch"));
    TH2F *hist37 = dynamic_cast<TH2F*>(file37->Get("CRT_Opch"));
    TH2F *hist38 = dynamic_cast<TH2F*>(file38->Get("CRT_Opch"));
    TH2F *hist39 = dynamic_cast<TH2F*>(file39->Get("CRT_Opch"));

   
    //Create new hist to store cathode 8 XAs-----------------------
    TH2F *hist_cathode8XA = new TH2F(*hist04);
    hist_cathode8XA->Add(hist05);
    hist_cathode8XA->Add(hist06);
    hist_cathode8XA->Add(hist07);
    hist_cathode8XA->Add(hist08);
    hist_cathode8XA->Add(hist09);
    hist_cathode8XA->Add(hist10);
    hist_cathode8XA->Add(hist11);

    hist_cathode8XA->SetName("cathode8XA");
    hist_cathode8XA->SetTitle("Response of Cathode 8 XAs");
    hist_cathode8XA->GetXaxis()->SetTitle("Distance [cm]");
	hist_cathode8XA->GetYaxis()->SetTitle("#photon / event");
    hist_cathode8XA->SetMarkerStyle(21);
    hist_cathode8XA->SetMarkerSize(1.0);


    //Create new hist to store XA: 0, 2, 17, 23-----------------------
    TH2F *hist_m4XA_1 = new TH2F(*hist00);
    hist_m4XA_1->Add(hist02);
    hist_m4XA_1->Add(hist17);
    hist_m4XA_1->Add(hist23);

    hist_m4XA_1->SetName("membrane1");
    hist_m4XA_1->SetTitle("Response of OpCh00, 02, 17 & 23");
    hist_m4XA_1->GetXaxis()->SetTitle("Distance [cm]");
	hist_m4XA_1->GetYaxis()->SetTitle("#photon / event");
    hist_m4XA_1->SetMarkerStyle(21);
    hist_m4XA_1->SetMarkerSize(1.0);

    //Create new hist to store XA: 1, 3, 16, 22-----------------------
    TH2F *hist_m4XA_2 = new TH2F(*hist01);
    hist_m4XA_2->Add(hist03);
    hist_m4XA_2->Add(hist16);
    hist_m4XA_2->Add(hist22);

    hist_m4XA_2->SetName("membrane2");
    hist_m4XA_2->SetTitle("Response of OpCh01, 03, 16 & 22");
    hist_m4XA_2->GetXaxis()->SetTitle("Distance [cm]");
	hist_m4XA_2->GetYaxis()->SetTitle("#photon / event");
    hist_m4XA_2->SetMarkerStyle(21);
    hist_m4XA_2->SetMarkerSize(1.0);

    //Create new hist to store PMT: 31, 33-----------------------
    TH2F *hist_2PMT_1 = new TH2F(*hist31);
    hist_2PMT_1->Add(hist33);

    hist_2PMT_1->SetName("pmt1");
    hist_2PMT_1->SetTitle("Response of OpCh31 & 33");
    hist_2PMT_1->GetXaxis()->SetTitle("Distance [cm]");
	hist_2PMT_1->GetYaxis()->SetTitle("#photon / event");
    hist_2PMT_1->SetMarkerStyle(21);
    hist_2PMT_1->SetMarkerSize(1.0);

    //Create new hist to store PMT: 12~15, 18~21, 24~29. 30, 32, 34~39---------
    TH2F *hist_22PMT_2 = new TH2F(*hist12);
    hist_22PMT_2->Add(hist13);
    hist_22PMT_2->Add(hist14);
    hist_22PMT_2->Add(hist15);
    hist_22PMT_2->Add(hist18);
    hist_22PMT_2->Add(hist19);
    hist_22PMT_2->Add(hist20);
    hist_22PMT_2->Add(hist21);
    hist_22PMT_2->Add(hist24);
    hist_22PMT_2->Add(hist25);
    hist_22PMT_2->Add(hist26);
    hist_22PMT_2->Add(hist27);
    hist_22PMT_2->Add(hist28);
    hist_22PMT_2->Add(hist29);
    hist_22PMT_2->Add(hist30);
    hist_22PMT_2->Add(hist32);
    hist_22PMT_2->Add(hist34);
    hist_22PMT_2->Add(hist35);
    hist_22PMT_2->Add(hist36);
    hist_22PMT_2->Add(hist37);
    hist_22PMT_2->Add(hist38);
    hist_22PMT_2->Add(hist39);

    hist_22PMT_2->SetName("pmt2");
    hist_22PMT_2->SetTitle("Response of OpCh 12~15, 18~21, 24~29. 30, 32, 34~39");
    hist_22PMT_2->GetXaxis()->SetTitle("Distance [cm]");
	hist_22PMT_2->GetYaxis()->SetTitle("#photon / event");
    hist_22PMT_2->SetMarkerStyle(21);
    hist_22PMT_2->SetMarkerSize(1.0);

   //Create NEW root file--------------------------------------------------
    TFile *outputFile = new TFile(TString(output_path)+"Combine"+TString(input_name), "RECREATE");
    if (!outputFile || outputFile->IsZombie()) {
        std::cerr << "Error: Unable to create combined_file.root" << std::endl;
        return 1;
    }

    // Write the combined histogram to the new ROOT file
    hist_cathode8XA->Write();
    hist_m4XA_1->Write();   
    hist_m4XA_2->Write();
    hist_2PMT_1->Write();
    hist_22PMT_2->Write();


    delete file00;
    delete file01;
    delete file02;
    delete file03;
    delete file04;
    delete file05;
    delete file06;
    delete file07;
    delete file08;
    delete file09;
    delete file10;
    delete file11;
    delete file12;
    delete file13;
    delete file14;
    delete file15;
    delete file16;
    delete file17;
    delete file18;
    delete file19;
    delete file20;
    delete file21;
    delete file22;
    delete file23;
    delete file24;
    delete file25;
    delete file26;
    delete file27;
    delete file28;
    delete file29;
    delete file30;
    delete file31;
    delete file32;
    delete file33;
    delete file34;
    delete file35;
    delete file36;
    delete file37;
    delete file38;
    delete file39;

    delete outputFile;

    return 0;
}



int main(int argc, char *argv[]) {
    // Check if the correct number of command-line arguments is provided
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <param1> <param2> <param3> <param4>" << std::endl;
        return 1; // Return an error code
    }

    // Convert the command-line arguments to the respective types
    std::string param1 = argv[1];
    std::string param2 = argv[2];
    std::string param3 = argv[3];

    opCh_combine(param1, param2, param3);

    return 0;
}
