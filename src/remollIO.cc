#include "remollIO.hh"

#include <TFile.h>
#include <TTree.h>
#include <TClonesArray.h>

#include "G4ParticleDefinition.hh"

#include "remollGenericDetectorHit.hh"
#include "remollGenericDetectorSum.hh"
#include "remollCalDetectorSum.hh"
#include "remollEvent.hh"
#include "remollRun.hh"
#include "remollRunData.hh"
#include "remollBeamTarget.hh"

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNode.hpp>


remollIO::remollIO(){
    fTree = NULL;
    InitializeTree();
    // Default filename
    strcpy(fFilename, "remollout.root");
    fEv_num=0;//set event number zero at the beginning Rakitha Wed Nov 20 17:20:02 EST 2013
    fFile = NULL;
}

remollIO::~remollIO(){
    if( fTree ){ delete fTree; }
    fTree = NULL;
    if( fFile ){ delete fFile; }
    fFile = NULL;
}

void remollIO::SetFilename(G4String fn){
    G4cout << "Setting output file to " << fn << G4endl;
    strcpy(fFilename, fn.data());
}

void remollIO::InitializeTree(){
    if( fFile ){
	fFile->Close();
	delete fFile;
    }

    fFile = new TFile(fFilename, "RECREATE");

    if( fTree ){ delete fTree; }

    fTree = new TTree("T", "Geant4 Moller Simulation");

    // Event information
    fTree->Branch("rate",     &fEvRate,   "rate/D");
    fTree->Branch("ev.A",     &fEvAsym,   "ev.A/D");
    fTree->Branch("ev.Am",    &fEvmAsym,  "ev.Am/D");
    fTree->Branch("ev.xs",    &fEvEffXS,  "ev.xs/D");
    fTree->Branch("ev.Q2",    &fEvQ2,     "ev.Q2/D");
    fTree->Branch("ev.W2",    &fEvW2,     "ev.W2/D");
    fTree->Branch("ev.xbj",    &fEvXbj,     "ev.xbj/D");
    fTree->Branch("ev.thcom", &fEvThCoM,  "ev.thcom/D");
    fTree->Branch("ev.beamp",  &fEvBeamP,   "ev.beamp/D");
    fTree->Branch("ev.evnum",  &fEv_num,   "ev.evnum/I");

    fTree->Branch("ev.npart", &fNEvPart   ,     "ev.npart/I");
    fTree->Branch("ev.pid",   &fEvPID,      "ev.pid[ev.npart]/I");
    fTree->Branch("ev.vx",    &fEvPart_X,   "ev.vx[ev.npart]/D");
    fTree->Branch("ev.vy",    &fEvPart_Y,   "ev.vy[ev.npart]/D");
    fTree->Branch("ev.vz",    &fEvPart_Z,   "ev.vz[ev.npart]/D");
    fTree->Branch("ev.p",     &fEvPart_P,   "ev.p[ev.npart]/D");
    fTree->Branch("ev.px",    &fEvPart_Px,  "ev.px[ev.npart]/D");
    fTree->Branch("ev.py",    &fEvPart_Py,  "ev.py[ev.npart]/D");
    fTree->Branch("ev.pz",    &fEvPart_Pz,  "ev.pz[ev.npart]/D");
    fTree->Branch("ev.th",    &fEvPart_Th,     "ev.th[ev.npart]/D");
    fTree->Branch("ev.ph",    &fEvPart_Ph,     "ev.ph[ev.npart]/D");
    fTree->Branch("ev.tpx",    &fEvPart_tPx,  "ev.tpx[ev.npart]/D");
    fTree->Branch("ev.tpy",    &fEvPart_tPy,  "ev.tpy[ev.npart]/D");
    fTree->Branch("ev.tpz",    &fEvPart_tPz,  "ev.tpz[ev.npart]/D");

    fTree->Branch("bm.x",    &fBmX,  "bm.x/D");
    fTree->Branch("bm.y",    &fBmY,  "bm.y/D");
    fTree->Branch("bm.z",    &fBmZ,  "bm.z/D");
    fTree->Branch("bm.dx",    &fBmdX,  "bm.dx/D");
    fTree->Branch("bm.dy",    &fBmdY,  "bm.dy/D");
    fTree->Branch("bm.dz",    &fBmdZ,  "bm.dz/D");

    // GenericDetectorHit
    fTree->Branch("hit.n",    &fNGenDetHit,     "hit.n/I");
    fTree->Branch("hit.det",  &fGenDetHit_det,  "hit.det[hit.n]/I");
    fTree->Branch("hit.vid",  &fGenDetHit_id,   "hit.vid[hit.n]/I");

    fTree->Branch("hit.pid",  &fGenDetHit_pid,   "hit.pid[hit.n]/I");
    fTree->Branch("hit.trid", &fGenDetHit_trid,  "hit.trid[hit.n]/I");
    fTree->Branch("hit.mtrid",&fGenDetHit_mtrid, "hit.mtrid[hit.n]/I");
    fTree->Branch("hit.gen",  &fGenDetHit_gen,   "hit.gen[hit.n]/I");

    fTree->Branch("hit.x",    &fGenDetHit_X,   "hit.x[hit.n]/D");
    fTree->Branch("hit.y",    &fGenDetHit_Y,   "hit.y[hit.n]/D");
    fTree->Branch("hit.z",    &fGenDetHit_Z,   "hit.z[hit.n]/D");
    fTree->Branch("hit.r",    &fGenDetHit_R,   "hit.r[hit.n]/D");
    fTree->Branch("hit.t",    &fGenDetHit_T,   "hit.t[hit.n]/D");

    fTree->Branch("hit.px",   &fGenDetHit_Px,   "hit.px[hit.n]/D");
    fTree->Branch("hit.py",   &fGenDetHit_Py,   "hit.py[hit.n]/D");
    fTree->Branch("hit.pz",   &fGenDetHit_Pz,   "hit.pz[hit.n]/D");

    fTree->Branch("hit.vx",   &fGenDetHit_Vx,   "hit.vx[hit.n]/D");
    fTree->Branch("hit.vy",   &fGenDetHit_Vy,   "hit.vy[hit.n]/D");
    fTree->Branch("hit.vz",   &fGenDetHit_Vz,   "hit.vz[hit.n]/D");

    fTree->Branch("hit.vdx",   &fGenDetHit_Vdx,   "hit.vdx[hit.n]/D");
    fTree->Branch("hit.vdy",   &fGenDetHit_Vdy,   "hit.vdy[hit.n]/D");
    fTree->Branch("hit.vdz",   &fGenDetHit_Vdz,   "hit.vdz[hit.n]/D");

    fTree->Branch("hit.p",    &fGenDetHit_P,   "hit.p[hit.n]/D");
    fTree->Branch("hit.e",    &fGenDetHit_E,   "hit.e[hit.n]/D");
    fTree->Branch("hit.m",    &fGenDetHit_M,   "hit.m[hit.n]/D");
    
    // GenericDetectorSum
    fTree->Branch("sum.n",    &fNGenDetSum,     "sum.n/I");
    fTree->Branch("sum.det",  &fGenDetSum_det,  "sum.det[sum.n]/I");
    fTree->Branch("sum.vid",  &fGenDetSum_id,   "sum.vid[sum.n]/I");
    fTree->Branch("sum.edep", &fGenDetSum_edep, "sum.edep[sum.n]/D");
    
    // CalDetectorSum
    fTree->Branch("cal.n",    &fNCalDetSum,     "cal.n/I");
    //PID based edep
    //fTree->Branch("cal.pid",  &fCalDetSum_pid,  "cal.pid[cal.n]/I");
    fTree->Branch("cal.det",  &fCalDetSum_det,  "cal.det[cal.n]/I");
    fTree->Branch("cal.vid",  &fCalDetSum_id,   "cal.vid[cal.n]/I");
    fTree->Branch("cal.edep", &fCalDetSum_edep, "cal.edep[cal.n]/D");
    fTree->Branch("cal.phot", &fCalDetSum_photon, "cal.phot[cal.n]/D");
    fTree->Branch("cal.x",    &fCalDetSum_x,    "cal.x[cal.n]/D");
    fTree->Branch("cal.y",    &fCalDetSum_y,    "cal.y[cal.n]/D");
    fTree->Branch("cal.st",    &fCalDetSum_starttime,    "cal.st[cal.n]/D");
    fTree->Branch("cal.et",    &fCalDetSum_endtime,    "cal.et[cal.n]/D");
    

    fTree->Branch("cal.det_x",    &fCalDetPos_X,    "cal.det_x[cal.n]/D");
    fTree->Branch("cal.det_y",    &fCalDetPos_Y,    "cal.det_y[cal.n]/D");
    fTree->Branch("cal.det_z",    &fCalDetPos_Z,    "cal.det_z[cal.n]/D");

    fEv_num=0;//set event number zero at the beginning Rakitha Wed Nov 20 17:20:02 EST 2013
    

    return;
}

