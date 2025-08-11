#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4UImanager.hh"

// ROOT başlık dosyaları
#include "TFile.h"
#include "TH1D.h"

namespace B1 {

SteppingAction::SteppingAction(EventAction* eventAction)
    : fEventAction(eventAction) {
    // ROOT dosyasını aç ve histogramları oluştur
    fOutputFile = new TFile("proton_collision13TeV.root", "UPDATE");
    hEnergy = new TH1D("hEnergy", "Kinetic Energy Distribution;Energy (MeV);Counts", 100, 0., 4000.);
    hMomentum = new TH1D("hMomentum", "Momentum Distribution;Momentum (MeV/c);Counts", 100, 0., 4000.);
}

SteppingAction::~SteppingAction() {
    if (fOutputFile) {
    fOutputFile->cd(); // ROOT dosyasına geç
        hEnergy->Write("", TObject::kOverwrite); // Histogramları güncelle
        hMomentum->Write("", TObject::kOverwrite);
        fOutputFile->Close();
        delete fOutputFile;
    }
}

void SteppingAction::UserSteppingAction(const G4Step* step) {
    G4Track* track = step->GetTrack();
    G4ThreeVector momentum = track->GetMomentum();
    G4double kineticEnergy = track->GetKineticEnergy();
    G4double energyDeposit = step->GetTotalEnergyDeposit();
    
    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    G4int trackID = track->GetTrackID();
    G4ThreeVector position = track->GetPosition();
    
    G4double energy = track->GetKineticEnergy();
    G4double totalEnergy = kineticEnergy + track->GetDefinition()->GetPDGMass();
    
    
    G4cout << "Event ID: " << eventID
           << ", Track ID: " << trackID
           << ", Particle: " << track->GetDefinition()->GetParticleName()
           << ", Energy:  " << energy
           << ", Total Energy:  " << totalEnergy
           << ", Position: " << position << G4endl;
    // ROOT histogramlarına veri ekle
    hEnergy->Fill(kineticEnergy / MeV);
    hMomentum->Fill(momentum.mag() / MeV);
}

} 

