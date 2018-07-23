#include <iostream>
#include <math.h>
#include <fstream>
#include <vector>
#include <typeinfo>
#include <cstdlib>

void learn_trees(){
  //gROOT->ProcessLine(".L $SBN_LIB_DIR/libsbnanalysis_Event.so"); //somehow it is not working
  /*
  open a root file, extract the TTree, and print out all its branches
  */
  TFile *myFile=new TFile("output_ExampleAnalysis_ExampleSelection.root");
  TTree *myTree=(TTree*)myFile->Get("sbnana");
  myTree->Print();
  auto nevent = myTree->GetEntries();

  /*
  new experiment: inspired by Andy's AnalysisMacro Code
  */

  //set up branches
  Event* event = new Event;
  myTree->SetBranchAddress("events", &event);

  for (long i=0; i<nevent;i++){
    myTree->GetEntry(i);
    std::cout<<"Event "<<i<<": ";

    for (size_t j=0;j<event->interactions.size();j++){
      double this_energy = event->interactions[j].neutrino.energy;
      double this_type = event->interactions[j].neutrino.genie_intcode;

      std::cout<<"Neutrino "<<j<<": "
      <<"E="<<this_energy<<" GeV"
      <<"Type="<<this_type;

      if (j<event->interactions.size()-1){
        std::cout<<"; ";
      }
    }

    std::cout<<std::endl;
  }


  }



  /*
  //auto genieintcode = myTree->GetBranch("interactions.neutrino.genie_intcode");
  TBranch* nu_energies=0;
  //auto nu_energies = myTree->GetBranch("interactions.neutrino.energy");
  auto nevent = myTree->GetEntries();
  Double_t *double_entry = 0;
  //nu_energies -> SetAddress(&event);

  //find min and max of energies
  auto E_min=20;
  auto E_max=0;
  myTree->SetBranchAddress("interactions.neutrino.energy", &double_entry, &nu_energies);
  for (int i =0;i<nevent;i++){
    auto this_energy = nu_energies->GetEntry(i);
    if (this_energy < E_min){
      E_min=this_energy;
    }
    if (this_energy > E_max){
      E_max=this_energy;
    }
    std::cout<<"E_"<<i<<": "<<this_energy<<std::endl;
  }

  std::cout<<"number of events: "<<nevent<<std::endl;
  std::cout<<"E_min: "<<E_min<<std::endl;
  std::cout<<"E_max: "<<E_max<<std::endl;
  */
  /*
  TLeaf* nu_energy=myTree->GetLeaf("interactions.neutrino.energy");
  TLeaf* nu_type=myTree->GetLeaf("interactions.neutrino.genie_intcode");
  auto energy_min=10;
  auto energy_max=0;
  auto nentries=nu_energy->GetBranch()->GetEntries();
  auto ntypes=nu_type->GetBranch()->GetEntries();
  for (int i=0;i<nentries;i++){
    nu_energy->GetBranch()->GetEntry(i);
    auto energy=nu_energy->GetValue();
    if (energy<energy_min) energy_min=energy;
    if (energy>energy_max) energy_max=energy;
    std::cout<<energy<<std::endl;
  }
  std::cout<<"# energy entries:"<<" "<<nentries<<std::endl;
  std::cout<<"energy min: "<<energy_min<<std::endl;
  std::cout<<"energy max: "<<energy_max<<std::endl;
*/

/*
  THStack* parents=new THStack("parents", "parent particles");
  for (int i=0;i<ntypes;i++){
    nu_energy->GetBranch()->GetEntry(i);
    nu_type->GetBranch()->GetEntry(i);
    auto energy=nu_energy->GetValue(); //read the ith neutrino energy (double)
    auto type=nu_type->GetValue();} //read the ith neutrino type (int)
    */
