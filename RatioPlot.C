#include "TFile.h"
#include "TTree.h"
#include <vector>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <algorithm>

void RatioPlot(){
  TFile *myFile = TFile::Open("output_ExampleAnalysis_ExampleSelection.root");
  if (myFile==0){
    printf("File not correctly opened!\n");
    return;
  } //if the file is not correctly opened, spit out an error message
  TH1D *nuhist = (TH1D*)myFile->Get("initial_numu");
  TH1D *goodnuhist = (TH1D*)myFile->Get("good_nu_energy_hist");

  int nbins = nuhist->GetNbinsX();
  Double_t nuhistcontent [nbins];
  Double_t goodnuhistcontent [nbins];
  Double_t ratios [nbins];
  Double_t xcoord [nbins];
  for (int i=0; i<nbins; i++) {
    xcoord[i] = nuhist->GetXaxis()->GetBinCenter(i+1);
    nuhistcontent[i]=nuhist->GetBinContent(i+1);
    goodnuhistcontent[i]=goodnuhist->GetBinContent(i+1);
    if (nuhistcontent[i]!=0.) ratios[i]=goodnuhistcontent[i]/nuhistcontent[i];
    else ratios[i]=0.;
  }

  TGraph *ratioplot = new TGraph (nbins, xcoord, ratios);
  ratioplot->SetTitle("Ratio of numu in the events at SBND (truth level); Neutrino energy (GeV); # Events");
  ratioplot->Draw();







}
