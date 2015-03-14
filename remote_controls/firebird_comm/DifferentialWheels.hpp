/*
 * File:         DifferentialWheels.hpp
 * Date:         June 2012
 * Description:  Abstraction of a differential wheels
 * Author:       fabien.rohrer@cyberbotics.com
 * Modifications:
 */

#ifndef DIFFERENTIAL_WHEELS_HPP
#define DIFFERENTIAL_WHEELS_HPP

#include "Sensor.hpp"

#define sgn(x) (x > 0.0) ? 1.0 : ((x < 0.0) ? -1.0 : 0.0)
#define maxspeed 0.6

class DifferentialWheels : public Sensor {
public:
  // Device Manager is responsible to create/destroy devices
  DifferentialWheels() :
    Sensor((WbDeviceTag) 0, 0),
    mLSpeed(0.0),
    mRSpeed(0.0),
    mLEncoder(0),
    mREncoder(0)
  {}
  virtual ~DifferentialWheels() {}

  double leftSpeed() const { return mLSpeed; }
  double rightSpeed() const { return mRSpeed; }
  int leftEncoder() const { return mLEncoder; }
  int rightEncoder() const { return mREncoder; }

  //void setLeftSpeed(double speed) { if(speed > maxspeed) speed = maxspeed; if(speed < -1.0 * maxspeed) speed = -1.0 * maxspeed; mLSpeed = sgn(speed)*(maxspeed - speed) * 212; }
  //void setRightSpeed(double speed) { if(speed > maxspeed) speed = maxspeed; if(speed < -1.0 * maxspeed) speed = -1.0 * maxspeed; mRSpeed = sgn(speed)*(maxspeed - speed) * 212; }
  void setLeftSpeed(double speed) { mLSpeed = speed; }
  void setRightSpeed(double speed) { mRSpeed = speed; }
  void setLeftEncoder(double speed) { mLEncoder = speed; }
  void setRightEncoder(double speed) { mREncoder = speed; }

  bool isSpeedRequested() const { return mSpeedRequested; }
  void resetSpeedRequested() { mSpeedRequested = false; }
  void setSpeedRequested() { mSpeedRequested = true; }

  bool isEncoderRequested() const { return mEncoderRequested; }
  void resetEncoderRequested() { mEncoderRequested = false; }
  void setEncoderRequested() { mEncoderRequested = true; }

private:
  double mLSpeed;
  double mRSpeed;
  int mLEncoder;
  int mREncoder;

  bool mSpeedRequested;
  bool mEncoderRequested;
};

#endif
