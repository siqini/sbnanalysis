#ifndef __sbnanalysis_ana_ExampleAnalysis_ExampleTools__
#define __sbnanalysis_ana_ExampleAnalysis_ExampleTools__

/**
 * \file ExampleTools.h
 *
 * Example library code.
 *
 * This is some auxiliary code that is not a selection, but does a piece
 * of the analysis. We can define any number of other functions, classes,
 * etc. which we use in the selection.
 *
 * Author: A. Mastbaum <mastbaum@uchicago.edu>
 */

namespace ana {
  namespace ExampleAnalysis {

/** A function that says hello. */
void hello();

/** A function that tells us whether we are looking at a muon track **/
bool AreYouMuon (int pdgcode);

/** A function that separates final state particles in or out of the TPC volume **/
bool AreYouIn (Double_t finalX, Double_t finalY, Double_t finalZ); //true=in, false=not in

/** A function that filters out MCTracks that are not primary **/
bool AreYouPrimary (const sim::MCTrack& mct);




  }  // namespace ExampleAnalysis
}  // namespace ana

#endif  // __sbnanalysis_ana_ExampleAnalysis_ExampleTools__
