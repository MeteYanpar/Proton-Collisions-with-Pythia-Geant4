#include "UserTrackInformation.hh"

UserTrackInformation::UserTrackInformation()
: G4VUserTrackInformation(),
  fStatusFlags("")
{}

UserTrackInformation::~UserTrackInformation() {}

void UserTrackInformation::AddStatusFlag(G4String flag) {
  if (fStatusFlags.length() > 0) {
    fStatusFlags += " ";
  }
  fStatusFlags += flag;
}

G4String UserTrackInformation::GetStatusFlags() const {
  return fStatusFlags;
}

