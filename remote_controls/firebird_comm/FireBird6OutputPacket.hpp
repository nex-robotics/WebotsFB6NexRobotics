/*
 * File:         FireBird6OutputPacket.hpp
 * Date:         August 2013
 * Description:  Defines a packet sent from remote control library to the Fire Bird 6
 * Author:       fabien.rohrer@cyberbotics.com
 * Modifications: Anant Malewar
 */

#ifndef FIREBIRD6_OUTPUT_PACKET_HPP
#define FIREBIRD6_OUTPUT_PACKET_HPP

#include "Packet.hpp"

class Device;

class FireBird6OutputPacket : public Packet {
public:
  FireBird6OutputPacket();
  virtual ~FireBird6OutputPacket();

  virtual void clear();

  int answerSize() const { return mAnswerSize; }

  void apply(int simulationTime);

  bool areDistanceSensorRequested() const { return mDistanceSensorRequested; }
  bool areSharpDistanceSensorRequested() const { return mSharpDistanceSensorRequested; }
  bool areLightSensorRequested() const { return mLightSensorRequested; }
  bool isAccelerometerRequested() const { return mAccelerometerRequested; }
  bool isGyroRequested() const { return mGyroRequested; }
  bool isMagnetometerRequested() const { return mMagnetometerRequested; }
  bool isEncoderRequested() const { return mEncoderRequested; }

private:
  int mAnswerSize;

  bool mDistanceSensorRequested;
  bool mSharpDistanceSensorRequested;
  bool mLightSensorRequested;
  bool mAccelerometerRequested;
  bool mGyroRequested;
  bool mMagnetometerRequested;
  bool mEncoderRequested;
};

#endif
