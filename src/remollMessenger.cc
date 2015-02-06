#include "remollMessenger.hh"

#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"

#include "remollOpticalPhysics.hh"
#include "remollDetectorConstruction.hh"
#include "remollIO.hh"
#include "remollEventAction.hh"
#include "remollVEventGen.hh"
#include "remollGenPion.hh"
#include "remollGenFlat.hh"
#include "remollPrimaryGeneratorAction.hh"
#include "remollBeamTarget.hh"
#include "remollRun.hh"
#include "remollRunData.hh"
#include "remollSteppingAction.hh"

#include "G4UImanager.hh"
#include "G4RunManager.hh"

#include "G4GDMLParser.hh"
#include "G4VPhysicalVolume.hh"

#include <iostream>

remollMessenger::remollMessenger(){
    /*  Initialize all the things it talks to to NULL */

    fIO           = NULL;
    fdetcon       = NULL;
    fevact        = NULL;
    fprigen       = NULL;
    fField        = NULL;
    fBeamTarg     = NULL;
    fStepAct      = NULL;
    fPhysicsList  = NULL;

    fRemollDir = new G4UIdirectory("/remoll/");
    fRemollDir->SetGuidance("UI commands of this code");

    // Grab singleton beam/target
    fBeamTarg = remollBeamTarget::GetBeamTarget();

    physListCmd = new G4UIcmdWithAString("/remoll/setphysicslist",this);
    physListCmd->SetGuidance("Set the Physics List");
    physListCmd->SetParameterName("physlist", false);

    detfilesCmd = new G4UIcmdWithAString("/remoll/setgeofile",this);
    detfilesCmd->SetGuidance("Set geometry GDML files");
    detfilesCmd->SetParameterName("geofilename", false);
    detfilesCmd->AvailableForStates(G4State_PreInit); // Only have this in pre-init or GDML freaks out

    seedCmd = new G4UIcmdWithAnInteger("/remoll/seed",this);
    seedCmd->SetGuidance("Set random engine seed");
    seedCmd->SetParameterName("seed", false);

    kryptCmd = new G4UIcmdWithABool("/remoll/likekryptonite",this);
    kryptCmd->SetGuidance("Treat heavy mats as kryptonite");
    kryptCmd->SetParameterName("krypt", false);

    trackEnergyCutCmd = new G4UIcmdWithADoubleAndUnit("/remoll/minEnergyCut",this);
    trackEnergyCutCmd->SetGuidance("Minimum Energy below which tracks are killed");
    trackEnergyCutCmd->SetParameterName("minEnergyCut", false);

    opticalCmd = new G4UIcmdWithABool("/remoll/optical",this);
    opticalCmd->SetGuidance("Enable optical physics");
    opticalCmd->SetParameterName("optical", false);
//    opticalCmd->AvailableForStates(G4State_Idle); // Only have this AFTER we've initalized geometry
//    opticalCmd->AvailableForStates(G4State_PreInit); // Only have this AFTER we've initalized geometry
    opticalCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    newfieldCmd = new G4UIcmdWithAString("/remoll/addfield",this);
    newfieldCmd->SetGuidance("Add magnetic field");
    newfieldCmd->SetParameterName("filename", false);

    fieldScaleCmd = new G4UIcmdWithAString("/remoll/scalefield",this);
    fieldScaleCmd->SetGuidance("Scale magnetic field");
    fieldScaleCmd->SetParameterName("filename", false);

    fieldCurrCmd = new G4UIcmdWithAString("/remoll/magcurrent",this);
    fieldCurrCmd->SetGuidance("Scale magnetic field by current");
    fieldCurrCmd->SetParameterName("filename", false);

    beamEneCmd = new G4UIcmdWithADoubleAndUnit("/remoll/beamene",this);
    beamEneCmd->SetGuidance("Beam energy");
    beamEneCmd->SetParameterName("beamene", false);

    beamCurrCmd = new G4UIcmdWithADoubleAndUnit("/remoll/beamcurr",this);
    beamCurrCmd->SetGuidance("Beam current");
    beamCurrCmd->SetParameterName("beamcurr", false);

    genSelectCmd = new G4UIcmdWithAString("/remoll/gen",this);
    genSelectCmd->SetGuidance("Select physics generator");
    genSelectCmd->SetParameterName("generator", false);

    fileCmd = new G4UIcmdWithAString("/remoll/filename",this);
    fileCmd->SetGuidance("Output filename");
    fileCmd->SetParameterName("filename", false);

    pionCmd = new G4UIcmdWithAString("/remoll/piontype",this);
    pionCmd->SetGuidance("Generate pion type");
    pionCmd->SetParameterName("piontype", false);

    flatCmd = new G4UIcmdWithAString("/remoll/flattype",this);
    flatCmd->SetGuidance("Generate flat distribution type");
    flatCmd->SetParameterName("flattype", false);

    thminCmd = new G4UIcmdWithADoubleAndUnit("/remoll/thmin",this);
    thminCmd->SetGuidance("Minimum generation angle");
    thminCmd->SetParameterName("thmin", false);

    thmaxCmd = new G4UIcmdWithADoubleAndUnit("/remoll/thmax",this);
    thmaxCmd->SetGuidance("Maximum generation angle");
    thmaxCmd->SetParameterName("thmax", false);

    phminCmd = new G4UIcmdWithADoubleAndUnit("/remoll/phmin",this);
    phminCmd->SetGuidance("Minimum generation azimuthal angle");
    phminCmd->SetParameterName("phmin", false);

    phmaxCmd = new G4UIcmdWithADoubleAndUnit("/remoll/phmax",this);
    phmaxCmd->SetGuidance("Maximum azimuthal generation angle");
    phmaxCmd->SetParameterName("phmax", false);

    thCoMminCmd = new G4UIcmdWithADoubleAndUnit("/remoll/thcommin",this);
    thCoMminCmd->SetGuidance("Minimum CoM generation angle");
    thCoMminCmd->SetParameterName("thcommin", false);

    thCoMmaxCmd = new G4UIcmdWithADoubleAndUnit("/remoll/thcommax",this);
    thCoMmaxCmd->SetGuidance("Maximum CoM generation angle");
    thCoMmaxCmd->SetParameterName("thcommax", false);

    EminCmd = new G4UIcmdWithADoubleAndUnit("/remoll/emin",this);
    EminCmd->SetGuidance("Minimum generation energy");
    EminCmd->SetParameterName("emin", false);

    EmaxCmd = new G4UIcmdWithADoubleAndUnit("/remoll/emax",this);
    EmaxCmd->SetGuidance("Maximum generation energy");
    EmaxCmd->SetParameterName("emax", false);


    //////////////////////////////////////////////////
    // beam info

    rasXCmd = new G4UIcmdWithADoubleAndUnit("/remoll/rasx",this);
    rasXCmd->SetGuidance("Square raster width in x (horizontal)");
    rasXCmd->SetParameterName("rasx", false);

    rasYCmd = new G4UIcmdWithADoubleAndUnit("/remoll/rasy",this);
    rasYCmd->SetGuidance("Square raster width y (vertical)");
    rasYCmd->SetParameterName("rasy", false);

    beamX0Cmd = new G4UIcmdWithADoubleAndUnit("/remoll/beam_x0",this);
    beamX0Cmd->SetGuidance("beam initial position in x (horizontal)");
    beamX0Cmd->SetParameterName("beamX0", false);

    beamY0Cmd = new G4UIcmdWithADoubleAndUnit("/remoll/beam_y0",this);
    beamY0Cmd->SetGuidance("beam initial position in y (vertical)");
    beamY0Cmd->SetParameterName("beamY0", false);

    beamth0Cmd = new G4UIcmdWithADoubleAndUnit("/remoll/beam_th0",this);
    beamth0Cmd->SetGuidance("beam initial direction in x (horizontal)");
    beamth0Cmd->SetParameterName("beamth0", false);

    beamph0Cmd = new G4UIcmdWithADoubleAndUnit("/remoll/beam_ph0",this);
    beamph0Cmd->SetGuidance("beam initial direction in y (vertical)");
    beamph0Cmd->SetParameterName("beamph0", false);

    beamdthCmd = new G4UIcmdWithADoubleAndUnit("/remoll/beam_dth",this);
    beamdthCmd->SetGuidance("beam gaussian spread in direction x (horizontal)");
    beamdthCmd->SetParameterName("beamdth", false);

    beamdphCmd = new G4UIcmdWithADoubleAndUnit("/remoll/beam_dph",this);
    beamdphCmd->SetGuidance("beam gaussian spread in direction y (vertical)");
    beamdphCmd->SetParameterName("beamdph", false);

}

