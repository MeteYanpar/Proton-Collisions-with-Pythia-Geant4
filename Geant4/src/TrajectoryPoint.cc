#include "TrajectoryPoint.hh"

TrajectoryPoint::TrajectoryPoint() 
: G4VTrajectoryPoint(),
  fPosition(G4ThreeVector())
{}

TrajectoryPoint::TrajectoryPoint(const G4ThreeVector& pos) 
: G4VTrajectoryPoint(),
  fPosition(pos)
{}

TrajectoryPoint::TrajectoryPoint(const G4Track* aTrack)
: G4VTrajectoryPoint(),
  fPosition(aTrack->GetPosition())
{}

TrajectoryPoint::TrajectoryPoint(const G4Step* aStep)
: G4VTrajectoryPoint(),
  fPosition(aStep->GetPreStepPoint()->GetPosition())
{}

TrajectoryPoint::~TrajectoryPoint() {}

const G4ThreeVector TrajectoryPoint::GetPosition() const {
  return fPosition;
}

