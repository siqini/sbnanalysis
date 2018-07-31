#include <cassert>
#include <iostream>
#include <vector>
#include "TFile.h"
#include "TTree.h"
#include <Event.hh>

void CCNC_vector(){
  TFile* myFile = TFile::Open("output_ExampleAnalysis_ExampleSelection.root");
  if (myFile==0){
    printf("File not correctly opened!\n");
    return;
  }
  TTree* myTree = (TTree*) myFile->Get("sbnana");

  //set up branches
  Event* event = new Event;
  std::vector<int>* CCNC = new std::vector<int>;
  myTree->SetBranchAddress("events", &event);
  myTree->SetBranchAddress("CCNC",&CCNC);

  for (long i=0;i<myTree->GetEntries();i++){
    myTree->GetEntry(i);
    assert(event->interactions.size() == CCNC->size());

    std::cout<<"Event"<<i<<": ";

    for (size_t j=0;j<event->interactions.size();j++){
      auto enu = event->interactions[j].neutrino.energy;
      auto ccnc_type = CCNC->at(j);

      std::cout<<"Neutrino "<<j<<": "
               <<"E="<<enu<<" GeV"
               <<"CCNC type ="<<ccnc_type;
    }
    std::cout<<std::endl;
  }


}
