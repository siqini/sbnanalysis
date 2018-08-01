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

ExampleSelection::ExampleSelection() : SelectionBase(), fNuCount(0), fEventCounter(0) {}


void ExampleSelection::Initialize(Json::Value* config) {
  // Make a histogram
  fNuVertexXZHist = new TH2D("nu_vtx_XZ", "",
                             100, -1000, 1000, 100, -1000, 1000);

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
  // Use some library code
  hello();
}


void ExampleSelection::Finalize() {
  // Output our histograms to the ROOT file
  fOutputFile->cd();
  fNuVertexXZHist->Write();
}


bool ExampleSelection::ProcessEvent(gallery::Event& ev) {
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



  // Iterate through the neutrinos
  for (size_t i=0; i<mctruths.size(); i++) {
    auto const& mctruth = mctruths.at(i);

    // Fill neutrino vertex position histogram
    fNuVertexXZHist->Fill(mctruth.GetNeutrino().Nu().Vx(),
                          mctruth.GetNeutrino().Nu().Vz());

    // Fill CCNC vector
    fCCNC.push_back(mctruth.GetNeutrino().CCNC());
  }

  for (size_t j=0;j<mctracks.size();j++){
    auto const& mctrack = mctracks.at(j);
    fTrackLength.push_back((mctrack.End().Position().Vect()-mctrack.Start().Position().Vect()).Mag());

    fEndX.push_back(mctrack.End().X());
    fEndY.push_back(mctrack.End().Y());
    fEndZ.push_back(mctrack.End().Z());
  }

  return true;
}

  }  // namespace ExampleAnalysis
}  // namespace ana


// This line must be included for all selections!
DECLARE_SBN_PROCESSOR(ana::ExampleAnalysis::ExampleSelection)
