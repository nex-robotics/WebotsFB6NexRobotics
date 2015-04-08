#include "FireBird6InputPacket.hpp"

#include "Device.hpp"
#include "DeviceManager.hpp"
#include "DifferentialWheels.hpp"
#include "FireBird6OutputPacket.hpp"
#include "SingleValueSensor.hpp"
#include "Sensor.hpp"
#include "TripleValuesSensor.hpp"

#include <webots/remote_control.h>

#include <iostream>

#include <cstdlib>

using namespace std;

FireBird6InputPacket::FireBird6InputPacket(int maxSize) :
  Packet(maxSize, maxSize)
{
}

FireBird6InputPacket::~FireBird6InputPacket() {
}

void FireBird6InputPacket::decode(int simulationTime, const FireBird6OutputPacket &outputPacket) {
  // ---
  // Sensors
  // ---

  // the order of the sensors should match with FireBird6OutputPacket::apply()

  int currentPos = 5;

  if (outputPacket.areDistanceSensorRequested()) {
    for (int i=0; i<8; i++) {
      double value = readUCharAt(currentPos+i) * 0.05; // sonar distance in meters

      SingleValueSensor *ds = DeviceManager::instance()->distanceSensor(i);
      wbr_distance_sensor_set_value(ds->tag(), value);
      ds->resetSensorRequested();
      ds->setLastRefreshTime(simulationTime);
    }
  }
  currentPos += 8*sizeof(unsigned char); // sonars

  if (outputPacket.areSharpDistanceSensorRequested()) {
    for (int i=0; i<8; i++) {
      double value = readUCharAt(currentPos+i);

      if(value > 220) value = 220;
      if(value < 30) value = 30;
      value = ( ((12.5 * 255)/(value * 3.3)) - 0.42) / 100; // sharp sensor distance in meters

      SingleValueSensor *ds = DeviceManager::instance()->sharpDistanceSensor(i);
      wbr_distance_sensor_set_value(ds->tag(), value);
      ds->resetSensorRequested();
      ds->setLastRefreshTime(simulationTime);
    }
  }
  currentPos += 8*sizeof(unsigned char); // sharp distance sensors

  // read value of optional ground sensor
  currentPos += 8*sizeof(unsigned char); // ground / line sensors

  if (outputPacket.isAccelerometerRequested()) {

    double values[3];
    for (int i=0; i<3; i++) {
      values[i] = readShortAt(currentPos+(i*2))*9.80665/4096.0; // convert to m/sec squared
    }

    TripleValuesSensor *accelerometer = DeviceManager::instance()->accelerometer();
    wbr_accelerometer_set_values(accelerometer->tag(), values);
    accelerometer->resetSensorRequested();
  }
  currentPos += 6*sizeof(unsigned char); // accelerometer

  if (outputPacket.isGyroRequested()) {

    double values[3];
    for (int i=0; i<3; i++) {
      values[i] = readShortAt(currentPos+(i*2))*3.055555/(10000); // convert to rad per sec for range 500 DPS in L3G4200D
    }

    TripleValuesSensor *gyroscope = DeviceManager::instance()->gyroscope();
    wbr_gyro_set_values(gyroscope->tag(), values);
    gyroscope->resetSensorRequested();
  }
  currentPos += 6*sizeof(unsigned char); // gyro

  if (outputPacket.isMagnetometerRequested()) {
    double values[3];
    for (int i=0; i<2; i++) {
      values[i] = readShortAt_MSBFirst(currentPos+(i*2))/1100.0;

    // different scaling for Z Axis
    values[2] = readShortAt_MSBFirst(currentPos+(2*2))/980.0;
    }

    // set the 3 values in X, Y and Z of both XY and Z sensor.
    TripleValuesSensor *magXY = DeviceManager::instance()->magnetometerXY();
    wbr_compass_set_values(magXY->tag(), values);
    magXY->resetSensorRequested();

    TripleValuesSensor *magZ = DeviceManager::instance()->magnetometerZ();
    wbr_compass_set_values(magZ->tag(), values);
    magZ->resetSensorRequested();

  }
  currentPos += 6*sizeof(unsigned char); // magnetometer

  currentPos += 2*sizeof(unsigned char); // pot
  currentPos += 2*sizeof(unsigned char); // servo
  currentPos += 3*sizeof(unsigned char); // batt


  if (outputPacket.isEncoderRequested()) {
    int values[2];

    currentPos += 5; // size of received header for encoders
    for (int i=0; i<2; i++) {
      values[i] = readIntAt(currentPos);
      currentPos += 9;
    }

    wbr_differential_wheels_set_encoders(values[0], values[1]);

    DifferentialWheels *dw = DeviceManager::instance()->differentialWheels();
    dw->resetSensorRequested();
  }

}
