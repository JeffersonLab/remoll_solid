#include "remollGenLUND.hh"

#include "CLHEP/Random/RandFlat.h"

#include "remollBeamTarget.hh"
#include "remollEvent.hh"
#include "remollVertex.hh"
#include "G4Material.hh"

#include "remolltypes.hh"
#include "remollRun.hh"
#include "remollRunData.hh"

#include<iostream> 
#include <sstream>
#include <iterator>
#include<string>

remollGenLUND::remollGenLUND(){
  pidname[111]="pi0";
  pidname[211]="pi+";
  pidname[-211]="pi-";
  pidname[2112]="neutron";
  pidname[2212]="proton";
  bLUND=kFALSE;
  
}
remollGenLUND::~remollGenLUND(){
}

void remollGenLUND::SamplePhysics(remollVertex *vert, remollEvent *evt){
  fRunData  = remollRun::GetRun()->GetData();
  if(!bLUND){//to make sure no.of events thrown out by G4 is same as lund file. 
    string keyword;
    string line;
    int line_len;
    std::vector<std::string> words;
    while (std::getline(LUND_in, line)){
      std::stringstream stream(line);   
      line_len = std::distance(std::istream_iterator<std::string>(stream), std::istream_iterator<std::string>());
      //stream.clear(); // reset stream flags
      if (line_len==10){
	//use stream2.clear() to clear all the flags but sometimes it fails
	//therefore always have a new string stream to avoid unexpected results
	std::stringstream stream2(line);  
	//read the ratefactor to the variable words
	words.clear();
	std::copy(std::istream_iterator<std::string>(stream2),std::istream_iterator<std::string>(),std::back_inserter(words));
	break;
      }//else
	//G4cerr << " line len "<<line_len<< G4endl;
    }    
    //reset the file back to first line
    LUND_in.clear();
    LUND_in.seekg(0, ios::beg);
    //this simple trick only works for when there are one particle per event as input in the lund file
    //Int_t linecount = std::count(std::istreambuf_iterator<char>(LUND_in2),std::istreambuf_iterator<char>(), '\n')/2;
    //count no.of lines in LUND file with the ratefactor
    Int_t linecount = std::count(std::istream_iterator<std::string>(LUND_in), std::istream_iterator<std::string>(),words[9]);
    //printf("No. of LUND events %i No of events thrown out by G4 %i; rate factor %s\n",linecount,fRunData->GetNthrown(),keyword.data() );
    if (linecount != fRunData->GetNthrown()){
      G4cerr << __FILE__ << " line " << __LINE__ << " - ERROR : LUND file events " << linecount << " mis-match with G4,fRunData->GetNthrown() " <<  fRunData->GetNthrown()<< G4endl;
      exit(1);
    }else
      bLUND=kTRUE;
    //reset the file back to first line
    LUND_in.clear();
    LUND_in.seekg(0, ios::beg);
  
  }
  //read two lines from the LUND file
  //line1[9] is the rate factor
  LUND_in >> line1[0] >> line1[1] >> line1[2] >> line1[3] >> line1[4] >> line1[5] >> line1[6] >> line1[7] >> line1[8] >> line1[9];
  //line2[3]-pid, line[6,7,8] - momentum, line[9]-energy,line[11,12,13] - vertex
  LUND_in >> line2[0] >> line2[1] >> line2[2] >> line2[3] >> line2[4] >> line2[5] >> line2[6] >> line2[7] >> line2[8] >> line2[9] >> line2[10] >> line2[11] >> line2[12] >> line2[13];
  ratefactor = line1[9];
  momentum[0]=line2[6];//x momentum GeV
  momentum[1]=line2[7];//y momentum GeV
  momentum[2]=line2[8];//z momentum GeV
  momentum[0]*= GeV;
  momentum[1]*= GeV;
  momentum[2]*= GeV;
  pid = (Int_t)line2[3];

  //printf("%i %s %f %f %f %f \n",pid,pidname[pid].data(),ratefactor,momentum[0],momentum[1],momentum[2]);
  evt->SetRate(ratefactor);
  evt->SetAsymmetry(0.0);
  evt->SetEffCrossSection(1.0);
  evt->ProduceNewParticle( G4ThreeVector(0.0, 0.0, 0.0),G4ThreeVector(momentum[0] ,momentum[1] , momentum[2] ),pidname[pid]);


  
  //LUND_in.close();

};

void remollGenLUND::SetLUNDFile(G4String f){
  //open the LUND file
  LUND_filename = f;
  LUND_in.open(f,ios::in);
  
  if (!LUND_in.good()){
    G4cerr << "LUND file does not exist."<< G4endl;
    exit(1);
  }
  
  printf("LUND file set to %s \n",f.data());
  
  /*
    LUND_in.close();
  */
}
