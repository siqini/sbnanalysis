#include <iostream>
#include <math.h>
#include <fstream>
#include <vector>
#include <typeinfo>
#include <cstdlib>
#include <string>
#include "Util.h"

void muon_track_cut(){
  TFile* myFile = TFile::Open("output_ExampleAnalysis_ExampleSelection.root");
  if (myFile==0){
    printf("File not correctly opened!\n");
    return;
  }
  TTree* myTree = (TTree*) myFile->Get("sbnana");


  //set up branches
  Event* event = new Event;
  std::vector<int>* CCNC = new std::vector<int>;
  std::vector<Double_t>* EndX = new std::vector<Double_t>;
  std::vector<Double_t>* EndY = new std::vector<Double_t>;
  std::vector<Double_t>* EndZ = new std::vector<Double_t>;
  std::vector<int>* PDGCode = new std::vector<int>;
  std::vector<Double_t>* track_length = new std::vector<Double_t>;
  myTree->SetBranchAddress("events", &event);
  myTree->SetBranchAddress("CCNC",&CCNC);
  myTree->SetBranchAddress("EndX",&EndX);
  myTree->SetBranchAddress("EndY",&EndY);
  myTree->SetBranchAddress("EndZ",&EndZ);
  myTree->SetBranchAddress("PDGCode",&PDGCode);
  myTree->SetBranchAddress("track_length",&track_length);


  for (long i=0;i<myTree->GetEntries();i++){
    myTree->GetEntry(i);
    assert(event->interactions.size() == CCNC->size());

    for (size_t j=0;j<PDGCode->size();j++){
      auto this_pdg_code = PDGCode->at(j);

      //select out only daughter particles that are muons
      if (this_pdg_code == 13) {
        Double_t this_endx = EndX->at(j);
        Double_t this_endy = EndY->at(j);
        Double_t this_endz = EndZ->at(j);
        //separate whether the muon final state is in TPC or not
        if (AreYouIn(this_endx, this_endy, this_endz)){
          auto this_track_length = track_length->at(j);
          if (this_track_length>=50.){
            for (size_t k=0; k<event->interactions.size();k++){
              
            }
          } // select final state muons that are in the TPC volume AND has a track length > 50cm
        } //select out final state muons that are in the TPC volume
      } //select out the muons
    } //an MCTrack
  } //an entry of the TTree



}


bool AreYouIn (Double_t finalX, Double_t finalY, Double_t finalZ){
  if(((-199.15 < finalX && finalX < -2.65) || (2.65 < finalX && finalX < 199.15)) && (-200 < finalY && finalY < 200) && (0 < finalZ && finalZ < 500)) return true;
   else return false;
}
