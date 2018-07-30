#include "TFile.h"
#include "TTree.h"
#include <vector>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>

void TTree_tutorial(){
  /*
  Exercises from the Introductory Tutorials: https://root.cern.ch/introductory-tutorials
  */
  TH1F *hPosX;
  /* 
  Calculate the sum of all event sizes
  */
  //variables used to store the data
  //Int_t totalSize=0; //sum of data size (in bytes) of all events
  TFile *file = TFile::Open("http://lcg-heppkg.web.cern.ch/lcg-heppkg/ROOT/eventdata.root");
  //notify if the file is not opened
  if (file==0){
    printf("Error: cannot open http://lcg-heppkg.web.cern.ch/lcg-heppkg/ROOT/eventdata.root! \n");
    return;
  }
  TTreeReader myReader("EventTree", file);
  TTreeReaderArray<Double_t> particlesPosX(myReader, "fParticles.fPosX");
  TTreeReaderArray<Double_t> particlesMomentum(myReader, "fParticles.fMomentum");
  //TTree *tree = (TTree*)file->Get("EventTree");
  //TTreeReaderValue<Int_t> eventSize (myReader, "fEventSize"); //make the fEventSize branch (and the data stored within) available

  //Loop over all entries of the TTree
  /*
  while (myReader.Next()){
    //Get the data from the current TTree entry by getting the value from the connected reader (eventSize):
    totalSize += *eventSize;
  }
  */

  //Int_t sizeInMB = totalSize/1024/1024;
  //printf("Total size of all events: %d MB\n", sizeInMB);

  /*
  Histogram with the value of fPosX from the tree from the tree for all particles with a momentum > 40.0
  */
  //TH1D* hPosX = new TH1D("hPosX", "Position in X", 20, -5, 5);//initialize the histogram

  hPosX = new TH1F ("hPosX", "Position in X", 20, -5, 5);
  hPosX->Sumw2();
  while (myReader.Next()){
    for (int iParticle=0;iParticle<particlesPosX.GetSize();++iParticle){
       if (particlesMomentum[iParticle] > 40.0)
        hPosX->Fill(particlesPosX[iParticle]);
    }
  }
  hPosX->Fit("pol2");

  hPosX->Draw();





  //tree->Print();
  /*
  Print() output:
  ******************************************************************************
*Tree    :EventTree : Tutorial tree                                          *
*Entries :      200 : Total =        24688321 bytes  File  Size =    1183802 *
*        :          : Tree compression factor =  20.90                       *
******************************************************************************
*Branch  :event                                                              *
*Entries :      200 : BranchElement (see below)                              *
*............................................................................*
*Br    0 :fParticles : Int_t fParticles_                                     *
*Entries :      200 : Total  Size=      10197 bytes  File Size  =        781 *
*Baskets :        1 : Basket Size=      32000 bytes  Compression=   2.16     *
*............................................................................*
*Br    1 :fParticles.fPosX : Double_t fPosX[fParticles_]                     *
*Entries :      200 : Total  Size=     185947 bytes  File Size  =     178891 *
*Baskets :        6 : Basket Size=      32000 bytes  Compression=   1.04     *
*............................................................................*
*Br    2 :fParticles.fPosY : Double_t fPosY[fParticles_]                     *
*Entries :      200 : Total  Size=     185947 bytes  File Size  =     170127 *
*Baskets :        6 : Basket Size=      32000 bytes  Compression=   1.09     *
*............................................................................*
*Br    3 :fParticles.fPosZ : Double_t fPosZ[fParticles_]                     *
*Entries :      200 : Total  Size=     185947 bytes  File Size  =     169249 *
*Baskets :        6 : Basket Size=      32000 bytes  Compression=   1.10     *
*............................................................................*
*Br    4 :fParticles.fMomentum : Double_t fMomentum[fParticles_]             *
*Entries :      200 : Total  Size=     185987 bytes  File Size  =     177298 *
*Baskets :        6 : Basket Size=      32000 bytes  Compression=   1.05     *
*............................................................................*
*Br    5 :fParticles.fMomentumPhi : Double_t fMomentumPhi[fParticles_]       *
*Entries :      200 : Total  Size=     186017 bytes  File Size  =     177631 *
*Baskets :        6 : Basket Size=      32000 bytes  Compression=   1.04     *
*............................................................................*
*Br    6 :fParticles.fMomentumEta : Double_t fMomentumEta[fParticles_]       *
*Entries :      200 : Total  Size=     186017 bytes  File Size  =     180142 *
*Baskets :        6 : Basket Size=      32000 bytes  Compression=   1.03     *
*............................................................................*
*Br    7 :fParticles.fTags[128] : Long64_t fTags[fParticles_]                *
*Entries :      200 : Total  Size=   23568033 bytes  File Size  =     125923 *
*Baskets :      173 : Basket Size=      32000 bytes  Compression= 187.13     *
*............................................................................*
*Br    8 :fEventSize : Int_t                                                 *
*Entries :      200 : Total  Size=       1391 bytes  File Size  =        651 *
*Baskets :        1 : Basket Size=      32000 bytes  Compression=   1.35     *
*............................................................................*
*/
  /*
  Drawing (with or without cuts)
  */
  /*
  tree->Draw("fPosY");
  TCut c1 = "fMomentum > 50.0";
  tree->Draw("fPosY", c1);
  */


}
