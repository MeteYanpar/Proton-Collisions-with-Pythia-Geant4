#ifndef TRAJECTORY_POINT_HH
#define TRAJECTORY_POINT_HH

#include "G4VTrajectoryPoint.hh"
#include "G4ThreeVector.hh"
#include "G4Track.hh"
#include "G4Step.hh"

class TrajectoryPoint : public G4VTrajectoryPoint {
public:
  TrajectoryPoint();
  TrajectoryPoint(const G4ThreeVector& pos);
  TrajectoryPoint(const G4Track* aTrack);
  TrajectoryPoint(const G4Step* aStep);
  virtual ~TrajectoryPoint();

  virtual const G4ThreeVector GetPosition() const override;

  void SetPosition(const G4ThreeVector& pos) { fPosition = pos; }

private:
  G4ThreeVector fPosition;
};

#endif


