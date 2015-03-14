/*
 * File:         Led.hpp
 * Date:         June 2012
 * Description:  Abstraction of a LED
 * Author:       fabien.rohrer@cyberbotics.com
 * Modifications:
 */

#ifndef LED_HPP
#define LED_HPP

#include "Device.hpp"

class Led : public Device {
public:
  // Device Manager is responsible to create/destroy devices
  Led(WbDeviceTag tag, int index) :
    Device(tag, index),
    mState(false),
    mLedRequested(false)
  {}
  virtual ~Led() {}

  bool state() const { return mState; }
  void setState(bool state) { mState = state; }

  bool isLedRequested() const { return mLedRequested; }
  void resetLedRequested() { mLedRequested = false; }
  void setLedRequested() { mLedRequested = true; }

private:
  bool mState;
  bool mLedRequested;
};

#endif
