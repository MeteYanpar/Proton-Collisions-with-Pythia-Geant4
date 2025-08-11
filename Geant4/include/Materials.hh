#ifndef MATERIALS_HH
#define MATERIALS_HH 

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "globals.hh"

class G4Material;

class Materials
{
  public:
    Materials();
    ~Materials();

    static  Materials* GetInstance();

    G4Material* GetMaterial(const G4String&);

  private:

    void Initialise();

    static Materials* fgInstance;
    
    G4NistManager* fNistMan = nullptr;

    G4Material* elC;
    G4Material* elO;
    G4Material* elH;
    G4Material* elBe;
    G4Material* world_mat; //Air
    G4Material* waterMaterial;  //Water
    G4Material* plexi_mat;
    G4Material* TiO2mat;
    G4Material* scintmat;
    G4Material* glass_mat;
    G4Material* wls;
    G4Material* Vacuum;
    G4Material* silicon;
    G4Material* demirMaterial;
};

#endif
