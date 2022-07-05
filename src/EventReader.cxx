//_____________________________________________________________________________
//
// generator reader for COMRAD
//_____________________________________________________________________________

//C++ headers
#include <string.h>
#include <sstream>
#include <vector>
#include <map>

//Geant headers
#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "G4PrimaryVertex.hh"
#include "G4Event.hh"
#include "Randomize.hh"

//local headers
#include "EventReader.h"
#include "MCEvent.h"
#include "G4ParticleGun.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"

#include "TMath.h"

using namespace std;

//_____________________________________________________________________________
EventReader::EventReader(DetectorConstruction *dc) : 
  G4VUserPrimaryGeneratorAction(), fIev(0), 
  fUseBeam(0), fBeamE(18*GeV), fvertexRotY(0.0103371112*rad)
  ,fvertexPosX(0), fvertexPosY(0), fvertexPosZ(0)
  ,fEmmX(0),fEmmY(0),flaserXY(0)
  ,fvertexAlphaX(0),fvertexAlphaY(0)
  ,fvertexBetaX(0),fvertexBetaY(0)
  ,fvertexSmearX(0),fvertexSmearY(0)
  ,fvertexSmearXp(0),fvertexSmearYp(0)
  ,fvertexSmearZ(0){

  //default input name
  fInputName = "events.dat";
  //command for name of input file
  fMsg = new G4GenericMessenger(this, "/EPol/input/");
  fMsg->DeclareProperty("name", fInputName);

  fDetConst = dc;

  //set use beam flag
  fMsg->DeclareProperty("useBeam", fUseBeam);
  fMsg->DeclarePropertyWithUnit("beamEnergy","GeV",fBeamE,"beam energy for particle gun");
  fMsg->DeclarePropertyWithUnit("vertexPosX", "cm",fvertexPosX,"initial vertex position x");
  fMsg->DeclarePropertyWithUnit("vertexPosY", "cm",fvertexPosY,"initial vertex position y");
  fMsg->DeclarePropertyWithUnit("vertexPosZ", "cm",fvertexPosZ,"initial vertex position z");
  fMsg->DeclarePropertyWithUnit("vertexRotY", "rad",fvertexRotY,"momentum direction rotation");
  fMsg->DeclarePropertyWithUnit("vertexBetaX", "m",fvertexBetaX,"beta beam parameter for X");
  fMsg->DeclarePropertyWithUnit("vertexBetaY", "m",fvertexBetaY,"beta beam parameter for Y");
  fMsg->DeclarePropertyWithUnit("beamEmmX", "nm",fEmmX,"emmitance beam parameter for X");
  fMsg->DeclarePropertyWithUnit("beamEmmY", "nm",fEmmY,"emmitance beam parameter for Y");
  fMsg->DeclarePropertyWithUnit("laserXYwidth", "m",flaserXY,"laser transvserse width");
  fMsg->DeclareProperty("vertexAlphaX", fvertexAlphaX,"alpha beam parameter for X");
  fMsg->DeclareProperty("vertexAlphaY", fvertexAlphaY,"alpha beam parameter for Y");
  fMsg->DeclarePropertyWithUnit("vertexSmearZ" , "cm",fvertexSmearZ ,"longitudinal vertex smearing width");

}//EventReader

