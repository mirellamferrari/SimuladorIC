//
//  Macro.c
//  
//
//  Created by Lucas Maia Rios on 4/26/18.
//
//

#include <stdio.h>
#include "TFile.h"
#include "TGaxis.h"

void ArMacro(){
    TFile *e= TFile::Open("DataAr70.root");
    H4->GetXaxis()->SetTitle("Kinetic Energy of Primary Electron (MeV)");
    H4->GetYaxis()->SetTitle("Counts");
    
    const Int_t nbins = H4->GetNbinsX();
    TH1D *hmix = new TH1D("hmix", "Linear Combination of Ar and CO2", nbins, 0., 6.0);
    
    Double_t content;
    for (Int_t i=1; i<=nbins; i++) {
        content = H4->GetBinContent(i);
        hmix->SetBinContent(i,content);
    }
    
    
    hmix->SetMarkerStyle(7);
    hmix->GetXaxis()->SetTitle("Kinetic Energy of Primary Electron (keV)");
    hmix->GetYaxis()->SetTitle("Counts");
    hmix->Draw("p");
    
    auto legend = new TLegend(0.1,0.7,0.48,0.9);
    legend->SetHeader("Legend:");
    legend->AddEntry(hmix,"Mixture","p"); //"lep" for error bars
    
    TFile *f= TFile::Open("DataAr100.root");
    
    H4->GetXaxis()->SetTitle("Kinetic Energy of Primary Electron (MeV)");
    H4->GetYaxis()->SetTitle("Counts");
    
    TH1D *har = new TH1D("hxe", " ", nbins, 0., 6.0);
    content=0;
    for (Int_t i=1; i<=nbins; i++) {
        content = H4->GetBinContent(i);
        har->SetBinContent(i, content);
    }
    
    har->Scale(0.9); //0.8979
    har->SetMarkerColor(2);
    har->SetMarkerStyle(4);
    
    legend->AddEntry(har,"Ar 100%","p");
    
    har->Draw("same p");

    TFile *g= TFile::Open("DataCO2.root");
    
    TH1D *hco2 = new TH1D("hco2", " ", nbins, 0., 6.0);
    
    content=0;
    for (Int_t i=1; i<=nbins; i++) {
        content = H4->GetBinContent(i);
        hco2->SetBinContent(i, content);
    }
    
    hco2->Scale(0.2); // Scale for Ar mix
    hco2->SetMarkerColor(3);
    hco2->SetMarkerStyle(26);
    
    legend->AddEntry(hco2,"CO2 100%","p");
    
    H4->GetXaxis()->SetTitle("Kinetic Energy of Primary Electron (MeV)");
    H4->GetYaxis()->SetTitle("Counts");

    
    //hsum->Add(hco2,1);
    
    /* Integral for finding out the coefficients of the sum
     
    int bin_min = hmix->GetXaxis()->FindBin(0.00264); //0.0052
    int bin_max = hmix->GetXaxis()->FindBin(0.003); //0.00557
    
    double mix_integral = hmix->Integral(bin_min,bin_max);
    cout<< mix_integral << endl;
    
    double sum_integral = hsum->Integral(bin_min,bin_max);
    cout<< sum_integral << endl;
    
    cout<< sum_integral/mix_integral << endl;
    */
    
    hco2->Draw("same p");
    
    legend->Draw();
    //new TBrowser;
    
}
