#include <iostream>
#include <vector>
#include <TH2D.h>
#include <TVector3.h>
#include <json/json.h>
#include <cmath>
#include "gallery/ValidHandle.h"
#include "canvas/Utilities/InputTag.h"
#include "nusimdata/SimulationBase/MCTruth.h"
#include "nusimdata/SimulationBase/MCNeutrino.h"
#include "lardataobj/MCBase/MCTrack.h"
#include "lardataobj/MCBase/MCShower.h"
#include "nusimdata/SimulationBase/MCParticle.h"
#include "nusimdata/SimulationBase/MCFlux.h"
#include "larsim/EventWeight/Base/MCEventWeight.h"
#include "gallery/Event.h"
#include <TLorentzVector.h>
#include "ExampleSelection.h"
#include "ExampleTools.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <random>
#include "nusimdata/SimulationBase/SimPhotons.h"

#define PI 3.14159265

namespace ana {
  namespace NueAnalysis {
    NueSelection::NueSelection(): SelectionBase(), fNueCount(0),fEventCounter(0),fElectronCount(0), fGoodElectronCount(0),fGoodNueCount(0) {}

    void NueSelection::Initialize(Json::Value* config){
      // make some histograms
      fNueVertexXZHist = new TH2D ("nu_vtx_XZ","", 100,-1000,1000,100,-1000,1000);
      fInitialFluxHist = new TH1D ("initial_flux_hist","",100,0,10);
      fInitialNueHist = new TH1D ("initial_nue_hist","",100,0,10);
      fInitialContaminationHist = new TH1D ("initial_contam","",100,0,10);
      fGoodNueHist = new TH1D ("good_nue_hist","",100,0,10);
      fGoodNueRecoHist = new TH1D ("good_nue_reco_hist","",100,0,10);
      fShowerEnergyHist = new TH1D ("shower_energy_hist","",100,0,10);

      // Load config parameters
      fTruthTag = { "generator" };
      fTrackTag = { "mcreco" };
      fShowerTag = { "mcreco" };
      fPhotonTag = { "largeant" };

      if (config) {
        fTruthTag = { (*config)["NueAnalysis"].get("MCTruthTag","generator").asString() };
        fTrackTag = { (*config)["NueAnalysis"].get("MCTrackTag","mcreco").asString()};
        fShowerTag = { (*config)["NueAnalysis"].get("MCShowerTag","mcreco").asString()};
        fPhotonTag = { (*config)["NueAnalysis"].get("SimPhotonsTag","largeant").asString()};
      }

      // Add custom branches
      AddBranch("nue_count",&fNueCount);
      AddBranch("photon_count",&fPhotonCount);
      AddBranch("electron_count",&fElectronCount);
      AddBranch("good_electron_count",&fGoodElectronCount);
      AddBranch("good_nue_count",&fGoodNueCount);

      AddBranch("shower_energy",&fShowerEnergy);
      AddBranch("photon_energy",&fPhotonEnergy);
      AddBranch("track_PDGCode",&fTrackPDGCode);
      AddBranch("Vx",&fVx);
      AddBranch("Vy",&fVy);
      AddBranch("Vz",&fVz);
      AddBranch("showerX",&fShowerX);
      AddBranch("showerY",&fShowerY);
      AddBranch("showerZ",&fShowerZ);
      AddBranch("shower_dEdx",&fShowerdEdX);
      AddBranch("track_length",&fTrackLength);
      AddBranch("electron_momenta",&fElectronMomenta);
      //AddBranch("electronX",&fElectronX);
      //AddBranch("electronY",&fElectronY);
      //AddBranch("electronZ",&fElectronZ);
      AddBranch("electron_cos_theta",&fElectronCosTheta);
      AddBranch("reco_energy",&fRecoE);

      hello();
    }
    void NueSelection::Finalize(){
      //output our histograms to the ROOT file
      fOutputFile->cd();
      fNueVertexXZHist->Write();
      fInitialFluxHist->Write();
      fInitialNueHist->Write();
      fInitialContaminationHist->Write();
      fGoodNueHist->Write();
      fGoodNueRecoHist->Write();
      fShowerEnergyHist->Write();
    }

