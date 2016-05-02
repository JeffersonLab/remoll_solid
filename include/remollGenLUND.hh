#ifndef __REMOLLGENLUND_HH 
#define __REMOLLGENLUND_HH 
/*!
 * event generator based on LUND files
 *
 * Rakitha Beminiwattha
 * Fri Apr 15 12:51:25 EDT 2016
 *
*/

#include "remollVEventGen.hh"
#include "Riostream.h"
#include <map>
#include <set>

class remollRunData;

class remollGenLUND : public remollVEventGen {
public:
  remollGenLUND();
  ~remollGenLUND();
  
  void SetParticleType(G4String t) { fParticleType = t; }
  void SetLUNDFile(G4String f);
  
  
  
private:
  void SamplePhysics(remollVertex *, remollEvent *);
  
  G4String fParticleType;
  
  remollRunData    *fRunData;
  ifstream LUND_in; 

  G4String LUND_filename;
  Bool_t bLUND;
  std::map<int,G4String> pidname;

  Double_t line1[10];
  Double_t line2[14];
  Double_t momentum[3];
  Double_t ratefactor;
  Int_t pid;

};

#endif//__REMOLLGENLUND_HH 
