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
#include "TLegend.h"

void XeMacro(){
    TFile *e= TFile::Open("DataXe70.root");
    H4->GetXaxis()->SetTitle("Kinetic Energy of Primary Electron (keV)");
    H4->GetYaxis()->SetTitle("Counts");
    
    //To draw line histogram comment the previous line and uncomment this section
    const Int_t nbins = H4->GetNbinsX();
    TH1D *hmix = new TH1D("hmix","Linear Combination of Xe and CO2", nbins, 0., 6.0);
    
    Double_t content;
    for (Int_t i=1; i<=nbins; i++) {
        content = H4->GetBinContent(i);
        hmix->SetBinContent(i, content);
    }
    //hmix->SetLineColor(1);
    
    hmix->SetMarkerStyle(7);
    hmix->GetXaxis()->SetTitle("Kinetic Energy of Primary Electron (keV)");
    hmix->GetYaxis()->SetTitle("Counts");
    hmix->Draw("p");
    
    auto legend = new TLegend(0.1,0.7,0.48,0.9);
    legend->SetHeader("Legend:");
    legend->AddEntry(hmix,"Mixture","p"); //"lep" for error bars
    
    TFile *f= TFile::Open("DataXe100.root");
    
    H4->GetXaxis()->SetTitle("Kinetic Energy of Primary Electron (keV)");
    H4->GetYaxis()->SetTitle("Counts");
    
    TH1D *hxe = new TH1D("hxe", " ", nbins, 0., 6.0);
    
    content=0;
    for (Int_t i=1; i<=nbins; i++) {
        content = H4->GetBinContent(i);
        hxe->SetBinContent(i, content);
    }
    
    hxe->SetMarkerColor(2);
    hxe->SetMarkerStyle(4);
    
    legend->AddEntry(hxe,"Xe 100%","p");
    
    hxe->Draw("same p");

    TFile *g= TFile::Open("DataCO2.root");
    
    TH1D *hco2 = new TH1D("hco2", " ", nbins, 0., 6.0);
    
    content=0;
    for (Int_t i=1; i<=nbins; i++) {
        content = H4->GetBinContent(i);
        hco2->SetBinContent(i, content);
    }
    
    hco2->Scale(0.1); // Scale for Xe mix
    hco2->SetMarkerColor(3);
    hco2->SetMarkerStyle(26);
    
    legend->AddEntry(hco2,"CO2 100%","p");
    
    H4->GetXaxis()->SetTitle("Kinetic Energy of Primary Electron (MeV)");
    H4->GetYaxis()->SetTitle("Counts");
    
    hco2->Draw("same p");
    
    legend->Draw();
    
    //hsum->Add(hco2,1);
    //hsum->Draw("same");
    
}
