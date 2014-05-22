#include "remollEventAction.hh"
#include "remollGenericDetectorHit.hh"
#include "remollGenericDetectorSum.hh"
#include "remollCalDetectorSum.hh"
#include "remollIO.hh"
#include "remollTrajectory.hh"
#include "remollTrackingAction.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4TrajectoryContainer.hh"
#include "G4VVisManager.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"
#include "G4RunManagerKernel.hh"
#include "G4TrackingManager.hh"

remollEventAction::remollEventAction() {
}

remollEventAction::~remollEventAction(){
}


void remollEventAction::BeginOfEventAction(const G4Event*ev) {
    // Pretty ongoing status with flush
    if( (ev->GetEventID() % 1000) == 0 ){
	printf("Event %8d\r", ev->GetEventID() );
	fflush(stdout);
    }

    return;
}

void remollEventAction::EndOfEventAction(const G4Event* evt ) {
  //G4SDManager   *SDman = G4SDManager::GetSDMpointer();
  G4HCofThisEvent *HCE = evt->GetHCofThisEvent();

  G4VHitsCollection *thiscol;

  G4int storeTraj = G4RunManagerKernel::GetRunManagerKernel()->GetTrackingManager()->GetStoreTrajectory();

  // Traverse all hit collections, sort by output type
  for( int hcidx = 0; hcidx < HCE->GetCapacity(); hcidx++ ){
      thiscol = HCE->GetHC(hcidx);
      if(thiscol){ // This is NULL if nothing is stored
	  // Dyanmic cast to test types, process however see fit and feed to IO
	  
	  ////  Generic Detector Hits ///////////////////////////////////
	  if( remollGenericDetectorHitsCollection *thiscast = 
		  dynamic_cast<remollGenericDetectorHitsCollection *>(thiscol)){
	      for( unsigned int hidx = 0; hidx < thiscast->GetSize(); hidx++ ){
  		  remollGenericDetectorHit* h = (remollGenericDetectorHit *) thiscast->GetHit(hidx);
		  fIO->AddGenericDetectorHit(h);

		  // Process the trajectory and its predecessors

		  if (storeTraj)
		    {
		      G4int tid =  h->fTrID;
		      while (tid != 0)
			{
			  // Loop over trajectories in hit's history
			  
			  remollTrajectory* trajectory = GetTrajectory (tid, evt);
			  if (trajectory == 0 || trajectory->Seen())
			    break; // already written or history broken, call it done
			  fIO->AddTrajectory (trajectory);
			  
			  // Mark this one seen, and go on to its parent
			  trajectory->SetSeen();
			  tid = trajectory->GetParentID();
			}
		    }
	      }
	  }
	  
	  ////  Generic Detector Sum ////////////////////////////////////
	  if( remollGenericDetectorSumCollection *thiscast = 
		  dynamic_cast<remollGenericDetectorSumCollection *>(thiscol)){
	      for( unsigned int hidx = 0; hidx < thiscast->GetSize(); hidx++ ){
		  fIO->AddGenericDetectorSum(
			  (remollGenericDetectorSum *) thiscast->GetHit(hidx) );
	      }
	  }
	  
	  ////  CalDetector Sum ////////////////////////////////////
	  if( remollCalDetectorSumCollection *thiscast = 
		  dynamic_cast<remollCalDetectorSumCollection *>(thiscol)){
	      for( unsigned int hidx = 0; hidx < thiscast->GetSize(); hidx++ ){
		  fIO->AddCalDetectorSum(
			  (remollCalDetectorSum *) thiscast->GetHit(hidx) );
	      }
	  }


      }
  }

  // Fill tree and reset buffers
  fIO->FillTree();
  fIO->Flush();

  return;
}


remollTrajectory* remollEventAction::GetTrajectory (G4int id, const G4Event* evt)
{
  G4TrajectoryContainer* container = evt->GetTrajectoryContainer();
  if (container == 0) return 0;
  size_t nTraj = container->size();
  for(size_t i = 0; i < nTraj; i++)
    {
      remollTrajectory* tr1 = (remollTrajectory*)((*container)[i]);
      if (tr1->GetTrackID() == id) return tr1;
    }
  return 0;
}
