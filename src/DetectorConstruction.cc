#include "DetectorConstruction.hh"
#include "Materials.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4SubtractionSolid.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4Tubs.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Material.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"
///vis/modeling/trajectories/drawByCharge-0/set OpticalPhoton blue optik foton rengini değiştirmek için programa bunu yaz
DetectorConstruction::DetectorConstruction()
{
    fScoringVolume = nullptr;
    materials = Materials::GetInstance();
    Construct();
}

DetectorConstruction::~DetectorConstruction()
{
    if (materials) delete materials;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    // Get nist material manager
    G4NistManager* nistManager = G4NistManager::Instance();   
    
    // Materials
    G4Material* plexi_mat = materials->GetMaterial("G4_PLEXIGLASS");
    G4Material* glass_mat = materials->GetMaterial("G4_GLASS_PLATE");
    G4Material* scint_mat = materials->GetMaterial("scintmat");
    G4Material* TiO2 = materials->GetMaterial("G4_TITANIUM_DIOXIDE");
    G4Material* world_mat = materials->GetMaterial("G4_AIR");
    G4Material* Vacuum = materials->GetMaterial("G4_Galactic");
    G4Material* silicon = materials->GetMaterial("G4_Si");
    G4Material* demirMaterial = materials->GetMaterial("G4_Fe");
    G4Material* elBe = materials->GetMaterial("G4_Be");
    // Geometry parameters
    double L = 1000; // Silindir uzunluk
    double B = 300; // Yükseklik
    double A = 300;  // Genişlik

    // World dimensions
    G4double env_sizeX = (B) * cm, env_sizeY = (B) * cm, env_sizeZ = (L) * cm;
    G4double world_sizeX = 1.2 * env_sizeX;
    G4double world_sizeY = 1.2 * env_sizeY;
    G4double world_sizeZ = 1.2 * env_sizeZ;

    // World volume
    auto solidWorld = new G4Box("World", 0.5 * world_sizeX, 0.5 * world_sizeY, 0.5 * world_sizeZ);
    auto logicWorld = new G4LogicalVolume(solidWorld, Vacuum, "World");
    auto physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "World", nullptr, false, 0, true);

    // Envelope
    auto solidEnv = new G4Box("Envelope", 0.5 * env_sizeX, 0.5 * env_sizeY, 0.5 * env_sizeZ);
    auto logicEnv = new G4LogicalVolume(solidEnv, Vacuum, "Envelope");
    auto physEnv = new G4PVPlacement(nullptr, G4ThreeVector(), logicEnv, "Envelope", logicWorld, false, 0, true);
   
   // İç vakum 
   
   G4ThreeVector pos1 = G4ThreeVector(0 * cm, 0 * cm, 0 * cm);
   G4double Interradius_Vakum = 0.*cm;
   G4double Outerradius_Vakum = 2.3*cm;
   G4double Length_Vakum = 900.0 *cm;
   G4double Start_Angle_Vakum = 0.*deg;
   G4double SpanningAngle_Vakum = 360.*deg;
   
   auto VakumSilindir = new G4Tubs("Silindirs", Interradius_Vakum, Outerradius_Vakum, 0.5*Length_Vakum, Start_Angle_Vakum, SpanningAngle_Vakum);
   auto VakumLog = new G4LogicalVolume(VakumSilindir, Vacuum, "VakumSilindir");
   new G4PVPlacement(0,pos1,VakumLog,"VakumSilindir", logicEnv, false, 0);
   auto SurfaceVisAtt_Vakum = new G4VisAttributes(G4Colour::White());
   SurfaceVisAtt_Vakum->SetVisibility(true);
   SurfaceVisAtt_Vakum->SetForceWireframe(true);
   VakumLog->SetVisAttributes(SurfaceVisAtt_Vakum);
   
   //Beam Pipe
   G4double Interradius_BeamPipe = 2.3*cm;
   G4double Outerradius_BeamPipe = 2.38*cm;
   G4double Length_BeamPipe = 900.0*cm;
   G4double Start_Angle_BeamPipe = 0.*deg;
   G4double SpanningAngle_BeamPipe = 360.*deg;
   
   auto BeamPipeSilindir = new G4Tubs("BeamPipe", Interradius_BeamPipe, Outerradius_BeamPipe, 0.5*Length_BeamPipe, Start_Angle_BeamPipe, SpanningAngle_BeamPipe);
   auto BeamPipeLog = new G4LogicalVolume(BeamPipeSilindir, elBe, "BeamPipeSilindir");
   new G4PVPlacement(0,pos1,BeamPipeLog, "BeamPipeSilindir", logicEnv, false, 0);
   auto SurfaceVisAtt_BeamPipe = new G4VisAttributes(G4Colour::Blue());
   SurfaceVisAtt_BeamPipe->SetVisibility(true);
   SurfaceVisAtt_BeamPipe->SetForceWireframe(true);
   BeamPipeLog->SetVisAttributes(SurfaceVisAtt_BeamPipe);
   
   //ECAL1
   G4double Interradius_SCHL = 2.38*cm;
   G4double Outerradius_SCHL = 7.5*cm;
   G4double Length_SCHL = 2.0*cm;
   G4double Start_Angle_SCHL = 0.*deg;
   G4double SpanningAngle_SCHL = 360.*deg;
   G4ThreeVector poss = G4ThreeVector(0 * cm, 0 * cm, -14 * cm);
   auto SCHL = new G4Tubs("SCHL", Interradius_SCHL, Outerradius_SCHL, Length_SCHL, Start_Angle_SCHL, SpanningAngle_SCHL);
   auto SCHLLog = new G4LogicalVolume(SCHL, Vacuum, "SCHL");
   new G4PVPlacement(0,poss, SCHLLog, "SCHL", logicEnv, false, 0);
   auto SurfaceVisAtt_SCHL = new G4VisAttributes(G4Colour::Green());
   SCHLLog->SetVisAttributes(SurfaceVisAtt_SCHL);
   
   //ECAL2
   G4ThreeVector pos2 = G4ThreeVector(0*cm, 0*cm, -8*cm);
   auto SCHL2 = new G4Tubs("SCHl2", Interradius_SCHL, Outerradius_SCHL, Length_SCHL, Start_Angle_SCHL, SpanningAngle_SCHL);
   auto SCHLLog2 = new G4LogicalVolume(SCHL2, Vacuum, "SCHL2");
   new G4PVPlacement(0,pos2, SCHLLog2, "SCHL2", logicEnv, false, 0);
   auto SurfaceVisAtt_SCHL2 = new G4VisAttributes(G4Colour::Green());
   SCHLLog2->SetVisAttributes(SurfaceVisAtt_SCHL2);
   
   //ECAL3
   G4ThreeVector posss = G4ThreeVector(0 * cm, 0 * cm, 10 * cm);
   auto SCHL3 = new G4Tubs("SCHL3", Interradius_SCHL, Outerradius_SCHL, Length_SCHL, Start_Angle_SCHL, SpanningAngle_SCHL);
   auto SCHL3Log = new G4LogicalVolume(SCHL3, Vacuum, "SCHL3");
   new G4PVPlacement(0,posss, SCHL3Log, "SCHL3", logicEnv, false, 0);
   auto SurfaceVisAtt_SCHL3 = new G4VisAttributes(G4Colour::Green());
   SCHL3Log->SetVisAttributes(SurfaceVisAtt_SCHL3);
   
   //ECAL4
   G4ThreeVector post = G4ThreeVector(0*cm, 0*cm, 16*cm);
   auto SCHL4 = new G4Tubs("SCHl4", Interradius_SCHL, Outerradius_SCHL, Length_SCHL, Start_Angle_SCHL, SpanningAngle_SCHL);
   auto SCHL4Log = new G4LogicalVolume(SCHL4, Vacuum, "SCHL4");
   new G4PVPlacement(0,post, SCHL4Log, "SCHL4", logicEnv, false, 0);
   auto SurfaceVisAtt_SCHL4 = new G4VisAttributes(G4Colour::Green());
   SCHL4Log->SetVisAttributes(SurfaceVisAtt_SCHL4);
   
   //ST1
   G4ThreeVector pos3 = G4ThreeVector(0.*cm,9.0*cm,0.*cm);
   G4double ST_x = 6.0*cm;
   G4double ST_y = 3.0*cm;
   G4double ST_z = 1.0*m;
   auto ST1 = new G4Box("ST1", 0.5 * ST_x, 0.5 * ST_y, 0.5 * ST_z);
   auto ST1Log = new G4LogicalVolume(ST1, Vacuum, "ST1");
   auto physST1 = new G4PVPlacement(0,pos3, ST1Log, "ST1", logicEnv, false, 0);
   auto SurfaceVisAtt_ST1 = new G4VisAttributes(G4Colour::Blue());
   ST1Log->SetVisAttributes(SurfaceVisAtt_ST1);
   
   //ST2
   G4ThreeVector pos4 = G4ThreeVector(6.5*cm,6.5*cm,0.*cm);
   G4RotationMatrix* rot = new G4RotationMatrix();
   rot->rotateZ(45*deg);
   auto ST2 = new G4Box("ST2", 0.5 * ST_x, 0.5 * ST_y, 0.5 * ST_z);
   auto ST2Log = new G4LogicalVolume(ST2, Vacuum, "ST2");
   auto physST2 = new G4PVPlacement(rot,pos4, ST2Log, "ST2", logicEnv, false, 0);
   auto SurfaceVisAtt_ST2 = new G4VisAttributes(G4Colour::Blue());
   ST2Log->SetVisAttributes(SurfaceVisAtt_ST2);
   
   //ST3
   G4ThreeVector pos5 = G4ThreeVector(9.0*cm,0.*cm,0.*cm);
   G4RotationMatrix* rott = new G4RotationMatrix();
   rott->rotateZ(90*deg);
   auto ST3 = new G4Box("ST3", 0.5 * ST_x, 0.5 * ST_y, 0.5 * ST_z);
   auto ST3Log = new G4LogicalVolume(ST3, Vacuum, "ST3");
   auto physST3 = new G4PVPlacement(rott,pos5, ST3Log, "ST3", logicEnv, false, 0);
   auto SurfaceVisAtt_ST3 = new G4VisAttributes(G4Colour::Blue());
   ST3Log->SetVisAttributes(SurfaceVisAtt_ST3);
   
   //ST4
   G4ThreeVector pos6 = G4ThreeVector(0.*cm,-9.*cm,0.*cm);
   auto ST4 = new G4Box("ST4", 0.5 * ST_x, 0.5 * ST_y, 0.5 * ST_z);
   auto ST4Log = new G4LogicalVolume(ST4, Vacuum, "ST4");
   auto physST4 = new G4PVPlacement(0,pos6, ST4Log, "ST4", logicEnv, false, 0);
   auto SurfaceVisAtt_ST4 = new G4VisAttributes(G4Colour::Blue());
   ST4Log->SetVisAttributes(SurfaceVisAtt_ST4);
   
   //ST5
   G4ThreeVector pos7 = G4ThreeVector(-9.0*cm,0.*cm,0.*cm);
   G4RotationMatrix* rottt = new G4RotationMatrix();
   rottt->rotateZ(270*deg);
   auto ST5 = new G4Box("ST5", 0.5 * ST_x, 0.5 * ST_y, 0.5 * ST_z);
   auto ST5Log = new G4LogicalVolume(ST5, Vacuum, "ST5");
   auto physST5 = new G4PVPlacement(rottt,pos7, ST5Log, "ST5", logicEnv, false, 0);
   auto SurfaceVisAtt_ST5 = new G4VisAttributes(G4Colour::Blue());
   ST5Log->SetVisAttributes(SurfaceVisAtt_ST5);
   
   //ST6
   G4ThreeVector pos8 = G4ThreeVector(-6.5*cm,-6.5*cm,0.*cm);
   auto ST6 = new G4Box("ST6", 0.5 * ST_x, 0.5 * ST_y, 0.5 * ST_z);
   auto ST6Log = new G4LogicalVolume(ST6, Vacuum, "ST6");
   auto physST6 = new G4PVPlacement(rot,pos8, ST6Log, "ST6", logicEnv, false, 0);
   auto SurfaceVisAtt_ST6 = new G4VisAttributes(G4Colour::Blue());
   ST6Log->SetVisAttributes(SurfaceVisAtt_ST6);
   
   //ST7
   G4ThreeVector pos9 = G4ThreeVector(6.5*cm,-6.5*cm,0.*cm);
   G4RotationMatrix* rot5 = new G4RotationMatrix();
   rot5->rotateZ(135*deg);
   auto ST7 = new G4Box("ST7", 0.5 * ST_x, 0.5 * ST_y, 0.5 * ST_z);
   auto ST7Log = new G4LogicalVolume(ST7, Vacuum, "ST7");
   auto physST7 = new G4PVPlacement(rot5,pos9, ST7Log, "ST7", logicEnv, false, 0);
   auto SurfaceVisAtt_ST7 = new G4VisAttributes(G4Colour::Blue());
   ST7Log->SetVisAttributes(SurfaceVisAtt_ST7);
   
   //ST8
   G4ThreeVector pos10 = G4ThreeVector(-6.5*cm,6.5*cm,0.*cm);
   G4RotationMatrix* rot6 = new G4RotationMatrix();
   rot6->rotateZ(315*deg);
   auto ST8 = new G4Box("ST8", 0.5 * ST_x, 0.5 * ST_y, 0.5 * ST_z);
   auto ST8Log = new G4LogicalVolume(ST8, Vacuum, "ST8");
   auto physST8 = new G4PVPlacement(rot6,pos10, ST8Log, "ST8", logicEnv, false, 0);
   auto SurfaceVisAtt_ST8 = new G4VisAttributes(G4Colour::Blue());
   ST8Log->SetVisAttributes(SurfaceVisAtt_ST8);
   
   //ST9
   G4ThreeVector pos11 = G4ThreeVector(0.*cm,12.0*cm,0.*cm);
   G4double ST9_x = 6.5*cm;
   auto ST9 = new G4Box("ST9", 0.5 * ST9_x, 0.5 * ST_y, 0.5 * ST_z);
   auto ST9Log = new G4LogicalVolume(ST9, Vacuum, "ST9");
   auto physST9 = new G4PVPlacement(0,pos11, ST9Log, "ST9", logicEnv, false, 0);
   auto SurfaceVisAtt_ST9 = new G4VisAttributes(G4Colour::Blue());
   ST9Log->SetVisAttributes(SurfaceVisAtt_ST9);
   
   //ST10
   G4ThreeVector pos12 = G4ThreeVector(12.*cm,0.*cm,0.*cm);
   auto ST10 = new G4Box("ST10", 0.5 * ST9_x, 0.5 * ST_y, 0.5 * ST_z);
   auto ST10Log = new G4LogicalVolume(ST10, Vacuum, "ST10");
   auto physST10 = new G4PVPlacement(rott,pos12, ST10Log, "ST10", logicEnv, false, 0);
   auto SurfaceVisAtt_ST10 = new G4VisAttributes(G4Colour::Blue());
   ST10Log->SetVisAttributes(SurfaceVisAtt_ST10);
   
   //ST11
   G4ThreeVector pos13 = G4ThreeVector(0.*cm,-12.*cm,0.*cm);
   auto ST11 = new G4Box("ST11", 0.5 * ST9_x, 0.5 * ST_y, 0.5 * ST_z);
   auto ST11Log = new G4LogicalVolume(ST11, Vacuum, "ST11");
   auto physST11 = new G4PVPlacement(0,pos13, ST11Log, "ST11", logicEnv, false, 0);
   auto SurfaceVisAtt_ST11 = new G4VisAttributes(G4Colour::Blue());
   ST11Log->SetVisAttributes(SurfaceVisAtt_ST11);
   
   //ST12
   G4ThreeVector pos14 = G4ThreeVector(-12.*cm,0.*cm,0.*cm);
   auto ST12 = new G4Box("ST12", 0.5 * ST9_x, 0.5 * ST_y, 0.5 * ST_z);
   auto ST12Log = new G4LogicalVolume(ST12, Vacuum, "ST12");
   auto physST12 = new G4PVPlacement(rottt,pos14, ST12Log, "ST12", logicEnv, false, 0);
   auto SurfaceVisAtt_ST12 = new G4VisAttributes(G4Colour::Blue());
   ST12Log->SetVisAttributes(SurfaceVisAtt_ST12);
   
   //ST13
   G4ThreeVector pos15 = G4ThreeVector(8.8*cm,8.8*cm,0.*cm);
   G4double ST13_x = 5.5*cm;
   auto ST13 = new G4Box("ST13", 0.5 * ST13_x, 0.5 * ST_y, 0.5 * ST_z);
   auto ST13Log = new G4LogicalVolume(ST13, Vacuum, "ST13");
   auto physST13 = new G4PVPlacement(rot,pos15, ST13Log, "ST13", logicEnv, false, 0);
   auto SurfaceVisAtt_ST13 = new G4VisAttributes(G4Colour::Blue());
   ST13Log->SetVisAttributes(SurfaceVisAtt_ST13);
   
   //ST14
   G4ThreeVector pos16 = G4ThreeVector(8.8*cm,-8.8*cm,0.*cm);
   auto ST14 = new G4Box("ST14", 0.5 * ST13_x, 0.5 * ST_y, 0.5 * ST_z);
   auto ST14Log = new G4LogicalVolume(ST14, Vacuum, "ST14");
   auto physST14 = new G4PVPlacement(rot5,pos16, ST14Log, "ST14", logicEnv, false, 0);
   auto SurfaceVisAtt_ST14 = new G4VisAttributes(G4Colour::Blue());
   ST14Log->SetVisAttributes(SurfaceVisAtt_ST14);
   
   //ST15
   G4ThreeVector pos17 = G4ThreeVector(-8.8*cm,-8.8*cm,0.*cm);
   auto ST15 = new G4Box("ST15", 0.5 * ST13_x, 0.5 * ST_y, 0.5 * ST_z);
   auto ST15Log = new G4LogicalVolume(ST15, Vacuum, "ST15");
   auto physST15 = new G4PVPlacement(rot,pos17, ST15Log, "ST15", logicEnv, false, 0);
   auto SurfaceVisAtt_ST15 = new G4VisAttributes(G4Colour::Blue());
   ST15Log->SetVisAttributes(SurfaceVisAtt_ST15);
   
   //ST16
   G4ThreeVector pos18 = G4ThreeVector(-8.8*cm,8.8*cm,0.*cm);
   auto ST16 = new G4Box("ST16", 0.5 * ST13_x, 0.5 * ST_y, 0.5 * ST_z);
   auto ST16Log = new G4LogicalVolume(ST16, Vacuum, "ST16");
   auto physST16 = new G4PVPlacement(rot5,pos18, ST16Log, "ST16", logicEnv, false, 0);
   auto SurfaceVisAtt_ST16 = new G4VisAttributes(G4Colour::Blue());
   ST16Log->SetVisAttributes(SurfaceVisAtt_ST16);
   
   //ST17
   G4ThreeVector pos19 = G4ThreeVector(0.*cm,15.*cm,0.*cm);
   auto ST17 = new G4Box("ST17", 0.5 * ST13_x, 0.5 * ST_y, 0.5 * ST_z);
   auto ST17Log = new G4LogicalVolume(ST17, Vacuum, "ST17");
   auto physST17 = new G4PVPlacement(0,pos19, ST17Log, "ST17", logicEnv, false, 0);
   auto SurfaceVisAtt_ST17 = new G4VisAttributes(G4Colour::Blue());
   ST17Log->SetVisAttributes(SurfaceVisAtt_ST17);
   
   //ST18
   G4ThreeVector pos20 = G4ThreeVector(6.*cm,14.*cm,0.*cm);
   G4RotationMatrix* rot13 = new G4RotationMatrix();
   rot13->rotateZ(20*deg);
   G4double ST18_x = 4.5*cm;
   G4double ST18_y = 3.0*cm;
   G4double ST18_z = 1.0*m;
   auto ST18 = new G4Box("ST18", 0.5 * ST18_x, 0.5 * ST18_y, 0.5 * ST18_z);
   auto ST18Log = new G4LogicalVolume(ST18, Vacuum, "ST18");
   auto physST18 = new G4PVPlacement(rot13,pos20, ST18Log, "ST18", logicEnv, false, 0);
   auto SurfaceVisAtt_ST18 = new G4VisAttributes(G4Colour::Blue());
   ST18Log->SetVisAttributes(SurfaceVisAtt_ST18);
   
       
   //ST19
   G4ThreeVector pos21 = G4ThreeVector(10.7*cm,11.7*cm,0.*cm);
   G4RotationMatrix* rot14 = new G4RotationMatrix();
   rot14->rotateZ(40*deg);
   auto ST19 = new G4Box("ST19", 0.5 * ST18_x, 0.5 * ST18_y, 0.5 * ST18_z);
   auto ST19Log = new G4LogicalVolume(ST19, Vacuum, "ST19");
   auto physST19 = new G4PVPlacement(rot14,pos21, ST19Log, "ST19", logicEnv, false, 0);
   auto SurfaceVisAtt_ST19 = new G4VisAttributes(G4Colour::Blue());
   ST19Log->SetVisAttributes(SurfaceVisAtt_ST19); 
 
 
   //ST20
   G4ThreeVector pos22 = G4ThreeVector(14.2*cm,7.5*cm,0.*cm);
   G4RotationMatrix* rot15 = new G4RotationMatrix();
   rot15->rotateZ(60*deg);
   auto ST20 = new G4Box("ST20", 0.5 * ST18_x, 0.5 * ST18_y, 0.5 * ST18_z);
   auto ST20Log = new G4LogicalVolume(ST20, Vacuum, "ST20");
   auto physST20 = new G4PVPlacement(rot15,pos22, ST20Log, "ST20", logicEnv, false, 0);
   auto SurfaceVisAtt_ST20 = new G4VisAttributes(G4Colour::Blue());
   ST20Log->SetVisAttributes(SurfaceVisAtt_ST20); 
   
   
   //ST21
   G4ThreeVector pos23 = G4ThreeVector(15.*cm,2.*cm,0.*cm);
   auto ST21 = new G4Box("ST21", 0.5 * ST18_x, 0.5 * ST18_y, 0.5 * ST18_z);
   auto ST21Log = new G4LogicalVolume(ST21, Vacuum, "ST21");
   auto physST21 = new G4PVPlacement(rott,pos23, ST21Log, "ST21", logicEnv, false, 0);
   auto SurfaceVisAtt_ST21 = new G4VisAttributes(G4Colour::Blue());
   ST21Log->SetVisAttributes(SurfaceVisAtt_ST21); 
   
   
   //S22
   G4ThreeVector pos24 = G4ThreeVector(15.*cm,-3.*cm,0.*cm);
   auto ST22 = new G4Box("ST22", 0.5 * ST18_x, 0.5 * ST18_y, 0.5 * ST18_z);
   auto ST22Log = new G4LogicalVolume(ST22, Vacuum, "ST22");
   auto physST22 = new G4PVPlacement(rott,pos24, ST22Log, "ST22", logicEnv, false, 0);
   auto SurfaceVisAtt_ST22 = new G4VisAttributes(G4Colour::Blue());
   ST22Log->SetVisAttributes(SurfaceVisAtt_ST22); 
   
   //ST23
   G4ThreeVector pos25 = G4ThreeVector(13.8*cm,-8.8*cm,0.*cm);
   G4RotationMatrix* rot18 = new G4RotationMatrix();
   rot18->rotateZ(125*deg);
   auto ST23 = new G4Box("ST23", 0.5 * ST18_x, 0.5 * ST18_y, 0.5 * ST18_z);
   auto ST23Log = new G4LogicalVolume(ST23, Vacuum, "ST23");
   auto physST23 = new G4PVPlacement(rot18,pos25, ST23Log, "ST23", logicEnv, false, 0);
   auto SurfaceVisAtt_ST23 = new G4VisAttributes(G4Colour::Blue());
   ST23Log->SetVisAttributes(SurfaceVisAtt_ST23); 
   
   //ST24
   G4ThreeVector pos26 = G4ThreeVector(10.*cm,-12.6*cm,0.*cm);
   G4RotationMatrix* rot19 = new G4RotationMatrix();
   rot19->rotateZ(145*deg);
   auto ST24 = new G4Box("ST24", 0.5 * ST18_x, 0.5 * ST18_y, 0.5 * ST18_z);
   auto ST24Log = new G4LogicalVolume(ST24, Vacuum, "ST24");
   auto physST24 = new G4PVPlacement(rot19,pos26, ST24Log, "ST24", logicEnv, false, 0);
   auto SurfaceVisAtt_ST24 = new G4VisAttributes(G4Colour::Blue());
   ST24Log->SetVisAttributes(SurfaceVisAtt_ST24); 
    
    
   //ST25
   G4ThreeVector pos27 = G4ThreeVector(5.*cm,-15.*cm,0.*cm);
   G4RotationMatrix* rot20 = new G4RotationMatrix();
   rot20->rotateZ(160*deg);
   auto ST25 = new G4Box("ST25", 0.5 * ST18_x, 0.5 * ST18_y, 0.5 * ST18_z);
   auto ST25Log = new G4LogicalVolume(ST25, Vacuum, "ST25");
   auto physST25 = new G4PVPlacement(rot20,pos27, ST25Log, "ST25", logicEnv, false, 0);
   auto SurfaceVisAtt_ST25 = new G4VisAttributes(G4Colour::Blue());
   ST25Log->SetVisAttributes(SurfaceVisAtt_ST25); 
   
   
   //ST26
   G4ThreeVector pos28 = G4ThreeVector(-0.5*cm,-15.*cm,0.*cm);
   auto ST26 = new G4Box("HCAL26", 0.5 * ST18_x, 0.5 * ST18_y, 0.5 * ST18_z);
   auto ST26Log = new G4LogicalVolume(ST26, Vacuum, "ST26");
   auto physST26 = new G4PVPlacement(0,pos28, ST26Log, "ST26", logicEnv, false, 0);
   auto SurfaceVisAtt_ST26 = new G4VisAttributes(G4Colour::Blue());
   ST26Log->SetVisAttributes(SurfaceVisAtt_ST26); 
   
   //ST27
   G4ThreeVector pos29 = G4ThreeVector(-6.*cm,-14.*cm,0.*cm);
   G4RotationMatrix* rot21 = new G4RotationMatrix();
   rot21->rotateZ(200*deg);
   auto ST27 = new G4Box("ST27", 0.5 * ST18_x, 0.5 * ST18_y, 0.5 * ST18_z);
   auto ST27Log = new G4LogicalVolume(ST27, Vacuum, "ST27");
   auto physST27 = new G4PVPlacement(rot21,pos29, ST27Log, "ST27", logicEnv, false, 0);
   auto SurfaceVisAtt_ST27 = new G4VisAttributes(G4Colour::Blue());
   ST27Log->SetVisAttributes(SurfaceVisAtt_ST27); 
   
   
   //ST28
   G4ThreeVector pos30 = G4ThreeVector(-11.*cm,-11.5*cm,0.*cm);
   G4RotationMatrix* rot22 = new G4RotationMatrix();
   rot22->rotateZ(220*deg);
   auto ST28 = new G4Box("ST28", 0.5 * ST18_x, 0.5 * ST18_y, 0.5 * ST18_z);
   auto ST28Log = new G4LogicalVolume(ST28, Vacuum, "ST28");
   auto physST28 = new G4PVPlacement(rot22,pos30, ST28Log, "ST28", logicEnv, false, 0);
   auto SurfaceVisAtt_ST28 = new G4VisAttributes(G4Colour::Blue());
   ST28Log->SetVisAttributes(SurfaceVisAtt_ST28); 
   
   //ST29
   G4ThreeVector pos31 = G4ThreeVector(-14.5*cm,-7.*cm,0.*cm);
   G4RotationMatrix* rot23 = new G4RotationMatrix();
   rot23->rotateZ(240*deg);
   auto ST29 = new G4Box("ST29", 0.5 * ST18_x, 0.5 * ST18_y, 0.5 * ST18_z);
   auto ST29Log = new G4LogicalVolume(ST29, Vacuum, "ST29");
   auto physST29 = new G4PVPlacement(rot23,pos31, ST29Log, "ST29", logicEnv, false, 0);
   auto SurfaceVisAtt_ST29 = new G4VisAttributes(G4Colour::Blue());
   ST29Log->SetVisAttributes(SurfaceVisAtt_ST29); 
   
   //ST30
   G4ThreeVector pos32 = G4ThreeVector(-15*cm,-1.5*cm,0.*cm);
   auto ST30 = new G4Box("ST30", 0.5 * ST18_x, 0.5 * ST18_y, 0.5 * ST18_z);
   auto ST30Log = new G4LogicalVolume(ST30, Vacuum, "ST30");
   auto physST30 = new G4PVPlacement(rottt,pos32, ST30Log, "ST30", logicEnv, false, 0);
   auto SurfaceVisAtt_ST30 = new G4VisAttributes(G4Colour::Blue());
   ST30Log->SetVisAttributes(SurfaceVisAtt_ST30); 
   
   //ST31
   G4ThreeVector pos33 = G4ThreeVector(-15*cm,3.5*cm,0.*cm);
   auto ST31 = new G4Box("ST31", 0.5 * ST18_x, 0.5 * ST18_y, 0.5 * ST18_z);
   auto ST31Log = new G4LogicalVolume(ST31, Vacuum, "ST31");
   auto physST31 = new G4PVPlacement(rottt,pos33, ST31Log, "ST31", logicEnv, false, 0);
   auto SurfaceVisAtt_ST31 = new G4VisAttributes(G4Colour::Blue());
   ST31Log->SetVisAttributes(SurfaceVisAtt_ST31); 
   
   
   //ST32
   G4ThreeVector pos34 = G4ThreeVector(-13.5*cm,9.*cm,0.*cm);
   G4RotationMatrix* rot25 = new G4RotationMatrix();
   rot25->rotateZ(295*deg);
   auto ST32 = new G4Box("ST32", 0.5 * ST18_x, 0.5 * ST18_y, 0.5 * ST18_z);
   auto ST32Log = new G4LogicalVolume(ST32, Vacuum, "ST32");
   auto physST32 = new G4PVPlacement(rot25,pos34, ST32Log, "ST32", logicEnv, false, 0);
   auto SurfaceVisAtt_ST32 = new G4VisAttributes(G4Colour::Blue());
   ST32Log->SetVisAttributes(SurfaceVisAtt_ST32); 
   
   //ST33
   G4ThreeVector pos35 = G4ThreeVector(-10.*cm,13.*cm,0.*cm);
   G4RotationMatrix* rot26 = new G4RotationMatrix();
   rot26->rotateZ(315*deg);
   auto ST33 = new G4Box("ST33", 0.5 * ST18_x, 0.5 * ST18_y, 0.5 * ST18_z);
   auto ST33Log = new G4LogicalVolume(ST33, Vacuum, "ST33");
   auto physST33 = new G4PVPlacement(rot26,pos35, ST33Log, "ST33", logicEnv, false, 0);
   auto SurfaceVisAtt_ST33 = new G4VisAttributes(G4Colour::Blue());
   ST33Log->SetVisAttributes(SurfaceVisAtt_ST33); 
   
   
   //ST34
   G4ThreeVector pos36 = G4ThreeVector(-5.5*cm,14.5*cm,0.*cm);
   G4RotationMatrix* rot27 = new G4RotationMatrix();
   rot27->rotateZ(340*deg);
   G4double ST34_x = 3.5*cm;
   auto ST34 = new G4Box("ST34", 0.5 * ST34_x, 0.5 * ST18_y, 0.5 * ST18_z);
   auto ST34Log = new G4LogicalVolume(ST34, Vacuum, "ST34");
   auto physST34 = new G4PVPlacement(rot27,pos36, ST34Log, "ST34", logicEnv, false, 0);
   auto SurfaceVisAtt_ST34 = new G4VisAttributes(G4Colour::Blue());
   ST34Log->SetVisAttributes(SurfaceVisAtt_ST34); 
   
   
   //ST35
   G4ThreeVector pos37 = G4ThreeVector(0*cm,18.2*cm,0.*cm);
   G4double ST35_x = 4.5*cm;
   G4double ST35_y = 2.0*cm;
   G4double ST35_z = 1.0*m;
   auto ST35 = new G4Box("ST35", 0.5 * ST35_x, 0.5 * ST35_y, 0.5 * ST35_z);
   auto ST35Log = new G4LogicalVolume(ST35, Vacuum, "ST35");
   auto physST35 = new G4PVPlacement(0,pos37, ST35Log, "ST35", logicEnv, false, 0);
   auto SurfaceVisAtt_ST35 = new G4VisAttributes(G4Colour::Blue());
   ST35Log->SetVisAttributes(SurfaceVisAtt_ST35); 
   
   
   //ST36
   G4ThreeVector pos38 = G4ThreeVector(5*cm,17.5*cm,0.*cm);
   auto ST38 = new G4Box("ST38", 0.5 * ST35_x, 0.5 * ST35_y, 0.5 * ST35_z);
   auto ST38Log = new G4LogicalVolume(ST38, Vacuum, "ST38");
   auto physST38 = new G4PVPlacement(rot13,pos38, ST38Log, "ST38", logicEnv, false, 0);
   auto SurfaceVisAtt_ST38 = new G4VisAttributes(G4Colour::Blue());
   ST38Log->SetVisAttributes(SurfaceVisAtt_ST38); 
   
   //ST37
   G4ThreeVector pos39 = G4ThreeVector(9.7*cm,15.8*cm,0.*cm);
   G4RotationMatrix* rot29 = new G4RotationMatrix();
   rot29->rotateZ(30*deg);
   auto ST39 = new G4Box("ST39", 0.5 * ST35_x, 0.5 * ST35_y, 0.5 * ST35_z);
   auto ST39Log = new G4LogicalVolume(ST39, Vacuum, "ST39");
   auto physST39 = new G4PVPlacement(rot29,pos39, ST39Log, "ST39", logicEnv, false, 0);
   auto SurfaceVisAtt_ST39 = new G4VisAttributes(G4Colour::Blue());
   ST39Log->SetVisAttributes(SurfaceVisAtt_ST39); 
   
   //ST38
   G4ThreeVector pos40 = G4ThreeVector(13.8*cm,13.3*cm,0.*cm);
   auto ST40 = new G4Box("ST40", 0.5 * ST35_x, 0.5 * ST35_y, 0.5 * ST35_z);
   auto ST40Log = new G4LogicalVolume(ST40, Vacuum, "ST40");
   auto physST40 = new G4PVPlacement(rot14,pos40, ST40Log, "ST40", logicEnv, false, 0);
   auto SurfaceVisAtt_ST40 = new G4VisAttributes(G4Colour::Blue());
   ST40Log->SetVisAttributes(SurfaceVisAtt_ST40); 
   
   //ST39
   G4ThreeVector pos41 = G4ThreeVector(16.8*cm,9.5*cm,0.*cm);
   auto ST41 = new G4Box("ST41", 0.5 * ST35_x, 0.5 * ST35_y, 0.5 * ST35_z);
   auto ST41Log = new G4LogicalVolume(ST41, Vacuum, "ST41");
   auto physST41 = new G4PVPlacement(rot15,pos41, ST41Log, "ST41", logicEnv, false, 0);
   auto SurfaceVisAtt_ST41 = new G4VisAttributes(G4Colour::Blue());
   ST41Log->SetVisAttributes(SurfaceVisAtt_ST41); 
   
   //ST40
   G4ThreeVector pos42 = G4ThreeVector(18.*cm,4.5*cm,0.*cm);
   auto ST42 = new G4Box("ST42", 0.5 * ST35_x, 0.5 * ST35_y, 0.5 * ST35_z);
   auto ST42Log = new G4LogicalVolume(ST42, Vacuum, "ST42");
   auto physST42 = new G4PVPlacement(rott,pos42, ST42Log, "ST42", logicEnv, false, 0);
   auto SurfaceVisAtt_ST42 = new G4VisAttributes(G4Colour::Blue());
   ST42Log->SetVisAttributes(SurfaceVisAtt_ST42); 
   
   //ST41
   G4ThreeVector pos43 = G4ThreeVector(18.*cm,0.*cm,0.*cm);
   auto ST43 = new G4Box("ST43", 0.5 * ST35_x, 0.5 * ST35_y, 0.5 * ST35_z);
   auto ST43Log = new G4LogicalVolume(ST43, Vacuum, "ST43");
   auto physST43 = new G4PVPlacement(rott,pos43, ST43Log, "ST43", logicEnv, false, 0);
   auto SurfaceVisAtt_ST43 = new G4VisAttributes(G4Colour::Blue());
   ST43Log->SetVisAttributes(SurfaceVisAtt_ST43); 
   
   //ST42
   G4ThreeVector pos44 = G4ThreeVector(18.*cm,-4.5*cm,0.*cm);
   auto ST44 = new G4Box("ST44", 0.5 * ST35_x, 0.5 * ST35_y, 0.5 * ST35_z);
   auto ST44Log = new G4LogicalVolume(ST44, Vacuum, "ST44");
   auto physST44 = new G4PVPlacement(rott,pos44, ST43Log, "ST43", logicEnv, false, 0);
   auto SurfaceVisAtt_ST44 = new G4VisAttributes(G4Colour::Blue());
   ST44Log->SetVisAttributes(SurfaceVisAtt_ST44); 
   
   //ST43
   G4ThreeVector pos45 = G4ThreeVector(16.8*cm,-9.7*cm,0.*cm);
   G4RotationMatrix* rot33 = new G4RotationMatrix();
   rot33->rotateZ(120*deg);
   auto ST45 = new G4Box("ST45", 0.5 * ST35_x, 0.5 * ST35_y, 0.5 * ST35_z);
   auto ST45Log = new G4LogicalVolume(ST45, Vacuum, "ST45");
   auto physST45 = new G4PVPlacement(rot33,pos45, ST45Log, "ST45", logicEnv, false, 0);
   auto SurfaceVisAtt_ST45 = new G4VisAttributes(G4Colour::Blue());
   ST45Log->SetVisAttributes(SurfaceVisAtt_ST45); 
   
   //ST44
   G4ThreeVector pos46 = G4ThreeVector(13.8*cm,-13.5*cm,0.*cm);
   G4RotationMatrix* rot34 = new G4RotationMatrix();
   rot34->rotateZ(140*deg);
   auto ST46 = new G4Box("ST46", 0.5 * ST35_x, 0.5 * ST35_y, 0.5 * ST35_z);
   auto ST46Log = new G4LogicalVolume(ST46, Vacuum, "ST46");
   auto physST46 = new G4PVPlacement(rot34,pos46, ST46Log, "ST46", logicEnv, false, 0);
   auto SurfaceVisAtt_ST46 = new G4VisAttributes(G4Colour::Blue());
   ST46Log->SetVisAttributes(SurfaceVisAtt_ST46);
   
   //ST45
   G4ThreeVector pos47 = G4ThreeVector(9.4*cm,-16.3*cm,0.*cm);
   G4RotationMatrix* rot35 = new G4RotationMatrix();
   rot35->rotateZ(160*deg);
   auto ST47 = new G4Box("ST47", 0.5 * ST35_x, 0.5 * ST35_y, 0.5 * ST35_z);
   auto ST47Log = new G4LogicalVolume(ST47, Vacuum, "ST47");
   auto physST47 = new G4PVPlacement(rot35,pos47, ST47Log, "ST47", logicEnv, false, 0);
   auto SurfaceVisAtt_ST47 = new G4VisAttributes(G4Colour::Blue());
   ST47Log->SetVisAttributes(SurfaceVisAtt_ST47); 
   
   //ST46
   G4ThreeVector pos48 = G4ThreeVector(5*cm,-18.2*cm,0.*cm);
   G4RotationMatrix* rot36 = new G4RotationMatrix();
   rot36->rotateZ(170*deg);
   auto ST48 = new G4Box("ST48", 0.5 * ST35_x, 0.5 * ST35_y, 0.5 * ST35_z);
   auto ST48Log = new G4LogicalVolume(ST48, Vacuum, "ST48");
   auto physST48 = new G4PVPlacement(rot36,pos48, ST48Log, "ST48", logicEnv, false, 0);
   auto SurfaceVisAtt_ST48 = new G4VisAttributes(G4Colour::Blue());
   ST48Log->SetVisAttributes(SurfaceVisAtt_ST48); 
   
   //ST47
   G4ThreeVector pos49 = G4ThreeVector(0*cm,-18.2*cm,0.*cm);
   auto ST49 = new G4Box("ST49", 0.5 * ST35_x, 0.5 * ST35_y, 0.5 * ST35_z);
   auto ST49Log = new G4LogicalVolume(ST49, Vacuum, "ST49");
   auto physST49 = new G4PVPlacement(0,pos49, ST49Log, "ST49", logicEnv, false, 0);
   auto SurfaceVisAtt_ST49 = new G4VisAttributes(G4Colour::Blue());
   ST49Log->SetVisAttributes(SurfaceVisAtt_ST49); 
   
   //ST48
   G4ThreeVector pos50 = G4ThreeVector(-5*cm,-17.7*cm,0.*cm);
   G4RotationMatrix* rot37 = new G4RotationMatrix();
   rot37->rotateZ(200*deg);
   auto ST50 = new G4Box("ST50", 0.5 * ST35_x, 0.5 * ST35_y, 0.5 * ST35_z);
   auto ST50Log = new G4LogicalVolume(ST50, Vacuum, "ST50");
   auto physST50 = new G4PVPlacement(rot37,pos50, ST50Log, "ST50", logicEnv, false, 0);
   auto SurfaceVisAtt_ST50 = new G4VisAttributes(G4Colour::Blue());
   ST50Log->SetVisAttributes(SurfaceVisAtt_ST50); 
   
   //ST49
   G4ThreeVector pos51 = G4ThreeVector(-9.6*cm,-16.3*cm,0.*cm);
   G4RotationMatrix* rot38 = new G4RotationMatrix();
   rot38->rotateZ(210*deg);
   auto ST51 = new G4Box("ST51", 0.5 * ST35_x, 0.5 * ST35_y, 0.5 * ST35_z);
   auto ST51Log = new G4LogicalVolume(ST51, Vacuum, "ST51");
   auto physST51 = new G4PVPlacement(rot38,pos51, ST51Log, "ST51", logicEnv, false, 0);
   auto SurfaceVisAtt_ST51 = new G4VisAttributes(G4Colour::Blue());
   ST51Log->SetVisAttributes(SurfaceVisAtt_ST51); 
   
   //ST50
   G4ThreeVector pos52 = G4ThreeVector(-13.8*cm,-13.5*cm,0.*cm);
   G4RotationMatrix* rot39 = new G4RotationMatrix();
   rot39->rotateZ(225*deg);
   auto ST52 = new G4Box("ST52", 0.5 * ST35_x, 0.5 * ST35_y, 0.5 * ST_z);
   auto ST52Log = new G4LogicalVolume(ST52, Vacuum, "ST52");
   auto physST52 = new G4PVPlacement(rot39,pos52, ST52Log, "ST52", logicEnv, false, 0);
   auto SurfaceVisAtt_ST52 = new G4VisAttributes(G4Colour::Blue());
   ST52Log->SetVisAttributes(SurfaceVisAtt_ST52); 
   
   //ST51
   G4ThreeVector pos53 = G4ThreeVector(-16.8*cm,-9.7*cm,0.*cm);
   G4RotationMatrix* rot40 = new G4RotationMatrix();
   rot40->rotateZ(240*deg);
   auto ST53 = new G4Box("ST53", 0.5 * ST35_x, 0.5 * ST35_y, 0.5 * ST35_z);
   auto ST53Log = new G4LogicalVolume(ST53, Vacuum, "ST53");
   auto physST53 = new G4PVPlacement(rot40,pos53, ST53Log, "ST53", logicEnv, false, 0);
   auto SurfaceVisAtt_ST53 = new G4VisAttributes(G4Colour::Blue());
   ST53Log->SetVisAttributes(SurfaceVisAtt_ST53); 
   
   //ST52
   G4ThreeVector pos54 = G4ThreeVector(-18.*cm,-4.5*cm,0.*cm);
   G4RotationMatrix* rot41 = new G4RotationMatrix();
   rot41->rotateZ(270*deg);
   auto ST54 = new G4Box("ST54", 0.5 * ST35_x, 0.5 * ST35_y, 0.5 * ST35_z);
   auto ST54Log = new G4LogicalVolume(ST54, Vacuum, "ST54");
   auto physST54 = new G4PVPlacement(rot41,pos54, ST54Log, "ST54", logicEnv, false, 0);
   auto SurfaceVisAtt_ST54 = new G4VisAttributes(G4Colour::Blue());
   ST54Log->SetVisAttributes(SurfaceVisAtt_ST54); 
   
   //ST53
   G4ThreeVector pos55 = G4ThreeVector(-18.2*cm,0.*cm,0.*cm);
   auto ST55 = new G4Box("ST55", 0.5 * ST35_x, 0.5 * ST35_y, 0.5 * ST35_z);
   auto ST55Log = new G4LogicalVolume(ST55, Vacuum, "ST55");
   auto physST55 = new G4PVPlacement(rot41,pos55, ST55Log, "ST55", logicEnv, false, 0);
   auto SurfaceVisAtt_ST55 = new G4VisAttributes(G4Colour::Blue());
   ST55Log->SetVisAttributes(SurfaceVisAtt_ST55); 
   
   //ST54
   G4ThreeVector pos56 = G4ThreeVector(-18.*cm,4.5*cm,0.*cm);
   auto ST56 = new G4Box("ST56", 0.5 * ST35_x, 0.5 * ST35_y, 0.5 * ST35_z);
   auto ST56Log = new G4LogicalVolume(ST56, Vacuum, "ST56");
   auto physST56 = new G4PVPlacement(rot41,pos56, ST56Log, "ST56", logicEnv, false, 0);
   auto SurfaceVisAtt_ST56 = new G4VisAttributes(G4Colour::Blue());
   ST56Log->SetVisAttributes(SurfaceVisAtt_ST56); 
   
   //ST55
   G4ThreeVector pos57 = G4ThreeVector(-16.8*cm,9.5*cm,0.*cm);
   G4RotationMatrix* rot42 = new G4RotationMatrix();
   rot42->rotateZ(300*deg);
   auto ST57 = new G4Box("ST57", 0.5 * ST35_x, 0.5 * ST35_y, 0.5 * ST35_z);
   auto ST57Log = new G4LogicalVolume(ST57, Vacuum, "ST57");
   auto physST57 = new G4PVPlacement(rot42,pos57, ST57Log, "ST57", logicEnv, false, 0);
   auto SurfaceVisAtt_ST57 = new G4VisAttributes(G4Colour::Blue());
   ST57Log->SetVisAttributes(SurfaceVisAtt_ST57); 
   
   //ST56
   G4ThreeVector pos58 = G4ThreeVector(-13.8*cm,13.3*cm,0.*cm);
   G4RotationMatrix* rot43 = new G4RotationMatrix();
   rot43->rotateZ(320*deg);
   auto ST58 = new G4Box("ST58", 0.5 * ST35_x, 0.5 * ST35_y, 0.5 * ST35_z);
   auto ST58Log = new G4LogicalVolume(ST58, Vacuum, "ST58");
   auto physST58 = new G4PVPlacement(rot43,pos58, ST58Log, "ST58", logicEnv, false, 0);
   auto SurfaceVisAtt_ST58 = new G4VisAttributes(G4Colour::Blue());
   ST58Log->SetVisAttributes(SurfaceVisAtt_ST58); 
   
   //ST57
   G4ThreeVector pos59 = G4ThreeVector(-9.7*cm,17.*cm,0.*cm);
   G4RotationMatrix* rot44 = new G4RotationMatrix();
   rot44->rotateZ(340*deg);
   auto ST59 = new G4Box("ST59", 0.5 * ST35_x, 0.5 * ST35_y, 0.5 * ST35_z);
   auto ST59Log = new G4LogicalVolume(ST59, Vacuum, "ST59");
   auto physST59 = new G4PVPlacement(rot44,pos59, ST59Log, "ST59", logicEnv, false, 0);
   auto SurfaceVisAtt_ST59 = new G4VisAttributes(G4Colour::Blue());
   ST59Log->SetVisAttributes(SurfaceVisAtt_ST59); 
   
   //ST58
   G4ThreeVector pos60 = G4ThreeVector(-4.5*cm,18.*cm,0.*cm);
   auto ST60 = new G4Box("ST60", 0.5 * ST35_x, 0.5 * ST35_y, 0.5 * ST_z);
   auto ST60Log = new G4LogicalVolume(ST60, Vacuum, "ST60");
   auto physST60 = new G4PVPlacement(0,pos60, ST60Log, "ST60", logicEnv, false, 0);
   auto SurfaceVisAtt_ST60 = new G4VisAttributes(G4Colour::Blue());
   ST60Log->SetVisAttributes(SurfaceVisAtt_ST60); 
   
   //ST59
   G4ThreeVector pos61 = G4ThreeVector(-1.*cm,21.*cm,0.*cm);
   G4double ST61_x = 6.5*cm;
   G4double ST61_y = 2.0*cm;
   auto ST61 = new G4Box("ST61", 0.5 * ST61_x, 0.5 * ST61_y, 0.5 * ST_z);
   auto ST61Log = new G4LogicalVolume(ST61, Vacuum, "ST61");
   auto physST61 = new G4PVPlacement(0,pos61, ST61Log, "ST61", logicEnv, false, 0);
   auto SurfaceVisAtt_ST61 = new G4VisAttributes(G4Colour::Blue());
   ST61Log->SetVisAttributes(SurfaceVisAtt_ST61); 
   
   //ST60
   G4ThreeVector pos62 = G4ThreeVector(7.*cm,19.6*cm,0.*cm);
   auto ST62 = new G4Box("ST62", 0.5 * ST61_x, 0.5 * ST61_y, 0.5 * ST_z);
   auto ST62Log = new G4LogicalVolume(ST62, Vacuum, "ST62");
   auto physST62 = new G4PVPlacement(rot13,pos62, ST62Log, "ST62", logicEnv, false, 0);
   auto SurfaceVisAtt_ST62 = new G4VisAttributes(G4Colour::Blue());
   ST62Log->SetVisAttributes(SurfaceVisAtt_ST62); 
   
   //ST61
   G4ThreeVector pos63 = G4ThreeVector(13.3*cm,16.8*cm,0.*cm);
   auto ST63 = new G4Box("ST63", 0.5 * ST61_x, 0.5 * ST61_y, 0.5 * ST_z);
   auto ST63Log = new G4LogicalVolume(ST63, Vacuum, "ST63");
   auto physST63 = new G4PVPlacement(rot29,pos63, ST63Log, "ST63", logicEnv, false, 0);
   auto SurfaceVisAtt_ST63 = new G4VisAttributes(G4Colour::Blue());
   ST63Log->SetVisAttributes(SurfaceVisAtt_ST63); 
   
   //ST62
   G4ThreeVector pos64 = G4ThreeVector(18.6*cm,12.4*cm,0.*cm);
   G4RotationMatrix* rot45 = new G4RotationMatrix();
   rot45->rotateZ(50*deg);
   auto ST64 = new G4Box("ST64", 0.5 * ST61_x, 0.5 * ST61_y, 0.5 * ST_z);
   auto ST64Log = new G4LogicalVolume(ST64, Vacuum, "ST64");
   auto physST64 = new G4PVPlacement(rot45,pos64, ST64Log, "ST64", logicEnv, false, 0);
   auto SurfaceVisAtt_ST64 = new G4VisAttributes(G4Colour::Blue());
   ST64Log->SetVisAttributes(SurfaceVisAtt_ST64); 
   
   //ST63
   G4ThreeVector pos65 = G4ThreeVector(21.*cm,5.5*cm,0.*cm);
   auto ST65 = new G4Box("ST65", 0.5 * ST61_x, 0.5 * ST61_y, 0.5 * ST_z);
   auto ST65Log = new G4LogicalVolume(ST65, Vacuum, "ST65");
   auto physST65 = new G4PVPlacement(rott,pos65, ST65Log, "ST65", logicEnv, false, 0);
   auto SurfaceVisAtt_ST65 = new G4VisAttributes(G4Colour::Blue());
   ST65Log->SetVisAttributes(SurfaceVisAtt_ST65); 
   
   //ST64
   G4ThreeVector pos66 = G4ThreeVector(21.*cm,-1.*cm,0.*cm);
   auto ST66 = new G4Box("ST66", 0.5 * ST61_x, 0.5 * ST61_y, 0.5 * ST_z);
   auto ST66Log = new G4LogicalVolume(ST66, Vacuum, "ST66");
   auto physST66 = new G4PVPlacement(rott,pos66, ST66Log, "ST66", logicEnv, false, 0);
   auto SurfaceVisAtt_ST66 = new G4VisAttributes(G4Colour::Blue());
   ST66Log->SetVisAttributes(SurfaceVisAtt_ST66); 
   
   //ST65
   G4ThreeVector pos67 = G4ThreeVector(21.*cm,-7.5*cm,0.*cm);
   auto ST67 = new G4Box("ST67", 0.5 * ST61_x, 0.5 * ST61_y, 0.5 * ST_z);
   auto ST67Log = new G4LogicalVolume(ST67, Vacuum, "ST67");
   auto physST67 = new G4PVPlacement(rott,pos67, ST67Log, "ST67", logicEnv, false, 0);
   auto SurfaceVisAtt_ST67 = new G4VisAttributes(G4Colour::Blue());
   ST67Log->SetVisAttributes(SurfaceVisAtt_ST67); 
   
   //ST66
   G4ThreeVector pos68 = G4ThreeVector(17.6*cm,-13.4*cm,0.*cm);
   G4RotationMatrix* rot46 = new G4RotationMatrix();
   rot46->rotateZ(130*deg);
   auto ST68 = new G4Box("ST68", 0.5 * ST61_x, 0.5 * ST61_y, 0.5 * ST_z);
   auto ST68Log = new G4LogicalVolume(ST68, Vacuum, "ST68");
   auto physST68 = new G4PVPlacement(rot46,pos68, ST68Log, "ST68", logicEnv, false, 0);
   auto SurfaceVisAtt_ST68 = new G4VisAttributes(G4Colour::Blue());
   ST68Log->SetVisAttributes(SurfaceVisAtt_ST68); 
   
   //ST67
   G4ThreeVector pos69 = G4ThreeVector(12.2*cm,-17.8*cm,0.*cm);
   G4RotationMatrix* rot47 = new G4RotationMatrix();
   rot47->rotateZ(150*deg);
   auto ST69 = new G4Box("ST69", 0.5 * ST61_x, 0.5 * ST61_y, 0.5 * ST_z);
   auto ST69Log = new G4LogicalVolume(ST69, Vacuum, "ST69");
   auto physST69 = new G4PVPlacement(rot47,pos69, ST69Log, "ST69", logicEnv, false, 0);
   auto SurfaceVisAtt_ST69 = new G4VisAttributes(G4Colour::Blue());
   ST69Log->SetVisAttributes(SurfaceVisAtt_ST69); 
   
   //ST68
   G4ThreeVector pos70 = G4ThreeVector(5.7*cm,-20.8*cm,0.*cm);
   G4RotationMatrix* rot48 = new G4RotationMatrix();
   rot48->rotateZ(170*deg);
   auto ST70 = new G4Box("ST70", 0.5 * ST61_x, 0.5 * ST61_y, 0.5 * ST_z);
   auto ST70Log = new G4LogicalVolume(ST70, Vacuum, "ST70");
   auto physST70 = new G4PVPlacement(rot48,pos70, ST70Log, "ST70", logicEnv, false, 0);
   auto SurfaceVisAtt_ST70 = new G4VisAttributes(G4Colour::Blue());
   ST70Log->SetVisAttributes(SurfaceVisAtt_ST70); 
   
   //ST69
   G4ThreeVector pos71 = G4ThreeVector(-1.2*cm,-20.8*cm,0.*cm);
   auto ST71 = new G4Box("ST71", 0.5 * ST61_x, 0.5 * ST61_y, 0.5 * ST_z);
   auto ST71Log = new G4LogicalVolume(ST71, Vacuum, "ST71");
   auto physST71 = new G4PVPlacement(0,pos71, ST71Log, "ST71", logicEnv, false, 0);
   auto SurfaceVisAtt_ST71 = new G4VisAttributes(G4Colour::Blue());
   ST71Log->SetVisAttributes(SurfaceVisAtt_ST71); 
   
   //ST70
   G4ThreeVector pos72 = G4ThreeVector(-8.0*cm,-20.1*cm,0.*cm);
   auto ST72 = new G4Box("ST72", 0.5 * ST61_x, 0.5 * ST61_y, 0.5 * ST_z);
   auto ST72Log = new G4LogicalVolume(ST72, Vacuum, "ST72");
   auto physST72 = new G4PVPlacement(rot21,pos72, ST72Log, "ST72", logicEnv, false, 0);
   auto SurfaceVisAtt_ST72 = new G4VisAttributes(G4Colour::Blue());
   ST72Log->SetVisAttributes(SurfaceVisAtt_ST72); 
   
   //ST71
   G4ThreeVector pos73 = G4ThreeVector(-14.5*cm,-16.6*cm,0.*cm);
   auto ST73 = new G4Box("ST73", 0.5 * ST61_x, 0.5 * ST61_y, 0.5 * ST_z);
   auto ST73Log = new G4LogicalVolume(ST73, Vacuum, "ST73");
   auto physST73 = new G4PVPlacement(rot22,pos73, ST73Log, "ST73", logicEnv, false, 0);
   auto SurfaceVisAtt_ST73 = new G4VisAttributes(G4Colour::Blue());
   ST73Log->SetVisAttributes(SurfaceVisAtt_ST73); 
   
   //ST72
   G4ThreeVector pos74 = G4ThreeVector(-19.3*cm,-11.0*cm,0.*cm);
   auto ST74 = new G4Box("ST74", 0.5 * ST61_x, 0.5 * ST61_y, 0.5 * ST_z);
   auto ST74Log = new G4LogicalVolume(ST74, Vacuum, "ST74");
   auto physST74 = new G4PVPlacement(rot23,pos74, ST74Log, "ST74", logicEnv, false, 0);
   auto SurfaceVisAtt_ST74 = new G4VisAttributes(G4Colour::Blue());
   ST74Log->SetVisAttributes(SurfaceVisAtt_ST74); 
   
   //ST73
   G4ThreeVector pos75 = G4ThreeVector(-21.*cm,-4.*cm,0.*cm);
   auto ST75 = new G4Box("ST75", 0.5 * ST61_x, 0.5 * ST61_y, 0.5 * ST_z);
   auto ST75Log = new G4LogicalVolume(ST75, Vacuum, "ST75");
   auto physST75 = new G4PVPlacement(rottt,pos75, ST75Log, "ST75", logicEnv, false, 0);
   auto SurfaceVisAtt_ST75 = new G4VisAttributes(G4Colour::Blue());
   ST75Log->SetVisAttributes(SurfaceVisAtt_ST75); 
   
   //ST74
   G4ThreeVector pos76 = G4ThreeVector(-21.*cm,2.5*cm,0.*cm);
   auto ST76 = new G4Box("ST76", 0.5 * ST61_x, 0.5 * ST61_y, 0.5 * ST_z);
   auto ST76Log = new G4LogicalVolume(ST76, Vacuum, "ST76");
   auto physST76 = new G4PVPlacement(rottt,pos76, ST76Log, "ST76", logicEnv, false, 0);
   auto SurfaceVisAtt_ST76 = new G4VisAttributes(G4Colour::Blue());
   ST76Log->SetVisAttributes(SurfaceVisAtt_ST76); 
   
   //ST75
   G4ThreeVector pos77 = G4ThreeVector(-19.5*cm,9.5*cm,0.*cm);
   auto ST77 = new G4Box("ST77", 0.5 * ST61_x, 0.5 * ST61_y, 0.5 * ST_z);
   auto ST77Log = new G4LogicalVolume(ST77, Vacuum, "ST77");
   auto physST77 = new G4PVPlacement(rot42,pos77, ST77Log, "ST77", logicEnv, false, 0);
   auto SurfaceVisAtt_ST77 = new G4VisAttributes(G4Colour::Blue());
   ST77Log->SetVisAttributes(SurfaceVisAtt_ST77); 
   
   //ST76
   G4ThreeVector pos78 = G4ThreeVector(-15.4*cm,15.2*cm,0.*cm);
   auto ST78 = new G4Box("ST78", 0.5 * ST61_x, 0.5 * ST61_y, 0.5 * ST_z);
   auto ST78Log = new G4LogicalVolume(ST78, Vacuum, "ST78");
   auto physST78 = new G4PVPlacement(rot43,pos78, ST78Log, "ST78", logicEnv, false, 0);
   auto SurfaceVisAtt_ST78 = new G4VisAttributes(G4Colour::Blue());
   ST78Log->SetVisAttributes(SurfaceVisAtt_ST78); 
   
   //ST77
   G4ThreeVector pos79 = G4ThreeVector(-9.5*cm,19.8*cm,0.*cm);
   auto ST79 = new G4Box("ST79", 0.5 * ST61_x, 0.5 * ST61_y, 0.5 * ST_z);
   auto ST79Log = new G4LogicalVolume(ST79, Vacuum, "ST79");
   auto physST79 = new G4PVPlacement(rot27,pos79, ST79Log, "ST79", logicEnv, false, 0);
   auto SurfaceVisAtt_ST79 = new G4VisAttributes(G4Colour::Blue());
   ST79Log->SetVisAttributes(SurfaceVisAtt_ST79); 
   
   //ST78
   G4ThreeVector pos80 = G4ThreeVector(0.*cm,24.2*cm,0.*cm);
   G4double ST80_x = 12.*cm;
   G4double ST80_y = 3.*cm;
   auto ST80 = new G4Box("ST80", 0.5 * ST80_x, 0.5 * ST80_y, 0.5 * ST_z);
   auto ST80Log = new G4LogicalVolume(ST80, Vacuum, "ST80");
   auto physST80 = new G4PVPlacement(0,pos80, ST80Log, "ST80", logicEnv, false, 0);
   auto SurfaceVisAtt_ST80 = new G4VisAttributes(G4Colour::Blue());
   ST80Log->SetVisAttributes(SurfaceVisAtt_ST80); 
   
   //ST79
   G4ThreeVector pos81 = G4ThreeVector(12.5*cm,20.5*cm,0.*cm);
   auto ST81 = new G4Box("ST81", 0.5 * ST80_x, 0.5 * ST80_y, 0.5 * ST_z);
   auto ST81Log = new G4LogicalVolume(ST81, Vacuum, "ST81");
   auto physST81 = new G4PVPlacement(rot29,pos81, ST81Log, "ST81", logicEnv, false, 0);
   auto SurfaceVisAtt_ST81 = new G4VisAttributes(G4Colour::Blue());
   ST81Log->SetVisAttributes(SurfaceVisAtt_ST81); 
   
   //ST80
   G4ThreeVector pos82 = G4ThreeVector(22.5*cm,13.*cm,0.*cm);
   auto ST82 = new G4Box("ST82", 0.5 * ST80_x, 0.5 * ST80_y, 0.5 * ST_z);
   auto ST82Log = new G4LogicalVolume(ST82, Vacuum, "ST82");
   auto physST82 = new G4PVPlacement(rot15,pos82, ST82Log, "ST82", logicEnv, false, 0);
   auto SurfaceVisAtt_ST82 = new G4VisAttributes(G4Colour::Blue());
   ST82Log->SetVisAttributes(SurfaceVisAtt_ST82); 
   
   //ST81
   G4ThreeVector pos83 = G4ThreeVector(24.3*cm,-0.5*cm,0.*cm);
   auto ST83 = new G4Box("ST83", 0.5 * ST80_x, 0.5 * ST80_y, 0.5 * ST_z);
   auto ST83Log = new G4LogicalVolume(ST83, Vacuum, "ST83");
   auto physST83 = new G4PVPlacement(rott,pos83, ST83Log, "ST83", logicEnv, false, 0);
   auto SurfaceVisAtt_ST83 = new G4VisAttributes(G4Colour::Blue());
   ST83Log->SetVisAttributes(SurfaceVisAtt_ST83); 
   
   //ST82
   G4ThreeVector pos84 = G4ThreeVector(20.8*cm,-14.7*cm,0.*cm);
   auto ST84 = new G4Box("ST84", 0.5 * ST80_x, 0.5 * ST80_y, 0.5 * ST_z);
   auto ST84Log = new G4LogicalVolume(ST84, Vacuum, "ST84");
   auto physST84 = new G4PVPlacement(rot46,pos84, ST84Log, "ST84", logicEnv, false, 0);
   auto SurfaceVisAtt_ST84 = new G4VisAttributes(G4Colour::Blue());
   ST84Log->SetVisAttributes(SurfaceVisAtt_ST84); 
   
   //ST83
   G4ThreeVector pos85 = G4ThreeVector(10.5*cm,-22.3*cm,0.*cm);
   auto ST85 = new G4Box("ST85", 0.5 * ST80_x, 0.5 * ST80_y, 0.5 * ST_z);
   auto ST85Log = new G4LogicalVolume(ST85, Vacuum, "ST85");
   auto physST85 = new G4PVPlacement(rot47,pos85, ST85Log, "ST85", logicEnv, false, 0);
   auto SurfaceVisAtt_ST85 = new G4VisAttributes(G4Colour::Blue());
   ST85Log->SetVisAttributes(SurfaceVisAtt_ST85); 
   
   //ST84
   G4ThreeVector pos86 = G4ThreeVector(-2.*cm,-24.2*cm,0.*cm);
   auto ST86 = new G4Box("ST86", 0.5 * ST80_x, 0.5 * ST80_y, 0.5 * ST_z);
   auto ST86Log = new G4LogicalVolume(ST86, Vacuum, "ST86");
   auto physST86 = new G4PVPlacement(0,pos86, ST86Log, "ST86", logicEnv, false, 0);
   auto SurfaceVisAtt_ST86 = new G4VisAttributes(G4Colour::Blue());
   ST86Log->SetVisAttributes(SurfaceVisAtt_ST86); 
   
   //ST85
   G4ThreeVector pos87 = G4ThreeVector(-14.*cm,-21.*cm,0.*cm);
   auto ST87 = new G4Box("ST87", 0.5 * ST80_x, 0.5 * ST80_y, 0.5 * ST_z);
   auto ST87Log = new G4LogicalVolume(ST87, Vacuum, "ST87");
   auto physST87 = new G4PVPlacement(rot38,pos87, ST87Log, "ST87", logicEnv, false, 0);
   auto SurfaceVisAtt_ST87 = new G4VisAttributes(G4Colour::Blue());
   ST87Log->SetVisAttributes(SurfaceVisAtt_ST87); 
   
   //ST86
   G4ThreeVector pos100 = G4ThreeVector(-22.5*cm,-11.8*cm,0.*cm);
   auto ST88 = new G4Box("ST88", 0.5 * ST80_x, 0.5 * ST80_y, 0.5 * ST_z);
   auto ST88Log = new G4LogicalVolume(ST88, Vacuum, "ST88");
   auto physST88 = new G4PVPlacement(rot23,pos100, ST88Log, "ST88", logicEnv, false, 0);
   auto SurfaceVisAtt_ST88 = new G4VisAttributes(G4Colour::Blue());
   ST88Log->SetVisAttributes(SurfaceVisAtt_ST88); 
   
   //ST87
   G4ThreeVector pos101 = G4ThreeVector(-24.2*cm, 0.3*cm,0.*cm);
   auto ST89 = new G4Box("ST89", 0.5 * ST80_x, 0.5 * ST80_y, 0.5 * ST_z);
   auto ST89Log = new G4LogicalVolume(ST89, Vacuum, "ST89");
   auto physST89 = new G4PVPlacement(rottt,pos101, ST89Log, "ST89", logicEnv, false, 0);
   auto SurfaceVisAtt_ST89 = new G4VisAttributes(G4Colour::Blue());
   ST89Log->SetVisAttributes(SurfaceVisAtt_ST89); 
   
   //ST88
   G4ThreeVector pos102 = G4ThreeVector(-21.2*cm, 12.8*cm,0.*cm);
   auto ST90 = new G4Box("ST90", 0.5 * ST80_x, 0.5 * ST80_y, 0.5 * ST_z);
   auto ST90Log = new G4LogicalVolume(ST90, Vacuum, "ST90");
   auto physST90 = new G4PVPlacement(rot42,pos102, ST90Log, "ST90", logicEnv, false, 0);
   auto SurfaceVisAtt_ST90 = new G4VisAttributes(G4Colour::Blue());
   ST90Log->SetVisAttributes(SurfaceVisAtt_ST90); 
   
   //ST89
   G4ThreeVector pos103 = G4ThreeVector(-12.7*cm, 22.*cm,0.*cm);
   G4RotationMatrix* rot49 = new G4RotationMatrix();
   rot49->rotateZ(330*deg);
   auto ST91 = new G4Box("ST91", 0.5 * ST80_x, 0.5 * ST80_y, 0.5 * ST_z);
   auto ST91Log = new G4LogicalVolume(ST91, Vacuum, "ST91");
   auto physST91 = new G4PVPlacement(rot49,pos103, ST91Log, "ST91", logicEnv, false, 0);
   auto SurfaceVisAtt_ST91 = new G4VisAttributes(G4Colour::Blue());
   ST91Log->SetVisAttributes(SurfaceVisAtt_ST91); 
   
   //ST90
   G4ThreeVector pos104 = G4ThreeVector(0.*cm, 30.*cm,0.*cm);
   G4double ST90_x = 15.*cm;
   G4double ST90_y = 3.*cm;
   auto ST92 = new G4Box("ST92", 0.5 * ST90_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST92Log = new G4LogicalVolume(ST92, Vacuum, "ST92");
   auto physST92 = new G4PVPlacement(0,pos104, ST92Log, "ST92", logicEnv, false, 0);
   auto SurfaceVisAtt_ST92 = new G4VisAttributes(G4Colour::Blue());
   ST92Log->SetVisAttributes(SurfaceVisAtt_ST92); 
   
   //ST91
   G4ThreeVector pos105 = G4ThreeVector(15.*cm, 26.*cm,0.*cm);
   auto ST93 = new G4Box("ST93", 0.5 * ST90_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST93Log = new G4LogicalVolume(ST93, Vacuum, "ST93");
   auto physST93 = new G4PVPlacement(rot29,pos105, ST93Log, "ST93", logicEnv, false, 0);
   auto SurfaceVisAtt_ST93 = new G4VisAttributes(G4Colour::Blue());
   ST93Log->SetVisAttributes(SurfaceVisAtt_ST93); 
   
   //ST92
   G4ThreeVector pos106 = G4ThreeVector(26.5*cm, 15.*cm,0.*cm);
   auto ST94 = new G4Box("ST94", 0.5 * ST90_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST94Log = new G4LogicalVolume(ST94, Vacuum, "ST94");
   auto physST94 = new G4PVPlacement(rot15,pos106, ST94Log, "ST94", logicEnv, false, 0);
   auto SurfaceVisAtt_ST94 = new G4VisAttributes(G4Colour::Blue());
   ST94Log->SetVisAttributes(SurfaceVisAtt_ST94); 
   
   //ST93
   G4ThreeVector pos107 = G4ThreeVector(30.*cm, 0.*cm,0.*cm);
   auto ST95 = new G4Box("ST95", 0.5 * ST90_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST95Log = new G4LogicalVolume(ST95, Vacuum, "ST95");
   auto physST95 = new G4PVPlacement(rott,pos107, ST95Log, "ST95", logicEnv, false, 0);
   auto SurfaceVisAtt_ST95 = new G4VisAttributes(G4Colour::Blue());
   ST95Log->SetVisAttributes(SurfaceVisAtt_ST95); 
   
   //ST94
   G4ThreeVector pos108 = G4ThreeVector(26.5*cm, -15.*cm,0.*cm);
   auto ST96 = new G4Box("ST96", 0.5 * ST90_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST96Log = new G4LogicalVolume(ST96, Vacuum, "ST96");
   auto physST96 = new G4PVPlacement(rot33,pos108, ST96Log, "ST96", logicEnv, false, 0);
   auto SurfaceVisAtt_ST96 = new G4VisAttributes(G4Colour::Blue());
   ST96Log->SetVisAttributes(SurfaceVisAtt_ST96); 
   
   //ST95
   G4ThreeVector pos109 = G4ThreeVector(15.*cm, -26.*cm,0.*cm);
   auto ST97 = new G4Box("ST97", 0.5 * ST90_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST97Log = new G4LogicalVolume(ST97, Vacuum, "ST97");
   auto physST97 = new G4PVPlacement(rot47,pos109, ST97Log, "ST97", logicEnv, false, 0);
   auto SurfaceVisAtt_ST97 = new G4VisAttributes(G4Colour::Blue());
   ST97Log->SetVisAttributes(SurfaceVisAtt_ST97); 
   
   //ST96
   G4ThreeVector pos110 = G4ThreeVector(0.*cm, -30.*cm,0.*cm);
   auto ST98 = new G4Box("ST98", 0.5 * ST90_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST98Log = new G4LogicalVolume(ST98, Vacuum, "ST98");
   auto physST98 = new G4PVPlacement(0,pos110, ST98Log, "ST98", logicEnv, false, 0);
   auto SurfaceVisAtt_ST98 = new G4VisAttributes(G4Colour::Blue());
   ST98Log->SetVisAttributes(SurfaceVisAtt_ST98); 
   
   //ST97
   G4ThreeVector pos111 = G4ThreeVector(-15.*cm, -26.*cm,0.*cm);
   auto ST99 = new G4Box("ST99", 0.5 * ST90_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST99Log = new G4LogicalVolume(ST99, Vacuum, "ST99");
   auto physST99 = new G4PVPlacement(rot38,pos111, ST99Log, "ST99", logicEnv, false, 0);
   auto SurfaceVisAtt_ST99 = new G4VisAttributes(G4Colour::Blue());
   ST99Log->SetVisAttributes(SurfaceVisAtt_ST99); 
   
   //ST98
   G4ThreeVector pos112 = G4ThreeVector(-26.5*cm, -15.*cm,0.*cm);
   auto ST100 = new G4Box("ST100", 0.5 * ST90_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST100Log = new G4LogicalVolume(ST100, Vacuum, "ST100");
   auto physST100 = new G4PVPlacement(rot23,pos112, ST100Log, "ST100", logicEnv, false, 0);
   auto SurfaceVisAtt_ST100 = new G4VisAttributes(G4Colour::Blue());
   ST100Log->SetVisAttributes(SurfaceVisAtt_ST100); 
   
   //ST99
   G4ThreeVector pos113 = G4ThreeVector(-30.*cm, 0.*cm,0.*cm);
   auto ST101 = new G4Box("ST101", 0.5 * ST90_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST101Log = new G4LogicalVolume(ST101, Vacuum, "ST101");
   auto physST101 = new G4PVPlacement(rottt,pos113, ST101Log, "ST101", logicEnv, false, 0);
   auto SurfaceVisAtt_ST101 = new G4VisAttributes(G4Colour::Blue());
   ST101Log->SetVisAttributes(SurfaceVisAtt_ST101); 
   
   //ST100
   G4ThreeVector pos114 = G4ThreeVector(-26.5*cm, 15.*cm,0.*cm);
   auto ST102 = new G4Box("ST102", 0.5 * ST90_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST102Log = new G4LogicalVolume(ST102, Vacuum, "ST102");
   auto physST102 = new G4PVPlacement(rot42,pos114, ST102Log, "ST102", logicEnv, false, 0);
   auto SurfaceVisAtt_ST102 = new G4VisAttributes(G4Colour::Blue());
   ST102Log->SetVisAttributes(SurfaceVisAtt_ST102); 
   
   //ST101
   G4ThreeVector pos115 = G4ThreeVector(-15.*cm, 26.*cm,0.*cm);
   auto ST103 = new G4Box("ST103", 0.5 * ST90_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST103Log = new G4LogicalVolume(ST103, Vacuum, "ST103");
   auto physST103 = new G4PVPlacement(rot49,pos115, ST103Log, "ST103", logicEnv, false, 0);
   auto SurfaceVisAtt_ST103 = new G4VisAttributes(G4Colour::Blue());
   ST103Log->SetVisAttributes(SurfaceVisAtt_ST103); 
   
   //ST102
   G4ThreeVector pos116 = G4ThreeVector(0.*cm, 36.*cm,0.*cm);
   G4double ST100_x = 18.*cm;
   auto ST104 = new G4Box("ST104", 0.5 * ST100_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST104Log = new G4LogicalVolume(ST104, Vacuum, "ST104");
   auto physST104 = new G4PVPlacement(0,pos116, ST104Log, "ST104", logicEnv, false, 0);
   auto SurfaceVisAtt_ST104 = new G4VisAttributes(G4Colour::Blue());
   ST104Log->SetVisAttributes(SurfaceVisAtt_ST104); 
   
   //ST103
   G4ThreeVector pos117 = G4ThreeVector(18.*cm, 31.*cm,0.*cm);
   auto ST105 = new G4Box("ST105", 0.5 * ST100_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST105Log = new G4LogicalVolume(ST105, Vacuum, "ST105");
   auto physST105 = new G4PVPlacement(rot29,pos117, ST105Log, "ST105", logicEnv, false, 0);
   auto SurfaceVisAtt_ST105 = new G4VisAttributes(G4Colour::Blue());
   ST105Log->SetVisAttributes(SurfaceVisAtt_ST105); 
   
   //ST104
   G4ThreeVector pos118 = G4ThreeVector(31.*cm, 18.*cm,0.*cm);
   auto ST106 = new G4Box("ST106", 0.5 * ST100_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST106Log = new G4LogicalVolume(ST106, Vacuum, "ST106");
   auto physST106 = new G4PVPlacement(rot15,pos118, ST106Log, "ST106", logicEnv, false, 0);
   auto SurfaceVisAtt_ST106 = new G4VisAttributes(G4Colour::Blue());
   ST106Log->SetVisAttributes(SurfaceVisAtt_ST106); 
   
   //ST105
   G4ThreeVector pos119 = G4ThreeVector(36.*cm, 0.*cm,0.*cm);
   auto ST107 = new G4Box("ST107", 0.5 * ST100_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST107Log = new G4LogicalVolume(ST107, Vacuum, "ST107");
   auto physST107 = new G4PVPlacement(rott,pos119, ST107Log, "ST107", logicEnv, false, 0);
   auto SurfaceVisAtt_ST107 = new G4VisAttributes(G4Colour::Blue());
   ST107Log->SetVisAttributes(SurfaceVisAtt_ST107); 
   
   //ST106
   G4ThreeVector pos120 = G4ThreeVector(31.*cm, -18.*cm,0.*cm);
   auto ST108 = new G4Box("ST108", 0.5 * ST100_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST108Log = new G4LogicalVolume(ST108, Vacuum, "ST108");
   auto physST108 = new G4PVPlacement(rot33,pos120, ST108Log, "ST108", logicEnv, false, 0);
   auto SurfaceVisAtt_ST108 = new G4VisAttributes(G4Colour::Blue());
   ST108Log->SetVisAttributes(SurfaceVisAtt_ST108); 
   
   //ST107
   G4ThreeVector pos121 = G4ThreeVector(18.*cm, -31.*cm,0.*cm);
   auto ST109 = new G4Box("ST109", 0.5 * ST100_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST109Log = new G4LogicalVolume(ST109, Vacuum, "ST109");
   auto physST109 = new G4PVPlacement(rot47,pos121, ST109Log, "ST109", logicEnv, false, 0);
   auto SurfaceVisAtt_ST109 = new G4VisAttributes(G4Colour::Blue());
   ST109Log->SetVisAttributes(SurfaceVisAtt_ST109); 
   
   //ST108
   G4ThreeVector pos122 = G4ThreeVector(0.*cm, -36.*cm,0.*cm);
   auto ST110 = new G4Box("ST110", 0.5 * ST100_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST110Log = new G4LogicalVolume(ST110, Vacuum, "ST110");
   auto physST110 = new G4PVPlacement(0,pos122, ST110Log, "ST110", logicEnv, false, 0);
   auto SurfaceVisAtt_ST110 = new G4VisAttributes(G4Colour::Blue());
   ST110Log->SetVisAttributes(SurfaceVisAtt_ST110); 
   
   //ST109
   G4ThreeVector pos123 = G4ThreeVector(-18.*cm, -31.*cm,0.*cm);
   auto ST111 = new G4Box("ST111", 0.5 * ST100_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST111Log = new G4LogicalVolume(ST111, Vacuum, "ST111");
   auto physST111 = new G4PVPlacement(rot38,pos123, ST111Log, "ST111", logicEnv, false, 0);
   auto SurfaceVisAtt_ST111 = new G4VisAttributes(G4Colour::Blue());
   ST111Log->SetVisAttributes(SurfaceVisAtt_ST111); 
   
   //ST110
   G4ThreeVector pos124 = G4ThreeVector(-31.*cm, -18.*cm,0.*cm);
   auto ST112 = new G4Box("ST112", 0.5 * ST100_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST112Log = new G4LogicalVolume(ST112, Vacuum, "ST112");
   auto physST112 = new G4PVPlacement(rot23,pos124, ST112Log, "ST112", logicEnv, false, 0);
   auto SurfaceVisAtt_ST112 = new G4VisAttributes(G4Colour::Blue());
   ST112Log->SetVisAttributes(SurfaceVisAtt_ST112); 
   
   //ST111
   G4ThreeVector pos125 = G4ThreeVector(-36.*cm, 0.*cm,0.*cm);
   auto ST113 = new G4Box("ST113", 0.5 * ST100_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST113Log = new G4LogicalVolume(ST113, Vacuum, "ST113");
   auto physST113 = new G4PVPlacement(rott,pos125, ST113Log, "ST113", logicEnv, false, 0);
   auto SurfaceVisAtt_ST113 = new G4VisAttributes(G4Colour::Blue());
   ST113Log->SetVisAttributes(SurfaceVisAtt_ST113); 
   
   //ST112
   G4ThreeVector pos126 = G4ThreeVector(-31.*cm, 18.*cm,0.*cm);
   auto ST114 = new G4Box("ST114", 0.5 * ST100_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST114Log = new G4LogicalVolume(ST114, Vacuum, "ST114");
   auto physST114 = new G4PVPlacement(rot42,pos126, ST114Log, "ST114", logicEnv, false, 0);
   auto SurfaceVisAtt_ST114 = new G4VisAttributes(G4Colour::Blue());
   ST114Log->SetVisAttributes(SurfaceVisAtt_ST114); 
   
   //ST113
   G4ThreeVector pos127 = G4ThreeVector(-18.*cm, 31.*cm,0.*cm);
   auto ST115 = new G4Box("ST115", 0.5 * ST100_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST115Log = new G4LogicalVolume(ST115, Vacuum, "ST115");
   auto physST115 = new G4PVPlacement(rot49,pos127, ST115Log, "ST115", logicEnv, false, 0);
   auto SurfaceVisAtt_ST115 = new G4VisAttributes(G4Colour::Blue());
   ST115Log->SetVisAttributes(SurfaceVisAtt_ST115); 
   
   //ST114
   G4ThreeVector pos128 = G4ThreeVector(0.*cm, 42.*cm,0.*cm);
   G4double ST110_x = 21.*cm;
   auto ST116 = new G4Box("ST116", 0.5 * ST110_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST116Log = new G4LogicalVolume(ST116, Vacuum, "ST116");
   auto physST116 = new G4PVPlacement(0,pos128, ST116Log, "ST116", logicEnv, false, 0);
   auto SurfaceVisAtt_ST116 = new G4VisAttributes(G4Colour::Blue());
   ST116Log->SetVisAttributes(SurfaceVisAtt_ST116); 
   
   //ST115
   G4ThreeVector pos129 = G4ThreeVector(21.*cm, 36.*cm,0.*cm);
   auto ST117 = new G4Box("ST117", 0.5 * ST110_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST117Log = new G4LogicalVolume(ST117, Vacuum, "ST117");
   auto physST117 = new G4PVPlacement(rot29,pos129, ST117Log, "ST117", logicEnv, false, 0);
   auto SurfaceVisAtt_ST117 = new G4VisAttributes(G4Colour::Blue());
   ST117Log->SetVisAttributes(SurfaceVisAtt_ST117); 
   
   //ST116
   G4ThreeVector pos130 = G4ThreeVector(36.*cm, 21.*cm,0.*cm);
   auto ST118 = new G4Box("ST118", 0.5 * ST110_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST118Log = new G4LogicalVolume(ST118, Vacuum, "ST118");
   auto physST118 = new G4PVPlacement(rot15,pos130, ST118Log, "ST118", logicEnv, false, 0);
   auto SurfaceVisAtt_ST118 = new G4VisAttributes(G4Colour::Blue());
   ST118Log->SetVisAttributes(SurfaceVisAtt_ST118); 
   
   //ST117
   G4ThreeVector pos131 = G4ThreeVector(42.*cm, 0.*cm,0.*cm);
   auto ST119 = new G4Box("ST119", 0.5 * ST110_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST119Log = new G4LogicalVolume(ST119, Vacuum, "ST119");
   auto physST119 = new G4PVPlacement(rott,pos131, ST119Log, "ST119", logicEnv, false, 0);
   auto SurfaceVisAtt_ST119 = new G4VisAttributes(G4Colour::Blue());
   ST119Log->SetVisAttributes(SurfaceVisAtt_ST119); 
   
   //ST118
   G4ThreeVector pos132 = G4ThreeVector(36.*cm, -21.*cm,0.*cm);
   auto ST120 = new G4Box("ST120", 0.5 * ST110_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST120Log = new G4LogicalVolume(ST120, Vacuum, "ST120");
   auto physST120 = new G4PVPlacement(rot33,pos132, ST120Log, "ST120", logicEnv, false, 0);
   auto SurfaceVisAtt_ST120 = new G4VisAttributes(G4Colour::Blue());
   ST120Log->SetVisAttributes(SurfaceVisAtt_ST120); 
   
   //ST119
   G4ThreeVector pos133 = G4ThreeVector(21.*cm, -36.*cm,0.*cm);
   auto ST121 = new G4Box("ST121", 0.5 * ST110_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST121Log = new G4LogicalVolume(ST121, Vacuum, "ST121");
   auto physST121 = new G4PVPlacement(rot47,pos133, ST121Log, "ST121", logicEnv, false, 0);
   auto SurfaceVisAtt_ST121 = new G4VisAttributes(G4Colour::Blue());
   ST121Log->SetVisAttributes(SurfaceVisAtt_ST121); 
   
   //ST120
   G4ThreeVector pos134 = G4ThreeVector(0.*cm, -42.*cm,0.*cm);
   auto ST122 = new G4Box("ST122", 0.5 * ST110_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST122Log = new G4LogicalVolume(ST122, Vacuum, "ST122");
   auto physST122 = new G4PVPlacement(0,pos134, ST122Log, "ST122", logicEnv, false, 0);
   auto SurfaceVisAtt_ST122 = new G4VisAttributes(G4Colour::Blue());
   ST122Log->SetVisAttributes(SurfaceVisAtt_ST122); 
   
   //ST121
   G4ThreeVector pos135 = G4ThreeVector(-21.*cm, -36.*cm,0.*cm);
   auto ST123 = new G4Box("ST123", 0.5 * ST110_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST123Log = new G4LogicalVolume(ST123, Vacuum, "ST123");
   auto physST123 = new G4PVPlacement(rot38,pos135, ST123Log, "ST123", logicEnv, false, 0);
   auto SurfaceVisAtt_ST123 = new G4VisAttributes(G4Colour::Blue());
   ST123Log->SetVisAttributes(SurfaceVisAtt_ST123); 
   
   //ST122
   G4ThreeVector pos136 = G4ThreeVector(-36.*cm, -21.*cm,0.*cm);
   auto ST124 = new G4Box("ST124", 0.5 * ST110_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST124Log = new G4LogicalVolume(ST124, Vacuum, "ST124");
   auto physST124 = new G4PVPlacement(rot23,pos136, ST124Log, "ST124", logicEnv, false, 0);
   auto SurfaceVisAtt_ST124 = new G4VisAttributes(G4Colour::Blue());
   ST124Log->SetVisAttributes(SurfaceVisAtt_ST124); 
   
   //ST123
   G4ThreeVector pos137 = G4ThreeVector(-42.*cm, 0.*cm,0.*cm);
   auto ST125 = new G4Box("ST125", 0.5 * ST110_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST125Log = new G4LogicalVolume(ST125, Vacuum, "ST125");
   auto physST125 = new G4PVPlacement(rott,pos137, ST125Log, "ST125", logicEnv, false, 0);
   auto SurfaceVisAtt_ST125 = new G4VisAttributes(G4Colour::Blue());
   ST125Log->SetVisAttributes(SurfaceVisAtt_ST125); 
   
   //ST124
   G4ThreeVector pos138 = G4ThreeVector(-36.*cm, 21.*cm,0.*cm);
   auto ST126 = new G4Box("ST126", 0.5 * ST110_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST126Log = new G4LogicalVolume(ST126, Vacuum, "ST126");
   auto physST126 = new G4PVPlacement(rot42,pos138, ST126Log, "ST126", logicEnv, false, 0);
   auto SurfaceVisAtt_ST126 = new G4VisAttributes(G4Colour::Blue());
   ST126Log->SetVisAttributes(SurfaceVisAtt_ST126); 
   
   //ST125
   G4ThreeVector pos139 = G4ThreeVector(-21.*cm, 36.*cm,0.*cm);
   auto ST127 = new G4Box("ST127", 0.5 * ST110_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST127Log = new G4LogicalVolume(ST127, Vacuum, "ST127");
   auto physST127 = new G4PVPlacement(rot49,pos139, ST127Log, "ST127", logicEnv, false, 0);
   auto SurfaceVisAtt_ST127 = new G4VisAttributes(G4Colour::Blue());
   ST127Log->SetVisAttributes(SurfaceVisAtt_ST127); 
   
   //ST126
   G4ThreeVector pos140 = G4ThreeVector(0.*cm, 48.*cm,0.*cm);
   G4double ST120_x = 24.*cm;
   auto ST128 = new G4Box("ST128", 0.5 * ST120_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST128Log = new G4LogicalVolume(ST128, Vacuum, "ST128");
   auto physST128 = new G4PVPlacement(0,pos140, ST128Log, "ST128", logicEnv, false, 0);
   auto SurfaceVisAtt_ST128 = new G4VisAttributes(G4Colour::Blue());
   ST128Log->SetVisAttributes(SurfaceVisAtt_ST128); 
   
   //ST127
   G4ThreeVector pos141 = G4ThreeVector(24.*cm, 41.*cm,0.*cm);
   auto ST129 = new G4Box("ST129", 0.5 * ST120_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST129Log = new G4LogicalVolume(ST129, Vacuum, "ST129");
   auto physST129 = new G4PVPlacement(rot29,pos141, ST129Log, "ST129", logicEnv, false, 0);
   auto SurfaceVisAtt_ST129 = new G4VisAttributes(G4Colour::Blue());
   ST129Log->SetVisAttributes(SurfaceVisAtt_ST129); 
   
   //ST128
   G4ThreeVector pos142 = G4ThreeVector(41.*cm, 24.*cm,0.*cm);
   auto ST130 = new G4Box("ST130", 0.5 * ST120_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST130Log = new G4LogicalVolume(ST130, Vacuum, "ST130");
   auto physST130 = new G4PVPlacement(rot15,pos142, ST130Log, "ST130", logicEnv, false, 0);
   auto SurfaceVisAtt_ST130 = new G4VisAttributes(G4Colour::Blue());
   ST130Log->SetVisAttributes(SurfaceVisAtt_ST130);
   
   //ST129
   G4ThreeVector pos143 = G4ThreeVector(48.*cm, 0.*cm,0.*cm);
   auto ST131 = new G4Box("ST131", 0.5 * ST120_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST131Log = new G4LogicalVolume(ST131, Vacuum, "ST131");
   auto physST131 = new G4PVPlacement(rott,pos143, ST131Log, "ST131", logicEnv, false, 0);
   auto SurfaceVisAtt_ST131 = new G4VisAttributes(G4Colour::Blue());
   ST131Log->SetVisAttributes(SurfaceVisAtt_ST131);
   
   //ST130
   G4ThreeVector pos144 = G4ThreeVector(41.*cm, -24.*cm,0.*cm);
   auto ST132 = new G4Box("ST132", 0.5 * ST120_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST132Log = new G4LogicalVolume(ST132, Vacuum, "ST132");
   auto physST132 = new G4PVPlacement(rot33,pos144, ST132Log, "ST132", logicEnv, false, 0);
   auto SurfaceVisAtt_ST132 = new G4VisAttributes(G4Colour::Blue());
   ST132Log->SetVisAttributes(SurfaceVisAtt_ST132);
   
   //ST131
   G4ThreeVector pos145 = G4ThreeVector(24.*cm, -41.*cm,0.*cm);
   auto ST133 = new G4Box("ST133", 0.5 * ST120_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST133Log = new G4LogicalVolume(ST133, Vacuum, "ST133");
   auto physST133 = new G4PVPlacement(rot47,pos145, ST133Log, "ST133", logicEnv, false, 0);
   auto SurfaceVisAtt_ST133 = new G4VisAttributes(G4Colour::Blue());
   ST133Log->SetVisAttributes(SurfaceVisAtt_ST133);
   
   //ST132
   G4ThreeVector pos146 = G4ThreeVector(0.*cm, -48.*cm,0.*cm);
   auto ST134 = new G4Box("ST134", 0.5 * ST120_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST134Log = new G4LogicalVolume(ST134, Vacuum, "ST134");
   auto physST134 = new G4PVPlacement(0,pos146, ST134Log, "ST134", logicEnv, false, 0);
   auto SurfaceVisAtt_ST134 = new G4VisAttributes(G4Colour::Blue());
   ST134Log->SetVisAttributes(SurfaceVisAtt_ST134);
   
   //ST133
   G4ThreeVector pos147 = G4ThreeVector(-24.*cm, -41.*cm,0.*cm);
   auto ST135 = new G4Box("ST135", 0.5 * ST120_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST135Log = new G4LogicalVolume(ST135, Vacuum, "ST135");
   auto physST135 = new G4PVPlacement(rot38,pos147, ST135Log, "ST135", logicEnv, false, 0);
   auto SurfaceVisAtt_ST135 = new G4VisAttributes(G4Colour::Blue());
   ST135Log->SetVisAttributes(SurfaceVisAtt_ST135);
   
   //ST134
   G4ThreeVector pos148 = G4ThreeVector(-41.*cm, -24.*cm,0.*cm);
   auto ST136 = new G4Box("ST136", 0.5 * ST120_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST136Log = new G4LogicalVolume(ST136, Vacuum, "ST136");
   auto physST136 = new G4PVPlacement(rot23,pos148, ST136Log, "ST136", logicEnv, false, 0);
   auto SurfaceVisAtt_ST136 = new G4VisAttributes(G4Colour::Blue());
   ST136Log->SetVisAttributes(SurfaceVisAtt_ST136);
   
   //ST135
   G4ThreeVector pos149 = G4ThreeVector(-48.*cm, 0.*cm,0.*cm);
   auto ST137 = new G4Box("ST137", 0.5 * ST120_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST137Log = new G4LogicalVolume(ST137, Vacuum, "ST137");
   auto physST137 = new G4PVPlacement(rottt,pos149, ST137Log, "ST137", logicEnv, false, 0);
   auto SurfaceVisAtt_ST137 = new G4VisAttributes(G4Colour::Blue());
   ST137Log->SetVisAttributes(SurfaceVisAtt_ST137);
   
   //ST136
   G4ThreeVector pos150 = G4ThreeVector(-41.*cm, 24.*cm,0.*cm);
   auto ST138 = new G4Box("ST138", 0.5 * ST120_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST138Log = new G4LogicalVolume(ST138, Vacuum, "ST138");
   auto physST138 = new G4PVPlacement(rot42,pos150, ST138Log, "ST138", logicEnv, false, 0);
   auto SurfaceVisAtt_ST138 = new G4VisAttributes(G4Colour::Blue());
   ST138Log->SetVisAttributes(SurfaceVisAtt_ST138);
   
   //ST137
   G4ThreeVector pos151 = G4ThreeVector(-24.*cm, 41.*cm,0.*cm);
   auto ST139 = new G4Box("ST139", 0.5 * ST120_x, 0.5 * ST90_y, 0.5 * ST_z);
   auto ST139Log = new G4LogicalVolume(ST139, Vacuum, "ST139");
   auto physST139 = new G4PVPlacement(rot49,pos151, ST139Log, "ST139", logicEnv, false, 0);
   auto SurfaceVisAtt_ST139 = new G4VisAttributes(G4Colour::Blue());
   ST139Log->SetVisAttributes(SurfaceVisAtt_ST139);
   
   //STR1
   G4ThreeVector pos88 = G4ThreeVector(0 * cm, 0 * cm, 60 * cm);
   G4double Interradius_STR1 = 7.5*cm;
   G4double Outerradius_STR1 = 50.*cm;
   G4double Length_STR1 = 3.0 *cm;
   G4double Start_Angle_STR1 = 0.*deg;
   G4double SpanningAngle_STR1 = 360.*deg;
   auto STR1 = new G4Tubs("STR1", Interradius_STR1, Outerradius_STR1, Length_STR1, Start_Angle_STR1, SpanningAngle_STR1);
   auto STR1Log = new G4LogicalVolume(STR1, Vacuum, "STR1");
   new G4PVPlacement(0,pos88,STR1Log,"STR1", logicEnv, false, 0);
   auto SurfaceVisAtt_STR1 = new G4VisAttributes(G4Colour::Blue());
   STR1Log->SetVisAttributes(SurfaceVisAtt_STR1);
   
   //STR2
   G4ThreeVector pos89 = G4ThreeVector(0 * cm, 0 * cm, 72 * cm);
   auto STR2 = new G4Tubs("STR2", Interradius_STR1, Outerradius_STR1, Length_STR1, Start_Angle_STR1, SpanningAngle_STR1);
   auto STR2Log = new G4LogicalVolume(STR2, Vacuum, "STR2");
   new G4PVPlacement(0,pos89,STR2Log,"STR2", logicEnv, false, 0);
   auto SurfaceVisAtt_STR2 = new G4VisAttributes(G4Colour::Blue());
   STR2Log->SetVisAttributes(SurfaceVisAtt_STR2);
   
   //STR3
   G4ThreeVector pos90 = G4ThreeVector(0 * cm, 0 * cm, 84 * cm); 
   auto STR3 = new G4Tubs("STR3", Interradius_STR1, Outerradius_STR1, Length_STR1, Start_Angle_STR1, SpanningAngle_STR1);
   auto STR3Log = new G4LogicalVolume(STR3, Vacuum, "STR3");
   new G4PVPlacement(0,pos90,STR3Log,"STR3", logicEnv, false, 0);
   auto SurfaceVisAtt_STR3 = new G4VisAttributes(G4Colour::Blue());
   STR3Log->SetVisAttributes(SurfaceVisAtt_STR3);
   
   //STR4
   G4ThreeVector pos91 = G4ThreeVector(0 * cm, 0 * cm, 96 * cm); 
   auto STR4 = new G4Tubs("STR4", Interradius_STR1, Outerradius_STR1, Length_STR1, Start_Angle_STR1, SpanningAngle_STR1);
   auto STR4Log = new G4LogicalVolume(STR4, Vacuum, "STR4");
   new G4PVPlacement(0,pos91,STR4Log,"STR4", logicEnv, false, 0);
   auto SurfaceVisAtt_STR4 = new G4VisAttributes(G4Colour::Blue());
   STR4Log->SetVisAttributes(SurfaceVisAtt_STR4);
   
   //STR5
   G4ThreeVector pos92 = G4ThreeVector(0 * cm, 0 * cm, 108 * cm); 
   auto STR5 = new G4Tubs("STR5", Interradius_STR1, Outerradius_STR1, Length_STR1, Start_Angle_STR1, SpanningAngle_STR1);
   auto STR5Log = new G4LogicalVolume(STR5, Vacuum, "STR5");
   new G4PVPlacement(0,pos92,STR5Log,"STR5", logicEnv, false, 0);
   auto SurfaceVisAtt_STR5 = new G4VisAttributes(G4Colour::Blue());
   STR5Log->SetVisAttributes(SurfaceVisAtt_STR5);
   
   //STR6
   G4ThreeVector pos93 = G4ThreeVector(0 * cm, 0 * cm, 120 * cm); 
   auto STR6 = new G4Tubs("STR6", Interradius_STR1, Outerradius_STR1, Length_STR1, Start_Angle_STR1, SpanningAngle_STR1);
   auto STR6Log = new G4LogicalVolume(STR6, Vacuum, "STR6");
   new G4PVPlacement(0,pos93,STR6Log,"STR6", logicEnv, false, 0);
   auto SurfaceVisAtt_STR6 = new G4VisAttributes(G4Colour::Blue());
   STR6Log->SetVisAttributes(SurfaceVisAtt_STR6);
   
   //STR7
   G4ThreeVector pos94 = G4ThreeVector(0 * cm, 0 * cm, 132 * cm); 
   auto STR7 = new G4Tubs("STR7", Interradius_STR1, Outerradius_STR1, Length_STR1, Start_Angle_STR1, SpanningAngle_STR1);
   auto STR7Log = new G4LogicalVolume(STR7, Vacuum, "STR7");
   new G4PVPlacement(0,pos94,STR7Log,"STR7", logicEnv, false, 0);
   auto SurfaceVisAtt_STR7 = new G4VisAttributes(G4Colour::Blue());
   STR7Log->SetVisAttributes(SurfaceVisAtt_STR7);
   
   //STR8
   G4ThreeVector pos95 = G4ThreeVector(0 * cm, 0 * cm, 144 * cm); 
   auto STR8 = new G4Tubs("STR8", Interradius_STR1, Outerradius_STR1, Length_STR1, Start_Angle_STR1, SpanningAngle_STR1);
   auto STR8Log = new G4LogicalVolume(STR8, Vacuum, "STR8");
   new G4PVPlacement(0,pos95,STR8Log,"STR8", logicEnv, false, 0);
   auto SurfaceVisAtt_STR8 = new G4VisAttributes(G4Colour::Blue());
   STR8Log->SetVisAttributes(SurfaceVisAtt_STR8);
   
   //STR9
   G4ThreeVector pos96 = G4ThreeVector(0 * cm, 0 * cm, 156 * cm); 
   auto STR9 = new G4Tubs("STR9", Interradius_STR1, Outerradius_STR1, Length_STR1, Start_Angle_STR1, SpanningAngle_STR1);
   auto STR9Log = new G4LogicalVolume(STR9, Vacuum, "STR9");
   new G4PVPlacement(0,pos96,STR9Log,"STR9", logicEnv, false, 0);
   auto SurfaceVisAtt_STR9 = new G4VisAttributes(G4Colour::Blue());
   STR9Log->SetVisAttributes(SurfaceVisAtt_STR9);
   
   //ECALCYLINDER
   G4double InterradiusMin_ECALC = 6.*cm;
   G4double OuterradiusMin_ECALC = 8.*cm;
   G4double InterradiusMax_ECALC = 10.*cm;
   G4double OuterradiusMax_ECALC = 12.*cm;
   G4double Length_ECALC = 20.*cm;
   G4double Start_Angle_ECALC = 0.*deg;
   G4double SpanningAngle_ECALC = 360.*deg;
   G4ThreeVector pos97 = G4ThreeVector(0 * cm, 0 * cm, 192 * cm); 
   auto ECALC = new G4Cons("ECALC", InterradiusMin_ECALC, OuterradiusMin_ECALC, InterradiusMax_ECALC, OuterradiusMax_ECALC, Length_ECALC, Start_Angle_ECALC, SpanningAngle_ECALC);
   auto ECALCLog = new G4LogicalVolume(ECALC, Vacuum, "ECALC");
   new G4PVPlacement(0,pos97,ECALCLog,"ECALC", logicEnv, false, 0);
   auto SurfaceVisAtt_ECALC = new G4VisAttributes(G4Colour::Green());
   ECALCLog->SetVisAttributes(SurfaceVisAtt_ECALC);
   
   //ECALCYLINDER_Back
   G4double Interradius_ECALCB = 9.*cm;
   G4double Outerradius_ECALCB = 9.5*cm;
   G4double Length_ECALCB = 2.*cm;
   G4double Start_Angle_ECALCB = 0.*deg;
   G4double SpanningAngle_ECALCB = 360.*deg;
   G4ThreeVector pos98 = G4ThreeVector(0 * cm, 0 * cm, 173.9 * cm); 
   auto ECALCB = new G4Tubs("ECALCB", Interradius_ECALCB, Outerradius_ECALCB, Length_ECALCB, Start_Angle_ECALCB, SpanningAngle_ECALCB);
   auto ECALCBLog = new G4LogicalVolume(ECALCB, Vacuum, "ECALCB");
   new G4PVPlacement(0,pos98,ECALCBLog,"ECALCB", logicEnv, false, 0);
   auto SurfaceVisAtt_ECALCB = new G4VisAttributes(G4Colour::Green());
   ECALCBLog->SetVisAttributes(SurfaceVisAtt_ECALCB);
   
   //ECALCYLINDER_Front
   G4double Interradius_ECALCF = 12.5*cm;
   G4double Outerradius_ECALCF = 13.*cm;
   G4double Length_ECALCF = 1.*cm;
   G4double Start_Angle_ECALCF = 0.*deg;
   G4double SpanningAngle_ECALCF = 360.*deg;
   G4ThreeVector pos99 = G4ThreeVector(0 * cm, 0 * cm, 211.1 * cm); 
   auto ECALCF = new G4Tubs("ECALCF", Interradius_ECALCF, Outerradius_ECALCF, Length_ECALCF, Start_Angle_ECALCF, SpanningAngle_ECALCF);
   auto ECALCFLog = new G4LogicalVolume(ECALCF, Vacuum, "ECALCF");
   new G4PVPlacement(0,pos99,ECALCFLog,"ECALCF", logicEnv, false, 0);
   auto SurfaceVisAtt_ECALCF = new G4VisAttributes(G4Colour::Green());
   ECALCFLog->SetVisAttributes(SurfaceVisAtt_ECALCF);
   
   //STR10
   G4ThreeVector pos152 = G4ThreeVector(0 * cm, 0 * cm, -60 * cm);
   auto STR10 = new G4Tubs("STR10", Interradius_STR1, Outerradius_STR1, Length_STR1, Start_Angle_STR1, SpanningAngle_STR1);
   auto STR10Log = new G4LogicalVolume(STR10, Vacuum, "STR10");
   new G4PVPlacement(0,pos152,STR10Log,"STR10", logicEnv, false, 0);
   auto SurfaceVisAtt_STR10 = new G4VisAttributes(G4Colour::Blue());
   STR10Log->SetVisAttributes(SurfaceVisAtt_STR10);
   
   //STR11
   G4ThreeVector pos153 = G4ThreeVector(0 * cm, 0 * cm, -72 * cm);
   auto STR11 = new G4Tubs("STR11", Interradius_STR1, Outerradius_STR1, Length_STR1, Start_Angle_STR1, SpanningAngle_STR1);
   auto STR11Log = new G4LogicalVolume(STR11, Vacuum, "STR11");
   new G4PVPlacement(0,pos153,STR11Log,"STR11", logicEnv, false, 0);
   auto SurfaceVisAtt_STR11 = new G4VisAttributes(G4Colour::Blue());
   STR11Log->SetVisAttributes(SurfaceVisAtt_STR11);
   
   //STR12
   G4ThreeVector pos154 = G4ThreeVector(0 * cm, 0 * cm, -84 * cm);
   auto STR12 = new G4Tubs("STR12", Interradius_STR1, Outerradius_STR1, Length_STR1, Start_Angle_STR1, SpanningAngle_STR1);
   auto STR12Log = new G4LogicalVolume(STR12, Vacuum, "STR12");
   new G4PVPlacement(0,pos154,STR12Log,"STR12", logicEnv, false, 0);
   auto SurfaceVisAtt_STR12 = new G4VisAttributes(G4Colour::Blue());
   STR12Log->SetVisAttributes(SurfaceVisAtt_STR12);
   
   //STR13
   G4ThreeVector pos155 = G4ThreeVector(0 * cm, 0 * cm, -96 * cm);
   auto STR13 = new G4Tubs("STR13", Interradius_STR1, Outerradius_STR1, Length_STR1, Start_Angle_STR1, SpanningAngle_STR1);
   auto STR13Log = new G4LogicalVolume(STR13, Vacuum, "STR13");
   new G4PVPlacement(0,pos155,STR13Log,"STR13", logicEnv, false, 0);
   auto SurfaceVisAtt_STR13 = new G4VisAttributes(G4Colour::Blue());
   STR13Log->SetVisAttributes(SurfaceVisAtt_STR13);
   
   //STR14
   G4ThreeVector pos156 = G4ThreeVector(0 * cm, 0 * cm, -108 * cm);
   auto STR14 = new G4Tubs("STR14", Interradius_STR1, Outerradius_STR1, Length_STR1, Start_Angle_STR1, SpanningAngle_STR1);
   auto STR14Log = new G4LogicalVolume(STR14, Vacuum, "STR14");
   new G4PVPlacement(0,pos156,STR14Log,"STR14", logicEnv, false, 0);
   auto SurfaceVisAtt_STR14 = new G4VisAttributes(G4Colour::Blue());
   STR14Log->SetVisAttributes(SurfaceVisAtt_STR14);
   
   //STR15
   G4ThreeVector pos157 = G4ThreeVector(0 * cm, 0 * cm, -120 * cm);
   auto STR15 = new G4Tubs("STR15", Interradius_STR1, Outerradius_STR1, Length_STR1, Start_Angle_STR1, SpanningAngle_STR1);
   auto STR15Log = new G4LogicalVolume(STR15, Vacuum, "STR15");
   new G4PVPlacement(0,pos157,STR15Log,"STR15", logicEnv, false, 0);
   auto SurfaceVisAtt_STR15 = new G4VisAttributes(G4Colour::Blue());
   STR15Log->SetVisAttributes(SurfaceVisAtt_STR15);
   
   //STR16
   G4ThreeVector pos158 = G4ThreeVector(0 * cm, 0 * cm, -132 * cm);
   auto STR16 = new G4Tubs("STR16", Interradius_STR1, Outerradius_STR1, Length_STR1, Start_Angle_STR1, SpanningAngle_STR1);
   auto STR16Log = new G4LogicalVolume(STR16, Vacuum, "STR16");
   new G4PVPlacement(0,pos158,STR16Log,"STR16", logicEnv, false, 0);
   auto SurfaceVisAtt_STR16 = new G4VisAttributes(G4Colour::Blue());
   STR16Log->SetVisAttributes(SurfaceVisAtt_STR16);
   
   //STR17
   G4ThreeVector pos159 = G4ThreeVector(0 * cm, 0 * cm, -144 * cm);
   auto STR17 = new G4Tubs("STR17", Interradius_STR1, Outerradius_STR1, Length_STR1, Start_Angle_STR1, SpanningAngle_STR1);
   auto STR17Log = new G4LogicalVolume(STR17, Vacuum, "STR17");
   new G4PVPlacement(0,pos159,STR17Log,"STR17", logicEnv, false, 0);
   auto SurfaceVisAtt_STR17 = new G4VisAttributes(G4Colour::Blue());
   STR17Log->SetVisAttributes(SurfaceVisAtt_STR17);
   
   //STR18
   G4ThreeVector pos163 = G4ThreeVector(0 * cm, 0 * cm, -156 * cm);
   auto STR18 = new G4Tubs("STR18", Interradius_STR1, Outerradius_STR1, Length_STR1, Start_Angle_STR1, SpanningAngle_STR1);
   auto STR18Log = new G4LogicalVolume(STR18, Vacuum, "STR18");
   new G4PVPlacement(0,pos163,STR18Log,"STR18", logicEnv, false, 0);
   auto SurfaceVisAtt_STR18 = new G4VisAttributes(G4Colour::Blue());
   STR18Log->SetVisAttributes(SurfaceVisAtt_STR18);
   
   //ECALCYLINDER2
   G4double InterradiusMin_ECALC2 = 10.*cm;
   G4double OuterradiusMin_ECALC2 = 12.*cm;
   G4double InterradiusMax_ECALC2 = 6.*cm;
   G4double OuterradiusMax_ECALC2 = 8.*cm;
   G4double Length_ECALC2 = 20.*cm;
   G4ThreeVector pos160 = G4ThreeVector(0 * cm, 0 * cm, -192 * cm); 
   auto ECALC2 = new G4Cons("ECALC2", InterradiusMin_ECALC2, OuterradiusMin_ECALC2, InterradiusMax_ECALC2, OuterradiusMax_ECALC2, Length_ECALC2, Start_Angle_ECALC, SpanningAngle_ECALC);
   auto ECALC2Log = new G4LogicalVolume(ECALC2, Vacuum, "ECALC2");
   new G4PVPlacement(0,pos160,ECALC2Log,"ECALC2", logicEnv, false, 0);
   auto SurfaceVisAtt_ECALC2 = new G4VisAttributes(G4Colour::Green());
   ECALC2Log->SetVisAttributes(SurfaceVisAtt_ECALC2);
   
   
   //ECALCYLINDER_Back2
   G4ThreeVector pos161 = G4ThreeVector(0 * cm, 0 * cm, -173.9 * cm); 
   auto ECALCB2 = new G4Tubs("ECALCB2", Interradius_ECALCB, Outerradius_ECALCB, Length_ECALCB, Start_Angle_ECALCB, SpanningAngle_ECALCB);
   auto ECALCB2Log = new G4LogicalVolume(ECALCB2, Vacuum, "ECALCB2");
   new G4PVPlacement(0,pos161,ECALCB2Log,"ECALCB2", logicEnv, false, 0);
   auto SurfaceVisAtt_ECALCB2 = new G4VisAttributes(G4Colour::Green());
   ECALCB2Log->SetVisAttributes(SurfaceVisAtt_ECALCB2);
   
   //ECALCYLINDER_Front2
   G4ThreeVector pos162 = G4ThreeVector(0 * cm, 0 * cm, -211.1 * cm); 
   auto ECALCF2 = new G4Tubs("ECALCF2", Interradius_ECALCF, Outerradius_ECALCF, Length_ECALCF, Start_Angle_ECALCF, SpanningAngle_ECALCF);
   auto ECALCF2Log = new G4LogicalVolume(ECALCF2, Vacuum, "ECALCF2");
   new G4PVPlacement(0,pos162,ECALCF2Log,"ECALCF2", logicEnv, false, 0);
   auto SurfaceVisAtt_ECALCF2 = new G4VisAttributes(G4Colour::Green());
   ECALCF2Log->SetVisAttributes(SurfaceVisAtt_ECALCF2);
   
   //ECALCYLINDERLEFT
   G4double InterradiusMin_ECALCL = 11.*cm;
   G4double OuterradiusMin_ECALCL = 66.*cm;
   G4double InterradiusMax_ECALCL = 11.*cm;
   G4double OuterradiusMax_ECALCL = 63.*cm;
   G4double Length_ECALCL = 4.*cm;
   G4ThreeVector pos164 = G4ThreeVector(0 * cm, 0 * cm, -195 * cm); 
   auto ECALCL = new G4Cons("ECALCL", InterradiusMin_ECALCL, OuterradiusMin_ECALCL, InterradiusMax_ECALCL, OuterradiusMax_ECALCL, Length_ECALCL, Start_Angle_ECALC, SpanningAngle_ECALC);
   auto ECALCLLog = new G4LogicalVolume(ECALCL, Vacuum, "ECALCL");
   new G4PVPlacement(0,pos164,ECALCLLog,"ECALCL", logicEnv, false, 0);
   auto SurfaceVisAtt_ECALCL = new G4VisAttributes(G4Colour::Green());
   ECALCLLog->SetVisAttributes(SurfaceVisAtt_ECALCL);
   
   //ECALCYLINDERRIGHT
   G4double InterradiusMin_ECALCR = 11.*cm;
   G4double OuterradiusMin_ECALCR = 63.*cm;
   G4double InterradiusMax_ECALCR = 11.*cm;
   G4double OuterradiusMax_ECALCR = 66.*cm;
   G4ThreeVector pos165 = G4ThreeVector(0 * cm, 0 * cm, 195 * cm); 
   auto ECALCR = new G4Cons("ECALCR", InterradiusMin_ECALCR, OuterradiusMin_ECALCR, InterradiusMax_ECALCR, OuterradiusMax_ECALCR, Length_ECALCL, Start_Angle_ECALC, SpanningAngle_ECALC);
   auto ECALCRLog = new G4LogicalVolume(ECALCR, Vacuum, "ECALCR");
   new G4PVPlacement(0,pos165,ECALCRLog,"ECALCR", logicEnv, false, 0);
   auto SurfaceVisAtt_ECALCR = new G4VisAttributes(G4Colour::Green());
   ECALCRLog->SetVisAttributes(SurfaceVisAtt_ECALCR);
   
   //I
   G4double Interradius_I = 60.*cm;
   G4double Outerradius_I = 65.*cm;
   G4double Length_I = 350.0*cm;
   auto SilindirI = new G4Tubs("SilindirI", Interradius_I, Outerradius_I, 0.5*Length_I, Start_Angle_BeamPipe, SpanningAngle_BeamPipe);
   auto ILog = new G4LogicalVolume(SilindirI, Vacuum, "SilindirI");
   new G4PVPlacement(0,pos1,ILog, "SilindirI", logicEnv, false, 0);
   auto SurfaceVisAtt_I = new G4VisAttributes(G4Colour::Green());
   SurfaceVisAtt_I->SetVisibility(true);
   SurfaceVisAtt_I->SetForceWireframe(true);
   ILog->SetVisAttributes(SurfaceVisAtt_I);
   
   //1
   G4double Interradius_1 = 12.*cm;
   G4double Outerradius_1 = 65.*cm;
   G4double Length_1 = 3.0*cm;
   G4ThreeVector pos166 = G4ThreeVector(0 * cm, 0 * cm, 205 * cm); 
   auto Silindir1 = new G4Tubs("Silindir1", Interradius_1, Outerradius_1, 0.5*Length_1, Start_Angle_BeamPipe, SpanningAngle_BeamPipe);
   auto Logg = new G4LogicalVolume(Silindir1, Vacuum, "Silindir1");
   new G4PVPlacement(0,pos166,Logg, "Silindir1", logicEnv, false, 0);
   auto SurfaceVisAtt_ = new G4VisAttributes(G4Colour::Green());
   Logg->SetVisAttributes(SurfaceVisAtt_);
   
   //3
   G4double Interradius_3 = 62.*cm;
   G4double Outerradius_3 = 65.*cm;
   G4double Length_3 = 5.0*cm;
   G4ThreeVector pos168 = G4ThreeVector(0 * cm, 0 * cm, 211.1 * cm); 
   auto Silindir3 = new G4Tubs("Silindir3", Interradius_3, Outerradius_3, 0.5*Length_3, Start_Angle_BeamPipe, SpanningAngle_BeamPipe);
   auto Logg3 = new G4LogicalVolume(Silindir3, Vacuum, "Silindir3");
   new G4PVPlacement(0,pos168,Logg3, "Silindir3", logicEnv, false, 0);
   auto SurfaceVisAtt_3 = new G4VisAttributes(G4Colour::Green());
   Logg3->SetVisAttributes(SurfaceVisAtt_3);
   
   //2
   G4ThreeVector pos167 = G4ThreeVector(0 * cm, 0 * cm, -205 * cm); 
   auto Silindir2 = new G4Tubs("Silindir2", Interradius_1, Outerradius_1, 0.5*Length_1, Start_Angle_BeamPipe, SpanningAngle_BeamPipe);
   auto Logg2 = new G4LogicalVolume(Silindir2, Vacuum, "Silindir2");
   new G4PVPlacement(0,pos167,Logg, "Silindir2", logicEnv, false, 0);
   auto SurfaceVisAtt_2 = new G4VisAttributes(G4Colour::Green());
   Logg2->SetVisAttributes(SurfaceVisAtt_2);
   
   //4
   auto Silindir4 = new G4Tubs("Silindir4", Interradius_3, Outerradius_3, 0.5*Length_3, Start_Angle_BeamPipe, SpanningAngle_BeamPipe);
   auto Logg4 = new G4LogicalVolume(Silindir4, Vacuum, "Silindir4");
   new G4PVPlacement(0,pos162,Logg4, "Silindir4", logicEnv, false, 0);
   auto SurfaceVisAtt_4 = new G4VisAttributes(G4Colour::Green());
   Logg4->SetVisAttributes(SurfaceVisAtt_4);
   
   //5
   G4double Interradius_5 = 12.*cm;
   G4double Outerradius_5 = 90.*cm;
   G4double Length_5 = 40.0*cm;
   G4ThreeVector pos169 = G4ThreeVector(0 * cm, 0 * cm, 254. * cm); 
   auto Silindir5 = new G4Tubs("Silindir5", Interradius_5, Outerradius_5, Length_5, Start_Angle_BeamPipe, SpanningAngle_BeamPipe);
   auto Logg5 = new G4LogicalVolume(Silindir5, Vacuum, "Silindir5");
   new G4PVPlacement(0,pos169,Logg5, "Silindir5", logicEnv, false, 0);
   auto SurfaceVisAtt_5 = new G4VisAttributes(G4Colour::Yellow());
   Logg5->SetVisAttributes(SurfaceVisAtt_5);
   
   //6
   G4ThreeVector pos170 = G4ThreeVector(0 * cm, 0 * cm, -254. * cm); 
   auto Silindir6 = new G4Tubs("Silindir6", Interradius_5, Outerradius_5, Length_5, Start_Angle_BeamPipe, SpanningAngle_BeamPipe);
   auto Logg6 = new G4LogicalVolume(Silindir6, Vacuum, "Silindir6");
   new G4PVPlacement(0,pos170,Logg6, "Silindir6", logicEnv, false, 0);
   auto SurfaceVisAtt_6 = new G4VisAttributes(G4Colour::Yellow());
   Logg6->SetVisAttributes(SurfaceVisAtt_6);

    fScoringVolume = logicEnv;

    return physWorld;
}
