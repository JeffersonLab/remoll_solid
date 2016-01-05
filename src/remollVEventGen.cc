#include "remollVEventGen.hh"

#include "G4RotationMatrix.hh"

#include "remollBeamTarget.hh"
#include "remollVertex.hh"
#include "remollEvent.hh"
#include "remollRun.hh"
#include "remollRunData.hh"

remollVEventGen::remollVEventGen(){
    fBeamTarg = remollBeamTarget::GetBeamTarget();
    fRunData  = remollRun::GetRun()->GetData();

    fSampType       = kCryogen;
    fApplyMultScatt = false;

    fThCoM_min = -180.0*deg;
    fThCoM_max =  180.0*deg;
    fTh_min =   20.0*deg;
    fTh_max =   50.0*deg;
    fPh_min = -180.0*deg;
    fPh_max =  180.0*deg;
    fE_max  = gDefaultBeamE;
    fE_min  = 0.0;

}

remollVEventGen::~remollVEventGen(){
}

remollEvent *remollVEventGen::GenerateEvent(){
    // Set up beam/target vertex
    remollVertex vert   = fBeamTarg->SampleVertex(fSampType);

    /////////////////////////////////////////////////////////////////////
    // Create and initialize values for event
    remollEvent *thisev = new remollEvent();
    thisev->fVertexPos    = fBeamTarg->fVer;
    if( fApplyMultScatt ){
	thisev->fBeamMomentum = fBeamTarg->fSampE*(fBeamTarg->fDir.unit());
    } else {
	thisev->fBeamMomentum = fBeamTarg->fSampE*G4ThreeVector(0.0, 0.0, 1.0);
    }
    /////////////////////////////////////////////////////////////////////
   
    SamplePhysics(&vert, thisev);

    PolishEvent(thisev);

    return thisev;
}


void remollVEventGen::PolishEvent(remollEvent *ev){
    /*!
       Here it's our job to:
          Make sure the event is sane
          Apply multiple scattering effects to the final
	    products if applicable
	  Calculate rates from our given luminosity
	  Calculate measured asymmetry from polarization
	  Calculate vertex offsets
     */

    if( !ev->EventIsSane() ){
	G4cerr << __FILE__ << " line " << __LINE__ << ":  Event check failed for generator " << fName << ".  Aborting" << G4endl;
	ev->Print();
	exit(1);
    }

    G4ThreeVector rotax      = (fBeamTarg->fDir.cross(G4ThreeVector(0.0, 0.0, 1.0))).unit();
    G4RotationMatrix msrot;
    msrot.rotate(fBeamTarg->fDir.theta(), rotax);

    std::vector<G4ThreeVector>::iterator iter;

    if( fApplyMultScatt ){
	for( iter = ev->fPartRealMom.begin(); iter != ev->fPartRealMom.end(); iter++ ){
	    //  rotate direction vectors based on multiple scattering
	    (*iter) *= msrot;
	}

	// Rotate position offsets due to multiple scattering
	for( iter = ev->fPartPos.begin(); iter != ev->fPartPos.end(); iter++ ){
	    //  rotate direction vectors based on multiple scattering
	    (*iter) *= msrot;
	}
    }

    // Add base vertex
    for( iter = ev->fPartPos.begin(); iter != ev->fPartPos.end(); iter++ ){
	(*iter) += ev->fVertexPos;
    }

    /*
    //set rate only if it not already set. for hall D based MC generator I set the rate not the cross section
    //For this case target is empty and vertices from standalone generator are place in the target space. Therefore lumi is zero
    //For this reason I have to set the rate (normalization factor) in the SamplePhysics routine - Rakitha 10/15/15
    */
    if (ev->GetRate()==0){//if the rate is not already set 
      //ev->SetRate(ev->fEffXs*fBeamTarg->GetEffLumin()/((G4double) fRunData->GetNthrown()));
      ev->fRate = ev->fEffXs*fBeamTarg->GetEffLumin()/((G4double) fRunData->GetNthrown());
      //if (ev->fRate>0)
      //printf("DEBUG: get rate %e lumi %e xs %e \n ",ev->fEffXs*fBeamTarg->GetEffLumin()/((G4double) fRunData->GetNthrown()),fBeamTarg->GetEffLumin(),ev->fEffXs); 
    }
    ev->fmAsym = ev->fAsym*fBeamTarg->fBeamPol;

    return;
}


















