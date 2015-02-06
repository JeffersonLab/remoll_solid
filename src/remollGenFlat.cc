#include "remollGenFlat.hh"

#include "CLHEP/Random/RandFlat.h"

#include "remollBeamTarget.hh"
#include "remollEvent.hh"
#include "remollVertex.hh"
#include "G4Material.hh"

#include "remolltypes.hh"

remollGenFlat::remollGenFlat(){
    fTh_min =     5.0*deg;
    fTh_max =    60.0*deg;
    fPh_min =  -180.0*deg;
    fPh_max =   180.0*deg;

    fE_min =      0.0*GeV;
    fE_max =     11.0*GeV;

    fParticleType = "e-";

    fApplyMultScatt = false;
}

remollGenFlat::~remollGenFlat(){
}

void remollGenFlat::SamplePhysics(remollVertex *vert, remollEvent *evt){
    // Generate Moller event


    double ene  = CLHEP::RandFlat::shoot(fE_min, fE_max);
    double th = acos(CLHEP::RandFlat::shoot(cos(fTh_max), cos(fTh_min)));
    double ph = CLHEP::RandFlat::shoot(fPh_min, fPh_max);

    double sigma = 1.0;

    //  Multiply by Z because we have Z electrons
    //  here we must also divide by two because we are double covering 
    //  phasespace because of identical particles
    
    evt->SetEffCrossSection(sigma);

    G4double APV = 0.0;

    evt->SetAsymmetry(APV);

    // Use unradiated beam energy
    double beamE = remollBeamTarget::GetBeamTarget()->fBeamE;
    double Q2 = 2.0*ene*beamE*(1.0-cos(th)  );
    evt->SetQ2( Q2 );
    evt->SetW2( proton_mass_c2*proton_mass_c2 + 2.0*proton_mass_c2*(beamE-ene) - Q2 );
    evt->SetXbj( Q2/(4.0*proton_mass_c2*(beamE-ene)) );


    evt->ProduceNewParticle( G4ThreeVector(0.0, 0.0, 0.0), 
	                     G4ThreeVector( ene*sin(th)*cos(ph), ene*sin(th)*sin(ph), ene*cos(th) ),
			     fParticleType );

    return;

}
