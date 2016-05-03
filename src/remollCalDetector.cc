#include "remollCalDetector.hh"
#include "G4SDManager.hh"

remollCalDetector::remollCalDetector( G4String name, G4int detnum ) : G4VSensitiveDetector(name){
    char colname[255];

    fDetNo = detnum;
    assert( fDetNo > 0 );


    sprintf(colname, "calsum_%d", detnum);
    collectionName.insert(G4String(colname)); 

    fHCID = -1;
    fSCID = -1;
}

remollCalDetector::~remollCalDetector(){
}

void remollCalDetector::Initialize(G4HCofThisEvent *){

    fSumColl = new remollCalDetectorSumCollection ( SensitiveDetectorName, collectionName[0] );

    fSumMap.clear();
}

///////////////////////////////////////////////////////////////////////

G4bool remollCalDetector::ProcessHits( G4Step *step, G4TouchableHistory *){
    G4bool badedep = false;
    G4int copyID = 0;
    G4int ecal_id = 0;
    G4int ecal_block_id = 0;

    // Get touchable volume info
    G4TouchableHistory *hist = 
	(G4TouchableHistory*)(step->GetPreStepPoint()->GetTouchable());
    //G4TouchableHandle theTouchable = step->GetPreStepPoint()->GetTouchableHandle();
    G4StepPoint *prestep = step->GetPreStepPoint();
    G4Track     *track   = step->GetTrack();

    //For Scintillator, we need the ionization energy deposit. This can be obtained by subtracting non-ionization energy : Rakitha Fri Aug 15 10:20:13 EDT 2014
    //for some discussion : http://hypernews.slac.stanford.edu/HyperNews/geant4/get/eventtrackmanage/1043/1/1.html 
    //G4double edep = step->GetTotalEnergyDeposit() - step->GetNonIonizingEnergyDeposit();
    G4double edep = step->GetTotalEnergyDeposit();
    G4double globaltime_firsthit = 0;//step->GetPreStepPoint()->GetGlobalTime();
    G4double globaltime = step->GetPreStepPoint()->GetGlobalTime();
    
    /*
      Due to the geometrical design of the ecal, the copy id is not unique. This is due to one fraction of the ecal is first generated and then it is repeated along the X-Y plane. The copy id for ecal blocks within the first generated fraction (ecal_block_id) is unique. This id is then repeated in the other ecal blocks  (ecal_id). Therefore, ecal_id and ecal_block_id are both required to extract an unique id (copyID). In each ecal block there are 389 volumes with unique ids. There are 3461 ecal blocks in the ecal detector.
      : Rakitha Tue Nov  5 09:19:01 EST 2013
     */
    ecal_block_id = (hist->GetVolume(1)->GetCopyNo() - 35)/390;
    ecal_id = (hist->GetVolume()->GetCopyNo() - 1349045);

    copyID =  (ecal_block_id - 1) * 390 + ecal_id;

    //Folowing method is needed to access translation vectors of the physical volumes. Only Z is translated at each physical volume
    //But this physical volume is then placed at various X/Y locations of a mother volume. 
    //Therefore translation X/Y are taken from that mother volume.    

    G4double tr_xpos = hist->GetVolume(1)->GetTranslation().x();//Get X from the mother volume
    G4double tr_ypos = hist->GetVolume(1)->GetTranslation().y();//Get Y from the mother volume
    G4double tr_zpos = hist->GetVolume()->GetTranslation().z();//Get Z from the physical volume

    
    //debug code commented out : rakitha Tue Oct 29 13:04:08 EDT 2013
    /*   
    if (copyID){ 
    G4cout << "*************************** "<< G4endl;
    G4cout << "Mother copyID [" << hist->GetVolume(1)->GetCopyNo() << "]->copyID :" << copyID << G4endl;
    G4cout << "tr_xpos " << tr_xpos<< G4endl;
    G4cout << "tr_ypos " << tr_ypos<< G4endl;
    G4cout << "tr_zpos " << tr_zpos<< G4endl;
   G4cout << "*************************** "<< G4endl;
    } 
    */   
    

    //  Get pointer to our sum  /////////////////////////
    remollCalDetectorSum *thissum = NULL;

    if( !fSumMap.count(copyID) ){
	if( edep > 0.0 ){
	    thissum = new remollCalDetectorSum(fDetNo, copyID);
	    /*
	    G4cout << "*************************** "<< G4endl;
	    G4cout << "Mother copyID [" << ecal_block_id << "] ["<<ecal_id <<"]->copyID :" << copyID << G4endl;
	    G4cout << "tr_xpos " << tr_xpos<< G4endl;
	    G4cout << "tr_ypos " << tr_ypos<< G4endl;
	    G4cout << "tr_zpos " << tr_zpos<< G4endl;
	    G4cout << "*************************** "<< G4endl;	    
	    */
	    fSumMap[copyID] = thissum;
	    fSumColl->insert( thissum );
	    //get the global time of the first hit on the ecal block
	    globaltime_firsthit = step->GetPreStepPoint()->GetGlobalTime();
	} else {
	    badedep = true;
	}
    } else {
	thissum = fSumMap[copyID];
    }
    /////////////////////////////////////////////////////

    // Do the actual data grabbing

    G4double xpos = prestep->GetPosition().x();
    G4double ypos = prestep->GetPosition().y();

    if( !badedep ){
	// This is all we need to do for the sum
	thissum->fEdep   += edep;
	thissum->fPhoton += Edep2Photon(edep, prestep->GetPosition());
	thissum->fXsum   += edep*xpos;
	thissum->fYsum   += edep*ypos;
    	thissum->fX       = thissum->fXsum/thissum->fEdep;
    	thissum->fY       = thissum->fYsum/thissum->fEdep;
	thissum->fDet_X   = tr_xpos;
	thissum->fDet_Y   = tr_ypos;
	thissum->fDet_Z   = tr_zpos;

	//set the global time of the first hit into this cal detector
	if (globaltime_firsthit!=0){
	  thissum->ffT = globaltime_firsthit;
	  thissum->flT = globaltime_firsthit;
	} else { //update first hit time and last hit time for followup hits on the same cal block
	  if (globaltime < thissum->ffT) 
	  thissum->ffT = globaltime;
	
	  if  (globaltime > thissum->flT)
	    thissum->flT = globaltime;
	}
    }

    return !badedep;
}