void remollIO::FillTree(){
    if( !fTree ){ 
	fprintf(stderr, "Error %s: %s line %d - Trying to fill non-existant tree\n", __PRETTY_FUNCTION__, __FILE__, __LINE__ );
	return; 
    }

    fTree->Fill();
}

void remollIO::Flush(){
    //  Set arrays to 0
    fNGenDetHit = 0;
    fNGenDetSum = 0;
    fNCalDetSum = 0;
}

void remollIO::WriteTree(){
    assert( fFile );
    assert( fTree );

    if( !fFile->IsOpen() ){
	G4cerr << "ERROR: " << __FILE__ << " line " << __LINE__ << ": TFile not open" << G4endl;
	exit(1);
    }

    G4cout << "Writing output to " << fFile->GetName() << "... ";

    fFile->cd();

    fTree->Write("T", TObject::kOverwrite);
    remollRun::GetRun()->GetData()->Write("run_data", TObject::kOverwrite); 

    fTree->ResetBranchAddresses();
    delete fTree;
    fTree = NULL;

    fFile->Close();

    delete fFile;
    fFile = NULL;

    G4cout << "written" << G4endl;

    return;
}

///////////////////////////////////////////////////////////////////////////////
// Interfaces to output section ///////////////////////////////////////////////

// Event Data

