#include <iostream>
#include <math.h>
#include <fstream>
#include <vector>
#include <typeinfo>
#include <cstdlib>

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

  //vector to store ccnc type info
  std::vector<int> CCNC_vec;
  std::vector<int> CCNC_unique;
  std::vector<double> nu_energy_vec;


  for (long i=0;i<myTree->GetEntries();i++){
    myTree->GetEntry(i);
    assert(event->interactions.size() == CCNC->size());

    //std::cout<<"Event"<<i<<": ";

    for (size_t j=0;j<event->interactions.size();j++){
      auto enu = event->interactions[j].neutrino.energy;
      auto ccnc_type = CCNC->at(j);

      if (CCNC_unique.empty()){
        CCNC_unique.push_back(ccnc_type);
      }
      else {
        if (find(CCNC_unique.begin(),CCNC_unique.end(),ccnc_type)==CCNC_unique.end()) CCNC_unique.push_back(ccnc_type);
      }


      CCNC_vec.push_back(ccnc_type);
      nu_energy_vec.push_back(enu);

      //std::cout<<"Neutrino "<<j<<": "
              // <<"E="<<enu<<" GeV"
            //   <<"CCNC type ="<<ccnc_type;
    }
  //  std::cout<<std::endl;
  }

  std::cout<<"# int types="<<CCNC_unique.size()<<std::endl;

  assert (CCNC_vec.size()==nu_energy_vec.size());
  auto nparticles = CCNC_vec.size();
  std::cout<< "# particles="<<nparticles<<std::endl;
  THStack* interaction_types = new THStack("interaction_types", "Interaction types");
  std::vector<TH1D*>hists;

  //set fill colors
  //srand(123);
  //char buffer[10];
  for (int i=0;i<CCNC_unique.size();i++){
    TH1D *h = new TH1D(Form("h%d",i),"",100,0,10);
    hists.push_back(h);
  }
  std::cout<<"# histograms="<<hists.size()<<std::endl;


  hists[0]->SetFillColor(kViolet);
  hists[1]->SetFillColor(kGreen);




  for (long i=0;i<nparticles;i++){
    auto this_ccnc_type = CCNC_vec[i];
    auto this_nu_energy = nu_energy_vec[i];

    hists[this_ccnc_type]->Fill(this_nu_energy);
  }



  interaction_types->Add(hists[0]);
  interaction_types->Add(hists[1]);

  interaction_types->Draw("nostack");




}
