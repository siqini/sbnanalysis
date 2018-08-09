#include <iostream>
#include <vector>
#include <TH2D.h>
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

#define PI 3.14159265


namespace ana {
  namespace ExampleAnalysis {

ExampleSelection::ExampleSelection() : SelectionBase(), fNuCount(0), fEventCounter(0),fMuCount(0),fPrimaryMuCount(0),fGoodMuCount(0),fGoodNuCount(0) {}


void ExampleSelection::Initialize(Json::Value* config) {
  // Make a histogram
  fNuVertexXZHist = new TH2D("nu_vtx_XZ", "",
                             100, -1000, 1000, 100, -1000, 1000);
  fGoodNuEHist = new TH1D ("good_nu_energy_hist","",60,0,6);
  fNuEHist = new TH1D ("nu_energy_hist","",60,0,6);
  fInitialNumuHist = new TH1D("initial_numu","",60,0,6);
  fInitialContaminationHist = new TH1D("initial_contamination","",60,0,6);
  fGoodNuCCHist = new TH1D("good_nu_CC","",60,0,6);
  fGoodNuNCHist = new TH1D ("good_nu_NC","",60,0,6);
  fGoodNuRecoCCHist= new TH1D ("good_nu_reco_CC","",60,0,6);
  fGoodNuRecoNCHist= new TH1D ("good_nu_reco_NC","",60,0,6);
  fLeptonEHist = new TH1D ("lepton_energy_hist","",60,0,6);
  fGoodNuRecoEHist = new TH1D ("good_nu_reco_energy_hist","",60,0,6);
  fInitialNumuAfterCutHist = new TH1D ("initial_numu_after_cut","",60,0,6);

  // Load configuration parameters
  fMyParam = 0;
  fTruthTag = { "generator" };
  fTrackTag = { "mcreco" };
  fShowerTag = { "mcreco" };

  if (config) {
    fMyParam = (*config)["ExampleAnalysis"].get("parameter", 0).asInt();
    fTruthTag = { (*config)["ExampleAnalysis"].get("MCTruthTag", "generator").asString() };
    fTrackTag = { (*config)["ExampleAnalysis"].get("MCTrackTag", "mcreco").asString()};
    fShowerTag = { (*config)["ExampleAnalysis"].get("MCShowerTag","mcreco").asString()};
  }

  // Add custom branches
  AddBranch("nucount", &fNuCount);
  AddBranch("myvar", &fMyVar);
  AddBranch("CCNC",&fCCNC);
  AddBranch("track_length",&fTrackLength);
  AddBranch("EndX",&fEndX);
  AddBranch("EndY",&fEndY);
  AddBranch("EndZ",&fEndZ);
  AddBranch("PDGCode",&fPDGCode);
  AddBranch("mucount",&fNuCount);
  AddBranch("primary_mu_count",&fPrimaryMuCount);
  AddBranch("good_mu_count",&fGoodMuCount);
  AddBranch("good_nu_count",&fGoodNuCount);
  //AddBranch("NuEndX",&fNuEndX);
  //AddBranch("NuEndY",&fNuEndY);
  //AddBranch("NuEndZ",&fNuEndZ);
  //AddBranch("NuEndPos",&fNuEndPos);
  //AddBranch("MuStartPos",&fMuStartPos);
  AddBranch("diff_length",&fDiffLength);
  AddBranch("lepton_momenta",&fLeptonMomenta);
  AddBranch("lepton_energy",&fLeptonEnergy);
  AddBranch("lepton_X",&fLeptonX);
  AddBranch("lepton_Y",&fLeptonY);
  AddBranch("lepton_Z",&fLeptonZ);
  AddBranch("reco_energy",&fRecoE);
  AddBranch("lepton_cos_theta",&fLeptonCosTheta);
  // Use some library code
  hello();
}


void ExampleSelection::Finalize() {
  // Output our histograms to the ROOT file
  fOutputFile->cd();
  fNuVertexXZHist->Write();
  fGoodNuEHist->Write();
  fNuEHist->Write();
  fInitialNumuHist->Write();
  fInitialContaminationHist->Write();
  fGoodNuCCHist->Write();
  fGoodNuNCHist->Write();
  fGoodNuRecoCCHist->Write();
  fGoodNuRecoNCHist->Write();
  fLeptonEHist->Write();
  fGoodNuRecoEHist->Write();
  fInitialNumuAfterCutHist->Write();
}


bool ExampleSelection::ProcessEvent(gallery::Event& ev) {
  //std::vector<const TLorentzVector&> NuEndPos;
  //std::vector<const TLorentzVector&> MuStartPos;
  std::vector<int> PrimaryMuonIndices;
  std::vector<int> PrimaryMuonIndicesAfterCut;
  if (fEventCounter % 10 == 0) {
    std::cout << "ExampleSelection: Processing event " << fEventCounter << std::endl;
  }
  fEventCounter++;

  // Grab a data product from the event
  auto const& mctruths = *ev.getValidHandle<std::vector<simb::MCTruth> >(fTruthTag);
  auto const& mctracks = *ev.getValidHandle<std::vector<sim::MCTrack> >(fTrackTag);
  auto const& mcshowers = *ev.getValidHandle<std::vector<sim::MCShower> >(fShowerTag);

  // Fill in the custom branches
  fNuCount = mctruths.size();  // Number of neutrinos in this event
  fMyVar = fMyParam;
  fCCNC.clear();
  fTrackLength.clear();
  fEndX.clear();
  fEndY.clear();
  fEndZ.clear();
  //fNuEndX.clear();
  //fNuEndY.clear();
  //fNuEndZ.clear();
  fPDGCode.clear();
  //fNuEndPos.clear();
  //fMuStartPos.clear();
  fDiffLength.clear();
  fLeptonMomenta.clear();
  fLeptonEnergy.clear();
  fLeptonX.clear();
  fLeptonY.clear();
  fLeptonZ.clear();
  fRecoE.clear();
  fLeptonCosTheta.clear();
  fMuCount =0;
  fPrimaryMuCount=0;
  fGoodMuCount=0;
  fGoodNuCount=0;



  // Iterate through the neutrinos
  for (size_t i=0; i<mctruths.size(); i++) {
    auto const& mctruth = mctruths.at(i);

    // Fill neutrino vertex position histogram
    fNuVertexXZHist->Fill(mctruth.GetNeutrino().Nu().Vx(),
                          mctruth.GetNeutrino().Nu().Vz());
    // Fill neutrino energy histogram
    fNuEHist -> Fill(mctruth.GetNeutrino().Nu().E());

    // Fill CCNC vector
    fCCNC.push_back(mctruth.GetNeutrino().CCNC());
    //NuEndPos.push_back(mctruth.GetNeutrino().Nu().EndPosition());
    auto nu_endX = mctruth.GetNeutrino().Nu().EndX();
    auto nu_endY = mctruth.GetNeutrino().Nu().EndY();
    auto nu_endZ = mctruth.GetNeutrino().Nu().EndZ();
    bool IsInFiducial = ((-260.1<=nu_endX)&&(nu_endX<=260.1))&& ((-271.15<=nu_endY)&&(nu_endY<=271.15))&&((-143.1<=nu_endZ)&&(nu_endZ<=559.6));
    bool IsNumu = (mctruth.GetNeutrino().Nu().PdgCode() == 14);
    if (IsNumu){
      fInitialNumuHist->Fill(mctruth.GetNeutrino().Nu().E());
    }
    else fInitialContaminationHist->Fill(mctruth.GetNeutrino().Nu().E());
    if (IsNumu && IsInFiducial){
      fInitialNumuAfterCutHist->Fill(mctruth.GetNeutrino().Nu().E());
    }

  }
  //Iterate through all product tracks
  for (size_t j=0;j<mctracks.size();j++){
    auto const& mctrack = mctracks.at(j);
    fTrackLength.push_back((mctrack.End().Position().Vect()-mctrack.Start().Position().Vect()).Mag());

    fEndX.push_back(mctrack.End().X());
    fEndY.push_back(mctrack.End().Y());
    fEndZ.push_back(mctrack.End().Z());
    fPDGCode.push_back(mctrack.PdgCode());
    bool IsMuon = true;
    //bool IsPrimary = (mctrack.Process()=="primary");
    bool IsPrimary = true;
    if (IsMuon) {
      fMuCount++;
    }
    if (IsPrimary && IsMuon) {
      fPrimaryMuCount++;
      PrimaryMuonIndices.push_back((int)j);
      //MuStartPos.push_back(mctrack.Start().Position());
    }
  }

  for (auto iMu : PrimaryMuonIndices){
    auto finalX = fEndX.at(iMu);
    auto finalY = fEndY.at(iMu);
    auto finalZ = fEndZ.at(iMu);
    auto tracklength = fTrackLength.at(iMu);
    if(((-199.15 < finalX && finalX < -2.65) || (2.65 < finalX && finalX < 199.15)) && (-200 < finalY && finalY < 200) && (0 < finalZ && finalZ < 500)) {
      if (tracklength >=50.) {PrimaryMuonIndicesAfterCut.push_back(iMu);
      }
    }
    else {
      if (tracklength>=100.) {
        PrimaryMuonIndicesAfterCut.push_back(iMu);
      }
    }
  }
  fGoodMuCount = (int)PrimaryMuonIndicesAfterCut.size();

  for (auto k : PrimaryMuonIndicesAfterCut){
    auto const& mct = mctracks.at(k);
    std::vector<int> GoodNuIndices;
    for (size_t l=0;l<mctruths.size();l++){
      auto const& mctru = mctruths.at(l);
      auto diff = (mct.Start().Position().Vect()- mctru.GetNeutrino().Nu().EndPosition().Vect()).Mag();
      fDiffLength.push_back(diff);
      //bool IsFromNuVtx = util::IsFromNuVertex(mctru,mct);
      if (diff<=5.){
          GoodNuIndices.push_back((int)l);
      }
      else continue;
    }
    if (GoodNuIndices.empty()) continue;
    else {
      int GoodNuIndex = GoodNuIndices.at(0);
      auto GoodNuE = mctruths.at(GoodNuIndex).GetNeutrino().Nu().E();
      fGoodNuEHist->Fill(GoodNuE);
      int GoodNuCCNCType = mctruths.at(GoodNuIndex).GetNeutrino().CCNC();
      if (GoodNuCCNCType ==0 )fGoodNuCCHist->Fill(GoodNuE);
      if (GoodNuCCNCType ==1 )fGoodNuNCHist->Fill(GoodNuE);
      fGoodNuCount++;
      auto this_lepton_energy = mct.Start().E();
      auto this_lepton_momentum = mct.Start().Momentum().Vect().Mag();
      auto this_lepton_X = mct.Start().X();
      auto this_lepton_Y = mct.Start().Y();
      auto this_lepton_Z = mct.Start().Z();
      /*Aug 7*/
      auto this_lepton_Px = mct.Start().Px();
      auto this_lepton_Py = mct.Start().Py();
      auto this_lepton_Pz = mct.Start().Pz();
      auto this_lepton_P = sqrt(pow(this_lepton_Px, 2.)+pow(this_lepton_Py,2.)+pow(this_lepton_Pz,2.));
      fLeptonEnergy.push_back(this_lepton_energy);
      fLeptonEHist->Fill(this_lepton_energy);
      fLeptonMomenta.push_back(this_lepton_momentum);
      fLeptonX.push_back(this_lepton_X);
      fLeptonY.push_back(this_lepton_Y);
      fLeptonZ.push_back(this_lepton_Z);
      auto this_lepton_cos_theta = this_lepton_Z/sqrt(pow(this_lepton_X,2)+pow(this_lepton_Y,2)+pow(this_lepton_Z,2));
      fLeptonCosTheta.push_back(this_lepton_cos_theta);
      auto m_p = 0.938;
      auto m_n = 0.940;
      auto E_B = 0.306;
      auto m_mu = 0.1057;
      auto this_reco_energy_top = pow(m_p,2.) - pow((m_n - E_B),2.) - pow(m_mu,2.)+2*(m_n - E_B)*this_lepton_energy;
      auto this_reco_energy_bottom = (m_n - E_B)-this_lepton_energy + this_lepton_P * this_lepton_cos_theta;
      double this_reco_energy = 0.5*(this_reco_energy_top/this_reco_energy_bottom);
      auto current_track_length = fTrackLength.at(k);
      double efficiency = ((0.2*(atan(current_track_length - 30.)))/PI) + 0.5;
      //srand(time(NULL));
      double lower_bound=0.;
      double upper_bound=1.;
      std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
      std::random_device r;
      std::default_random_engine e1(r());
      double a_random_double = unif(e1);
      /*
      int rndnum = rand()%100;
      */
      if (a_random_double<=efficiency){
        fRecoE.push_back(this_reco_energy);
        if (GoodNuCCNCType ==0 )fGoodNuRecoCCHist->Fill(this_reco_energy);
        if (GoodNuCCNCType ==1 )fGoodNuRecoNCHist->Fill(this_reco_energy);
        fGoodNuRecoEHist->Fill(this_reco_energy);
      }
      else continue;
    }
  }



  return true;
}

  }  // namespace ExampleAnalysis
}  // namespace ana


// This line must be included for all selections!
DECLARE_SBN_PROCESSOR(ana::ExampleAnalysis::ExampleSelection)