void remollIO::SetEventData(remollEvent *ev){
    int n = ev->fPartType.size();
    if( n >= __IO_MAXHIT ){
//	G4cerr << "WARNING: " << __PRETTY_FUNCTION__ << " line " << __LINE__ << ":  Buffer size exceeded!" << G4endl;
	return;
    }
    fEv_num++; //increment the event counter Rakitha Wed Nov 20 17:20:02 EST 2013
    fNEvPart = n;

    fEvRate   = ev->fRate*s;
    fEvEffXS  = ev->fEffXs/microbarn;
    fEvAsym   = ev->fAsym/__ASYMM_SCALE;
    fEvmAsym  = ev->fmAsym/__ASYMM_SCALE;//beam polarization corrected
    fEvBeamP  = ev->fBeamMomentum.mag()/__E_UNIT;

    fEvQ2     = ev->fQ2/__E_UNIT/__E_UNIT;
    fEvW2     = ev->fW2/__E_UNIT/__E_UNIT;
    fEvXbj    = ev->fXbj;
    fEvThCoM  = ev->fThCoM/deg; // specify this in degrees over anything else

    int idx;
    for( idx = 0; idx < n; idx++ ){
	fEvPID[idx] = ev->fPartType[idx]->GetPDGEncoding();

	fEvPart_X[idx] = ev->fPartPos[idx].x()/__L_UNIT;
	fEvPart_Y[idx] = ev->fPartPos[idx].y()/__L_UNIT;
	fEvPart_Z[idx] = ev->fPartPos[idx].z()/__L_UNIT;

	fEvPart_Px[idx] = ev->fPartRealMom[idx].x()/__E_UNIT;
	fEvPart_Py[idx] = ev->fPartRealMom[idx].y()/__E_UNIT;
	fEvPart_Pz[idx] = ev->fPartRealMom[idx].z()/__E_UNIT;
	fEvPart_Th[idx] = ev->fPartRealMom[idx].theta();
	fEvPart_Ph[idx] = ev->fPartRealMom[idx].phi();

	fEvPart_P[idx] = ev->fPartRealMom[idx].mag()/__E_UNIT;

	fEvPart_tPx[idx] = ev->fPartMom[idx].x()/__E_UNIT;
	fEvPart_tPy[idx] = ev->fPartMom[idx].y()/__E_UNIT;
	fEvPart_tPz[idx] = ev->fPartMom[idx].z()/__E_UNIT;
    }

    /////////////////////////////////////////////////
    //  Set beam data as well

    remollBeamTarget *bt = remollBeamTarget::GetBeamTarget();

    fBmX = bt->fVer.x()/__L_UNIT;
    fBmY = bt->fVer.y()/__L_UNIT;
    fBmZ = bt->fVer.z()/__L_UNIT;
    
    fBmdX = bt->fDir.x();
    fBmdY = bt->fDir.y();
    fBmdZ = bt->fDir.z();

    return;
}

