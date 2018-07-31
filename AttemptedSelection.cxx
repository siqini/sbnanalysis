#include <iostream>
#include <vector>
#include <TH2D.h>
#include <json/json.h>
#include "gallery/ValidHandle.h"
#include "canvas/Utilities/InputTag.h"
#include "nusimdata/SimulationBase/MCTruth.h"
#include "nusimdata/SimulationBase/MCNeutrino.h"
#include "AttemptedSelection.h"


namespace ana{
  namespace AttemptedAnalysis{
    AttemptedSelection::AttemptedSelection() : SelectionBase(),fNuCount(0), fEventCounter(0){}

    void AttemptedSelection::Initialize(Json::Value* config){
      //make a histogram
      fNuVertexXZHist = new TH2D("nu_vtx_XZ","",100,-1000,1000,100,-1000,1000);

      //make another histogram
      fNuCCNCHist = new TH1I ("nu_ccnc","",10,-5,5);
      //load configuration parameters
      fTruthTag = {"generator"};

      if (config) {
        fTruthTag = {(*config)["AttemptedAnalysis"].get("MCTruthTag","generator").asString()};
      }

      //Add custom branches
      AddBranch("nucount",&fNuCount);
      AddBranch("energy_max",&fEnergyMax);
      AddBranch("energy_min",&fEnergyMin);
      AddBranch("CC_NC",&fCCNC)

    }

    void AttemptedSelection::Finalize(){
      //output our histograms to the ROOT file
      fOutputFile->cd();
      fNuVertexXZHist->Write();
    }

    bool AttemptedSelection::ProcessEvent(gallery::Event& ev){
      if (fEventCounter % 10 ==0){
        std::cout << "Attempted Selection: Processing event "<<fEventCounter<<std::endl;
      }
      fEventCounter++;
      fEnergyMax=0.;
      fEnergyMin=0.;
      //Grab a data product from the event
      auto const& mctruths = *ev.getValidHandle<std::vector<simb::MCTruth>>(fTruthTag);
      auto const& mcfluxs = *ev.getValidHandle<std::vector<simb::MCFlux>>(fTruthTag);
      assert(mctruths.size()==mcfluxs.size());

      fNuCount = mctruths.size();
      fParentPDG.clear();
      fCCNC.clear();

      //Iterate through the neutrinos
      for (size_t i=0;i<mctruths.size();i++){
        auto const& mctruth = mctruths.at(i);
        auto const& mcflux = mcfluxs.at(i);
        auto mctruth_energy = mctruth.GetNeutrino().Nu().E();
        if (mctruth_energy > fEnergyMax) {
          fEnergyMax = &mctruth_energy;
        }
        if (mctruth_energy < fEnergyMin) {
          fEnergyMin = &mctruth_energy;
        }
        fParentPDG.push_back(mcflux.fptype);
        fCCNC.push_back(mctruth.GetNeutrino().CCNC());

        fNuVertexXZHist->Fill(mctruth.GetNeutrino().Nu().Vx(), mctruth.GetNeutrino().Nu().Vz());

        fNuCCNCHist->Fill(mctruth().GetNeutrino().CCNC());


      }

      return true;
    }
  } //namespace AttemptedAnalysis
} //namespace ana

DECLARE_SBN_PROCESSOR(ana::AttemptedAnalysis::AttemptedSelection)
