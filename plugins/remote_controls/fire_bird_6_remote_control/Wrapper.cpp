#include "Wrapper.hpp"

//#include "Camera.hpp"
#include "Communication.hpp"
#include "Device.hpp"
#include "DeviceManager.hpp"
#include "DifferentialWheels.hpp"
#include "FireBird6OutputPacket.hpp"
#include "FireBird6InputPacket.hpp"
#include "Sensor.hpp"
#include "Serial.hpp"
#include "Time.hpp"

#include <vector>
#include <iostream>

#include <cstdlib>
#include <cstdio>

using namespace std;

Communication *Wrapper::cCommunication = NULL;
Time *Wrapper::cTime = NULL;
bool Wrapper::cSuccess = true;
int *Wrapper::cUploadReturnValue = NULL;

void Wrapper::init() {
  cout << "In Init" << endl;
  DeviceManager::instance();
  cout << "DeviceManager::instance() : Done" << endl;

  cCommunication = new Communication;
  cout << "new Communication : Done" << endl;
}

void Wrapper::cleanup() {
  delete cCommunication;
  delete cUploadReturnValue;

  DeviceManager::cleanup();
}

bool Wrapper::start(void *arg) {
  cout << "In Start" << endl;
  if (!arg) return false;
  string port(*((const string *) arg));
  cCommunication->initialize(port);
  cTime = new Time();
  cSuccess = cCommunication->isInitialized();
  return cSuccess;
}

void Wrapper::stop() {
  if (!hasFailed())
    stopActuators();

  cCommunication->cleanup();

  if (cTime) {
    delete cTime;
    cTime = NULL;
  }
}

int Wrapper::robotStep(int step) {
  // get simulation time at the beginning of this step
  int beginStepTime = cTime->currentSimulationTime();

  // apply to sensors
  DeviceManager::instance()->apply(beginStepTime);

  // setup the output packet
  FireBird6OutputPacket outputPacket;
  outputPacket.apply(beginStepTime);

  // 3 trials before giving up
  for (int i=0; i<3; i++) {

    // send the output packet
    cSuccess = cCommunication->sendPacket(&outputPacket);
    if (!cSuccess) {
      cerr << "Failed to send packet to the Fire Bird. Retry (" << (i+1) << ")..." << endl;
      continue;
    }

    // setup and receive the input packet
    int answerSize = outputPacket.answerSize();
    FireBird6InputPacket inputPacket(answerSize);
    cSuccess = cCommunication->receivePacket(&inputPacket);
    if (!cSuccess) {
      cerr << "Failed to receive packet from the Fire Bird. Retry (" << (i+1) << ")..." << endl;
      continue;
    }
    inputPacket.decode(beginStepTime, outputPacket);

    if (cSuccess) break;
  }
  if (!cSuccess) return 0;

  // get simulation time at the end of this step
  int endStepTime = cTime->currentSimulationTime();

  // according to the step duration, either wait
  // or returns the delay
  int deltaStepTime = endStepTime - beginStepTime;
  if (deltaStepTime <= step) { // the packet is sent at time
    Time::wait(step - deltaStepTime);
    return 0;
  }
  else // the delay asked is not fulfilled
    return deltaStepTime - step;
}

void Wrapper::stopActuators() {
  // reset all the requests
  DifferentialWheels *dw = DeviceManager::instance()->differentialWheels();
  dw->resetSpeedRequested();
  dw->resetEncoderRequested();

  vector<Device *>::const_iterator it;
  const vector<Device *> &devices = DeviceManager::instance()->devices();
  for (it=devices.begin() ; it < devices.end(); it++) {
    Sensor *s = dynamic_cast<Sensor *>(*it);
    if (s)
      s->resetSensorRequested();
  }

  // reset actuators
  differentialWheelsSetSpeed(0.0, 0.0);

  // send the packet
  robotStep(0);
}

void Wrapper::setRefreshRate(WbDeviceTag tag, int rate) {
  Device *device = DeviceManager::instance()->findDeviceFromTag(tag);
  Sensor *sensor = dynamic_cast<Sensor *>(device);
  if (sensor) {
    sensor->setLastRefreshTime(0);
    sensor->setRate(rate);
  } else
    cerr << "Wrapper::setRefreshRate: unknown device" << endl;
}

void Wrapper::differentialWheelsSetSpeed(double left, double right) {
  DifferentialWheels *dw = DeviceManager::instance()->differentialWheels();
  dw->setSpeedRequested();
  dw->setLeftSpeed(left);
  dw->setRightSpeed(right);
}

void Wrapper::differentialWheelsSetEncoders(double left, double right) {
  DifferentialWheels *dw = DeviceManager::instance()->differentialWheels();
  dw->setEncoderRequested();
  dw->setLeftEncoder(left);
  dw->setRightEncoder(right);
}

void *Wrapper::findAvailablePorts(void *) {
  Serial::updatePorts();
  const vector<std::string> *comPorts = Serial::availablePorts();
  return (void *) comPorts;
}

void *Wrapper::callCustomFunction(void *args) {
  if (args == NULL) {
    return findAvailablePorts(args);
  }

  return NULL;
}
