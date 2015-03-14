/*
 * File:         DeviceManager.hpp
 * Date:         June 2012
 * Description:  Class allowing to create or retrieve devices
 * Author:       fabien.rohrer@cyberbotics.com
 * Modifications:
 */

#ifndef DEVICE_MANAGER_HPP
#define DEVICE_MANAGER_HPP

#include <webots/types.h>

#include <vector>

class Device;
class DifferentialWheels;
//class Camera;
//class Led;
class SingleValueSensor;
class TripleValuesSensor;

class DeviceManager {
public:
  static DeviceManager *instance();
  static void cleanup();

  const std::vector<Device *> &devices() const { return mDevices; }
  Device *findDeviceFromTag(WbDeviceTag tag) const;

  DifferentialWheels *differentialWheels() const { return mDifferentialWheels; }
  //Camera *camera() const { return mCamera; }
  //Led *led(int at) const { return mLeds[at]; }
  SingleValueSensor *distanceSensor(int at) const { return mDistanceSensors[at]; }
  SingleValueSensor *sharpDistanceSensor(int at) const { return mSharpDistanceSensors[at]; }
  SingleValueSensor *lightSensor(int at) const { return mLightSensors[at]; }
  //SingleValueSensor *groundSensor(int at) const { return mGroundSensors[at]; }
  TripleValuesSensor *accelerometer() const { return mAccelerometer; }
  TripleValuesSensor *gyroscope() const { return mGyro; }
  TripleValuesSensor *magnetometerXY() const { return mMagXY; }
  TripleValuesSensor *magnetometerZ() const { return mMagZ; }

  void apply(int simulationTime);

private:
  static DeviceManager *cInstance;

  DeviceManager();
  virtual ~DeviceManager();

  void clear();

  std::vector<Device *> mDevices;
  DifferentialWheels *mDifferentialWheels;
  //Camera *mCamera;
  //Led *mLeds[10];
  SingleValueSensor *mDistanceSensors[8]; // 8 sonar distance sensors
  SingleValueSensor *mSharpDistanceSensors[8]; // 8 sharp distance sensors
  SingleValueSensor *mLightSensors[8];
  //SingleValueSensor *mGroundSensors[8];
  TripleValuesSensor *mAccelerometer;
  TripleValuesSensor *mGyro;
  TripleValuesSensor *mMagXY;
  TripleValuesSensor *mMagZ;
};

#endif
