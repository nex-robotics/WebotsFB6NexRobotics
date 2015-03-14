/*
 * File:         FireBird6InputPacket.hpp
 * Date:         August 2013
 * Description:  Defines a packet sent from the real Fire Bird 6 to the remote control library
 * Author:       fabien.rohrer@cyberbotics.com
 * Modifications: Anant Malewar
 */

#ifndef FIREBIRD6_INPUT_PACKET_HPP
#define FIREBIRD6_INPUT_PACKET_HPP

#include "Packet.hpp"

class FireBird6OutputPacket;

class FireBird6InputPacket : public Packet {
public:
  FireBird6InputPacket(int maxSize);
  virtual ~FireBird6InputPacket();

  void decode(int simulationTime, const FireBird6OutputPacket &outputPacket);
};

#endif
