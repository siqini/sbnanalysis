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
  TFile *output=new TFile("output_ExampleAnalysis_ExampleSelection.root");
  TTree *mytree=(TTree*)output->Get("sbnana");
  //mytree->Print();

  TLeaf* nu_energy=mytree->GetLeaf("interactions.neutrino.energy");
  TLeaf* nu_type=mytree->GetLeaf("interactions.neutrino.genie_intcode");
  auto energy_min=0;
  auto energy_max=10;
  auto nentries=nu_energy->GetBranch()->GetEntries();

  for (int i=0;i<nentries;i++){
    nu_energy->GetBranch()->GetEntry(i);
    auto energy=nu_energy->GetValue();
    if (energy<energy_min) energy_min=energy;
    if (energy>energy_max) energy_max=energy;
  }
  std::cout<<"# energy entries:"<<" "<<nentries<<std::endl;
  std::cout<<"energy min: "<<energy_min<<std::endl;
  std::cout<<"energy max: "<<energy_max<<std::endl;
  /*
  Read a TTree using TTreeReader
  */
  TTreeReader theReader("T",output);
}