// GenericDetectorHit

void remollIO::AddGenericDetectorHit(remollGenericDetectorHit *hit){
    int n = fNGenDetHit;
    if( n >= __IO_MAXHIT ){
//	G4cerr << "WARNING: " << __PRETTY_FUNCTION__ << " line " << __LINE__ << ":  Buffer size exceeded!" << G4endl;
	return;
    }

    fGenDetHit_det[n]  = hit->fDetID;
    fGenDetHit_id[n]   = hit->fCopyID;

    fGenDetHit_trid[n] = hit->fTrID;
    fGenDetHit_mtrid[n]= hit->fmTrID;
    fGenDetHit_pid[n]  = hit->fPID;
    fGenDetHit_gen[n]  = hit->fGen;

    fGenDetHit_X[n]  = hit->f3X.x()/__L_UNIT;
    fGenDetHit_Y[n]  = hit->f3X.y()/__L_UNIT;
    fGenDetHit_Z[n]  = hit->f3X.z()/__L_UNIT;
    fGenDetHit_R[n]  = sqrt(hit->f3X.x()*hit->f3X.x()+hit->f3X.y()*hit->f3X.y())/__L_UNIT;
    
    fGenDetHit_Px[n]  = hit->f3P.x()/__E_UNIT;
    fGenDetHit_Py[n]  = hit->f3P.y()/__E_UNIT;
    fGenDetHit_Pz[n]  = hit->f3P.z()/__E_UNIT;

    fGenDetHit_Vx[n]  = hit->f3V.x()/__L_UNIT;
    fGenDetHit_Vy[n]  = hit->f3V.y()/__L_UNIT;
    fGenDetHit_Vz[n]  = hit->f3V.z()/__L_UNIT;

    fGenDetHit_Vdx[n]  = hit->f3D.x();
    fGenDetHit_Vdy[n]  = hit->f3D.y();
    fGenDetHit_Vdz[n]  = hit->f3D.z();

    fGenDetHit_P[n]  = hit->fP/__E_UNIT;
    fGenDetHit_E[n]  = hit->fE/__E_UNIT;
    fGenDetHit_M[n]  = hit->fM/__E_UNIT;
    fGenDetHit_T[n]  = hit->fT/__T_UNIT;

    fNGenDetHit++;
}


// GenericDetectorSum

void remollIO::AddGenericDetectorSum(remollGenericDetectorSum *hit){
    int n = fNGenDetSum;
    if( n >= __IO_MAXHIT ){
//	G4cerr << "WARNING: " << __PRETTY_FUNCTION__ << " line " << __LINE__ << ":  Buffer size exceeded!" << G4endl;
	return;
    }

    fGenDetSum_edep[n] = hit->fEdep/__E_UNIT;
    fGenDetSum_det[n]  = hit->fDetID;
    fGenDetSum_id[n]   = hit->fCopyID;

    fNGenDetSum++;
}

// CalDetectorSum

