#include "TFile.h"
#include "TTree.h"
#include <vector>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <algorithm>

void AnalyzeNumuCut (){
  TFile *myFile = TFile::Open("output_ExampleAnalysis_ExampleSelection.root");
  if (myFile==0){
    printf("File not correctly opened!\n");
    return;
  } //if the file is not correctly opened, spit out an error message
  TH1D *nuhist = (TH1D*)myFile->Get("nu_energy_hist");
  TH1D *goodnuhist = (TH1D*)myFile->Get("good_nu_energy_hist");

  TTreeReader myReader("sbnana", myFile); //create a TTreeReader

  TTreeReaderValue<int> NuCount (myReader, "nucount");
  int totalNuCount=0;

  TTreeReaderValue<int> GoodNuCount (myReader, "good_nu_count");
  int totalGoodNuCount=0;
  while (myReader.Next()){
    totalNuCount+= *NuCount;
    totalGoodNuCount+= *GoodNuCount;
  }

  std::cout<<"Total nu count= "<<totalNuCount<<std::endl;
  std::cout<<"Total good nu count= "<<totalGoodNuCount<<std::endl;

  nuhist->SetFillColor(kAzure);
  goodnuhist->SetFillColor(kViolet);
  TCanvas *c = new TCanvas ("c", "Generated and recontructed hists",10,10,1000,800);
  THStack *nustack = new THStack("nustack","Generated and reconstructed #nu_#mu");
  nustack->Add(nuhist);
  nustack->Add(goodnuhist);
  nustack->Draw("nostack");
  nustack->GetXaxis()->SetTitle("Neutrino energy (GeV)");
  nustack->GetYaxis()->SetTitle("# Events");
  auto legend = new TLegend(0.75,0.75,0.95,0.95);
  legend->AddEntry(nuhist, "Generated #nu_#mu");
  legend->AddEntry(goodnuhist, "#nu_#mu reconstruted from muon tracks");
  legend->Draw();
  c-> Modified();






}
