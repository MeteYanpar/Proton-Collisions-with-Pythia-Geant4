#include "PrimaryGeneratorAction.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include <memory>
#include <HepMC3/GenEvent.h>
#include <HepMC3/ReaderAscii.h>
#include <HepMC3/Units.h>
#include <HepMC3/GenVertex.h>

// Yapıcı: HepMC3 dosyasını açar
PrimaryGeneratorAction::PrimaryGeneratorAction(const std::string& proton_collision13TeV_hepmc3) 
    : eventFilename(proton_collision13TeV_hepmc3) {
    // HepMC3 okuyucuyu oluştur ve dosyayı aç
    reader = std::make_unique<HepMC3::ReaderAscii>(proton_collision13TeV_hepmc3);
    if (!reader->failed()) {
        G4cout << "HepMC3 file successfully opened: " << proton_collision13TeV_hepmc3 << G4endl;
    } else {
        G4cerr << "Failed to open HepMC3 file: " << proton_collision13TeV_hepmc3 << G4endl;
        exit(EXIT_FAILURE);  // Hata durumunda programı durdur
    }
}

// Yıkıcı: Reader'ı kapatır
PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    if (reader) {
        reader->close();
    }
}

// Olayları HepMC3 dosyasından okur ve Geant4 olayına çevirir
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
    auto hepmcEvent = std::make_shared<HepMC3::GenEvent>();

    // Olayı okuyup okuyamadığımızı kontrol et
    if (!reader->read_event(*hepmcEvent)) {
        G4cout << "No more events to read!" << G4endl;
        return;
    }

    // Parçacık tanım tablosunu al
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();

    for (const auto& particle : hepmcEvent->particles()) {
        // Sadece final state parçacıkları işle
        if (particle->status() == 1) {
            // Momentum ve konum bilgilerini al
            auto momentum = particle->momentum();
            auto position = particle->production_vertex()->position();

            // PDG koduna göre parçacık tanımını bul
            G4ParticleDefinition* particleDef = 
                particleTable->FindParticle(particle->pid());

            if (!particleDef) {
                G4cout << "Particle with PDG code " 
                       << particle->pid() 
                       << " not found in Geant4 particle table!" << G4endl;
                continue;
            }

            // Geant4 birincil parçacığı oluştur
            G4PrimaryParticle* g4Particle = new G4PrimaryParticle(
                particleDef,
                momentum.px(),  // Momentum x bileşeni
                momentum.py(),  // Momentum y bileşeni
                momentum.pz()  // Momentum z bileşeni
            );

            // Geant4 birincil verteks oluştur
            G4PrimaryVertex* vertex = new G4PrimaryVertex(
                G4ThreeVector(
                    position.x(),  // Konum x bileşeni
                    position.y(),  // Konum y bileşeni
                    position.z()  // Konum z bileşeni
                ),
                position.t() * CLHEP::ns  // Zaman bileşeni
            );

            vertex->SetPrimary(g4Particle);
            anEvent->AddPrimaryVertex(vertex);
        }
    }

    // HepMC3 olayını işlediğimizi belirt
    hepmcEvent->clear();
}

