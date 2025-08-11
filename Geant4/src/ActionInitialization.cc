#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

namespace B1
{
void ActionInitialization::BuildForMaster() const
{
    auto runAction = new RunAction;
    SetUserAction(runAction);
}

void ActionInitialization::Build() const
{
    
    auto primaryGeneratorAction = new PrimaryGeneratorAction("/home/meteyanpar/pythia/pythia8312/examples/proton_collision13TeV.hepmc3");
    SetUserAction(primaryGeneratorAction);
    
    auto runAction = new RunAction;
    SetUserAction(runAction);
    
    auto eventAction = new EventAction(runAction);
    SetUserAction(eventAction);
    
    SetUserAction(new SteppingAction(eventAction));
}
} 
