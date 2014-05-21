#include "remollTrackingAction.hh"
#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "remollTrajectory.hh"

remollTrackingAction::remollTrackingAction()
{
}

remollTrackingAction::~remollTrackingAction()
{;}

void remollTrackingAction::PreUserTrackingAction (const G4Track* aTrack)
{
  if (fpTrackingManager && 
      fpTrackingManager->GetStoreTrajectory())
    fpTrackingManager->SetTrajectory (new remollTrajectory(aTrack));
}

void remollTrackingAction::PostUserTrackingAction (const G4Track* aTrack)
{
}
