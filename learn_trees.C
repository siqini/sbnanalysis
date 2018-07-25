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
  std::vector<Double_t> nu_energies;
  std::vector<Int_t> nu_types;
  for (long i=0; i<nevent;i++){
    myTree->GetEntry(i);
    std::cout<<"Event "<<i<<": ";

    for (size_t j=0;j<event->interactions.size();j++){
      double this_energy = event->interactions[j].neutrino.energy;
      double this_type = event->interactions[j].neutrino.genie_intcode;
      nu_energies.push_back(this_energy);
      nu_types.push_back(this_type);
      /*
      std::cout<<"Neutrino "<<j<<": "
      <<"E="<<this_energy<<" GeV"
      <<"Type="<<this_type;


      if (j<event->interactions.size()-1){
        std::cout<<"; ";
      }
      */
    }

    //std::cout<<std::endl;
  }

  Int_t nu_type_max=0;
  for (int k=0;k<nu_types.size();k++){
    if (nu_types[k]>nu_type_max){
      nu_type_max = nu_types[k];
    }
  }

  Double_t nu_e_max=0.;
  Double_t nu_e_min=10.;
  for (int iParticle=0;iParticle<nu_energies.size();iParticle++){
    if (nu_energies[iParticle]>nu_e_max){
      nu_e_max=nu_energies[iParticle];
    }
    if (nu_energies[iParticle]<nu_e_min){
      nu_e_min=nu_energies[iParticle];
    }
  }



    THStack* parents=new THStack("parents", "parent_particles");
    srand(123456);
    char buffer[nu_type_max];
    std::vector<TH1D*> hists=std::vector<TH1D*>();
    for (int iHist=0; iHist<nu_type_max;iHist++){
      hists.push_back(new TH1D(buffer[iHist], "", 50, 0, nu_e_max));
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

    for (int iEvent=0;iEvent<nevent;iEvent++){
      for (int iParticle=0;)
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
