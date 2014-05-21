#include "remollTrajectory.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleTypes.hh"
#include "G4Polyline.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4VVisManager.hh"
#include "G4UnitsTable.hh"
#include "G4VProcess.hh"


G4Allocator<remollTrajectory> myTrajectoryAllocator;

remollTrajectory::remollTrajectory()
{
   fpParticleDefinition = 0;
   ParticleName = "";
   PDGCharge = 0;
   PDGEncoding = 0;
   fTrackID = 0;
   fParentID = 0;
   positionRecord = 0;
   momentum = G4ThreeVector(0.,0.,0.);
   vertexPosition = G4ThreeVector(0.,0.,0.);
   globalTime = 0.;
   fCreator = "";
   fVisible = 0;
   fSeen = 0;
}

remollTrajectory::remollTrajectory(const G4Track* aTrack)
{
   fpParticleDefinition = aTrack->GetDefinition();
   ParticleName = fpParticleDefinition->GetParticleName();
   PDGCharge = fpParticleDefinition->GetPDGCharge();
   PDGEncoding = fpParticleDefinition->GetPDGEncoding();
   fTrackID = aTrack->GetTrackID();
   fParentID = aTrack->GetParentID();
   positionRecord = new remollTrajectoryPointContainer();
   positionRecord->push_back(new G4TrajectoryPoint(aTrack->GetPosition()));
   momentum = aTrack->GetMomentum();
   vertexPosition = aTrack->GetPosition();
   globalTime = aTrack->GetGlobalTime();
   const G4VProcess* vc = aTrack->GetCreatorProcess();
   if (vc != NULL)
     fCreator = vc->GetProcessName();
   fVisible = 0;
   fSeen = 0;
}

remollTrajectory::remollTrajectory(remollTrajectory & right)
{
  ParticleName = right.ParticleName;
  fpParticleDefinition = right.fpParticleDefinition;
  PDGCharge = right.PDGCharge;
  PDGEncoding = right.PDGEncoding;
  fTrackID = right.fTrackID;
  fParentID = right.fParentID;
  positionRecord = new remollTrajectoryPointContainer();
  for(unsigned int i=0;i<right.positionRecord->size();i++)
  {
    G4TrajectoryPoint* rightPoint = (G4TrajectoryPoint*)((*(right.positionRecord))[i]);
    positionRecord->push_back(new G4TrajectoryPoint(*rightPoint));
  }
   momentum = right.momentum;
   vertexPosition = right.vertexPosition;
   globalTime = right.globalTime;
   fCreator = right.fCreator;
   fVisible = right.fVisible;
   fSeen = right.fSeen;
}

remollTrajectory::~remollTrajectory()
{
  size_t i;
  for(i=0;i<positionRecord->size();i++){
    delete  (*positionRecord)[i];
  }
  positionRecord->clear();

  delete positionRecord;
}

void remollTrajectory::ShowTrajectory (std::ostream& os) const
{
   os << "TrackID =" << fTrackID 
	  << " : ParentID=" << fParentID;
   os << " Particle name : " << ParticleName;
   os << " Original momentum : " << std::setprecision (3)
	  << momentum.x()/MeV << " "
	  << momentum.y()/MeV << " "
	  << momentum.z()/MeV << " MeV";
   os << " Vertex : " 
	  << vertexPosition.x()/mm << " " 
	  << vertexPosition.y()/mm << " "
	  << vertexPosition.z()/mm << " mm";
   os << " Creator : " << fCreator;
}

void remollTrajectory::DrawTrajectory(G4int i_mode) const
{
  if (!fVisible)
    return;

   G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
   G4ThreeVector pos;

   G4Polyline pPolyline;
   for (unsigned int i = 0; i < positionRecord->size() ; i++) {
     G4TrajectoryPoint* aTrajectoryPoint = (G4TrajectoryPoint*)((*positionRecord)[i]);
     pos = aTrajectoryPoint->GetPosition();
     pPolyline.push_back( pos );
   }

   G4Colour colour(0.2,0.2,0.2);
   if(fpParticleDefinition==G4Gamma::GammaDefinition())
      colour = G4Colour(0.,0.,1.);
   else if(fpParticleDefinition==G4Electron::ElectronDefinition()
         ||fpParticleDefinition==G4Positron::PositronDefinition())
      colour = G4Colour(1.,1.,0.);
   else if(fpParticleDefinition==G4MuonMinus::MuonMinusDefinition()
         ||fpParticleDefinition==G4MuonPlus::MuonPlusDefinition())
      colour = G4Colour(0.,1.,0.);
   else if(fpParticleDefinition->GetParticleType()=="meson")
   {
      if(PDGCharge!=0.)
         colour = G4Colour(1.,0.,0.);
      else
         colour = G4Colour(0.5,0.,0.);
   }
   else if(fpParticleDefinition->GetParticleType()=="baryon")
   {
      if(PDGCharge!=0.)
         colour = G4Colour(0.,1.,1.);
      else
         colour = G4Colour(0.,0.5,0.5);
   }

   G4VisAttributes attribs(colour);
   pPolyline.SetVisAttributes(attribs);
   if(pVVisManager) pVVisManager->Draw(pPolyline);
}

void remollTrajectory::AppendStep(const G4Step* aStep)
{
   positionRecord->push_back( new G4TrajectoryPoint(aStep->GetPostStepPoint()->
                                 GetPosition() ));
}
  
G4ParticleDefinition* remollTrajectory::GetParticleDefinition()
{
   return (G4ParticleTable::GetParticleTable()->FindParticle(ParticleName));
}

void remollTrajectory::MergeTrajectory(G4VTrajectory* secondTrajectory)
{
  if(!secondTrajectory) return;

  remollTrajectory* seco = (remollTrajectory*)secondTrajectory;
  G4int ent = seco->GetPointEntries();
  for(int i=1;i<ent;i++) // initial point of the second trajectory should not be merged
  {
    positionRecord->push_back((*(seco->positionRecord))[i]);
  }
  delete (*seco->positionRecord)[0];
  seco->positionRecord->clear();

}
