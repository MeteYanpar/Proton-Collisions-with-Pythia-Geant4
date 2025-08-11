#include "Trajectory.hh"
#include "G4Step.hh"
#include "G4ParticleTypes.hh"
#include "G4VProcess.hh"
#include "G4TrajectoryPoint.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4VVisManager.hh"

G4ThreadLocal G4Allocator<Trajectory>* trajectoryAllocator = nullptr;

Trajectory::Trajectory(const G4Track* aTrack)
    : G4VTrajectory(),
      fParticleDefinition(aTrack->GetDefinition()),
      fInitialMomentum(aTrack->GetMomentum()),
      fTrackID(aTrack->GetTrackID()),
      fParentID(aTrack->GetParentID()),
      fParticleName(aTrack->GetDefinition()->GetParticleName()),
      fCharge(aTrack->GetDefinition()->GetPDGCharge()),
      fPDGEncoding(aTrack->GetDefinition()->GetPDGEncoding()) {
    fPoints.push_back(aTrack->GetPosition());
}

Trajectory::~Trajectory() {}

void Trajectory::ShowTrajectory(std::ostream& os) const {
    os << "Trajectory of particle: " << fParticleDefinition->GetParticleName() << "\n";
    for (const auto& point : fPoints) {
        os << point << "\n";
    }
}

void Trajectory::DrawTrajectory() const {
    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
    if (!pVVisManager) return;

    G4Polyline polyline;
    for (const auto& point : fPoints) {
        polyline.push_back(G4Point3D(point));
    }

    G4Colour colour;
    if (fParticleDefinition == G4OpticalPhoton::OpticalPhotonDefinition()) {
        colour = G4Colour(0.0, 0.0, 1.0); // Red for optical photons
    } else if (fParticleDefinition == G4Electron::ElectronDefinition()) {
        colour = G4Colour(0.0, 1.0, 0.0); // Green for electrons
    } else {
        colour = G4Colour(1.0, 0.0, 0.0); // Blue for other particles
    }

    G4VisAttributes attribs(colour);
    polyline.SetVisAttributes(attribs);
    pVVisManager->Draw(polyline);
}

G4int Trajectory::GetTrackID() const {
    return fTrackID;
}

G4int Trajectory::GetParentID() const {
    return fParentID;
}

G4String Trajectory::GetParticleName() const {
    return fParticleName;
}

G4double Trajectory::GetCharge() const {
    return fCharge;
}

G4int Trajectory::GetPDGEncoding() const {
    return fPDGEncoding;
}

G4ThreeVector Trajectory::GetInitialMomentum() const {
    return fInitialMomentum;
}

G4int Trajectory::GetPointEntries() const {
    return fPoints.size();
}

G4VTrajectoryPoint* Trajectory::GetPoint(G4int i) const {
    if (i >= 0 && i < GetPointEntries()) {
        return new G4TrajectoryPoint(fPoints[i]);
    } else {
        return nullptr;
    }
}

void Trajectory::AppendStep(const G4Step* aStep) {
    fPoints.push_back(aStep->GetPostStepPoint()->GetPosition());
}

void Trajectory::MergeTrajectory(G4VTrajectory* secondTrajectory) {
    if (!secondTrajectory) return;

    Trajectory* sec = static_cast<Trajectory*>(secondTrajectory);
    for (auto point : sec->GetPoints()) {
        fPoints.push_back(point);
    }
}

