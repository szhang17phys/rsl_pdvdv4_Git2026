#include <iostream>
#include <iomanip>
#include <cmath>

#include <TFile.h>
#include <vector>
#include <TH2F.h>
#include <TH1F.h>
#include <TF1.h>
#include <TROOT.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TGraph.h>
#include <TMath.h>

#include <TGraphErrors.h>

#include <RooRealVar.h>
#include <RooChebychev.h>
#include <RooDataSet.h>
#include <RooPlot.h>
#include <RooFitResult.h>
#include <RooGlobalFunc.h>


//Purpose of this scripts:
//Do fitting for scatter plots inside fitCLG1_compare.root--- 



//linear fitting---
void LinearFit(TGraphErrors* graph, double minX, double maxX) {
    //Perform linear fitting with chi-squared method
    TF1 *fit = new TF1("linear_fit", "[0] + [1]*x", minX, maxX);
    graph->Fit(fit, "Q", "", minX, maxX);//"Q" for quiet mode and chi-squared method
    
    //Get the fitted parameters
    double slope = fit->GetParameter(1);
    double slopeError = fit->GetParError(1); // Uncertainty of slope parameter
    double intercept = fit->GetParameter(0);
    double interceptError = fit->GetParError(0); // Uncertainty of intercept parameter


    //fit goodness---
    double chiSquare = fit->GetChisquare();//chi^2
    int ndf = fit->GetNDF();//ndf
    double chiNDF = chiSquare / ndf;

    //Print the results
    
    std::cout << "\nk = " << std::setprecision(6) << slope << " ± " << slopeError 
              << ", b = " << std::setprecision(3) << intercept << " ± " << interceptError 
              << ",  chi^2/ndf = " << chiSquare << "/" << ndf << " = " << chiNDF << "\n" << std::endl;


    //Set the fitted curve color to match the scatter plot color
    fit->SetLineColor(graph->GetMarkerColor());

    //Draw the fitted line on the canvas
    fit->Draw("same");
}





//zoom in y and y error values of graph---
void scaleGraph(TGraphErrors* graph, double factor) {
    for (int i = 0; i < graph->GetN(); ++i) {
        double x_val, y_val, y_err;
        graph->GetPoint(i, x_val, y_val);
        y_err = graph->GetErrorY(i);
        // Multiply y-values and y-errors by the factor
        graph->SetPoint(i, x_val, factor * y_val);
        graph->SetPointError(i, factor * y_err);
    }
}




//Chebyshev Fitting---
void ChebyshevFit(TGraphErrors* graph, double minX, double maxX) {

//    TF1 *fit = new TF1("Chebyshev_fit", "[0]*1 + [1]*x + [2]*(2*x*x - 1)", minX, maxX);
    TF1 *fit = new TF1("Chebyshev_fit", "[0]*1 + [1]*x + [2]*(2*x*x - 1) + [3]*(4*x*x*x - 3*x)", minX, maxX);


    graph->Fit(fit, "Q", "", minX, maxX);//"Q" for quiet mode and chi-squared method
    
    //Retrieve the parameters
    double param0 = fit->GetParameter(0);
    double param1 = fit->GetParameter(1);
    double param2 = fit->GetParameter(2);
    double param3 = fit->GetParameter(3);

    //fit goodness---
    double chiSquare = fit->GetChisquare();//chi^2
    int ndf = fit->GetNDF();//ndf
    double chiNDF = chiSquare / ndf;

    //Print the results
    std::cout<< std::fixed;
//    std::cout << "p0 = " << std::setprecision(3) << param0 << ", p1 = " << std::setprecision(6) << param1 << ", p2 = " << std::setprecision(6) << param2 << ",   chi^2/ndf = " << chiSquare << "/" << ndf << " = " << chiNDF <<std::endl;

    std::cout << "p0 = " << std::setprecision(3) << param0 << ", p1 = " << std::setprecision(6) << param1 << ", p2 = " << std::setprecision(6) << param2 << ", p3 = " << std::setprecision(6) << param3 << ",   chi^2/ndf = " << chiNDF <<std::endl;

    //Set the fitted curve color to match the scatter plot color
    fit->SetLineColor(graph->GetMarkerColor());

    //Draw the fitted line on the canvas
    fit->Draw("same");
}








