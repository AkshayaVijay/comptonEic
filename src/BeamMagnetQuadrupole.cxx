
//_____________________________________________________________________________
//
// beamline Quadrupole magnet
//
//_____________________________________________________________________________

//Geant headers
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4Cons.hh"
#include "G4SystemOfUnits.hh"
#include "G4PVPlacement.hh"
#include "G4FieldManager.hh"
#include "G4UniformMagField.hh"
#include "G4VisAttributes.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4RotationMatrix.hh"
#include "G4QuadrupoleMagField.hh"

//local headers
#include "BeamMagnetQuadrupole.h"

//_____________________________________________________________________________
BeamMagnetQuadrupole::BeamMagnetQuadrupole(G4String nam, G4double xpos,G4double ypos,G4double zpos,G4double length, G4double r1, G4double r2, G4double dout, G4double angle, G4double gradient,G4double col1,G4double col2,G4double col3, G4LogicalVolume *top, G4bool quadPartVis):
  Detector(), G4VSensitiveDetector(nam), fNam(nam) {

  //conical inner core
  invisible = quadPartVis;

  G4RotationMatrix* yRot = new G4RotationMatrix;
  yRot->rotateY(angle*rad);

  G4String nam_inner = fNam+"_inner";
  G4Cons *shape_inner = new G4Cons(nam_inner, 0, r2, 0, r1, length/2, 0, 360*deg);

  G4Material *mat_inner = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
  G4LogicalVolume *vol_inner = new G4LogicalVolume(shape_inner, mat_inner, nam_inner);
  vol_inner->SetVisAttributes( G4VisAttributes::GetInvisible() );

  //magnetic field inside the inner core
  G4QuadrupoleMagField *field = new G4QuadrupoleMagField(gradient, G4ThreeVector(xpos, ypos, zpos),yRot);
  G4FieldManager *fman = new G4FieldManager();
  fman->SetDetectorField(field);
  fman->CreateChordFinder(field);
  //fman->GetChordFinder()->SetDeltaChord(1e-7*meter);

  vol_inner->SetFieldManager(fman, true);

  //put the inner core to the top volume
  new G4PVPlacement(yRot, G4ThreeVector(xpos, ypos, zpos), vol_inner, nam_inner, top, false, 0);

  //cylindrical outer shape
  G4Tubs *shape_outer = new G4Tubs(fNam+"_outer", 0., dout*0.5, length/2-1e-4*meter, 0., 360.*deg);

  //magnet vessel around the inner magnetic core
  G4SubtractionSolid *shape_vessel = new G4SubtractionSolid(fNam, shape_outer, shape_inner);

  G4Material *mat_outer = G4NistManager::Instance()->FindOrBuildMaterial("G4_Fe");
  G4Material *mat_ves = mat_outer;
  if(invisible)
    mat_ves = mat_inner;
  G4LogicalVolume *vol_vessel = new G4LogicalVolume(shape_vessel, mat_ves, fNam);

  //vessel visibility
  G4VisAttributes *vis_vessel = new G4VisAttributes();
  vis_vessel->SetColor(col1, col2, col3); // blue
  vis_vessel->SetLineWidth(2);
  vis_vessel->SetForceSolid(true);
  //vis_vessel->SetForceAuxEdgeVisible(true);
  vol_vessel->SetVisAttributes(vis_vessel);

  //put the magnet vessel to the top volume
  new G4PVPlacement(yRot, G4ThreeVector(xpos, ypos, zpos), vol_vessel, fNam, top, false, 0);

}//BeamMagnetQuadrupole

//_____________________________________________________________________________
G4bool BeamMagnetQuadrupole::ProcessHits(G4Step *step, G4TouchableHistory*) {

  //remove the track entering the magnet vessel
  G4Track *track = step->GetTrack();
  if(!invisible)
    track->SetTrackStatus(fKillTrackAndSecondaries);

  return true;

}//ProcessHits



































