/*
 * File:         EPuckInputPacket.hpp
 * Date:         June 2012
 * Description:  Defines a packet sending from the real e-puck to the remote control library
 * Author:       fabien.rohrer@cyberbotics.com
 * Modifications:
 */

#ifndef EPUCK_INPUT_PACKET_HPP
#define EPUCK_INPUT_PACKET_HPP

#include "Packet.hpp"

class EPuckOutputPacket;

class EPuckInputPacket : public Packet {
public:
  EPuckInputPacket(int maxSize);
  virtual ~EPuckInputPacket();

  void decode(int simulationTime, const EPuckOutputPacket &outputPacket);
};

#endif