void remollIO::AddCalDetectorSum(remollCalDetectorSum *hit){
  // Particles that are payed attention to in our sums, 0 means all types
  int particle_to_track[N_PART_DIVISIONS] = {0, -11, 11, 13, 22, 2112, 2212, -211, 211};

    int n = fNCalDetSum;
    if( n >= __IO_MAXHIT ){
	G4cerr << "remollIO::WARNING: " << __PRETTY_FUNCTION__ << " line " << __LINE__ << ":  Buffer size exceeded!" << G4endl;
	return;
    }

    /*
    //PID based edep
    int j;

    for( j = 0; j < N_PART_DIVISIONS; j++ ){
	fCalDetSum_edep[n] = hit->GetEdep(particle_to_track[j])/__E_UNIT;
	fCalDetSum_pid[n]  = particle_to_track[j];
	fCalDetSum_photon[n] = hit->fPhoton;
	G4ThreeVector pos  = hit->GetPos(particle_to_track[j]);

	fCalDetSum_x[n]    = pos.x()/__L_UNIT;
	fCalDetSum_y[n]    = pos.y()/__L_UNIT; 

	fCalDetSum_det[n]  = hit->fDetID;
	fCalDetSum_id[n]   = hit->fCopyID;

	fCalDetSum_starttime[n] = hit->ffT/__T_UNIT;
	fCalDetSum_endtime[n] = hit->flT/__T_UNIT;

	fCalDetPos_X[n]     = hit->fDet_X/__L_UNIT;
	fCalDetPos_Y[n]     = hit->fDet_Y/__L_UNIT;
	fCalDetPos_Z[n]     = hit->fDet_Z/__L_UNIT;
	n++;
	fNCalDetSum++;

	if( n >= __IO_MAXHIT ){
	  G4cerr << "WARNING: " << __PRETTY_FUNCTION__ << " line " << __LINE__ << ":  Buffer size exceeded!" << ": detid "<< hit->fDetID <<": pid " << j << G4endl;
	    return;
	}
    }
    */
    //to save ecal hits with no pid information
    
    fCalDetSum_edep[n] = hit->fEdep/__E_UNIT;
    fCalDetSum_photon[n] = hit->fPhoton;
    fCalDetSum_det[n]  = hit->fDetID;
    fCalDetSum_id[n]   = hit->fCopyID;
    fCalDetSum_x[n]    = hit->fX/__L_UNIT;
    fCalDetSum_y[n]    = hit->fY/__L_UNIT;
    //Adding start and end time for ecal block in each event
    fCalDetSum_starttime[n] = hit->ffT/__T_UNIT;
    fCalDetSum_endtime[n] = hit->flT/__T_UNIT;

    //Adding ecal block phys volume location for each hit : rakitha Tue Oct 29 13:30:01 EDT 2013
    fCalDetPos_X[n]     = hit->fDet_X/__L_UNIT;
    fCalDetPos_Y[n]     = hit->fDet_Y/__L_UNIT;
    fCalDetPos_Z[n]     = hit->fDet_Z/__L_UNIT;

    fNCalDetSum++;
    
}
/*---------------------------------------------------------------------------------*/

void remollIO::GrabGDMLFiles(G4String fn){
    // Reset list
    fGDMLFileNames.clear();

    remollRunData *rundata = remollRun::GetRun()->GetData();
    rundata->ClearGDMLFiles();

    xercesc::XMLPlatformUtils::Initialize();
    SearchGDMLforFiles(fn);
    xercesc::XMLPlatformUtils::Terminate();


    // Store filename

    unsigned int idx;

    // Copy into buffers
    for( idx = 0; idx < fGDMLFileNames.size(); idx++ ){
	G4cout << "Found GDML file " << fGDMLFileNames[idx] << G4endl;
	rundata->AddGDMLFile(fGDMLFileNames[idx]);
    }

    return;
}

void remollIO::SearchGDMLforFiles(G4String fn){
    /*!  Chase down files to be included by GDML.
     *   Mainly look for file tags and perform recursively */

    struct stat thisfile;

    int ret = stat(fn.data(), &thisfile);

    if( ret != 0 ){
	G4cerr << "ERROR opening file " << fn <<  " in " << __PRETTY_FUNCTION__ << ": " << strerror(errno) << G4endl;
	exit(1);
    }

   xercesc::XercesDOMParser *xmlParser = new xercesc::XercesDOMParser();

   // Make sure file exists - otherwise freak out

   fGDMLFileNames.push_back(fn.data());

   xmlParser->parse( fn.data() );
   xercesc::DOMDocument* xmlDoc = xmlParser->getDocument();

   xercesc::DOMElement* elementRoot = xmlDoc->getDocumentElement();

   TraverseChildren( elementRoot );
   return;
}

void remollIO::TraverseChildren( xercesc::DOMElement *thisel ){

   xercesc::DOMNodeList*      children = thisel->getChildNodes();
   const XMLSize_t nodeCount = children->getLength();

   for( XMLSize_t xx = 0; xx < nodeCount; ++xx ){
       xercesc::DOMNode* currentNode = children->item(xx);
       if( currentNode->getNodeType() ){   // true is not NULL

	   if( currentNode->getNodeType() == xercesc::DOMNode::ELEMENT_NODE ){ // is element 
	       xercesc::DOMElement* currentElement
		   = dynamic_cast< xercesc::DOMElement* >( currentNode );
	       if( xercesc::XMLString::equals(currentElement->getTagName(), xercesc::XMLString::transcode("file"))){
		   SearchGDMLforFiles(G4String(xercesc::XMLString::transcode(currentElement->getAttribute(xercesc::XMLString::transcode("name")))));
	       }

	       if( currentElement->getChildNodes()->getLength() > 0 ){
		   TraverseChildren( currentElement );
	       }
	   }
       }
   }

}









