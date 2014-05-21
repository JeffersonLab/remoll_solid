#ifndef remollIO_HH
#define remollIO_HH

#include "TROOT.h"
#include "TObject.h"
#include "G4Run.hh"
#include "remolltypes.hh"

#include "G4String.hh"
#include <vector>
#include <string>

class TFile;
class TTree;

class remollGenericDetectorHit;
class remollGenericDetectorSum;
class remollCalDetectorSum;
class remollEvent;
class remollTrajectory;

#include <xercesc/dom/DOMElement.hpp>


#define __IO_MAXHIT 10000
#define __FILENAMELEN 255

// Units for output
#define __E_UNIT GeV
#define __L_UNIT m
#define __T_UNIT ns
#define __ANG_UNIT rad
#define __ASYMM_SCALE 1e-9 // ppb

class remollIO {
    public:
	 remollIO();
	~remollIO();

	void SetFilename(G4String  fn);
	G4String GetFilename(){return fFilename;}

	void FillTree();
	void Flush();
	void WriteTree();

	void WriteRun();

	void InitializeTree();

	void GrabGDMLFiles( G4String fn );

    private:
	TFile *fFile;
	TTree *fTree;

	char fFilename[__FILENAMELEN];

	std::vector<G4String>       fGDMLFileNames;

	void SearchGDMLforFiles(G4String );
	void TraverseChildren(  xercesc::DOMElement * );

	//  Interfaces and buffers to the tree
	//  This is potentially going to get very long...

	// Event data
    public:
	void SetEventData(remollEvent *);
    private:
        Int_t fEv_num;
	Int_t fNEvPart;

	Double_t fEvRate;
	Double_t fEvEffXS;
	Double_t fEvAsym;
	Double_t fEvmAsym;
	Double_t fEvBeamP;
	Double_t fEvQ2;
	Double_t fEvW2;
	Double_t fEvXbj;
	Double_t fEvThCoM;

	Double_t fBmX;
	Double_t fBmY;
	Double_t fBmZ;
	Double_t fBmdX;
	Double_t fBmdY;
	Double_t fBmdZ;

	Int_t fEvPID[__IO_MAXHIT];

	Double_t fEvPart_X[__IO_MAXHIT];
	Double_t fEvPart_Y[__IO_MAXHIT];
	Double_t fEvPart_Z[__IO_MAXHIT];
	Double_t fEvPart_Px[__IO_MAXHIT];
	Double_t fEvPart_Py[__IO_MAXHIT];
	Double_t fEvPart_Pz[__IO_MAXHIT];
	Double_t fEvPart_Th[__IO_MAXHIT];
	Double_t fEvPart_Ph[__IO_MAXHIT];
	Double_t fEvPart_P[__IO_MAXHIT];
	Double_t fEvPart_tPx[__IO_MAXHIT];
	Double_t fEvPart_tPy[__IO_MAXHIT];
	Double_t fEvPart_tPz[__IO_MAXHIT];


	//  GenericDetectorHit
    public:
	void AddGenericDetectorHit(remollGenericDetectorHit *);
    private:
	Int_t fNGenDetHit;
	Int_t fGenDetHit_det[__IO_MAXHIT];
	Int_t fGenDetHit_id[__IO_MAXHIT];

	Int_t fGenDetHit_trid[__IO_MAXHIT];
	Int_t fGenDetHit_pid[__IO_MAXHIT];
	Int_t fGenDetHit_gen[__IO_MAXHIT];
	Int_t fGenDetHit_mtrid[__IO_MAXHIT];

	Double_t fGenDetHit_X[__IO_MAXHIT];
	Double_t fGenDetHit_Y[__IO_MAXHIT];
	Double_t fGenDetHit_Z[__IO_MAXHIT];
	Double_t fGenDetHit_R[__IO_MAXHIT];
        Double_t fGenDetHit_T[__IO_MAXHIT];

	Double_t fGenDetHit_Px[__IO_MAXHIT];
	Double_t fGenDetHit_Py[__IO_MAXHIT];
	Double_t fGenDetHit_Pz[__IO_MAXHIT];
	Double_t fGenDetHit_P[__IO_MAXHIT];
	Double_t fGenDetHit_E[__IO_MAXHIT];
	Double_t fGenDetHit_M[__IO_MAXHIT];

	Double_t fGenDetHit_Vx[__IO_MAXHIT];
	Double_t fGenDetHit_Vy[__IO_MAXHIT];
	Double_t fGenDetHit_Vz[__IO_MAXHIT];
	Double_t fGenDetHit_Vdx[__IO_MAXHIT];
	Double_t fGenDetHit_Vdy[__IO_MAXHIT];
	Double_t fGenDetHit_Vdz[__IO_MAXHIT];

	//  GenericDetectorSum
    public:
	void AddGenericDetectorSum(remollGenericDetectorSum *);
    private:
	Int_t fNGenDetSum;
	Int_t fGenDetSum_det[__IO_MAXHIT];
	Int_t fGenDetSum_id[__IO_MAXHIT];
	Double_t fGenDetSum_edep[__IO_MAXHIT];
	//  CalDetectorSum
    public:
	void AddCalDetectorSum(remollCalDetectorSum *);
    private:
	Int_t fNCalDetSum;
	Int_t fCalDetSum_det[__IO_MAXHIT];
	Int_t fCalDetSum_id[__IO_MAXHIT];
	Double_t fCalDetSum_edep[__IO_MAXHIT];
	Double_t fCalDetSum_photon[__IO_MAXHIT];
	Double_t fCalDetSum_x[__IO_MAXHIT];
	Double_t fCalDetSum_y[__IO_MAXHIT];
        Double_t fCalDetPos_X[__IO_MAXHIT];
        Double_t fCalDetPos_Y[__IO_MAXHIT];
        Double_t fCalDetPos_Z[__IO_MAXHIT];
        Double_t fCalDetSum_starttime[__IO_MAXHIT];
        Double_t fCalDetSum_endtime[__IO_MAXHIT];

  // Trajectory
public:
  void AddTrajectory (remollTrajectory *);
private:
  Int_t fNTraj;
  Int_t fTraj_tid[__IO_MAXHIT];
  Int_t fTraj_mtid[__IO_MAXHIT];
  Int_t fTraj_pid[__IO_MAXHIT];
  Double_t fTraj_Px[__IO_MAXHIT];
  Double_t fTraj_Py[__IO_MAXHIT];
  Double_t fTraj_Pz[__IO_MAXHIT];
  Double_t fTraj_P[__IO_MAXHIT];
  Double_t fTraj_Vx[__IO_MAXHIT];
  Double_t fTraj_Vy[__IO_MAXHIT];
  Double_t fTraj_Vz[__IO_MAXHIT];
  Double_t fTraj_t[__IO_MAXHIT];
  std::vector<std::string> fTraj_proc;

};

#endif//remollIO_HH
