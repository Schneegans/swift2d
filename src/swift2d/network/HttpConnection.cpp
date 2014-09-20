////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/network/HttpConnection.hpp>

#include <raknet/src/HTTPConnection2.h>
#include <raknet/src/TCPInterface.h>

#include <thread>
#include <iostream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

HttpConnection::HttpConnection()
  : tcp_(RakNet::TCPInterface::GetInstance())
  , http_(RakNet::HTTPConnection2::GetInstance()) {

  tcp_->AttachPlugin(http_);
  tcp_->Start(0,0,1);
}

////////////////////////////////////////////////////////////////////////////////

HttpConnection::~HttpConnection() {
  RakNet::HTTPConnection2::DestroyInstance(http_);
  RakNet::TCPInterface::DestroyInstance(tcp_);
}

////////////////////////////////////////////////////////////////////////////////

void HttpConnection::get(std::string const& uri, std::string const& server, unsigned short port) {
  http_->TransmitRequest(RakNet::RakString::FormatForGET(uri.c_str()), server.c_str(), port);
}

////////////////////////////////////////////////////////////////////////////////

void HttpConnection::post(std::string const& uri, std::string const& content, std::string const& server, unsigned short port) {
  http_->TransmitRequest(RakNet::RakString::FormatForPOST(uri.c_str(), "text/plain; charset=UTF-8", content.c_str()), server.c_str(), port);
}

////////////////////////////////////////////////////////////////////////////////

void HttpConnection::del(std::string const& uri, std::string const& server, unsigned short port) {
  http_->TransmitRequest(RakNet::RakString::FormatForDELETE(uri.c_str()), server.c_str(), port);
}

////////////////////////////////////////////////////////////////////////////////

void HttpConnection::update() {

  RakNet::SystemAddress sa(tcp_->HasCompletedConnectionAttempt());

  for (RakNet::Packet* packet = tcp_->Receive(); packet; tcp_->DeallocatePacket(packet), packet = tcp_->Receive());
  sa = tcp_->HasFailedConnectionAttempt();
  sa = tcp_->HasLostConnection();

  RakNet::RakString stringTransmitted;
  RakNet::RakString hostTransmitted;
  RakNet::RakString responseReceived;
  RakNet::SystemAddress hostReceived;
  int contentOffset;
  if (http_->GetResponse(stringTransmitted, hostTransmitted, responseReceived, hostReceived, contentOffset)) {
    if (!responseReceived.IsEmpty()) {
      on_response.emit(responseReceived.C_String() + contentOffset);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

}
