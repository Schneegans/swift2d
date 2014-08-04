////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/steam/Steam.hpp>

#include <swift2d/utils/Logger.hpp>

#include <iostream>
#include <steam/steam_api.h>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Steam::Steam() {
  if (SteamAPI_Init()) {
    Logger::LOG_MESSAGE << "Initialized Steam." << std::endl;
  } else {
    Logger::LOG_WARNING << "Failed to initialized Steam!" << std::endl;
  }
}

////////////////////////////////////////////////////////////////////////////////

Steam::~Steam() {
  Logger::LOG_MESSAGE << "Shutting down steam connection." << std::endl;
  SteamAPI_Shutdown();
}

////////////////////////////////////////////////////////////////////////////////

void Steam::init() {

  if (SteamAPI_IsSteamRunning()) {
    Logger::LOG_MESSAGE << "Steam is running." << std::endl;
  } else {
    Logger::LOG_MESSAGE << "Steam is not running." << std::endl;
  }

  // if (SteamAPI_RestartAppIfNecessary(111)) {
  //   Logger::LOG_MESSAGE << "Game was not started through steam. Restarting!" << std::endl;
  //   return 0;
  // } else {
  //   Logger::LOG_MESSAGE << "Game was started through steam." << std::endl;
  // }


  Logger::LOG_MESSAGE << "Welcome " << SteamFriends()->GetPersonaName() << "!" << std::endl;

  int friends(SteamFriends()->GetFriendCount(k_EFriendFlagAll));
  for (int i(0); i<friends; ++i) {
    auto f = SteamFriends()->GetFriendByIndex(i, k_EFriendFlagAll);
    Logger::LOG_MESSAGE << "Freund " << i << ": " << SteamFriends()->GetFriendPersonaName(f) << std::endl;
  }
}

////////////////////////////////////////////////////////////////////////////////


}
