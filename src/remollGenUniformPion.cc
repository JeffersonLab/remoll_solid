#include "remollBeamTarget.hh"
#include "remollGenUniformPion.hh"

#include "G4String.hh"
#include "CLHEP/Random/RandFlat.h"

//#include "wiser_pion.h"
#include "remollEvent.hh"
#include "remollVertex.hh"
#include "G4Material.hh"

#include "remolltypes.hh"

remollGenUniformPion::remollGenUniformPion() : remollGenPion(){
  fHaveTotalXs=kFALSE;
}

remollGenUniformPion::~remollGenUniformPion(){

}

void remollGenUniformPion::SamplePhysics(remollVertex *vert, remollEvent *evt){
  // Generate Pion event
  double mass;
  G4String piontypestr;
  switch(fPionType){
  case kPiPlus:
    piontypestr = G4String("pi+");
    mass = 0.1396; // mass in GeV
    break;
  case kPiMinus:
    piontypestr = G4String("pi-");
    mass = 0.1396*GeV; // mass in GeV
    break;
  case kPi0:
    piontypestr = G4String("pi0");
    mass = 0.1350*GeV; // mass in GeV
    break;    
  default:
    piontypestr = G4String("pi-");
    mass = 0.1396*GeV; // mass in GeV
    break;
  }
  // Use radiated beam vertex
  //double beamE   = vert->GetBeamE();
  // Use unradiated beam vertex
  double beamE = remollBeamTarget::GetBeamTarget()->fBeamE;
  
  double rad_len = vert->GetRadLen();  
  double intrad = 2.0*alpha*log(beamE/electron_mass_c2)/pi;
  
  if( !fHaveTotalXs ){
    printf("Calculating total wiser cross section\n");
    fTotal0Xs = wiser_total_sigma( beamE/GeV, intrad, rad_len*4.0/3.0, 0)*nanobarn;
    fTotal1Xs = wiser_total_sigma( beamE/GeV, intrad, rad_len*4.0/3.0, 1)*nanobarn;
    //printf("Calculated %10.5e  %10.5e   %10.5e \n",fTotal0Xs,fTotal1Xs,(vert->GetMaterial()->GetZ()*fTotal1Xs +  (vert->GetMaterial()->GetA()*mole/g-vert->GetMaterial()->GetZ())*fTotal0Xs)*1e-37);
    printf("Calculated %10.5e  %10.5e \n",fTotal0Xs,fTotal1Xs);
    fHaveTotalXs = kTRUE;
  } 

  // Use radiated beam vertex
  beamE   = vert->GetBeamE();

  if (beamE < 2*mass){//to make sure radiated energy not to go below maximum pf at the cross section scan below (beamE > 0.156 GeV)
    beamE += 2*mass;
    //G4cout<<"DEBUG line  "<<beamE/GeV<<" "<<mass/GeV<<G4endl;
  }

  /*
  //For pion generation we don't set fE_min and fE_max so for now true_emax = beamE : rakitha Wed Sep 25 10:43:57 EDT 2013  
  double true_emax = 0.0;
  if( fE_max < 0.0 || fE_max > beamE ){
    true_emax = beamE;
  } else {
    true_emax = fE_max;
  }
  */
  

  double totalxs = 0.0;	  
  double weight_pip, weight_pim, weight_tot;
  double max =0.0;
  double scale = 1.2;

  double th;// = acos(CLHEP::RandFlat::shoot(cos(fTh_max), cos(fTh_min)));
  double ph;// = CLHEP::RandFlat::shoot(fPh_min, fPh_max);
  double pf;// = CLHEP::RandFlat::shoot(fE_min, true_emax);


  int npidx  = 5;
  int nthidx = 5;

  for(int i = 0; i < npidx; i++ ){
      for(int j = 0; j < nthidx; j++ ){
	// Scan around 2 GeV
	pf = (0.1*((double) i)*(beamE/MeV)/npidx + mass/MeV);  // These are guesses, but they work for E down to 0.3 GeV
	th = (10.0*((double) j)/nthidx/(beamE/MeV))*pi/180;
	assert( pf > 0.0 );
	assert( pf < beamE );
	weight_tot = 0.0;

	weight_pip = wiser_sigma( beamE/GeV,pf/GeV , th, rad_len*4.0/3.0 + intrad, 0)*nanobarn/GeV;
	weight_pim = wiser_sigma( beamE/GeV,pf/GeV , th, rad_len*4.0/3.0 + intrad, 1)*nanobarn/GeV;

	switch(fPionType){
	case kPiPlus:
	  weight_tot = vert->GetMaterial()->GetZ()*weight_pip + (vert->GetMaterial()->GetA()*mole/g-vert->GetMaterial()->GetZ())*weight_pim;
	  break;
	case kPiMinus:
	  weight_tot = vert->GetMaterial()->GetZ()*weight_pim + (vert->GetMaterial()->GetA()*mole/g-vert->GetMaterial()->GetZ())*weight_pip;
	  break;
	case kPi0:
	  weight_tot = vert->GetMaterial()->GetZ()*(weight_pip+weight_pim)/2.0 + (vert->GetMaterial()->GetA()*mole/g-vert->GetMaterial()->GetZ())*(weight_pip+weight_pim)/2.0;
	  break;    
	default:
	  weight_tot = vert->GetMaterial()->GetZ()*weight_pip + (vert->GetMaterial()->GetA()*mole/g-vert->GetMaterial()->GetZ())*weight_pim;
	  break;
	}
	if (weight_tot > max)
	  max   = weight_tot;
      }
  }

  max *= scale;

  if( max < 0.0 ){ printf("Kinematics too close to threshold\n"); exit(1);}

  int cnt = 0;
  
  double testval = 0.0;

   do { 
     th = acos(CLHEP::RandFlat::shoot(cos(fTh_max), cos(fTh_min)));
     //th = acos(CLHEP::RandFlat::shoot(-1.0,1.0));//since total cross section is computed for 4.pi range (ph range is 2.pi and th range is 2 rad)
     ph = CLHEP::RandFlat::shoot(fPh_min, fPh_max);
     pf = CLHEP::RandFlat::shoot(0., beamE);

     weight_pip = wiser_sigma( beamE/GeV,pf/GeV , th, rad_len*4.0/3.0 + intrad, 0)*nanobarn/GeV;
     weight_pim = wiser_sigma( beamE/GeV,pf/GeV , th, rad_len*4.0/3.0 + intrad, 1)*nanobarn/GeV;

     switch(fPionType){
     case kPiPlus:
       weight_tot = vert->GetMaterial()->GetZ()*weight_pip + (vert->GetMaterial()->GetA()*mole/g-vert->GetMaterial()->GetZ())*weight_pim;
       totalxs = vert->GetMaterial()->GetZ()*fTotal0Xs +  (vert->GetMaterial()->GetA()*mole/g-vert->GetMaterial()->GetZ())*fTotal1Xs;
       break;
     case kPiMinus:
       weight_tot = vert->GetMaterial()->GetZ()*weight_pim + (vert->GetMaterial()->GetA()*mole/g-vert->GetMaterial()->GetZ())*weight_pip;
       totalxs = vert->GetMaterial()->GetZ()*fTotal1Xs +  (vert->GetMaterial()->GetA()*mole/g-vert->GetMaterial()->GetZ())*fTotal0Xs;
       break;
     case kPi0:
       weight_tot = vert->GetMaterial()->GetZ()*(weight_pip+weight_pim)/2.0 + (vert->GetMaterial()->GetA()*mole/g-vert->GetMaterial()->GetZ())*(weight_pip+weight_pim)/2.0;
       totalxs = vert->GetMaterial()->GetZ()*(fTotal0Xs+fTotal1Xs)/2.0 + (vert->GetMaterial()->GetA()*mole/g-vert->GetMaterial()->GetZ())*(fTotal0Xs+fTotal1Xs)/2.0;
       break;    
     default:
       weight_tot = vert->GetMaterial()->GetZ()*weight_pip + (vert->GetMaterial()->GetA()*mole/g-vert->GetMaterial()->GetZ())*weight_pim;
       totalxs = vert->GetMaterial()->GetZ()*fTotal0Xs +  (vert->GetMaterial()->GetA()*mole/g-vert->GetMaterial()->GetZ())*fTotal1Xs;
       break;
     }
     cnt++;
     testval = max*CLHEP::RandFlat::shoot(0.,1.);
     

   } while ( weight_tot < testval );

   //assert( pf > 0.0 );
   //assert( pf < beamE );
    //solid angle in steradians times the integral of pion energies from 0 to beamE -> int dE from 0 to beamE: rakitha Tue Sep 24 14:11:36 EDT 2013
    //double V = (fPh_max-fPh_min)*(cos(fTh_min) - cos(fTh_max))*beamE;
  
   
    //totalxs is in nanobarns
    //and it is constant for all events
    evt->SetEffCrossSection(totalxs);
    if( vert->GetMaterial()->GetNumberOfElements() != 1 ){
	G4cerr << __FILE__ << " line " << __LINE__ << 
	    ": Error!  Some lazy programmer didn't account for complex materials in the moller process!" << G4endl;
	exit(1);
    }

    evt->SetAsymmetry(0.0);


    evt->ProduceNewParticle( G4ThreeVector(0.0, 0.0, 0.0), 
	    G4ThreeVector(pf*sin(ph)*sin(th), pf*cos(ph)*sin(th), pf*cos(th)), 
	    piontypestr );
    return;

}
