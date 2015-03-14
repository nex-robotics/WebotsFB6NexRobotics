#include "DeviceManager.hpp"

#include "Device.hpp"
#include "DifferentialWheels.hpp"
//#include "Camera.hpp"
#include "Led.hpp"
#include "SingleValueSensor.hpp"
#include "TripleValuesSensor.hpp"

#include <webots/robot.h>
#include <webots/device.h>

#include <cstdio>

using namespace std;

DeviceManager *DeviceManager::cInstance = NULL;

DeviceManager *DeviceManager::instance() {
  if (!cInstance)
    cInstance = new DeviceManager();
  return cInstance;
}

void DeviceManager::cleanup() {
  if (cInstance) {
    delete cInstance;
    cInstance = NULL;
  }
}

DeviceManager::DeviceManager() {
  clear();

  mDifferentialWheels = new DifferentialWheels();
  mDevices.push_back(mDifferentialWheels);

  //mCamera = new Camera(wb_robot_get_device("camera"));
  //mDevices.push_back(mCamera);
  
  mMagZ = new TripleValuesSensor(wb_robot_get_device("compassZ_01"), 0);
  mDevices.push_back(mMagZ);
  mMagXY = new TripleValuesSensor(wb_robot_get_device("compassXY_01"), 0);
  mDevices.push_back(mMagXY);
  

  mGyro = new TripleValuesSensor(wb_robot_get_device("gyro_01"), 0);
  mDevices.push_back(mGyro);

  mAccelerometer = new TripleValuesSensor(wb_robot_get_device("accelerometer_01"), 0);
  mDevices.push_back(mAccelerometer);

 /* for (int i=0; i<8; i++) {
    char name[4] = "gs0";
    name[2] += i;
    mGroundSensors[i] = new SingleValueSensor(wb_robot_get_device(name), i);
    mDevices.push_back(mGroundSensors[i]);
  }*/

  for (int i=0; i<8; i++) {
    char name[4] = "ps0";
    name[2] += i;
    mDistanceSensors[i] = new SingleValueSensor(wb_robot_get_device(name), i);
    mDevices.push_back(mDistanceSensors[i]);
  }

  for (int i=0; i<8; i++) {
    char name[9] = "sharp_00";
    name[7] += i;
    mSharpDistanceSensors[i] = new SingleValueSensor(wb_robot_get_device(name), i);
    mDevices.push_back(mSharpDistanceSensors[i]);
  }
  
  /*// init optional devices
  for (int i = 0; i<8; i++) {
    mGroundSensors[i] = NULL;
  }*/
  
  /*WbDeviceTag tag;
  WbNodeType deviceType;
  int groundSensorIndex, matchedItems = 0;
  int numberOfDevices = wb_robot_get_number_of_devices();
  for (int index = 0; index < numberOfDevices; index++) {
    tag = wb_robot_get_device_by_index(index);
    deviceType = wb_device_get_type(tag);
      
    if (deviceType == WB_NODE_DISTANCE_SENSOR) {
      const char *deviceName = wb_device_get_name(tag);
      matchedItems = sscanf(deviceName, "gs%d", &groundSensorIndex);
      if (matchedItems > 0) {
        // init ground sensors
        if (groundSensorIndex < 3 && ! mGroundSensors[groundSensorIndex]) {
          mGroundSensors[groundSensorIndex] = new SingleValueSensor(tag, groundSensorIndex);
          mDevices.push_back(mGroundSensors[groundSensorIndex]);
        }
      }
    }
  }*/
}

DeviceManager::~DeviceManager() {
  clear();
}

Device *DeviceManager::findDeviceFromTag(WbDeviceTag tag) const {
  vector<Device *>::const_iterator it;
  for (it = mDevices.begin(); it < mDevices.end(); it++) {
    Device *d = *it;
    if (d->tag() == tag)
      return d;
  }
  return NULL;
}

void DeviceManager::clear() {
  vector<Device *>::const_iterator it;
  for (it = mDevices.begin(); it < mDevices.end(); it++)
    delete *it;

  mDevices.clear();

  mDifferentialWheels = NULL;
  //mCamera = NULL;
  mAccelerometer = NULL;
  mGyro = NULL;
  mMagXY = NULL;
  mMagZ = NULL;

  //for (int i=0; i<10; i++)
    //mLeds[i] = NULL;
  //for (int i=0; i<8; i++)
    //mGroundSensors[i] = NULL;
  for (int i=0; i<8; i++) {
    mDistanceSensors[i] = NULL;
    mLightSensors[i] = NULL;
  }
}

void DeviceManager::apply(int simulationTime) {
  vector<Device *>::const_iterator it;

  // check if some sensors need to be requested
  for (it=mDevices.begin() ; it < mDevices.end(); it++) {
    Device *d = *it;
    Sensor *s = dynamic_cast<Sensor *>(d);

    if (s && s->isEnabled() && s->lastRefreshTime() + s->rate() <= simulationTime) {
      s->setLastRefreshTime(simulationTime);
      s->setSensorRequested();
    }
  }
}
