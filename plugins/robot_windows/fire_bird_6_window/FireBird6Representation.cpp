#include "FireBird6Representation.hpp"

#include <webots/accelerometer.h>
#include <webots/gyro.h>
#include <webots/compass.h>
#include <webots/camera.h>
#include <webots/device.h>
#include <webots/differential_wheels.h>
#include <webots/distance_sensor.h>
#include <webots/led.h>
#include <webots/light_sensor.h>
#include <webots/robot.h>

#include <cmath>
#include <cstdio>

// singleton design pattern stuff
FireBird6Representation *FireBird6Representation::cInstance = 0;

// constructor
// init the devices according to their names
FireBird6Representation::FireBird6Representation(int dummy) {
  mGroundSensorsExist = false;

  int i;

  // init distance sensors
  char psName[] = "ps0";
  for (i = 0; i<NUMBER_OF_DISTANCE_SENSORS; i++) {
    psName[2] = '0' + i;
    mDistanceSensors[i].tag = wb_robot_get_device(psName);
    mDistanceSensors[i].dValue = -1.0;
  }

  // init sharp sensors
  char lsName[] = "sharp_00";
  for (i = 0; i<NUMBER_OF_DISTANCE_SENSORS_SHARP; i++) {
    lsName[7] = '0' + i;
    mDistanceSensors_Sharp[i].tag = wb_robot_get_device(lsName);
    mDistanceSensors_Sharp[i].dValue = -1.0;
  }

  // init the accelerometer
  mAccelerometer.tag = wb_robot_get_device("accelerometer_01");
  mAccelerometer.dValues = 0;

  // init gyro
  mGyro.tag = wb_robot_get_device("gyro_01");
  mGyro.dValues = 0;

  // init magnetometer
  mMagnetometerXY.tag = wb_robot_get_device("compassXY_01");
  mMagnetometerXY.dValues = 0;
  mMagnetometerZ.tag = wb_robot_get_device("compassZ_01");
  mMagnetometerZ.dValues = 0;

  // init the differential wheel stuff
  mEncoderValues[LEFT] = 0.0;
  mEncoderValues[RIGHT] = 0.0;
  mSpeedValues[LEFT] = 0.0;
  mSpeedValues[RIGHT] = 0.0;


  // update the available values
  update();
}

// destructor
FireBird6Representation::~FireBird6Representation() {
  // nothing to delete
}

// singleton design pattern stuff
FireBird6Representation *FireBird6Representation::instance() {
  if (!cInstance)
    cInstance = new FireBird6Representation(0);
  return cInstance;
}

// singleton design pattern stuff
void FireBird6Representation::killInstance() {
  delete cInstance;
  cInstance = 0;
}

// update the values of the different components
void FireBird6Representation::update() {

  int i;

  // update the maxbotix distance sensors state
  for (i = 0; i<NUMBER_OF_DISTANCE_SENSORS; i++) {
    if (mDistanceSensors[i].tag) {
      mDistanceSensors[i].samplingPeriod = wb_distance_sensor_get_sampling_period(mDistanceSensors[i].tag);
      if (mDistanceSensors[i].samplingPeriod > 0)
        mDistanceSensors[i].dValue = wb_distance_sensor_get_value(mDistanceSensors[i].tag);
      else
        mDistanceSensors[i].dValue = -1.0;
    }
  }

  // update the sharp distance sensors state
  for (i = 0; i<NUMBER_OF_DISTANCE_SENSORS_SHARP; i++) {
    if (mDistanceSensors_Sharp[i].tag) {
      mDistanceSensors_Sharp[i].samplingPeriod = wb_distance_sensor_get_sampling_period(mDistanceSensors_Sharp[i].tag);
      if (mDistanceSensors_Sharp[i].samplingPeriod > 0)
        mDistanceSensors_Sharp[i].dValue = wb_distance_sensor_get_value(mDistanceSensors_Sharp[i].tag);
      else
        mDistanceSensors_Sharp[i].dValue = -1.0;
    }
  }

  // update the accelerometer state
  if (mAccelerometer.tag) {
    mAccelerometer.samplingPeriod = wb_accelerometer_get_sampling_period(mAccelerometer.tag);
    if (mAccelerometer.samplingPeriod > 0)
      mAccelerometer.dValues = wb_accelerometer_get_values(mAccelerometer.tag);
    else
      mAccelerometer.dValues = 0;
  }

  // update the gyro state
  if (mGyro.tag) {
    mGyro.samplingPeriod = wb_gyro_get_sampling_period(mGyro.tag);
    if (mGyro.samplingPeriod > 0)
      mGyro.dValues = wb_gyro_get_values(mGyro.tag);
    else
      mGyro.dValues = 0;
  }

  // update the XY magnetometer state
  if (mMagnetometerXY.tag) {
    mMagnetometerXY.samplingPeriod = wb_compass_get_sampling_period(mMagnetometerXY.tag);
    if (mMagnetometerXY.samplingPeriod > 0)
      mMagnetometerXY.dValues = wb_compass_get_values(mMagnetometerXY.tag);
    else
      mMagnetometerXY.dValues = 0;
  }

  // update the Z magnetometer state
  if (mMagnetometerZ.tag) {
    mMagnetometerZ.samplingPeriod = wb_compass_get_sampling_period(mMagnetometerZ.tag);
    if (mMagnetometerZ.samplingPeriod > 0)
      mMagnetometerZ.dValues = wb_compass_get_values(mMagnetometerZ.tag);
    else
      mMagnetometerZ.dValues = 0;
  }

  // update the differential wheels stuff
  mEncodersSamplingValues = wb_differential_wheels_get_encoders_sampling_period();
  if (mEncodersSamplingValues > 0) {
    mEncoderValues[LEFT] = wb_differential_wheels_get_left_encoder();
    mEncoderValues[RIGHT] = wb_differential_wheels_get_right_encoder();
  } else {
    mEncoderValues[LEFT] = 0.0;
    mEncoderValues[RIGHT] = 0.0;
  }
  mSpeedValues[LEFT] = wb_differential_wheels_get_left_speed();
  mSpeedValues[RIGHT] = wb_differential_wheels_get_right_speed();
}

