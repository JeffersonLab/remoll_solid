#ifndef __REMOLLGENFLAT_HH 
#define __REMOLLGENFLAT_HH 
/*!
 * Flat event generator
 *
 * Seamus Riordan
 * August 9, 2013
 *
*/

#include "remollVEventGen.hh"
//use to access hall D generator pion distributions
#include "TFile.h"
#include "TH2F.h"
#include "TH1F.h"

class remollRunData;

class remollGenFlat : public remollVEventGen {
    public:
	 remollGenFlat();
	~remollGenFlat();

         void SetParticleType(G4String t) { fParticleType = t; }

    private:
	void SamplePhysics(remollVertex *, remollEvent *);

	virtual void SetEmin( double emin ){ fE_min = emin; }
	virtual void SetEmax( double emax ){ fE_max = emax; }
  G4double pi0_rate,pim_rate,pip_rate,rate;

  G4String fParticleType;
  G4bool bUseExtGen;

  //for external generator
  TH2F * pion0kine;
  TH2F * pionmkine;
  TH2F * pionpkine;
  remollRunData    *fRunData;


};

#endif//__REMOLLGENFLAT_HH 
