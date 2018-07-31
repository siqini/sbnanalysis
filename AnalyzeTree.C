#include "TFile.h"
#include "TTree.h"
#include <vector>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <algorithm>
/*
* This script reads a given output file from the analysis code that contains data written in the form of a
* TTree using TTreeReader; in particular, we read the particle types and energies, and plot a stacked
* histogram by combining histograms of different particle types, indicated by different filled colors, of
* number of particles v. energies.
*/

void AnalyzeTree(){
  TFile *myFile = TFile::Open("output_ExampleAnalysis_ExampleSelection.root");
  if (myFile==0){
    printf("File not correctly opened!\n");
    return;
  } //if the file is not correctly opened, spit out an error message
  TTreeReader myReader("sbnana", myFile); //create a TTreeReader

  TTreeReaderArray<Int_t> int_nu_genie_intcode (myReader, "CCNC");
  vector<Int_t> genie_intcodes_vec;
  while (myReader.Next()){
    for (int iParticle=0;iParticle<int_nu_genie_intcode.GetSize();++iParticle){
      if (genie_intcodes_vec.empty()){ genie_intcodes_vec.push_back(int_nu_genie_intcode[iParticle]);}
      else {
        if (find(genie_intcodes_vec.begin(), genie_intcodes_vec.end(), int_nu_genie_intcode[iParticle])==genie_intcodes_vec.end()){
          genie_intcodes_vec.push_back(int_nu_genie_intcode[iParticle]);
        } //if statement that "pushes back" the intcode to the vector genie_intcodes_vec if it is unique
      }//if-else statement that only "pushes back" the read intcode into the vector genie_intcodes_vec if a)the vector is empty b)the intcode is unique for now
    }//loop through every particle in this event
  }//loop through all events

  std::cout<<genie_intcodes_vec.size()<<std::endl;
  THStack* parents = new THStack("parents", "parentsParticles");
  srand(123);
  char buffer [30];
  vector<TH1D*> hists = vector<TH1D*>();
  for (int iHist=0;iHist<genie_intcodes_vec.size();iHist++){
    sprintf(buffer, "%s%d","GenieCode", genie_intcodes_vec[iHist]);
    hists.push_back(new TH1D(buffer,"", 50,0,6));
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

  TTreeReader myReader2("sbnana", myFile);

    TTreeReaderArray<Double_t> int_nu_e2(myReader2, "interactions.neutrino.energy");
    TTreeReaderArray<Int_t> int_nu_genie_intcode2 (myReader2, "interactions.neutrino.genie_intcode");

  while (myReader2.Next()){
    for (int iParticle=0;iParticle<int_nu_genie_intcode2.GetSize();++iParticle){
      auto this_type = int_nu_genie_intcode2[iParticle];
      auto this_energy = int_nu_e2[iParticle];
      int type = (int) this_type;
      int type_index = std::find(genie_intcodes_vec.begin(), genie_intcodes_vec.end(), type) - genie_intcodes_vec.begin();
      hists[type_index]->Fill(this_energy);
    }
  }

  for (int i=0;i<hists.size();i++){
    parents->Add(hists[i]);
  }
  parents->Draw();
}
