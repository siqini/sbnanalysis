#include <iostream>
#include <math.h>
#include <fstream>
#include <vector>
#include <typeinfo>
#include <cstdlib>

void learn_trees(){
  /*
  open a root file, extract the TTree, and print out all its branches
  */
  TFile *output=new TFile("output_ExampleAnalysis_ExampleSelection.root");
  TTree *mytree=(TTree*)output->Get("sbnana");
  mytree->Print();
}
