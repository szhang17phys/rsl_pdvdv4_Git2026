#include <TFile.h>
#include <TH2F.h>
#include <TString.h>
#include <iostream>

// Function to transform a histogram's fillings by shifting y to y/2
TH2F* TransformHistogram(TH2F* hist) {
    if (!hist) return nullptr;

    //Create a new histogram with the same binning
    TString newName = TString(hist->GetName());
    TH2F* newHist = new TH2F(newName, hist->GetTitle(),
                             hist->GetNbinsX(), hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax(),
                             hist->GetNbinsY(), hist->GetYaxis()->GetXmin(), hist->GetYaxis()->GetXmax());

    //Loop over all bins in the original histogram
    for (int ix = 1; ix <= hist->GetNbinsX(); ++ix) {
        for (int iy = 1; iy <= hist->GetNbinsY(); ++iy) {
            double content = hist->GetBinContent(ix, iy); // Get the bin content
            if (content > 0) { // Only fill if there is content
                double x = hist->GetXaxis()->GetBinCenter(ix);
                double y = hist->GetYaxis()->GetBinCenter(iy);
                newHist->Fill(x, y / 2, content); // Fill with transformed y/2
            }
        }
    }

    return newHist;
}




//Main function===================================
void shiftY() {

    //Base file name
    const char* fileName = "Combine_corsika_rsl70_abs15_3_hist.root";

    const char* inputPath = "/Users/shuaixiangzhang/Work/current/FNAL_Work2024/rsl_analyses/new_20250127/block_compare_result/results_TEST/tmp/initial/";
    const char* outputPath = "/Users/shuaixiangzhang/Work/current/FNAL_Work2024/rsl_analyses/new_20250127/block_compare_result/results_TEST/tmp/";

    std::string inputFileName = std::string(inputPath) + fileName;
    std::string outputFileName = std::string(outputPath) + fileName;

    //Open the input ROOT file
    TFile* inputFile = TFile::Open(inputFileName.c_str(), "READ");
    if (!inputFile || inputFile->IsZombie()) {
        std::cerr << "Error: Could not open input file " << inputFileName << std::endl;
        return;
    }

    // Retrieve the histograms
    TH2F* hist1 = dynamic_cast<TH2F*>(inputFile->Get("cathode8XA"));
    if (!hist1) {
        std::cerr << "Error: Histogram 'cathode8XA' not found in file." << std::endl;
        inputFile->Close();
        return;
    }

    TH2F* hist2 = dynamic_cast<TH2F*>(inputFile->Get("membrane1"));
    if (!hist2) {
        std::cerr << "Error: Histogram 'membrane1' not found in file." << std::endl;
        inputFile->Close();
        return;
    }

    // Retrieve the histograms
    TH2F* hist3 = dynamic_cast<TH2F*>(inputFile->Get("membrane2"));
    if (!hist3) {
        std::cerr << "Error: Histogram 'cathode8XA' not found in file." << std::endl;
        inputFile->Close();
        return;
    }

    // Retrieve the histograms
    TH2F* hist4 = dynamic_cast<TH2F*>(inputFile->Get("pmt1"));
    if (!hist4) {
        std::cerr << "Error: Histogram 'cathode8XA' not found in file." << std::endl;
        inputFile->Close();
        return;
    }

    // Retrieve the histograms
    TH2F* hist5 = dynamic_cast<TH2F*>(inputFile->Get("pmt2"));
    if (!hist5) {
        std::cerr << "Error: Histogram 'cathode8XA' not found in file." << std::endl;
        inputFile->Close();
        return;
    }



    //Create the output ROOT file
    TFile* outputFile = TFile::Open(outputFileName.c_str(), "RECREATE");
    if (!outputFile || outputFile->IsZombie()) {
        std::cerr << "Error: Could not create output file " << outputFileName << std::endl;
        inputFile->Close();
        return;
    }

    // Transform and write the histograms
    TH2F* newHist1 = TransformHistogram(hist1);
    TH2F* newHist2 = TransformHistogram(hist2);
    TH2F* newHist3 = TransformHistogram(hist3);
    TH2F* newHist4 = TransformHistogram(hist4);
    TH2F* newHist5 = TransformHistogram(hist5);

    newHist1->Write();
    newHist2->Write();
    newHist3->Write();
    newHist4->Write();
    newHist5->Write();

    // Close files
    outputFile->Close();
    inputFile->Close();

    std::cout << "Transformed histograms saved to " << outputFileName << std::endl;
}




