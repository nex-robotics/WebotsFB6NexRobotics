/*
 * File:         Camera.hpp
 * Date:         June 2012
 * Description:  Abstraction of a camera
 * Author:       fabien.rohrer@cyberbotics.com
 * Modifications:
 */

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Sensor.hpp"

#include <string>

class Camera : public Sensor {
public:
  enum Model {
    PO3030K_V1 = 0xFFFF,
    PO3030K_V2 = 0xFFFE,
    PO6030K    = 0xFFF9
  };

  // Device Manager is responsible to create/destroy devices
  Camera(WbDeviceTag tag);
  virtual ~Camera() {}

  int width() const { return mWidth; }
  int height() const { return mHeight; }

  int mode() const { return mMode; }
  int zoom() const { return mZoom; }

  Model model() const { return mModel; }
  void setModel(int model);

  std::string generateInitialisationCommand() const;
  bool rawToBgraImage(unsigned char *bgraImage, const unsigned char *rawImage, int mode, int wh1, int wh2) const;

private:
  int mWidth;
  int mHeight;
  Model mModel;

  int mMode; // 0: B&W, 1: RGB_565, 2: YUV, 3: linear camera
  int mZoom;

  void computeMode();
  void computeZoom();
};

#endif
