#ifndef STEPPINGACTION_HH
#define STEPPINGACTION_HH

#include "G4UserSteppingAction.hh"
#include "globals.hh"
#include "G4Step.hh"
#include "EventAction.hh"

// ROOT başlık dosyaları
#include "TFile.h"
#include "TH1D.h"

namespace B1
{

class EventAction;
class SteppingAction : public G4UserSteppingAction {
public:
    SteppingAction(EventAction* eventAction);
    ~SteppingAction() override;
    
    virtual void UserSteppingAction(const G4Step* step);

private:
    EventAction* fEventAction = nullptr;

    // ROOT nesneleri
    TFile* fOutputFile = nullptr;
    TH1D* hEnergy = nullptr;
    TH1D* hMomentum = nullptr;
};

}

#endif
