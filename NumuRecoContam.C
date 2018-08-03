#include "TFile.h"
#include "TTree.h"
#include <vector>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <algorithm>

void NumuRecoContam(){
  TFile *myFile = TFile::Open("output_ExampleAnalysis_ExampleSelection.root");
  if (myFile==0){
    printf("File not correctly opened!\n");
    return;
  } //if the file is not correctly opened, spit out an error message
  TH1D *initialnumu = (TH1D*)myFile->Get("initial_numu");
  TH1D *goodnumu = (TH1D*)myFile->Get("good_nu_energy_hist");
  initialnumu->SetFillColor(kAzure);
  goodnumu->SetFillColor(kViolet);

  THStack *recostack = new THStack("recostack","Generated and reconstructed numus");
  contamstack->Add(initialnumu);
  contamstack->Add(goodnumu);

  contamstack->SetTitle("Generated and reconstructed numus; Neutrino energy(GeV); # Events");
  contamstack->Draw("nostack");
  auto legend = new TLegend(0.75,0.75,0.95,0.95);
  legend->AddEntry(initialnumu,"Generated numu");
  legend->AddEntry(goodnumu,"Reconstructed numu");
  legend->Draw();
}
