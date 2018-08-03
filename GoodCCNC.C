#include "TFile.h"
#include "TTree.h"
#include <vector>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <algorithm>

void GoodCCNC(){
  TFile *myFile = TFile::Open("output_ExampleAnalysis_ExampleSelection.root");
  if (myFile==0){
    printf("File not correctly opened!\n");
    return;
  } //if the file is not correctly opened, spit out an error message
  TH1D *goodCC = (TH1D*)myFile->Get("good_nu_CC");
  TH1D *goodNC = (TH1D*)myFile->Get("good_nu_NC");
  goodCC->SetFillColor(kAzure);
  goodNC->SetFillColor(kGreen);

  THStack *goodccncstack = new THStack("goodccncstack", "CC/NC of reconstructed numus");
  goodccncstack->Add(goodCC);
  goodccncstack->Add(goodNC);

  goodccncstack->SetTitle("CC/NC of reco numus; Neutrino energy(GeV); # Events/0.1 GeV");
  goodccncstack->Draw("nostack");
  auto legend = new TLegend(0.75,0.75,0.95,0.95);
  legend->AddEntry(goodCC,"CC->#mu + X","f");
  legend->AddEntry(goodNC,"NC->#pi^{#pm} + X","f");
  legend->Draw();

}
