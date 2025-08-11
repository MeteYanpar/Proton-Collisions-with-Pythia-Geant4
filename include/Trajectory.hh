#ifndef TRAJECTORY_HH
#define TRAJECTORY_HH

#include "G4VTrajectory.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"
#include "G4Polyline.hh"
#include "G4VisAttributes.hh"
#include "G4Track.hh" // G4Track sınıfını ekleyin
#include <vector>

class Trajectory : public G4VTrajectory {
public:
    Trajectory(const G4Track* aTrack);
    virtual ~Trajectory();

    virtual void ShowTrajectory(std::ostream& os = std::cout) const override;
    virtual void DrawTrajectory() const override;

    virtual G4int GetTrackID() const override;
    virtual G4int GetParentID() const override;
    virtual G4String GetParticleName() const override;
    virtual G4double GetCharge() const override;
    virtual G4int GetPDGEncoding() const override;
    virtual G4ThreeVector GetInitialMomentum() const override;
    virtual G4int GetPointEntries() const override;
    virtual G4VTrajectoryPoint* GetPoint(G4int i) const override;
    virtual void AppendStep(const G4Step* aStep) override;
    virtual void MergeTrajectory(G4VTrajectory* secondTrajectory) override;

    const std::vector<G4ThreeVector>& GetPoints() const { return fPoints; }

private:
    std::vector<G4ThreeVector> fPoints;
    G4ParticleDefinition* fParticleDefinition;
    G4ThreeVector fInitialMomentum;
    G4int fTrackID;
    G4int fParentID;
    G4String fParticleName;
    G4double fCharge;
    G4int fPDGEncoding;
};

extern G4ThreadLocal G4Allocator<Trajectory>* trajectoryAllocator;

#endif


