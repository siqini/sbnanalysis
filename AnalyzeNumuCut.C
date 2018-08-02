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
  TTreeReader myReader("sbnana", myFile); //create a TTreeReader

  TTreeReaderValue<int> NuCount (myReader, "nucount");
  int totalNuCount=0;

  TTreeReaderValue<int> GoodNuCount (myReader, "good_nu_count");
  int totalGoodNuCount=0;
  while (myReader.Next()){
    totalNuCount+=NuCount;
    totalGoodNuCount+=GoodNuCount;
  }

  std::cout<<"Total nu count= "<<totalNuCount<<std::endl;
  std::cout<<"Total good nu count= "<<totalGoodNuCount<<std::endl;

}
