#include "remollCalDetectorSum.hh"

G4Allocator<remollCalDetectorSum> remollCalDetectorSumAllocator;

remollCalDetectorSum::remollCalDetectorSum(int detid, int copyid){
    fDetID  = detid;
    fCopyID = copyid;
    fEdep   = 0.0;
    fPhoton = 0.0;
    fX      = 0.0;
    fY      = 0.0;
    ffT      = 0.0;
    flT      = 0.0;
    fXsum   = 0.0;
    fYsum   = 0.0;
    fDet_X  = 0.0;
    fDet_Y  = 0.0;
    fDet_Z  = 0.0;
}

remollCalDetectorSum::~remollCalDetectorSum(){
}

void remollCalDetectorSum::AddEDep( int pid, G4ThreeVector pos, double ene ){
    sumdata_t data;

    data.edep = ene;
    data.x    = pos.x();
    data.y    = pos.y();
    data.z    = pos.z();
    data.pid  = pid;

    fData.push_back(data);

    return;
}

double remollCalDetectorSum::GetEdep( int pid ){
    double esum = 0.0;

    for (std::vector<sumdata_t>::iterator it = fData.begin() ; it != fData.end(); ++it){
	if( pid==0 || (*it).pid == pid ){
	    esum += (*it).edep;
	}
    }

    return esum;
}

G4ThreeVector remollCalDetectorSum::GetPos( int pid ){
    double esum = 0.0;
    double xsum, ysum, zsum;
    xsum = ysum = zsum = 0.0;

    for (std::vector<sumdata_t>::iterator it = fData.begin() ; it != fData.end(); ++it){
	if( pid==0 || (*it).pid == pid ){
	    xsum += (*it).x*(*it).edep;
	    ysum += (*it).y*(*it).edep;
	    zsum += (*it).z*(*it).edep;
	    esum += (*it).edep;
	}
    }

    return G4ThreeVector(xsum/esum, ysum/esum, zsum/esum);
}


remollCalDetectorSum::remollCalDetectorSum(const remollCalDetectorSum &right) : G4VHit(){
    // copy constructor
    fDetID  = right.fDetID;
    fCopyID = right.fCopyID;
    fEdep   = right.fEdep;
    fPhoton = right.fPhoton;
    fX      = right.fX;
    fY      = right.fY;
    ffT      = right.ffT;
    flT      = right.flT;
    fXsum   = right.fXsum;
    fYsum   = right.fYsum;
    fDet_X  = right.fDet_X;
    fDet_Y  = right.fDet_Y;
    fDet_Z  = right.fDet_Z;

    fData   = right.fData;

}

const remollCalDetectorSum& remollCalDetectorSum::operator =(const remollCalDetectorSum &right){
    (*this) = right;
    return *this;
}

G4int remollCalDetectorSum::operator==(const remollCalDetectorSum &right ) const {
    return (this==&right) ? 1 : 0;
}
