/*
 * Description:  Class allowing to create or retrieve devices
 */

#ifndef DEVICE_MANAGER_HPP
#define DEVICE_MANAGER_HPP

#include <webots/types.h>

#include <vector>

class Device;
class DifferentialWheels;
class SingleValueSensor;
class TripleValuesSensor;

class DeviceManager {
public:
  static DeviceManager *instance();
  static void cleanup();

  const std::vector<Device *> &devices() const { return mDevices; }
  Device *findDeviceFromTag(WbDeviceTag tag) const;

  DifferentialWheels *differentialWheels() const { return mDifferentialWheels; }
  SingleValueSensor *distanceSensor(int at) const { return mDistanceSensors[at]; }
  SingleValueSensor *sharpDistanceSensor(int at) const { return mSharpDistanceSensors[at]; }
  SingleValueSensor *lightSensor(int at) const { return mLightSensors[at]; }
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
  SingleValueSensor *mDistanceSensors[8]; // 8 sonar distance sensors
  SingleValueSensor *mSharpDistanceSensors[8]; // 8 sharp distance sensors
  SingleValueSensor *mLightSensors[8];
  TripleValuesSensor *mAccelerometer;
  TripleValuesSensor *mGyro;
  TripleValuesSensor *mMagXY;
  TripleValuesSensor *mMagZ;
};

#endif
