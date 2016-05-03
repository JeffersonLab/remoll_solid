#ifndef __REMOLLGENBEAM_HH 
#define __REMOLLGENBEAM_HH 
/*!
 * Boring beam event generator
 *
 * Seamus Riordan
 * July 9, 2013
 *
*/

#include "remollVEventGen.hh"

class remollBeamTarget;

class remollGenBeam : public remollVEventGen {
    public:
	remollGenBeam();
	~remollGenBeam();
  void SetParticleType(G4String t) { 
    fParticleType = t;
    G4cout<<" Beam Generator particle type "<<fParticleType << G4endl;
  }
    private:
	void SamplePhysics(remollVertex *, remollEvent *);

	virtual void SetThMin(double){  G4cerr << __FILE__ << " line " << __LINE__ << " " << __PRETTY_FUNCTION__ << " :  Generator does not respond to this command" << G4endl; }
	virtual void SetThMax(double){  G4cerr << __FILE__ << " line " << __LINE__ << " " << __PRETTY_FUNCTION__ << " :  Generator does not respond to this command" << G4endl; }
	virtual void SetPhMin(double){  G4cerr << __FILE__ << " line " << __LINE__ << " " << __PRETTY_FUNCTION__ << " :  Generator does not respond to this command" << G4endl; }
	virtual void SetPhMax(double){  G4cerr << __FILE__ << " line " << __LINE__ << " " << __PRETTY_FUNCTION__ << " :  Generator does not respond to this command" << G4endl; }
  //use emax and min to set range of energies for remoll beam generator

  virtual void SetEmin( double emin ){ fE_min = emin; }
  virtual void SetEmax( double emax ){ fE_max = emax; }


  remollBeamTarget *fBeamTarg;

  double fZpos;
  G4String fParticleType;
};

#endif//__REMOLLGENBEAM_HH 
