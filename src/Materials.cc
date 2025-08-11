#include "globals.hh"
#include "Materials.hh"
#include "G4SystemOfUnits.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4MaterialPropertyVector.hh"
#include "G4MaterialTable.hh"
#include "Randomize.hh"  
#include "G4RunManager.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4NistManager.hh"

Materials* Materials::fgInstance = 0;

Materials* Materials::GetInstance()
{
  if (!fgInstance) 
  { 
    fgInstance = new Materials();
  }
  return fgInstance;
}
Materials::Materials()
{
  fNistMan = G4NistManager::Instance();
  fNistMan->SetVerbose(2);

  fgInstance = this;
  Initialise();
}
Materials::~Materials()
{}
void Materials::Initialise()
{
   
    //G4cout << "start Initialise" << G4endl; //hoca hatanın nerede olduğunu bulmak için yazdı
    G4NistManager* nistManager = G4NistManager::Instance();
    // Define carbon material
    elC = nistManager->FindOrBuildMaterial("G4_C");
     
    // Define water material
    waterMaterial = nistManager->FindOrBuildMaterial("G4_WATER");
    
    demirMaterial = nistManager->FindOrBuildMaterial("G4_Fe");

    //Define Hidrogen material
    elH = nistManager->FindOrBuildMaterial("G4_H");
    
    //Define Oxygen material
    elO = nistManager->FindOrBuildMaterial("G4_O");
    
    //Define Be material
    elBe = nistManager->FindOrBuildMaterial("G4_Be");
    
    //Define Plexiglass material
    plexi_mat = nistManager->FindOrBuildMaterial("G4_PLEXIGLASS");
    
    //Silicon
    silicon = nistManager->FindOrBuildMaterial("G4_Si");
    
    G4double density;
    G4int ncomponents;
    G4double fractionmass;
    std::vector<G4int> natoms;
    std::vector<G4double> fractionMass;
    std::vector<G4String> elements;


    
    std::vector<G4double> energy = {
    2.00 * eV, 2.03 * eV, 2.06 * eV, 2.09 * eV, 2.12 * eV, 2.15 * eV, 2.18 * eV,
    2.21 * eV, 2.24 * eV, 2.27 * eV, 2.30 * eV, 2.33 * eV, 2.36 * eV, 2.39 * eV,
    2.42 * eV, 2.45 * eV, 2.48 * eV, 2.51 * eV, 2.54 * eV, 2.57 * eV, 2.60 * eV,
    2.63 * eV, 2.66 * eV, 2.69 * eV, 2.72 * eV, 2.75 * eV, 2.78 * eV, 2.81 * eV,
    2.84 * eV, 2.87 * eV, 2.90 * eV, 2.93 * eV, 2.96 * eV, 2.99 * eV, 3.02 * eV,
    3.05 * eV, 3.08 * eV, 3.11 * eV, 3.14 * eV, 3.17 * eV, 3.20 * eV, 3.23 * eV,
    3.26 * eV, 3.29 * eV, 3.32 * eV, 3.35 * eV, 3.38 * eV, 3.41 * eV, 3.44 * eV,
    3.47 * eV
  };
    
    std::vector<G4double> energySmall = { 2.0 * eV, 3.47 * eV };
    std::vector<G4double> refractiveIndexAir = { 1.0, 1.0 };

    
    std::vector<G4double> plexi_Energy = { 2.0 * eV, 2.5 * eV, 3.0 * eV };
    std::vector<G4double> plexi_RIND  = { 1.49, 1.49, 1.49 };
    std::vector<G4double> plexi_ABSL  = { 40.0 * m, 40.0 * m, 40.0 * m };

    auto plexi_mt = new G4MaterialPropertiesTable();
    plexi_mt->AddProperty("RINDEX", plexi_Energy, plexi_RIND);
    plexi_mt->AddProperty("ABSLENGTH", plexi_Energy, plexi_ABSL);
    plexi_mat->SetMaterialPropertiesTable(plexi_mt);
   
    //G4cout << "elements" << G4endl;
    
    //Define scintillator material
    scintmat = nistManager->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  
   
    std::vector<G4double> ps_Energy = { 1.5 * eV, 2.0 * eV, 2.5 * eV, 3.0 * eV, 3.5 * eV };
    std::vector<G4double> ps_SCINT = { 0.1, 1.0, 0.1, 1.0, 0.1 };
    std::vector<G4double> ps_RIND  = { 1.59, 1.58, 1.57, 1.56, 1.55 };
    std::vector<G4double> ps_ABSL  = { 35. * cm, 35. * cm, 35. * cm, 35. * cm, 35. * cm };
    
    auto ps_mt = new G4MaterialPropertiesTable();
    ps_mt->AddProperty("SCINTILLATIONCOMPONENT1", ps_Energy, ps_SCINT);
    ps_mt->AddProperty("SCINTILLATIONCOMPONENT2", ps_Energy, ps_SCINT);
    ps_mt->AddProperty("RINDEX", ps_Energy, ps_RIND);
    ps_mt->AddProperty("ABSLENGTH", ps_Energy, ps_ABSL);
    ps_mt->AddConstProperty("SCINTILLATIONYIELD", 2400. / MeV);
    ps_mt->AddConstProperty("RESOLUTIONSCALE", 1.0);
    ps_mt->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 20. * ns);
    ps_mt->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 45. * ns);
    ps_mt->AddConstProperty("SCINTILLATIONYIELD1", 1.0);
    ps_mt->AddConstProperty("SCINTILLATIONYIELD2", 0.0);
    scintmat->SetMaterialPropertiesTable(ps_mt);
    scintmat->GetIonisation()->SetBirksConstant(0.126 * mm / MeV);
    
     // Define glass Material
    glass_mat = nistManager->FindOrBuildMaterial("G4_GLASS_PLATE");
    std::vector<G4double> glass_Energy = { 2.0 * eV, 3.0 * eV };
    std::vector<G4double> glass_RIND = { 1.0, 1.0 };
    std::vector<G4double> glass_ABSL = { 1000.0 * m, 1000.0 * m };
    std::vector<G4double> refractiveIndexglass = { 1.0, 1.0 };

    G4MaterialPropertiesTable* glass_mpt = new G4MaterialPropertiesTable();
    glass_mpt->AddProperty("RINDEX", energySmall, refractiveIndexglass);
    glass_mpt->AddProperty("ABSLENGTH", glass_Energy, glass_ABSL);
    glass_mat->SetMaterialPropertiesTable(glass_mpt);
    
    // Define Air Material
    world_mat = nistManager->FindOrBuildMaterial("G4_AIR");
    std::vector<G4double> air_Energy = { 2.0 * eV, 3.5 * eV };
    std::vector<G4double> air_RIND = { 1.0, 1.0 };
    std::vector<G4double> air_ABSL = { 1000.0 * m, 1000.0 * m };

    G4MaterialPropertiesTable* air_mpt = new G4MaterialPropertiesTable();
    air_mpt->AddProperty("RINDEX", energySmall, refractiveIndexAir);
    air_mpt->AddProperty("ABSLENGTH", air_Energy, air_ABSL);
    world_mat->SetMaterialPropertiesTable(air_mpt);
    
    // Define wls fiber Material
    elements.push_back("C");
    natoms.push_back(5);
    elements.push_back("H");
    natoms.push_back(8);
    elements.push_back("O");
    natoms.push_back(2);

    density = 1.190 * g / cm3;

    wls = fNistMan->ConstructNewMaterial("wls", elements, natoms, density);
    std::vector<G4double> refractiveIndexWLSfiber = {1.60, 1.60};

    std::vector<G4double> absWLSfiber = {
    5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m,
    5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m,
    5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m,
    5.40 * m, 5.40 * m, 1.10 * m, 1.10 * m, 1.10 * m, 1.10 * m, 1.10 * m, 1.10 * m, 1.10 * m,
    1. * mm,  1. * mm,  1. * mm,  1. * mm,  1. * mm,  1. * mm,  1. * mm,  1. * mm,  1. * mm,
    1. * mm,  1. * mm,  1. * mm,  1. * mm,  1. * mm};

    std::vector<G4double> emissionFib = {
    0.05, 0.10, 0.30, 0.50, 0.75, 1.00, 1.50, 1.85, 2.30, 2.75,
    3.25, 3.80, 4.50, 5.20, 6.00, 7.00, 8.50, 9.50, 11.1, 12.4,
    12.9, 13.0, 12.8, 12.3, 11.1, 11.0, 12.0, 11.0, 17.0, 16.9,
    15.0, 9.00, 2.50, 1.00, 0.05, 0.00, 0.00, 0.00, 0.00, 0.00,
    0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00};

     // Add entries into properties table
    auto mptWLSfiber = new G4MaterialPropertiesTable();
    mptWLSfiber->AddProperty("RINDEX", energySmall, refractiveIndexWLSfiber);
    mptWLSfiber->AddProperty("WLSABSLENGTH", energy, absWLSfiber);
    mptWLSfiber->AddProperty("WLSCOMPONENT", energy, emissionFib);
    mptWLSfiber->AddConstProperty("WLSTIMECONSTANT", 0.5 * ns);
    wls->SetMaterialPropertiesTable(mptWLSfiber);
    
    //Define reflector material
    TiO2mat = nistManager->FindOrBuildMaterial("G4_TITANIUM_DIOXIDE");
     
    std::vector<G4double> photonEnergy = { 2.0*eV, 3.0*eV};
    std::vector<G4double> refractiveIndex  = { 2.4, 2.3};
    std::vector<G4double> absorptionLength  = { 10.0*cm, 15.0*cm };
    
    
    G4MaterialPropertiesTable* TiO2_mpt = new G4MaterialPropertiesTable();
    TiO2_mpt->AddProperty("RINDEX", photonEnergy, refractiveIndex);
    TiO2_mpt->AddProperty("ABSLENGTH", photonEnergy, absorptionLength);
    TiO2mat->SetMaterialPropertiesTable(TiO2_mpt);
    
    //Vacumm material
    Vacuum = nistManager->FindOrBuildMaterial("G4_Galactic");
    /*
    G4double atomicNumber = 1.;
    G4double massOfMole = 1.008* g/mole;
    G4double density_vakum = 1.e-25* g/cm3;
    G4double temperature = 2.73* kelvin;
    G4double pressure = 3.e-18* pascal;
    */
    G4double z, aa, densityy;
    density = 1.0e-10 * g / cm3;  // Çok düşük yoğunluk
    aa = 1.01 * g / mole;
    G4Material* lowDensityGas = new G4Material("LowDensityGas", z = 1., aa, densityy);

}

G4Material* Materials::GetMaterial(const G4String& materialName) {
    // Burada malzeme adına göre ilgili malzemeyi döndürün
    if (materialName == "scintmat") {
        return scintmat;
    } else if (materialName == "G4_WATER") {
        return waterMaterial;
    } else if (materialName == "G4_Galactic") {
        return Vacuum;
    } else if (materialName == "G4_PLEXIGLASS") {
        return plexi_mat;
    } else if (materialName == "G4_TITANIUM_DIOXIDE") {
        return TiO2mat;
    } else if (materialName == "G4_H") {
        return elH;
    } else if (materialName == "G4_Be") {
        return elBe;
    } else if (materialName == "G4_C") {
        return elC;
    } else if (materialName == "G4_O") {
        return elO; 
    } else if (materialName == "G4_Fe") {
        return demirMaterial; 
    } else if (materialName == "G4_AIR") {
        return world_mat;   
    } else if (materialName == "wls") {
        return wls;
    } else if (materialName == "G4_Si") {
        return silicon;
    } else if (materialName == "G4_GLASS_PLATE") {
        return glass_mat;
  
    } else {
        // İstenilen malzeme adı tanımlanmamışsa NULL döndürün veya uygun bir işlem yapın
        return nullptr;
    }
}