///////////////////////////////////////////////////////////////////////

void remollCalDetector::EndOfEvent(G4HCofThisEvent*HCE) {
    G4SDManager *sdman = G4SDManager::GetSDMpointer();

    if(fSCID<0){ fSCID = sdman->GetCollectionID(collectionName[0]); }

    HCE->AddHitsCollection( fSCID, fSumColl );

    return;
}
 
///////////////////////////////////////////////////////////////////////

    /*  FIXME:   These need to be settable constants */
static const G4int    nScintlayer = 194; // 20 X0 for 0.5mm Pbs
static const G4double AbsorbThick = 0.5 * mm; // Sampling ratio = 0.253017(EM) 0.285811(MIP) 0.34+-0.04(H)
static const G4double ScintThick = 1.5 * mm;
static const G4double Shower_blockWidth = 2.0 * cm; // nX //?????? Rakitha Mon Jun 23 16:27:24 EDT 2014
static const G4double Shower_blockHeight = 2.0 * cm; // nY //?????? Rakitha Mon Jun 23 16:27:24 EDT 2014
static const G4double layerSep = 0.24 * mm + AbsorbThick + ScintThick;
static const G4double leading_Pb = 0.5137 * cm * 2; // 2 X0
static const G4double leading_Scint = 2  * cm;
static const G4double backend_Scint = 1.5 * mm;
static const G4double ShashlikBlockDepth = (nScintlayer - 2) * layerSep
        + leading_Pb + leading_Scint + backend_Scint;//?????? Rakitha Mon Jun 23 16:27:24 EDT 2014
static const G4double OverallShiftZ = 0 + ShashlikBlockDepth / 2;
static const G4int nX = 80;//?????? Rakitha Mon Jun 23 16:27:24 EDT 2014
static const G4int nY = 30;//?????? Rakitha Mon Jun 23 16:27:24 EDT 2014

G4double remollCalDetector::Edep2Photon(const G4double edep, const G4ThreeVector hitpos) {

    const G4double dis2middle = OverallShiftZ - hitpos.z();
    const G4double x = fmod(hitpos.x() - (nX % 2) * Shower_blockWidth / 2,
	    Shower_blockWidth);
    const G4double y = fmod(hitpos.y() - (nY % 2) * Shower_blockHeight / 2,
	    Shower_blockHeight);

    // 60 photon / MeV for 0.275mm PB - 1.5mm Scint, Nucl.Instrum.Meth.A584:291-303,2008
    G4double scale = 60 / MeV * (0.275 * mm / AbsorbThick)
	* (ScintThick / (1.5 * mm));

    // position dependence for one fiber per cm
    // approximately described in   Nucl.Instrum.Meth.A584:291-303,2008

    scale *= 1 - 5e-3 / 2 * cos(x / (1 * cm) * 2 * M_PI)
	- 5e-3 / 2 * cos(y / (1 * cm) * 2 * M_PI);

    // attenuation in the WLS fiber Y11(200)MS = 360cm, Nucl.Instrum.Meth.A584:291-303,2008
    scale *= exp(-dis2middle / (360 * cm));
    return edep * scale;
}

