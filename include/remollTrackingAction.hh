#ifndef remollTrackingAction_h
#define remollTrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "G4TrackingManager.hh"

class remollTrackingAction : public G4UserTrackingAction 
{
public:
  remollTrackingAction();
  virtual ~remollTrackingAction();
   
  virtual void PreUserTrackingAction(const G4Track*);
  virtual void PostUserTrackingAction(const G4Track*);
  
  G4int GetStoreTrajectory() const {return fpTrackingManager->GetStoreTrajectory();}

private:
};

#endif
