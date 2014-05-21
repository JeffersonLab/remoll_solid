#ifndef remollTrajectory_h
#define remollTrajectory_h 1

#include "G4VTrajectory.hh"
#include "G4Allocator.hh"
#include <stdlib.h>
#include "G4ThreeVector.hh"
#include "G4ios.hh"     
//#include "g4std/vector"
#include <vector>
#include "globals.hh" 
#include "G4ParticleDefinition.hh" 
#include "G4TrajectoryPoint.hh"   
#include "G4Track.hh"
#include "G4Step.hh"

class G4Polyline;

typedef std::vector<G4VTrajectoryPoint*> remollTrajectoryPointContainer;

class remollTrajectory : public G4VTrajectory
{
public:
  remollTrajectory();
  remollTrajectory(const G4Track* aTrack);
  remollTrajectory(remollTrajectory &);
  virtual ~remollTrajectory();
  
  inline void* operator new(size_t);
  inline void  operator delete(void*);
  inline int operator == (const remollTrajectory& right) const
  {return (this==&right);} 
  
  inline G4int GetTrackID() const
  { return fTrackID; }
  inline G4int GetParentID() const
  { return fParentID; }
  inline G4String GetParticleName() const
  { return ParticleName; }
  inline G4double GetCharge() const
  { return PDGCharge; }
  inline G4int GetPDGEncoding() const
  { return PDGEncoding; }
  inline G4ThreeVector GetInitialMomentum() const
  { return momentum; }
  inline const G4ThreeVector& GetVertexPosition() const
  { return vertexPosition; }
  inline G4double GetGlobalTime() const
  { return globalTime; }
  inline G4String GetCreator() const
  { return fCreator; }
  virtual int GetPointEntries() const
  { return positionRecord->size(); }
  virtual G4VTrajectoryPoint* GetPoint(G4int i) const 
  { return (*positionRecord)[i]; }
  void SetVisible (int v = 1) {fVisible = v;};
  
  virtual void ShowTrajectory(std::ostream& os=G4cout) const;
  virtual void DrawTrajectory(G4int i_mode=0) const;
  virtual void AppendStep(const G4Step* aStep);
  virtual void MergeTrajectory(G4VTrajectory* secondTrajectory);
  
  G4ParticleDefinition* GetParticleDefinition();
  
  G4int Seen() const { return fSeen; }
  void SetSeen (G4int s = 1) { fSeen = s; }
  
private:
  remollTrajectoryPointContainer* positionRecord;
  G4int                        fTrackID;
  G4int                        fParentID;
  G4ParticleDefinition*        fpParticleDefinition;
  G4String                     ParticleName;
  G4double                     PDGCharge;
  G4int                        PDGEncoding;
  G4ThreeVector                momentum;
  G4ThreeVector                vertexPosition;
  G4double                     globalTime;
  G4String                     fCreator;
  G4int                        fVisible;
  G4int                        fSeen;
};

extern G4Allocator<remollTrajectory> myTrajectoryAllocator;

inline void* remollTrajectory::operator new(size_t)
{
  void* aTrajectory;
  aTrajectory = (void*)myTrajectoryAllocator.MallocSingle();
  return aTrajectory;
}

inline void remollTrajectory::operator delete(void* aTrajectory)
{
  myTrajectoryAllocator.FreeSingle((remollTrajectory*)aTrajectory);
}

#endif
