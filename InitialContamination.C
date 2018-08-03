#include "TFile.h"
#include "TTree.h"
#include <vector>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <algorithm>

void InitialContamination(){
  TFile *myFile = TFile::Open("output_ExampleAnalysis_ExampleSelection.root");
  if (myFile==0){
    printf("File not correctly opened!\n");
    return;
  } //if the file is not correctly opened, spit out an error message
  TH1D *initialnumu = (TH1D*)myFile->Get("initial_numu");
  TH1D *initialcontam = (TH1D*)myFile->Get("initial_contamination");
  initialnumu->SetFillColor(kAzure);
  initialcontam->SetFillColor(kViolet);

  THStack *contamstack = new THStack("contamstack","Contamination in the generated numu");
  contamstack->Add(initialnumu);
  contamstack->Add(initialcontam);

  contamstack->SetTitle("Contamination in the generated numu; Neutrino energy(GeV); # Events");
  contamstack->Draw("nostack");
  auto legend = new TLegend(0.75,0.75,0.95,0.95);
  legend->AddEntry(initialnumu,"#nu_#mu");
  legend->AddEntry(initialcontam,"Contamination");
  legend->Draw();


}