//_____________________________________________________________________________
void EventReader::GeneratePrimaries(G4Event *evt) {

  //increment event count for progress printout
  fIev++;
  if( fIev%100000 == 0 ) {
    G4cout << "EventReader::GeneratePrimaries, event number: " << fIev << G4endl;
  }

  // G4cout << fvertexBetaX<< " "<<fvertexBetaY<< " "
  // 	 << fvertexAlphaX<< " "<<fvertexAlphaY<< " "
  // 	 << fEmmX<< " "<<fEmmY<< " "
  // 	 << flaserXY<<G4endl;
  // std::cin.ignore();

  if( fvertexBetaX!=0 && fvertexAlphaX!=0 && 
      fEmmX!=0 && fEmmY!=0 && 
      fvertexBetaY!=0 && fvertexAlphaY!=0 && 
      flaserXY!=0){
    G4double probSample,probRandom;
    do{
      G4double x0 = G4RandGauss::shoot(0,sqrt(fEmmX));
      G4double xp0 = G4RandGauss::shoot(0,sqrt(fEmmX));
      fvertexSmearX = sqrt(fvertexBetaX)*x0;
      fvertexSmearXp = - fvertexAlphaX*sqrt(1/fvertexBetaX)*x0 + xp0/sqrt(fvertexBetaX);
      probSample = TMath::Gaus(fvertexSmearX,0,flaserXY,1);
      G4double xl = G4RandGauss::shoot(0,flaserXY);
      probRandom = TMath::Gaus(xl,0,flaserXY,1);
      //G4cout<<"xl "<<xl<<" "<<probRandom<<" fv "<<fvertexSmearX<<" "<<probSample<<G4endl;
    }while(probSample < probRandom);
    // G4cout<<" "<<fvertexSmearX<<" "<<probSample<<" "<<probRandom<<G4endl;
    // std::cin.ignore();

    do{
      G4double y0 = G4RandGauss::shoot(0,sqrt(fEmmY));
      G4double yp0 = G4RandGauss::shoot(0,sqrt(fEmmY));
      fvertexSmearY = sqrt(fvertexBetaY)*y0;
      fvertexSmearYp = - fvertexAlphaY*sqrt(1/fvertexBetaY)*y0 + yp0/sqrt(fvertexBetaY);
      probSample = TMath::Gaus(fvertexSmearY,0,flaserXY,1);
      G4double yl = G4RandGauss::shoot(0,flaserXY);
      probRandom = TMath::Gaus(yl,0,flaserXY,1);
    }while(probSample < probRandom);

  }

  G4double smearedVx(fvertexPosX),smearedVy(fvertexPosY),smearedVz(fvertexPosZ);
  if(fvertexSmearX!=0){
    smearedVx += fvertexSmearX;
  }
  if(fvertexSmearY!=0){
    smearedVy += fvertexSmearY;
  }
  if(fvertexSmearZ!=0){
    smearedVz = G4RandGauss::shoot(fvertexPosZ,fvertexSmearZ);
  }
  G4double smearedRotX(fvertexSmearXp),smearedRotY(fvertexSmearYp);

  if(fUseBeam){

    G4PrimaryVertex *vtx = new G4PrimaryVertex(smearedVx, smearedVy, smearedVz, 0);
    G4ThreeVector momDir(0,0,-1);
    momDir.rotateY(fvertexRotY+smearedRotX);
    momDir.rotateX(smearedRotY);

    G4PrimaryParticle *part=new G4PrimaryParticle(11);
    part->SetMomentumDirection(momDir);
    part->SetTotalEnergy(fBeamE);
    vtx->SetPrimary(part);
    evt->AddPrimaryVertex(vtx);

  }else{

    //open COMRAD input
    if(!fIn.is_open()) OpenInput();
    if( !fIn.good() ) {
      G4cout << "EventReader::GeneratePrimaries: no more events" << G4endl;
      return;
    }
    string line;
    getline(fIn, line);
    stringstream ss(line);

    double partMomX[2],partMomY[2],partMomZ[2],partE[2];
    G4double uXsec(0),pXsec(0),uXsecAlphaCorr(0),pXsecAlphaCorr(0);
    ss>>partMomX[0]>>partMomY[0]>>partMomZ[0]>>partE[0]
      >>uXsec>>pXsec>>uXsecAlphaCorr>>pXsecAlphaCorr
      >>partMomX[1]>>partMomY[1]>>partMomZ[1]>>partE[1];

    fDetConst->getMCEvent()->SetPolXsec(pXsec);
    fDetConst->getMCEvent()->SetUnpolXsec(uXsec);

    G4PrimaryVertex *vtx = new G4PrimaryVertex(smearedVx, smearedVy, smearedVz, 0);

    const int partID[2]={22,11};
    for(int i=0;i<2;i++){
      G4ThreeVector mom(partMomX[i]*GeV,partMomY[i]*GeV,partMomZ[i]*GeV);
      mom.rotateY(fvertexRotY + acos(-1) + smearedRotX);
      mom.rotateX(smearedRotY);

      G4PrimaryParticle *part = new G4PrimaryParticle(partID[i],mom.x(),mom.y(),mom.z(),partE[i]*GeV);
      vtx->SetPrimary(part);
    }

    evt->AddPrimaryVertex(vtx);
  }
}//GeneratePrimaries

//_____________________________________________________________________________
void EventReader::OpenInput() {

  //open the input file

  G4cout<< __PRETTY_FUNCTION__<<" "<<__LINE__<<G4endl;
  G4cout << "EventReader::OpenInput: " << fInputName << G4endl;
  fIn.open(fInputName);

  if(fIn.fail()) {
    string description = "Can't open input: " + fInputName;
    G4Exception("EventReader::OpenInput", "InputNotOpen01", FatalException, description.c_str());
  }
  
  string line;
  G4cout<< " Parameters from ComRad file: "<< G4endl;
  for(int i=0;i<3;i++){
    getline(fIn, line);
    G4cout<<line<<G4endl;
  }


}//OpenInput


















