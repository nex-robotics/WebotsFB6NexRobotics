#include "FireBird6OutputPacket.hpp"

//#include "Camera.hpp"
#include "Device.hpp"
#include "DeviceManager.hpp"
#include "DifferentialWheels.hpp"
#include "Led.hpp"
#include "Sensor.hpp"
#include "SingleValueSensor.hpp"
#include "TripleValuesSensor.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

//#include <webots/camera.h>

using namespace std;

FireBird6OutputPacket::FireBird6OutputPacket() :
  Packet(64),
  mAnswerSize(0),
  mDistanceSensorRequested(false),
  mSharpDistanceSensorRequested(false),
  mGroundSensorRequested(false),
  mLightSensorRequested(false),
  mAccelerometerRequested(false),
  mGyroRequested(false),
  mMagnetometerRequested(false),
  //mCameraRequested(false),
  mEncoderRequested(false)
{
}

FireBird6OutputPacket::~FireBird6OutputPacket() {
}

void FireBird6OutputPacket::clear() {
  Packet::clear();

  mAnswerSize = 0;

  mDistanceSensorRequested = false;
  mSharpDistanceSensorRequested = false;
  mGroundSensorRequested = false;
  mLightSensorRequested = false;
  mAccelerometerRequested = false;
  mGyroRequested = false;
  mMagnetometerRequested = false;
  //mCameraRequested = false;
  mEncoderRequested = false;
}

void FireBird6OutputPacket::apply(int simulationTime) {

  // ---
  // Actuators
  // ---

  // send the differential wheels commands if required
  DifferentialWheels *dw = DeviceManager::instance()->differentialWheels();
  if (dw->isSpeedRequested()) {

	int leftSpeed = (char) (-10.66 * dw->leftSpeed() + 128);
	if (leftSpeed > 255) leftSpeed = 255;
	int rightSpeed = (char) (-10.66 * dw->rightSpeed() + 128);
	if (rightSpeed > 255) rightSpeed = 255;
    append(static_cast<char>('N'));
    append(static_cast<char>('E'));
    append(static_cast<char>('X'));
    append(static_cast<char>(0x95));
    append(static_cast<char>(leftSpeed));
    append(static_cast<char>('N'));
    append(static_cast<char>('E'));
    append(static_cast<char>('X'));
    append(static_cast<char>(0x96));
    append(static_cast<char>(rightSpeed));

	append(static_cast<char>('N'));
    append(static_cast<char>('E'));
    append(static_cast<char>('X'));
    append(static_cast<char>(0x94));
    append(static_cast<char>(0x01));

    dw->resetSpeedRequested();
	//printf("setting speed left = %d, right = %d\n", leftSpeed, rightSpeed);
  }

  if (dw->isEncoderRequested()) {
    //append(static_cast<char>(-'P'));
    //append(static_cast<short>(dw->leftEncoder()));
    //append(static_cast<short>(dw->rightEncoder()));
	
    dw->resetEncoderRequested();
  }


  // request all sensors
	append(static_cast<char>('N'));
	append(static_cast<char>('E'));
	append(static_cast<char>('X'));
	append(static_cast<char>(0x24));
	append(static_cast<char>(0x00));
  
	mAnswerSize += 54;

  // differential wheels encoder management
  if (dw->isSensorRequested()) {
    mEncoderRequested = true;

	append(static_cast<char>('N'));
	append(static_cast<char>('E'));
	append(static_cast<char>('X'));
	append(static_cast<char>(0x92));
	append(static_cast<char>(0x00));

	append(static_cast<char>('N'));
	append(static_cast<char>('E'));
	append(static_cast<char>('X'));
	append(static_cast<char>(0x93));
	append(static_cast<char>(0x00));

    mAnswerSize += 18 * sizeof(char);
  }

  //print();
  
  mDistanceSensorRequested = true;
  mSharpDistanceSensorRequested = true;
  mGroundSensorRequested = true;
  mAccelerometerRequested = true;
  mGyroRequested = true;
  mMagnetometerRequested = true;




  // camera management
  // it's better to put the camera at the end in case of
  // retrieval after transmission troubles
/*  Camera *camera = DeviceManager::instance()->camera();
  if (camera->isSensorRequested()) {
    mCameraRequested = true;
    append(static_cast<char>(-'I'));
    mAnswerSize += 3 + 2 * camera->width() * camera->height();
  }*/

  // This is require to end the packet
  // even if the size is correct
  //append('\0');
}
