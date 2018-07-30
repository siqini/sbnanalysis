#ifndef __sbnanalysis_ana_AttemptedAnalysis_AttemptedSelection__
#define __sbnanalysis_ana_AttemptedAnalysis_AttemptedSelection__

#include <iostream>
#include "canvas/Utilities/InputTag.h"
#include "core/SelectionBase.hh"

class TH2D;

namespace ana{
  namespace AttemptedAnalysis{
    class AttemptedSelection: public core::SelectionBase{
    public:
      /*constructor*/
      AttemptedSelection();

      /*Initialization*/
      void Initialize(Json::Value* config=NULL);

      /*Finalization*/
      void Finalize();

      /*event level processing*/
      bool ProcessEvent(gallery::Event& ev);

    protected:
      unsigned fEventCounter;

      /*configuration parameters*/
      int fNuCount; //number of neutrinos in an event

      /*user-defined parameters*/
      Double_t fEnergyMax;
      Double_t fEnergyMin;
      std::vector<Int_t> fCCNC;

      /*histograms*/
      TH2D* fNuVertexXZHist;
      TH1I* fNuCCNCHist;
    };
  }
}

#endif
