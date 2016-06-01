#ifndef __REMOLLCALDETECTOR_HH
#define __REMOLLCALDETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "remollCalDetectorSum.hh"
#include "G4EmSaturation.hh"
#include "G4Track.hh"
#include "G4LossTableManager.hh"

#include <map>

/*! 
      Calorimeter detector class.  This will record information on:
*/

class G4HCofThisEvent;
class G4Step;
class G4TouchableHistory;

class remollCalDetector : public G4VSensitiveDetector {
    public:
	remollCalDetector( G4String name, G4int detnum );
	virtual ~remollCalDetector();

	virtual void Initialize(G4HCofThisEvent*);
	virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
	virtual void EndOfEvent(G4HCofThisEvent*);

    private:
	remollCalDetectorSumCollection  *fSumColl;
	G4int fHCID, fSCID;

	std::map<int, remollCalDetectorSum *> fSumMap;

	G4int fDetNo;

	G4double Edep2Photon( const G4double edep, const G4ThreeVector hitpos );
};

#endif//__REMOLLCALDETECTOR_HH
