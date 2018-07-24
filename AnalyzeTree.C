#include "TFile.h"
#include "TTree.h"
#include <vector>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>

void AnalyzeTree(){
  TFile *myFile = TFile::Open("output_ExampleAnalysis_ExampleSelection.root");
  if (myFile==0){
    printf("File not correctly opened!\n");
    return;
  }
  TTree *myTree=(TTree*)myFile->Get("sbnana");
  auto nevent = myTree->GetEntries();
  TTreeReader myReader("sbnana", myFile);
  TTreeReaderArray<Double_t> int_nu_e(myReader, "interactions.neutrino.energy");
  TTreeReaderArray<Int_t> int_nu_genie_intcode (myReader, "interactions.neutrino.genie_intcode");
  int genie_intcode_max=0;
  vector<Int_t> genie_intcodes;
  while (myReader.Next()){
    for (int iParticle=0;iParticle<int_nu_genie_intcode.GetSize();++iParticle){
      genie_intcodes.push_back(int_nu_genie_intcode[iParticle]);
      if (int_nu_genie_intcode[iParticle]>genie_intcode_max){
        genie_intcode_max=int_nu_genie_intcode[iParticle];
      }
    }
  }
  THStack* parents = new THStack("parents", "parentsParticles");
  srand(123456);
  vector<TH1D*> hists = vector<TH1D*>();
  for (int iHist=0;iHist<genie_intcode_max;iHist++){
    hists.push_back(new TH1D(" ","", 30,0,6 ));
    auto num = rand()%12;
    if(num==0) hists[iHist]->SetFillColor(kOrange);
      else if(num==1) hists[iHist]->SetFillColor(kRed);
      else if(num==2) hists[iHist]->SetFillColor(kPink);
      else if(num==3) hists[iHist]->SetFillColor(kMagenta);
      else if(num==4) hists[iHist]->SetFillColor(kViolet);
      else if(num==5) hists[iHist]->SetFillColor(kBlue);
      else if(num==6) hists[iHist]->SetFillColor(kAzure);
      else if(num==7) hists[iHist]->SetFillColor(kCyan);
      else if(num==8) hists[iHist]->SetFillColor(kTeal);
      else if(num==9) hists[iHist]->SetFillColor(kGreen);
      else if(num==10) hists[iHist]->SetFillColor(kSpring);
      else if(num==11) hists[iHist]->SetFillColor(kYellow);
  }

  while (myReader.Next()){
    for (int iParticle=0;iParticle<int_nu_genie_intcode.GetSize();++iParticle){
      auto this_type = int_nu_genie_intcode[iParticle];
      auto this_energy = int_nu_e[iParticle];
      hists[this_type]->Fill(this_energy);
    }
  }

  for (int i=0;i<hists.size();i++){
    parents->Add(hists[i]);
  }
  parents->Draw();







}