//===tmp main function=========================================================
void compare_fit_TMP(const std::string& path, double fitRange1, double fitRange2){

    TFile *file = TFile::Open((path + "fitCLG1_compare.root").c_str(), "UPDATE");




    //fit rsl50--------------------------------------------------
    TCanvas *canvas_rsl50 = (TCanvas*)file->Get("fitBias_rsl50");    

    //Delete added canvas due to last execution---
    gDirectory->cd(file->GetName());
    gDirectory->Delete("linearFit_rsl50;*");

    TCanvas *new_canvas_rsl50 = (TCanvas*)canvas_rsl50->Clone();
    new_canvas_rsl50->SetName("linearFit_rsl50");
    new_canvas_rsl50->SetTitle("Plots with fitted lines");

    TIter next(new_canvas_rsl50->GetListOfPrimitives());
    TObject *obj;
    while ((obj = next())) {
        // Check if the object is a TGraphErrors
        if (obj->InheritsFrom(TGraphErrors::Class())) {

            TGraphErrors* graph = (TGraphErrors*)obj;
            new_canvas_rsl50->cd();

            LinearFit(graph, fitRange1, fitRange2);
        }
    }

    new_canvas_rsl50->Write();



    //fit rsl70------------------------------------------------------
    TCanvas *canvas_rsl70 = (TCanvas*)file->Get("fitBias_rsl70");    

    gDirectory->cd(file->GetName());
    gDirectory->Delete("linearFit_rsl70;*");

    TCanvas *new_canvas_rsl70 = (TCanvas*)canvas_rsl70->Clone();
    new_canvas_rsl70->SetName("linearFit_rsl70");
    new_canvas_rsl70->SetTitle("Plots with fitted lines");

    TIter next_rsl70(new_canvas_rsl70->GetListOfPrimitives());
    TObject *obj_rsl70;
    while ((obj_rsl70 = next_rsl70())) {
        if (obj_rsl70->InheritsFrom(TGraphErrors::Class())) {

            TGraphErrors* graph = (TGraphErrors*)obj_rsl70;
            new_canvas_rsl70->cd();

            LinearFit(graph, fitRange1, fitRange2);
        }
    }

    new_canvas_rsl70->Write();



    //fit rsl130------------------------------------------------------
    TCanvas *canvas_rsl130 = (TCanvas*)file->Get("fitBias_rsl130");    

    gDirectory->cd(file->GetName());
    gDirectory->Delete("linearFit_rsl130;*");

    TCanvas *new_canvas_rsl130 = (TCanvas*)canvas_rsl130->Clone();
    new_canvas_rsl130->SetName("linearFit_rsl130");
    new_canvas_rsl130->SetTitle("Plots with fitted lines");

    TIter next_rsl130(new_canvas_rsl130->GetListOfPrimitives());
    TObject *obj_rsl130;
    while ((obj_rsl130 = next_rsl130())) {
        if (obj_rsl130->InheritsFrom(TGraphErrors::Class())) {

            TGraphErrors* graph = (TGraphErrors*)obj_rsl130;
            new_canvas_rsl130->cd();

            LinearFit(graph, fitRange1, fitRange2);
        }
    }

    new_canvas_rsl130->Write();



    //fit rsl150------------------------------------------------------
    TCanvas *canvas_rsl150 = (TCanvas*)file->Get("fitBias_rsl150");    

    gDirectory->cd(file->GetName());
    gDirectory->Delete("linearFit_rsl150;*");

    TCanvas *new_canvas_rsl150 = (TCanvas*)canvas_rsl150->Clone();
    new_canvas_rsl150->SetName("linearFit_rsl150");
    new_canvas_rsl150->SetTitle("Plots with fitted lines");

    TIter next_rsl150(new_canvas_rsl150->GetListOfPrimitives());
    TObject *obj_rsl150;
    while ((obj_rsl150 = next_rsl150())) {
        if (obj_rsl150->InheritsFrom(TGraphErrors::Class())) {

            TGraphErrors* graph = (TGraphErrors*)obj_rsl150;
            new_canvas_rsl150->cd();

            LinearFit(graph, fitRange1, fitRange2);
        }
    }

    new_canvas_rsl150->Write();



    //fit ALL------------------------------------------------------
    TCanvas *canvas_All = (TCanvas*)file->Get("fitBias_All");    

    gDirectory->cd(file->GetName());
    gDirectory->Delete("linearFit_All;*");

    TCanvas *new_canvas_All = (TCanvas*)canvas_All->Clone();
    new_canvas_All->SetName("linearFit_All");
    new_canvas_All->SetTitle("Plots with fitted lines");

    TIter next_All(new_canvas_All->GetListOfPrimitives());
    TObject *obj_All;
    while ((obj_All = next_All())) {
        if (obj_All->InheritsFrom(TGraphErrors::Class())) {

            TGraphErrors* graph = (TGraphErrors*)obj_All;
            new_canvas_All->cd();

            LinearFit(graph, fitRange1, fitRange2);
        }
    }

    new_canvas_All->Write();



    file->Close();


}





//===Main Function================================
void compare_fit(){

    compare_fit_TMP("/Users/shuaixiangzhang/Work/current/FNAL_Work2024/rsl_analyses/v4_analysis/results/fit_Develop_slice10cm/cathode/", 50.0, 250.0);
}