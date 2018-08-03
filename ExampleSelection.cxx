#include <iostream>
#include <vector>
#include <TH2D.h>
#include <json/json.h>
#include "gallery/ValidHandle.h"
#include "canvas/Utilities/InputTag.h"
#include "nusimdata/SimulationBase/MCTruth.h"
#include "nusimdata/SimulationBase/MCNeutrino.h"
#include "lardataobj/MCBase/MCTrack.h"
#include "lardataobj/MCBase/MCShower.h"
#include "nusimdata/SimulationBase/MCParticle.h"
#include "gallery/Event.h"
#include <TLorentzVector.h>
#include "ExampleSelection.h"
#include "ExampleTools.h"

namespace ana {
  namespace ExampleAnalysis {

ExampleSelection::ExampleSelection() : SelectionBase(), fNuCount(0), fEventCounter(0),fMuCount(0),fPrimaryMuCount(0),fGoodMuCount(0),fGoodNuCount(0) {}


void ExampleSelection::Initialize(Json::Value* config) {
  // Make a histogram
  fNuVertexXZHist = new TH2D("nu_vtx_XZ", "",
                             100, -1000, 1000, 100, -1000, 1000);
  fGoodNuEHist = new TH1D ("good_nu_energy_hist","",100,0,10);
  fNuEHist = new TH1D ("nu_energy_hist","",100,0,10);
  fInitialNumuHist = new TH1D("initial_numu","",100,0,10);
  fInitialContaminationHist = new TH1D("initial_contamination","",100,0,10);
  fGoodNuCCHist = new TH1D("good_nu_CC","",100,0,10);
  fGoodNuNCHist = new TH1D ("good_nu_NC","",100,0,10);

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

    bool IsNumu = (mctruth.GetNeutrino().Nu().PdgCode() == 14);
    if (IsNumu){
      fInitialNumuHist->Fill(mctruth.GetNeutrino().Nu().E());
    }
    else fInitialContaminationHist->Fill(mctruth.GetNeutrino().Nu().E());
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
    bool IsPrimary = (mctrack.Process()=="primary");
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
    }
  }



  return true;
}

  }  // namespace ExampleAnalysis
}  // namespace ana


// This line must be included for all selections!
DECLARE_SBN_PROCESSOR(ana::ExampleAnalysis::ExampleSelection)
