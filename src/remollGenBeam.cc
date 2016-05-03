#include "remollGenBeam.hh"

#include "CLHEP/Random/RandFlat.h"

#include "remollEvent.hh"
#include "remollVertex.hh"
#include "remollBeamTarget.hh"

#include "G4Material.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "Randomize.hh"

#include "remolltypes.hh"

#include <math.h>

remollGenBeam::remollGenBeam(){
    fApplyMultScatt = false;
    fBeamTarg = remollBeamTarget::GetBeamTarget();

    fZpos = -5.0*m;
    
    fE_min =     11.0*GeV;
    fE_max =     11.0*GeV;

    fParticleType = "e-";
}

remollGenBeam::~remollGenBeam(){
}

void remollGenBeam::SamplePhysics(remollVertex *vert, remollEvent *evt){
    // Get initial beam energy instead of using other sampling
    double beamE = fBeamTarg->fBeamE;
    double beam_mass = electron_mass_c2;
    if (fE_min < fE_max)
      beamE  = CLHEP::RandFlat::shoot(fE_min, fE_max);

    if (fParticleType.compareTo("gamma")==0)//for gamma beam set mass to zero
      beam_mass = 0;
      
    evt->fBeamE = beamE;
    //Fix the mass based on the energy and mass of the fParticleType
    evt->fBeamMomentum = evt->fBeamMomentum.unit()*sqrt(beamE*beamE - beam_mass*beam_mass);;

    //G4cout << "Beam energy corrected " << evt->fBeamMomentum  << " uncorrected " << fBeamTarg->fBeamE << G4endl;
    // Override target sampling z
    evt->fVertexPos.setZ( fZpos );
    //evt->fVertexPos.setX( 0 );
    //evt->fVertexPos.setY( 0 );


    evt->ProduceNewParticle( G4ThreeVector(0.0, 0.0, 0.0), 
	    evt->fBeamMomentum, 
	    fParticleType );

    evt->SetEffCrossSection(0.0);
    evt->SetAsymmetry(0.0);

    evt->SetQ2(0.0);
    evt->SetW2(0.0);

    return;

}
