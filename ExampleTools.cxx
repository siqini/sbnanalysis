#include <iostream>
#include "lardataobj/MCBase/MCTrack.h"


namespace ana {
  namespace ExampleAnalysis {

void hello() {
  std::cout << "Hello!" << std::endl;
}

bool AreYouIn (Double_t finalX, Double_t finalY, Double_t finalZ){
  if(((-199.15 < finalX && finalX < -2.65) || (2.65 < finalX && finalX < 199.15)) && (-200 < finalY && finalY < 200) && (0 < finalZ && finalZ < 500)) return true;
   else return false;
}

bool AreYouMuon (int pdgcode){
  if (pdgcode==13) return true; //13 is the PDG code for muon
  else return false;
}

bool AreYouPrimary (const sim::MCTrack& mct){
  if (mct.Process()=="primary") return true;
  else return false;
}

  }  // namespace ExampleAnalysis
}  // namespace ana
