#ifndef __sbnanalysis_ana_ExampleAnalysis_ExampleSelection__
#define __sbnanalysis_ana_ExampleAnalysis_ExampleSelection__

/**
 * \file ExampleSelection.h
 *
 * An example event selection processor.
 *
 * This is an implementation of the core::SelectionBase class. We define
 * the methods called for initialization, finalization, and event-by-event
 * processing.
 *
 * Author: A. Mastbaum <mastbaum@uchicago.edu>
 */

// Includes
#include <iostream>
#include <string>
#include "canvas/Utilities/InputTag.h"
#include "core/SelectionBase.hh"

// Forward declarations
class TH2D;
class TH1D;

/** All analysis code is defined in namespace "ana" */
namespace ana {

  /** Code specific to the ExampleAnalysis. */
  namespace ExampleAnalysis {

/**
 * \class ExampleSelection
 * \brief An example selection analysis
 *
 * This selection analysis doesn't actually select events, it just
 * demonstrates the framework!
 */
class ExampleSelection : public core::SelectionBase {
public:
  /** Constructor. */
  ExampleSelection();

  /**
   * Initialization.
   *
   * Here we load configuration parameters, set up histograms for output, and
   * add our own branches to the output tree.
   *
   * \param config A configuration, as a JSON object
   */
  void Initialize(Json::Value* config=NULL);

  /** Finalize and write objects to the output file. */
  void Finalize();

  /**
   * Process one event.
   *
   * \param ev A single event, as a gallery::Event
   * \return True to keep event
   */
  bool ProcessEvent(gallery::Event& ev);

protected:
  unsigned fEventCounter;  //!< Count processed events

  /** Configuration parameters */
  art::InputTag fTruthTag;  //!< art tag for MCTruth information
  art::InputTag fTrackTag; //!< art tag for MCTrack information
  art::InputTag fShowerTag; //!< art tag for MCShower information
  int fMyParam;  //!< A parameter from the configuration file

  /** Custom data branches */
  int fNuCount;  //!< Number of neutrino interactions in the event
  int fMyVar;  //!< Another variable of interest
  int fMuCount;
  int fPrimaryMuCount;
  int fGoodMuCount;
  int fGoodNuCount;

  /** my data branches **/
  std::vector<Int_t> fCCNC;
  std::vector<Double_t> fTrackLength;
  std::vector<Double_t> fEndX;
  std::vector<Double_t> fEndY;
  std::vector<Double_t> fEndZ;
  std::vector<int> fPDGCode;
  //std::vector<const TLorentzVector&> fNuEndPos;
  //std::vector<const TLorentzVector&> fMuStartPos;
  std::vector<Double_t> fDiffLength;
  //std::vector<Double_t> fNuEndX;
  //std::vector<Double_t> fNuEndY;
  //std::vector<Double_t> fNuEndZ;
  //std::vector<simb::MCNeutrino> fGoodNu;


  /** Histograms */
  TH2D* fNuVertexXZHist; //!< Neutrino vertex XZ projection
  TH1D* fGoodNuEHist;
  TH1D* fNuEHist;
  TH1D* fInitialNumuHist;
  TH1D* fInitialContaminationHist;
  TH1D* fGoodNuCCHist;
  TH1D* fGoodNuNCHist;
};

  }  // namespace ExampleAnalysis
}  // namespace ana

#endif  // __sbnanalysis_ana_ExampleAnalysis_ExampleSelection__
