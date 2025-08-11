/*
#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#include "G4RunManagerFactory.hh"
#include "G4SteppingVerbose.hh"
#include "G4UImanager.hh"

#include "FTFP_BERT.hh"
#include "QGSP_BIC.hh"
#include "QGSP_BERT.hh"
#include "FTFP_BERT_HP.hh"
#include "G4PhysListFactory.hh"
#include "G4VModularPhysicsList.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"
#include "G4GeneralParticleSource.hh"
using namespace B1;

int main(int argc, char** argv) {
    // Etkileşimli mod kontrolü
    G4UIExecutive* ui = nullptr;
    if (argc == 1) {
        ui = new G4UIExecutive(argc, argv);
    }

    // Birim hassasiyeti
    G4int precision = 4;
    G4SteppingVerbose::UseBestUnit(precision);

    // Çalıştırma yöneticisi
    auto runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

    // Dedektör başlatma
    runManager->SetUserInitialization(new DetectorConstruction());

    // Fizik listesi seçimi ve başlatma
    G4PhysListFactory factory;
    G4VModularPhysicsList* phys = nullptr;
    G4String physName = "";

    // Argümanlarla fizik listesi belirlenmiş mi kontrol et
    if (argc >= 3) {
        physName = argv[2];
    }

    // Ortam değişkeni ile fizik listesi belirlenmiş mi kontrol et
    if (physName.empty()) {
        char* path = std::getenv("PHYSLIST");
        if (path) {
            physName = G4String(path);
        }
    }

    // Geçerli bir fizik listesi yoksa varsayılan olarak FTFP_BERT kullan
    if (physName.empty() || !factory.IsReferencePhysList(physName)) {
        physName = "FTFP_BERT";
    }

    // Fizik listesini ayarla
    phys = factory.GetReferencePhysList(physName);
    runManager->SetUserInitialization(phys);
    // Kullanıcı aksiyonlarını başlatma
    runManager->SetUserInitialization(new ActionInitialization());

    // Görselleştirme başlatma
    auto visManager = new G4VisExecutive();
    visManager->Initialize();

    // Kullanıcı arayüzü yöneticisi
    auto UImanager = G4UImanager::GetUIpointer();

    // Makro dosyasını çalıştır veya etkileşimli oturumu başlat
    if (!ui) {
        // Batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    } else {
        // Interactive mode
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        if (ui->IsGUI()) {
            UImanager->ApplyCommand("/control/execute gui.mac");
        }
        ui->SessionStart();
        delete ui;
    }

    // İş sonlandırma
    delete visManager;
    delete runManager;

    return 0;
}
*/

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#include "G4RunManagerFactory.hh"
#include "G4SteppingVerbose.hh"
#include "G4UImanager.hh"

#include "FTFP_BERT.hh"
#include "QGSP_BIC.hh"
#include "QGSP_BERT.hh"
#include "FTFP_BERT_HP.hh"
#include "G4PhysListFactory.hh"
#include "G4VModularPhysicsList.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4VisManager.hh"
#include "Randomize.hh"
#include "G4GeneralParticleSource.hh"
using namespace B1;

int main(int argc, char** argv) {
    // Etkileşimli mod kontrolü
    G4UIExecutive* ui = nullptr;
    if (argc == 1) {
        ui = new G4UIExecutive(argc, argv);
    }

    // Birim hassasiyeti
    G4int precision = 4;
    G4SteppingVerbose::UseBestUnit(precision);

    // Çalıştırma yöneticisi
    auto runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

    // Dedektör başlatma
    runManager->SetUserInitialization(new DetectorConstruction());

    // Fizik listesi seçimi ve başlatma
    G4PhysListFactory factory;
    G4VModularPhysicsList* phys = nullptr;
    G4String physName = "";

    // Argümanlarla fizik listesi belirlenmiş mi kontrol et
    if (argc >= 3) {
        physName = argv[2];
    }

    // Ortam değişkeni ile fizik listesi belirlenmiş mi kontrol et
    if (physName.empty()) {
        char* path = std::getenv("PHYSLIST");
        if (path) {
            physName = G4String(path);
        }
    }

    // Geçerli bir fizik listesi yoksa varsayılan olarak FTFP_BERT kullan
    if (physName.empty() || !factory.IsReferencePhysList(physName)) {
        physName = "FTFP_BERT_HP";  // FTFP_BERT_HP'yi varsayılan olarak seç
    }

    // Fizik listesini ayarla
    phys = factory.GetReferencePhysList(physName);
    runManager->SetUserInitialization(phys);

    // Kullanıcı aksiyonlarını başlatma
    runManager->SetUserInitialization(new ActionInitialization());

    // Görselleştirme başlatma
    auto visManager = new G4VisExecutive();
    visManager->Initialize();

    // Kullanıcı arayüzü yöneticisi
    auto UImanager = G4UImanager::GetUIpointer();

    // Makro dosyasını çalıştır veya etkileşimli oturumu başlat
    if (!ui) {
        // Batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    } else {
        // Interactive mode
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        if (ui->IsGUI()) {
            UImanager->ApplyCommand("/control/execute gui.mac");
        }
        ui->SessionStart();
        delete ui;
    }

    // İş sonlandırma
    delete visManager;
    delete runManager;

    return 0;
}

