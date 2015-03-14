/*
 * File:         EPuckOutputPacket.hpp
 * Date:         June 2012
 * Description:  Defines a packet sending from the remote control library to the e-puck
 * Author:       fabien.rohrer@cyberbotics.com
 * Modifications:
 */

#ifndef EPUCK_OUTPUT_PACKET_HPP
#define EPUCK_OUTPUT_PACKET_HPP

#include "Packet.hpp"

class Device;

class EPuckOutputPacket : public Packet {
public:
  EPuckOutputPacket();
  virtual ~EPuckOutputPacket();

  virtual void clear();

  int answerSize() const { return mAnswerSize; }

  void apply(int simulationTime);

  bool areDistanceSensorRequested() const { return mDistanceSensorRequested; }
  bool areGroundSensorRequested() const { return mGroundSensorRequested; }
  bool areLightSensorRequested() const { return mLightSensorRequested; }
  bool isAccelerometerRequested() const { return mAccelerometerRequested; }
  bool isCameraRequested() const { return mCameraRequested; }
  bool isEncoderRequested() const { return mEncoderRequested; }

private:
  int mAnswerSize;

  bool mDistanceSensorRequested;
  bool mGroundSensorRequested;
  bool mLightSensorRequested;
  bool mAccelerometerRequested;
  bool mCameraRequested;
  bool mEncoderRequested;
};

#endif
