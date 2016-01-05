#include "remollGenFlat.hh"

#include "CLHEP/Random/RandFlat.h"

#include "remollBeamTarget.hh"
#include "remollEvent.hh"
#include "remollVertex.hh"
#include "G4Material.hh"

#include "remolltypes.hh"
#include "remollRun.hh"
#include "remollRunData.hh"

remollGenFlat::remollGenFlat(){
    fTh_min =     5.0*deg;
    fTh_max =    60.0*deg;
    fPh_min =  -180.0*deg;
    fPh_max =   180.0*deg;

    fE_min =      0.0*GeV;
    fE_max =     11.0*GeV;

    fParticleType = "e-";

    fApplyMultScatt = false;
    bUseExtGen = false;//true;

    /*
      Temp fix to access hall D generated pions Rakitha Wed Oct 14 15:32:17 EDT 2015
     */
    if (bUseExtGen){
      //TFile *f = TFile::Open("/home/rakithab/Simulation_Analysis/Generators/HallD/ElectroProduction/bggen_electroprod_LD_pions_corrected.root");//this rootfile has correct rate for LD converted using LH hall D MC
      TFile *f = TFile::Open("/home/rakithab/Simulation_Analysis/Generators/HallD/ElectroProduction/bggen_electroprod_LD_hlf_pions.root");//this rootfile has correct rate for LD converted using LH hall D MC with half target length for rad_length used for bremss spectrum
      //for LH use, /home/rakithab/Simulation_Analysis/Generators/HallD/ElectroProduction/bggen_electroprod_LH_pions.root
      //TFile *f = TFile::Open("/home/rakithab/Simulation_Analysis/Generators/HallD/ElectroProduction/bggen_electroprod_LH_pions.root");
      //for LH with half target length for rad_length used for bremss spectrum
      //TFile *f = TFile::Open("/home/rakithab/Simulation_Analysis/Generators/HallD/ElectroProduction/bggen_electroprod_LH_hlf_pions.root");
      pion0kine = (TH2F *)f->Get("pion0kine");
      pionmkine = (TH2F *)f->Get("pionmkine");
      pionpkine = (TH2F *)f->Get("pionpkine");

      TH1F * pion0mom = (TH1F *)f->Get("pion0mom");
      pi0_rate=pion0mom->Integral()*1e3;//rate in Hz
      printf("hall D electro-prod Pi0 Total rate %f Hz \n",pi0_rate);
      TH1F * pionmmom = (TH1F *)f->Get("pionmmom");
      pim_rate=pionmmom->Integral()*1e3;
      printf("hall D electro-prod Pi- Total rate %f Hz \n",pim_rate);
      TH1F * pionpmom = (TH1F *)f->Get("pionpmom");
      pip_rate=pionpmom->Integral()*1e3;
      printf("hall D electro-prod Pi+ Total rate %f Hz \n",pip_rate);

      fRunData  = remollRun::GetRun()->GetData();
    }
}

remollGenFlat::~remollGenFlat(){
}

void remollGenFlat::SamplePhysics(remollVertex *vert, remollEvent *evt){
    // Generate Moller event


    double ene  = CLHEP::RandFlat::shoot(fE_min, fE_max);
    double th = acos(CLHEP::RandFlat::shoot(cos(fTh_max), cos(fTh_min)));
    double ph = CLHEP::RandFlat::shoot(fPh_min, fPh_max);

    double sigma = 1.0;

    //  Multiply by Z because we have Z electrons
    //  here we must also divide by two because we are double covering 
    //  phasespace because of identical particles
    
    evt->SetEffCrossSection(sigma);
    if (bUseExtGen){
      //get pion momentum and angle from external generator
      
      if (fParticleType.compareTo("pi-") == 0){
	evt->SetRate(pim_rate/((G4double) fRunData->GetNthrown()));
	pionmkine->GetRandom2(ene,th);//get momentum and scattering angle from external generator
	ene *= GeV;
	th *= deg;
      } else if (fParticleType.compareTo("pi+") == 0){
	evt->SetRate(pip_rate/((G4double) fRunData->GetNthrown()));
	pionpkine->GetRandom2(ene,th);//get momentum and scattering angle from external generator
	ene *= GeV;
	th *= deg;
      } else if (fParticleType.compareTo("pi0") == 0){
	evt->SetRate(pi0_rate/((G4double) fRunData->GetNthrown()));
	pion0kine->GetRandom2(ene,th);//get momentum and scattering angle from external generator
	ene *= GeV;
	th *= deg;
      }else
	evt->SetRate(0);
    }

    G4double APV = 0.0;

    evt->SetAsymmetry(APV);

    // Use unradiated beam energy
    double beamE = remollBeamTarget::GetBeamTarget()->fBeamE;
    double Q2 = 2.0*ene*beamE*(1.0-cos(th)  );
    evt->SetQ2( Q2 );
    evt->SetW2( proton_mass_c2*proton_mass_c2 + 2.0*proton_mass_c2*(beamE-ene) - Q2 );
    evt->SetXbj( Q2/(4.0*proton_mass_c2*(beamE-ene)) );


    evt->ProduceNewParticle( G4ThreeVector(0.0, 0.0, 0.0), 
	                     G4ThreeVector( ene*sin(th)*cos(ph), ene*sin(th)*sin(ph), ene*cos(th) ),
			     fParticleType );

    return;

}