// getters

double FireBird6Representation::distanceSensorValue(int id) const {
  if (id >= 0 && id < NUMBER_OF_DISTANCE_SENSORS)
    return mDistanceSensors[id].dValue;
  return -1.0;
}

double FireBird6Representation::distanceSensorSharpValue(int id) const {
  if (id >= 0 && id < NUMBER_OF_DISTANCE_SENSORS_SHARP)
    return mDistanceSensors_Sharp[id].dValue;
  return -1.0;
}

const double *FireBird6Representation::accelerometerValues() const {
  return mAccelerometer.dValues;
}

const double *FireBird6Representation::gyroValues() const {
  return mGyro.dValues;
}

const double *FireBird6Representation::magnetometerXYValues() const {
  return mMagnetometerXY.dValues;
}

const double *FireBird6Representation::magnetometerZValues() const {
  return mMagnetometerZ.dValues;
}

const unsigned char *FireBird6Representation::cameraValues() const {
  return mCamera.image;
}

double FireBird6Representation::rightSpeed() const {
  return mSpeedValues[RIGHT];
}

double FireBird6Representation::leftSpeed() const {
  return mSpeedValues[LEFT];
}

double FireBird6Representation::rightEncoderValue() const {
  return mEncoderValues[RIGHT];
}

double FireBird6Representation::leftEncoderValue() const {
  return mEncoderValues[LEFT];
}

int FireBird6Representation::cameraWidth() const {
  return mCameraWidth;
}

int FireBird6Representation::cameraHeight() const {
  return mCameraHeight;
}

bool FireBird6Representation::isDistanceSensorEnabled(int id) const {
  bool ret = false;
  if (id >= 0 && id < NUMBER_OF_DISTANCE_SENSORS)
    ret = (mDistanceSensors[id].samplingPeriod > 0);
  return ret;
}

bool FireBird6Representation::isDistanceSensorSharpEnabled(int id) const {
  bool ret = false;
  if (id >= 0 && id < NUMBER_OF_DISTANCE_SENSORS_SHARP)
    ret = (mDistanceSensors_Sharp[id].samplingPeriod > 0);
  return ret;
}

bool FireBird6Representation::isLightSensorEnabled(int id) const {
  bool ret = false;
  if (id >= 0 && id < NUMBER_OF_LIGHT_SENSORS)
    ret = (mLightSensors[id].samplingPeriod > 0);
  return ret;
}

bool FireBird6Representation::isAccelerometerEnabled() const {
  return (mAccelerometer.samplingPeriod > 0);
}

bool FireBird6Representation::isGyroEnabled() const {
  return (mGyro.samplingPeriod > 0);
}

bool FireBird6Representation::isCameraEnabled() const {
  return (mCamera.samplingPeriod > 0);
}

bool FireBird6Representation::areEncodersEnabled() const {
  return (mEncodersSamplingValues > 0);
}

void FireBird6Representation::enableAllSensors() {
  int basicTimeStep = floor(wb_robot_get_basic_time_step());
  int i;

  for (i = 0; i<NUMBER_OF_DISTANCE_SENSORS; i++)
    wb_distance_sensor_enable(mDistanceSensors[i].tag, basicTimeStep);

  for (i = 0; i<NUMBER_OF_DISTANCE_SENSORS_SHARP; i++)
    wb_distance_sensor_enable(mDistanceSensors_Sharp[i].tag, basicTimeStep);

  wb_accelerometer_enable(mAccelerometer.tag, basicTimeStep);
  wb_gyro_enable(mGyro.tag, basicTimeStep);
  wb_compass_enable(mMagnetometerXY.tag, basicTimeStep);
  wb_compass_enable(mMagnetometerZ.tag, basicTimeStep);
  wb_differential_wheels_enable_encoders(basicTimeStep);
}