remollMessenger::~remollMessenger(){
}


void remollMessenger::SetNewValue(G4UIcommand* cmd, G4String newValue){
    if( cmd == detfilesCmd ){
	fdetcon->SetDetectorGeomFile( newValue );
    }

    if( cmd == seedCmd ){
	G4int seed = seedCmd->GetNewIntValue(newValue);
	CLHEP::HepRandom::setTheSeed(seed);
	remollRun::GetRun()->GetData()->SetSeed(seed);
    }

    if( cmd == kryptCmd ){
	G4bool krypt = kryptCmd->GetNewBoolValue(newValue);
	fStepAct->SetEnableKryptonite(krypt);
    } 
    if( cmd == trackEnergyCutCmd ){
        G4double minecut = trackEnergyCutCmd->GetNewDoubleValue(newValue);
	fStepAct->SetMinimumEnergyCut(minecut);
    }
    if( cmd == physListCmd ){
      G4cout << "remollMessenger :: Setting physics list to " << newValue << G4endl;
      fPhysicsList->SetVerboseLevel(1);
      fPhysicsList->AddPhysicsList(newValue);
    }

    if( cmd == opticalCmd ){
      //G4cerr << __FILE__ << " line " << __LINE__ <<  ": FIXME:  Optical photons are on" << G4endl;
	G4bool optical = opticalCmd->GetNewBoolValue(newValue);
	fPhysicsList->SetOpticalPhysics(optical);
	/*
//If I do this way, I don't see remollOpticalPhysics::ConstructProcess() routine get called. Something could be wrong Therefore I'm called remollOpticalPhysics routines in the remollPhysicsList class: Rakitha Mon Dec 30 00:03:13 EST 2013
	if( optical ){
	    fPhysicsList->RegisterPhysics( new remollOpticalPhysics() );
	} else {
	    fPhysicsList->RemovePhysics("Optical");
	}
	*/

    }

    if( cmd == newfieldCmd ){
	fField->AddNewField( newValue );
    }

    if( cmd == fieldScaleCmd ){
	std::istringstream iss(newValue);

	G4String scalefile, scalestr;
	G4double scaleval;

	iss >> scalefile;
	iss >> scalestr;

	scaleval = atof(scalestr.data());
	fField->SetFieldScale( scalefile, scaleval );
	//fField->SetFieldScale(scaleval );
    }

    if( cmd == fieldCurrCmd ){
	std::istringstream iss(newValue);

	G4String scalefile, scalestr, scaleunit;
	G4double scaleval;

	iss >> scalefile;
	iss >> scalestr;
	iss >> scaleunit;

	if( scaleunit != "A" ){
	    // FIXME: less snark and more functionality?
	    G4cerr << __FILE__ << " line " << __LINE__ <<  ":\n\tGraaaah - just put the current for " <<  scalefile <<  " in amps..." << G4endl;
	    exit(1);
	}

	scaleval = atof(scalestr.data());
	fField->SetMagnetCurrent( scalefile, scaleval );
    }


    if( cmd == genSelectCmd ){
	fprigen->SetGenerator( newValue );
    }

    if( cmd == beamCurrCmd ){
	G4double cur = beamCurrCmd->GetNewDoubleValue(newValue);
	fBeamTarg->SetBeamCurrent(cur);
    }

    if( cmd == beamEneCmd ){
	G4double ene = beamEneCmd->GetNewDoubleValue(newValue);
	fBeamTarg->SetBeamEnergy(ene);
    }

    if( cmd == fileCmd ){
	fIO->SetFilename(newValue);
    }

    if( cmd == pionCmd ){
	remollVEventGen *agen = fprigen->GetGenerator();
	remollGenPion *apion = dynamic_cast<remollGenPion *>(agen);
	if( apion ){
	    if( newValue.compareTo("pi-") == 0 ){
		apion->SetPionType(remollGenPion::kPiMinus);
	    }
	    if( newValue.compareTo("pi+") == 0 ){
		apion->SetPionType(remollGenPion::kPiPlus);
	    }
	    if( newValue.compareTo("pi0") == 0 ){
		apion->SetPionType(remollGenPion::kPi0);
	    }

	} else {
	    G4cerr << __FILE__ << " line " << __LINE__ <<  ": Can't set pion type for non-pion generator" << G4endl;
	}
    }

    if( cmd == flatCmd ){
	remollVEventGen *agen = fprigen->GetGenerator();
	remollGenFlat *apion = dynamic_cast<remollGenFlat *>(agen);
	if( apion ){
	  if( newValue.compareTo("e-") == 0 ){
		apion->SetParticleType("e-");
	    }
	    if( newValue.compareTo("pi-") == 0 ){
		apion->SetParticleType("pi-");
	    }
	    if( newValue.compareTo("pi+") == 0 ){
		apion->SetParticleType("pi+");
	    }
	    if( newValue.compareTo("pi0") == 0 ){
		apion->SetParticleType("pi0");
	    }

	} else {
	  G4cerr << __FILE__ << " line " << __LINE__ <<  ": Can't set particle type for non-generator" << newValue << G4endl;
	}
    }

    if( cmd == EminCmd ){
	G4double en = EminCmd->GetNewDoubleValue(newValue);
	remollVEventGen *agen = fprigen->GetGenerator();
	if( agen ){
	    agen->SetEmin(en);
	}
    }

    if( cmd == EmaxCmd ){
	G4double en = EmaxCmd->GetNewDoubleValue(newValue);
	remollVEventGen *agen = fprigen->GetGenerator();
	if( agen ){
	    agen->SetEmax(en);
	}
    }

    if( cmd == thminCmd ){
	G4double th = thminCmd->GetNewDoubleValue(newValue);
	remollVEventGen *agen = fprigen->GetGenerator();
	if( agen ){
	    agen->SetThMin(th);
	}
    }

    if( cmd == thmaxCmd ){
	G4double th = thminCmd->GetNewDoubleValue(newValue);
	remollVEventGen *agen = fprigen->GetGenerator();
	if( agen ){
	    agen->SetThMax(th);
	}
    }

    if( cmd == phminCmd ){
	G4double ph = phminCmd->GetNewDoubleValue(newValue);
	remollVEventGen *agen = fprigen->GetGenerator();
	if( agen ){
	    agen->SetPhMin(ph);
	}
    }

    if( cmd == phmaxCmd ){
	G4double ph = phminCmd->GetNewDoubleValue(newValue);
	remollVEventGen *agen = fprigen->GetGenerator();
	if( agen ){
	    agen->SetPhMax(ph);
	}
    }

    if( cmd == thCoMminCmd ){
	G4double th = thCoMminCmd->GetNewDoubleValue(newValue);
	remollVEventGen *agen = fprigen->GetGenerator();
	if( agen ){
	    agen->SetThCoM_min(th);
	}
    }

    if( cmd == thCoMmaxCmd ){
	G4double th = thCoMminCmd->GetNewDoubleValue(newValue);
	remollVEventGen *agen = fprigen->GetGenerator();
	if( agen ){
	    agen->SetThCoM_max(th);
	}
    }

    if( cmd == rasXCmd ){
	G4double x = rasXCmd->GetNewDoubleValue(newValue);
	fBeamTarg->fRasterX = x;
    }

    if( cmd == rasYCmd ){
	G4double y = rasYCmd->GetNewDoubleValue(newValue);
	fBeamTarg->fRasterY = y;
    }

    if( cmd == beamX0Cmd ){
	G4double x = beamX0Cmd->GetNewDoubleValue(newValue);
	fBeamTarg->fX0 = x;
    }

    if( cmd == beamY0Cmd ){
	G4double y = beamY0Cmd->GetNewDoubleValue(newValue);
	fBeamTarg->fY0 = y;
    }

    if( cmd == beamth0Cmd ){
	G4double x = beamth0Cmd->GetNewDoubleValue(newValue);
	fBeamTarg->fTh0 = x;
    }

    if( cmd == beamph0Cmd ){
	G4double y = beamph0Cmd->GetNewDoubleValue(newValue);
	fBeamTarg->fPh0 = y;
    }

    if( cmd == beamdthCmd ){
	G4double x = beamdthCmd->GetNewDoubleValue(newValue);
	fBeamTarg->fdTh = x;
    }

    if( cmd == beamdphCmd ){
	G4double y = beamdphCmd->GetNewDoubleValue(newValue);
	fBeamTarg->fdPh = y;
    }
}
