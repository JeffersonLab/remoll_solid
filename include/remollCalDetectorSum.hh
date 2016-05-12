#ifndef __REMOLLCALDETECTORSUM_HH
#define __REMOLLCALDETECTORSUM_HH

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "remolltypes.hh"

#include <vector>

class remollCalDetectorSum : public G4VHit {
    public:
	remollCalDetectorSum(G4int, G4int);
	~remollCalDetectorSum();

	remollCalDetectorSum(const remollCalDetectorSum &right);
	const remollCalDetectorSum& operator=(const remollCalDetectorSum &right);
	G4int operator==(const remollCalDetectorSum &right) const;

	inline void *operator new(size_t);
	inline void operator delete(void *aHit);
	void *operator new(size_t,void*p){return p;}

    public:
	G4int    fDetID;
	G4int    fCopyID;
	G4double fEdep;
	G4double fPhoton;
        G4double fX, fY, ffT, flT;//ffT is time of the first hit on the cal block flT is time of the last hit in each event
        G4double fDet_X, fDet_Y, fDet_Z;
	G4double fXsum, fYsum;
  
  std::vector<sumdata_t> fData;
  
  void AddEDep( int pid, G4ThreeVector x, double ene );
  
  double GetEdep( int pid );
  G4ThreeVector GetPos( int pid );

private:
  int parttypes[N_PART_DIVISIONS];
  
};


typedef G4THitsCollection<remollCalDetectorSum> remollCalDetectorSumCollection;

extern G4Allocator<remollCalDetectorSum> remollCalDetectorSumAllocator;

inline void* remollCalDetectorSum::operator new(size_t){
    void *aHit;
    aHit = (void *) remollCalDetectorSumAllocator.MallocSingle();
    return aHit;
}

inline void remollCalDetectorSum::operator delete(void *aHit){
    remollCalDetectorSumAllocator.FreeSingle( (remollCalDetectorSum*) aHit);
}

#endif//__REMOLLCALDETECTORSUM_HH
