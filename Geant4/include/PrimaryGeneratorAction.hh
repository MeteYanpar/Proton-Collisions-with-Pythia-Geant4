/*#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction();

    virtual void GeneratePrimaries(G4Event* anEvent);

  private:
    G4ParticleGun* fParticleGun1; // İlk proton demeti
    G4ParticleGun* fParticleGun2; // İkinci proton demeti
};

#endif
*/


/*
#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
 
class G4GeneralParticleSource;
class G4Event;
 
class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
     PrimaryGeneratorAction();
    ~PrimaryGeneratorAction();
 
  public:
   virtual void GeneratePrimaries(G4Event* anEvent);
 
  private:
    G4GeneralParticleSource* fParticleSource;
 };
#endif
*/


#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "HepMC3/ReaderAscii.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"  
#include <memory>
#include <string>

class G4Event;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
    PrimaryGeneratorAction(const std::string& proton_collision13TeV_hepmc3);
    virtual ~PrimaryGeneratorAction();

    virtual void GeneratePrimaries(G4Event* anEvent) override;

private:
    std::unique_ptr<HepMC3::ReaderAscii> reader;
    std::string eventFilename;
};

#endif
