#ifndef __sbnanalysis_ana_NueAnalysis_NueSelection__
#define __sbnanalysis_ana_NueAnalysis_NueSelection__

//Includes
#include <iostream>
#include <string>
#include "canvas/Utilities/InputTag.h"
#include "core/SelectionBase.hh"

//Forward declarations
class TH2D;
class TH1D;
class TVector3;

/** All analysis code is defined in namespace "ana" */
namespace ana {

  /**code specific to the NueAnalysis. */
  namespace NueAnalysis{
    class NueSelection : public core::SelectionBase {
    public:
      /** Constructor. */
      NueSelection();

      /** Initialization */
      void Initialize(Json::Value* config=NULL);

      /** Finalize and write objects to the output file */
      void Finalize();

      /** Process one event */
      bool ProcessEvent(gallery::Event& ev);

    protected:
      unsigned fEventCounter; //!< Count processed events

      /** Configuration parameters */
      art::InputTag fTruthTag; //!<art tag for MCTruth info
      art::InputTag fTrackTag; //!<art tag for MCTrack info
      art::InputTag fShowerTag; //!<art tag for MCShower info
      art::InputTag fPhotonTag;

      /** Custom data branches */
      int fNueCount; //!< number of neutrino interactions in the event
      int fPhotonCount;
      int fElectronCount; //
      int fGoodElectronCount;
      int fGoodNueCount;

      /** my data branches */

      /** Shower energy cut: reject <200MeV */
      std::vector<Double_t> fShowerEnergy;

      /** Second photon cut: reject if a second photon is present and has energy >100MeV */
      std::vector<Double_t> fPhotonEnergy;

      /** Conversion gap cut: first, determine if the vertex is visible
      * 1) vertex within active volume
      * 2) charged hadronic activity > 50MeV
      */
      std::vector<Double_t> fTrackPDGCode;
      std::vector<Double_t> fVx;
      std::vector<Double_t> fVY;
      std::vector<Double_t> fVz;
      /** Conversion gap cut: then, if all photon showers convert more than 3 cm from the vertex, reject the event */
      std::vector<Double_t> fShowerX;
      std::vector<Double_t> fShowerY;
      std::vector<Double_t> fShowerZ;
      /** dE/dx cut */
      std::vector<Double_t> fShowerdEdX;
      /**numu CC track length cut: reject if track length >= 1m */
      std::vector<Double_t> fTrackLength;

      /** neutrino energy reco using CCQE cross section */
      std::vector<Double_t> fElectronMomenta;
      //std::vector<Double_t> fElectronX;
      //std::vector<Double_t> fElectronY;
      //std::vector<Double_t> fElectronZ;
      std::vector<Double_t> fElectronCosTheta;
      std::vector<Double_t> fRecoE;

      /** Histograms */
      TH2D* fNueVertexXZHist;
      TH1D* fInitialFluxHist;
      TH1D* fInitialNueHist;
      TH1D* fInitialContaminationHist;
      TH1D* fGoodNueHist;
      TH1D* fGoodNueRecoHist;
      TH1D* fShowerEnergyHist;


    };
  } // namespace NueAnalysis
} // namespace ana

#endif //__sbnanalysis_ana_NueAnalysis_NueSelection__
