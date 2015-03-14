/*
 * File:         Communication.cpp
 * Date:         June 2012
 * Description:  Implementation of Communication.hpp functions
 * Author:       fabien.rohrer@cyberbotics.com
 * Modifications:
 */

#include "Communication.hpp"
#include "Serial.hpp"
#include "Packet.hpp"

#include <iostream>
#include <sstream>
#include <stdexcept>

#include <cstdlib>
#include <cstring>

using namespace std;

static int trial = 3;

Communication::Communication() :
  mInitialized(false),
  mSerial(NULL)
{
}

Communication::~Communication() {
  cleanup();
}

bool Communication::initialize(const string &port) {
  try
  {
    mSerial = new Serial(port);
    char *answer;
	char testCMD[5] = {0x4e, 0x45, 0x58, 0x05, 0x01};
	const char safetyOFF[5] = {0x4e, 0x45, 0x58, 0x89, 0x00};

    // Note: first message always fail
    // whatever drain / readline is performed before.
	answer = talk(testCMD);
/*
    answer = talk("V\n");
    if (!answer)
      throw runtime_error("Cannot talk with the robot");
    cout << "Running real e-puck (" << &answer[2] << ")" << endl;
    free(answer);
    
    answer = mSerial->readLine();
    free(answer);

    answer = talk("S\n");*/
    if (!answer)
      throw runtime_error("Cannot talk with the robot");

    if (!mSerial)
      throw runtime_error("Cannot talk with the robot");

	// turn off safety
	mSerial->write(safetyOFF, 5);

	cout << "Running real Fire Bird (" << &answer[0] << ")" << endl;
    free(answer);
    
    mInitialized = true;
    return true;
  }
  catch (const runtime_error &e)
  {
    cerr << "Connection failed with this error" << endl;
    cerr << e.what() << endl;
    cerr << endl;
/*    cerr << "Please check:" << endl;
    cerr << "- your Bluetooth interface is setup properly on your computer" << endl;
    cerr << "- the ID of the robot corresponds to the bluetooth connection" << endl;
    cerr << "- the robot runs the latest version of the sercom program" << endl;
    cerr << "- the robot is not too far away from the Bluetooth antenna of the computer" << endl;
    cerr << "- the robot is switched on (the green led should be on)" << endl;
    cerr << "- the battery of the robot is fully charged (the red led should off)" << endl;
    cerr << "- the robot reset button was pressed" << endl;*/
    cleanup();
    return false;
  }
}

void Communication::cleanup() {
  mInitialized = false;
  if (mSerial) {
    delete mSerial;
    mSerial = NULL;
  }
}

bool Communication::sendPacket(const Packet *packet) {
  try {
    if (mSerial) {
      int packetSize = packet->size();
      if (packetSize > 0)
        mSerial->write(packet->data(), packetSize);
      return true;
    }
  }
  catch (const runtime_error &e)
  {
    cerr << "Cannot send packet to Fire Bird: " << e.what() << endl;
  }
  return false;
}

bool Communication::receivePacket(Packet *packet) {
  try
  {
    if (mSerial) {
      int packetSize = packet->size();
      if (packetSize > 0) {
        int readed = mSerial->read(packet->data(), packetSize, true);
        if (readed != packetSize) {
          stringstream s;
          s << "Expected data not received (expected=" << packetSize << ", received=" << readed << ")" << endl;
          mSerial->drain();
          throw runtime_error(s.str());
        }
      }
      return true;
    }
  }
  catch (const runtime_error &e)
  {
    cerr << "Cannot receive packet from Fire Bird: " << e.what() << endl;
  }
  return false;
}

char *Communication::talk(const char *source) {
  static const char *errorPrefix = "Cannot talk to Fire Bird: ";
  if (!mSerial) {
    cerr << errorPrefix << "serial failed" << endl;
    return NULL;
  }

  for (int i=0; i<trial; i++) { // several trials
    try {
      char *answer = (char *) malloc(13);
	  mSerial->write(source, 5);
	  mSerial->read(answer, 13, 1);  
      if (answer[0] == 'F')
        return answer;

	  free(answer);

      throw runtime_error("Unexpected result");
    }
    catch (const runtime_error &e)
    {
      cerr << errorPrefix << e.what() << endl;
    }
    cerr << "Talk to Fire Bird failed... Retry (" << (i+1) << ")" << endl;
  }

  return NULL;
}

char *Communication::readLine() {
  static const char *errorPrefix = "Cannot read line: ";
  if (!mSerial) {
    cerr << errorPrefix << "serial failed" << endl;
    return NULL;
  }

  try {
    return mSerial->readLine();
  }
  catch (const runtime_error &e)
  {
    cerr << errorPrefix << e.what() << endl;
  }
  return NULL;
}
