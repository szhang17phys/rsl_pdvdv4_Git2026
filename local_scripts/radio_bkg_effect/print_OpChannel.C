void print_OpChannel() {
    // Input ROOT file paths
    std::vector<std::string> filenames = {
        "/Users/shuaixiangzhang/Work/current/FNAL_Work2024/rsl_analyses/new_20250127/radio_bkg_effect/root_data/gen_e67_noRadio_hist.root",
        "/Users/shuaixiangzhang/Work/current/FNAL_Work2024/rsl_analyses/new_20250127/radio_bkg_effect/root_data/gen_e67_Radio_hist.root",
        "/Users/shuaixiangzhang/Work/current/FNAL_Work2024/rsl_analyses/new_20250127/radio_bkg_effect/root_data/gen_corsika_noRadio_hist.root",
        "/Users/shuaixiangzhang/Work/current/FNAL_Work2024/rsl_analyses/new_20250127/radio_bkg_effect/root_data/gen_corsika_Radio_hist.root"
    };

    // Output file to save all histograms
    TFile* fout = new TFile("./results/OpChannel_DetectedPhotons.root", "RECREATE");

    // Settings
    const int nBins = 41;

    // Loop over files
    for (const auto& fname : filenames) {
        std::cout << "Processing file: " << fname << std::endl;

        TFile* f = TFile::Open(fname.c_str());
        if (!f || f->IsZombie()) {
            std::cerr << "Failed to open file: " << fname << std::endl;
            continue;
        }

        f->cd("XAresponse");
        TTree* tree = (TTree*)gDirectory->Get("DetectedPhotons");
        if (!tree) {
            std::cerr << "Failed to find TTree 'DetectedPhotons' in file: " << fname << std::endl;
            f->Close();
            continue;
        }

        // Set up variable
        Int_t OpChannel;
        tree->SetBranchAddress("OpChannel", &OpChannel);

        // Create histogram
        std::string histname = "hOpChannel_" + fname.substr(fname.find_last_of('/') + 1); // hist name based on filename
        histname = histname.substr(0, histname.find("_hist.root")); // clean the suffix

        TH1D* hOpChannel = new TH1D(histname.c_str(), ("OpChannel Distribution: " + histname).c_str(), nBins, 0, nBins);

        // Fill event by event
        Long64_t nentries = tree->GetEntries();
        for (Long64_t i = 0; i < nentries; ++i) {
            tree->GetEntry(i);
            hOpChannel->Fill(OpChannel);
        }

        // Print non-zero bins
        for (int bin = 1; bin <= hOpChannel->GetNbinsX(); ++bin) {
            double count = hOpChannel->GetBinContent(bin);
            if (count > 0) {
                int opchannel_value = hOpChannel->GetXaxis()->GetBinCenter(bin);
                std::cout << "  OpChannel " << opchannel_value << " : " << count << " entries" << std::endl;
            }
        }

        // Save histogram into output file
        fout->cd();
        hOpChannel->Write();

        // Clean up this file
        f->Close();
        delete f;
    }

    // Close output file
    fout->Close();
    std::cout << "Saved all histograms into 'OpChannel_DetectedPhotons.root'!" << std::endl;
}
