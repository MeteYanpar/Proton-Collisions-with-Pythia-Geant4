#ifndef USER_TRACK_INFORMATION_HH
#define USER_TRACK_INFORMATION_HH

#include "globals.hh"
#include "G4VUserTrackInformation.hh"
#include "G4ThreeVector.hh"

class UserTrackInformation : public G4VUserTrackInformation {
public:
  UserTrackInformation();
  virtual ~UserTrackInformation();

  void AddStatusFlag(G4String flag);
  G4String GetStatusFlags() const;

private:
  G4String fStatusFlags;
};

#endif