    bool NueSelection::ProcessEvent(gallery::Event& ev){
      if (fEventCounter % 10==0) {
        std::cout<< "NueSelection: Processing event "<<fEventCounter <<std::endl;
      }
      fEventCounter++;

      //Grab data products from the event
      auto const& mctruths = *ev.getValidHandle<std::vector<simb::MCTruth>>(fTruthTag);
      auto const& mctracks = *ev.getValidHandle<std::vector<sim::MCTrack>>(fTrackTag);
      auto const& mcshowers = *ev.getValidHandle<std::vector<sim::MCShower>>(fShowerTag);
      auto const& simphotons = *ev.getValidHandle<std::vector<sim::SimPhotons>>(fPhotonTag);

      //clean things up
      fPhotonCount =0;
      fElectronCount=0;
      fGoodElectronCount=0;
      fGoodNueCount=0;
      fShowerEnergy.clear();
      fPhotonEnergy.clear();
      fTrackPDGCode.clear();
      fVx.clear();
      fVy.clear();
      fVz.clear();
      fShowerX.clear();
      fShowerY.clear();
      fShowerZ.clear();
      fShowerdEdX.clear();
      fTrackLength.clear();
      fElectronMomenta.clear();
      //fElectronX.clear();
      //fElectronY.clear();
      //fElectronZ.clear();
      fElectronCosTheta.clear();
      fRecoE.clear();

      fNueCount = mctruths.size();

      //Iterate through the neutrinos
      for (size_t i=0;i<mctruths.size();i++){
        auto const& mctruth = mctruths.at(i);

        //Fill the neutrino vertex pos his
        fNueVertexXZHist->Fill(mctruth.GetNeutrino().Nu().Vx(), mctruth.GetNeutrino().Nu().Vz());

        //Fill neutrino energy his
        fInitialFluxHist->Fill(mctruth.GetNeutrino().Nu().E());

        //Fill the nue/contam hists
        bool IsNue = (mctruth.GetNeutrino().Nu().PdgCode()) == 12);
        if (IsNue){
          fInitialNueHist->Fill(mctruth.GetNeutrino().Nu().E());
        }
        else fInitialContaminationHist->Fill(mctruth.GetNeutrino().Nu().E());
      }

      //Iterate through all showes
      for (size_t j=0;j<mcshowers.size();j++) {
        auto const& mcshower = mcshowers.at(j);
        fShowerEnergy.push_back(mcshower.DetProfile().E());
        fShowerEnergyHist->Fill(mcshower.DetProfile().E());
        auto showerx = mcshower.DetProfile().X();
        auto showery = mcshower.DetProfile().Y();
        auto showerz = mcshower.DetProfile().Z();
        fShowerX.push_back(showerx);
        fShowerY.push_back(showery);
        fShowerZ.push_back(showerz);
        fShowerdEdX.push_back(mcshower.dEdx());
        fElectronMomenta.push_back((mcshower.DetProfile().Momentum().Vect().Mag());
        auto costheta = showerz/sqrt(pow(showerx, 2)+pow(showery,2)+pow(showerz,2));
        fElectronCosTheta.push_back(costheta);
      }

      for (size_t k=0;k<mctracks.size();k++){
        auto const& mctrack = mctracks.at(k);
        fTrackLength.push_back((mctrack.End().Position().Vect()-mctrack.Start().Position().Vect()).Mag());
        fTrackPDGCode.push_back(mctrack.PdgCode());
      }

      for (size_t l=0;l<simphotons.size();l++){
        auto const& simphoton = simphotons.at(l);
        for (size_t m=0;m<simphoton.size();m++){
          auto const& onephoton = simphoton.at(m);
          fPhotonEnergy.push_back(onephoton.Energy);
        }
      }



    }
  }// namespace NueAnalysis
} //namespace ana
