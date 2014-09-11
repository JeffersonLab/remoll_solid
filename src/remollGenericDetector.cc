#include "remollGenericDetector.hh"
#include "G4SDManager.hh"

remollGenericDetector::remollGenericDetector( G4String name, G4int detnum ) : G4VSensitiveDetector(name){
    char colname[255];

    fDetNo = detnum;
    assert( fDetNo > 0 );

//    fTrackSecondaries = false;
    fTrackSecondaries = true;

    sprintf(colname, "genhit_%d", detnum);
    collectionName.insert(G4String(colname));

    sprintf(colname, "gensum_%d", detnum);
    collectionName.insert(G4String(colname));

    fHCID = -1;
    fSCID = -1;

    // Threshold for material density where we do not record secondaries
    // and only primaries when the cross in, basically separate out
    // solids from gasses and liquids
    fDontRecordThresh = 1.0*g/cm3;
    //fDontRecordThresh = 0.0*g/cm3;//for testing by rakitha Mon Apr 21 14:49:08 EDT 2014
}

remollGenericDetector::~remollGenericDetector(){
}

void remollGenericDetector::Initialize(G4HCofThisEvent *){

    fHitColl = new remollGenericDetectorHitsCollection( SensitiveDetectorName, collectionName[0] );
    fSumColl = new remollGenericDetectorSumCollection ( SensitiveDetectorName, collectionName[1] );

    fSumMap.clear();
}

///////////////////////////////////////////////////////////////////////

G4bool remollGenericDetector::ProcessHits( G4Step *step, G4TouchableHistory *){
    G4bool badedep = false;
    G4bool badhit  = false;

    // Get touchable volume info
    G4TouchableHistory *hist = 
	(G4TouchableHistory*)(step->GetPreStepPoint()->GetTouchable());
    G4int  copyID = hist->GetReplicaNumber();

    G4StepPoint *prestep = step->GetPreStepPoint();
    G4Track     *track   = step->GetTrack();

    G4Material* material = track->GetMaterial();

    G4double edep = step->GetTotalEnergyDeposit();


    // We're just going to record primary particles and things
    // that have just entered our boundary if the material is
    // sufficiently dense
    badhit = true;
    if( (material->GetDensity() < fDontRecordThresh) ||
	   ( (track->GetCreatorProcess() == 0 && prestep->GetStepStatus() == fGeomBoundary ) ||
	    (prestep->GetStepStatus() == fGeomBoundary && fTrackSecondaries) )
      ){
	badhit = false;
    }



    //  Make pointer to new hit if it's a valid track
    remollGenericDetectorHit *thishit;
    if( !badhit ){
	thishit = new remollGenericDetectorHit(fDetNo, copyID);
	fHitColl->insert( thishit );
    }

    //  Get pointer to our sum  /////////////////////////
    remollGenericDetectorSum *thissum = NULL;

    if( !fSumMap.count(copyID) ){
	if( edep > 0.0 ){
	    thissum = new remollGenericDetectorSum(fDetNo, copyID);
	    fSumMap[copyID] = thissum;
	    fSumColl->insert( thissum );
	} else {
	    badedep = true;
	}
    } else {
	thissum = fSumMap[copyID];
    }
    /////////////////////////////////////////////////////

    // Do the actual data grabbing

    if( !badedep ){
	// This is all we need to do for the sum
	thissum->fEdep += edep;
    }

    if( !badhit ){
	// Hit
	thishit->f3X = prestep->GetPosition();
	thishit->f3V = track->GetVertexPosition();
	thishit->f3D = track->GetVertexMomentumDirection();
	thishit->f3P = track->GetMomentum();

	thishit->fP = track->GetMomentum().mag();
	thishit->fE = track->GetTotalEnergy();
	thishit->fM = track->GetDefinition()->GetPDGMass();
	thishit->fT = step->GetPreStepPoint()->GetGlobalTime();

	thishit->fTrID  = track->GetTrackID();
	thishit->fmTrID = track->GetParentID();
	thishit->fPID   = track->GetDefinition()->GetPDGEncoding();

	// FIXME - Enumerate encodings
	thishit->fGen   = (long int) track->GetCreatorProcess();
    }

    return !badedep && !badhit;
}

///////////////////////////////////////////////////////////////////////

void remollGenericDetector::EndOfEvent(G4HCofThisEvent*HCE) {
    G4SDManager *sdman = G4SDManager::GetSDMpointer();

    if(fHCID<0){ fHCID = sdman->GetCollectionID(collectionName[0]); }
    if(fSCID<0){ fSCID = sdman->GetCollectionID(collectionName[1]); }

    HCE->AddHitsCollection( fHCID, fHitColl );
    HCE->AddHitsCollection( fSCID, fSumColl );

    return;
}


