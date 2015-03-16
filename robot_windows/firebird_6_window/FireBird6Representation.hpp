/*
 * File:         FireBird6Representation.hpp
 * Date:         September 2010
 * Description:  Facade of an Fire Bird 6. A call of the update function updates the representation using the Webots API
 * Author:       fabien.rohrer@cyberbotics.com
 * Modifications:
 */

#ifndef FIREBIRD6_REPRESENTATION_HPP
#define FIREBIRD6_REPRESENTATION_HPP

#include <webots/types.h>

//#define NUMBER_OF_LEDS 10
#define NUMBER_OF_DISTANCE_SENSORS 8
#define NUMBER_OF_DISTANCE_SENSORS_SHARP 8
#define NUMBER_OF_LIGHT_SENSORS 8
#define NUMBER_OF_GROUND_SENSORS 8

enum {LEFT, RIGHT};

typedef struct _Device {
  WbDeviceTag tag;
  int samplingPeriod;
  union {
    int iValue;
    double dValue;
    const double *dValues;
    const unsigned char *image;
  };
} Device;

class FireBird6Representation {

public:
  static FireBird6Representation *instance();
  static void killInstance();
  void update();
  
  //int ledValue(int id) const;
  double distanceSensorValue(int id) const;
  double distanceSensorSharpValue(int id) const;
  double lightSensorValue(int id) const;
  double groundSensorValue(int id) const;
  const double *accelerometerValues() const;
  const double *gyroValues() const;
  const double *magnetometerXYValues() const;
  const double *magnetometerZValues() const;
  const unsigned char *cameraValues() const;
  double rightSpeed() const;
  double leftSpeed() const;
  double rightEncoderValue() const;
  double leftEncoderValue() const;
  int cameraWidth() const;
  int cameraHeight() const;
  
  bool isDistanceSensorEnabled(int id) const;
  bool isDistanceSensorSharpEnabled(int id) const;
  bool isLightSensorEnabled(int id) const;
  bool isGroundSensorEnabled(int id) const;
  bool isAccelerometerEnabled() const;
  bool isGyroEnabled() const;
  bool isMagnetometerXYEnabled() const;
  bool isMagnetometerZEnabled() const;
  bool isCameraEnabled() const;
  bool areEncodersEnabled() const;

  bool groundSensorsExist() { return mGroundSensorsExist; }

  void enableAllSensors();

protected:
  static FireBird6Representation *cInstance;
  
  FireBird6Representation(int);
  virtual ~FireBird6Representation();

  //Device mLeds[NUMBER_OF_LEDS];
  Device mDistanceSensors[NUMBER_OF_DISTANCE_SENSORS];
  Device mDistanceSensors_Sharp[NUMBER_OF_DISTANCE_SENSORS_SHARP];
  Device mLightSensors[NUMBER_OF_LIGHT_SENSORS];
  //Device mGroundSensors[NUMBER_OF_GROUND_SENSORS];
  Device mAccelerometer;
  Device mGyro;
  Device mMagnetometerXY;
  Device mMagnetometerZ;
  Device mCamera;

  bool mGroundSensorsExist;

  double mEncoderValues[2];
  int mEncodersSamplingValues;
  double mSpeedValues[2];
  int mCameraWidth;
  int mCameraHeight;
};

#endif
