#include "TrackingAction.hh"
#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "G4ParticleTypes.hh"
#include "Trajectory.hh"

void TrackingAction::PreUserTrackingAction(const G4Track* aTrack) {
    fpTrackingManager->SetTrajectory(new Trajectory(aTrack));
}

void TrackingAction::PostUserTrackingAction(const G4Track* aTrack) {
    Trajectory* trajectory = (Trajectory*)fpTrackingManager->GimmeTrajectory();
    trajectory->DrawTrajectory();
}

