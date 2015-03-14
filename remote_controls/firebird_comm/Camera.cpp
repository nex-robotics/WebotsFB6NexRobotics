#include "Camera.hpp"

#include <webots/camera.h>

#include <sstream>
#include <iostream>

using namespace std;

Camera::Camera(WbDeviceTag tag) :
  Sensor(tag, 0),
  mModel(PO3030K_V1)
{
  mWidth = wb_camera_get_width(tag);
  mHeight = wb_camera_get_height(tag);

  if (mWidth > 127)
    cerr << "Camera resolution problem: the width must be smaller than 128" << endl;
  if (mHeight > 127)
    cerr << "Camera resolution problem: the height must be smaller than 128" << endl;
  if (mWidth * mHeight > 52*39)
    cerr << "Camera resolution problem: the resolution " << mWidth << "x" << mHeight << " is bigger than the buffer size" << endl;

  computeMode();
  computeZoom();
}

void Camera::setModel(int model) {
  switch (model) {
    case PO3030K_V1:
    case PO3030K_V2:
    case PO6030K:
      mModel = static_cast<Model>(model);
      return;
    default:
      mModel = PO3030K_V1;
      cerr << "Camera problem: model unknown" << endl;
  }
}

std::string Camera::generateInitialisationCommand() const {
  stringstream s;
  
  if (model() == PO3030K_V1)
    s << "J," << mode() << "," << height() << "," << width() << "," << zoom() << "\n";
  else
    s << "J," << mode() << "," << width() << "," << height() << "," << zoom() << "\n";

  return s.str();
}

void Camera::computeMode() {
  if (mHeight == 1)
    mMode = 3;
  else
    mMode = 1;
}

void Camera::computeZoom() {
  if (mMode == 3)
    mZoom = 480 / mWidth;
  else {
    mZoom = 1;
    if ( (double) mWidth / 480 > (double) mHeight / 640) {
      // dynamic zoom
      mZoom = 480 / mWidth;
      while (mZoom * mWidth > 480)
        mZoom--;

      //search the biggest power of 2 below the value
      int iter = 0;
      while (mZoom > 1) {
        mZoom /= 2;
        iter++;
      }
      mZoom = 1 << iter;
      if (mZoom < 1)
        mZoom = 1;
    }
    else {
      // dynamic zoom
      mZoom = 640 / mHeight;
      while (mZoom * mHeight > 640)
        mZoom--;

      //search the biggest power of 2 below the value
      int iter = 0;
      while (mZoom > 1) {
          mZoom /= 2;
          iter++;
      }
      mZoom = 1 << iter;
      if (mZoom < 1)
        mZoom = 1;
    }
  }
}

bool Camera::rawToBgraImage(unsigned char *bgraImage, const unsigned char *rawImage, int mode, int wh1, int wh2) const {
  int width  = wh1;
  int height = wh2;

  if (model() == PO3030K_V1) {
    width  = wh2;
    height = wh1;
  }

  if (this->mode() != mode || this->width() != width || this->height() != height) {
    cerr << "The received image doesn't match with the initialization sent" << endl;
    return false;
  }

  int index;
  int counter = 0;
  unsigned char red, green, blue;

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      if (model() == PO3030K_V2)
        index = 4 * (j + i * height);
      else if (model() == PO6030K)
        index = 4 * ((height - 1 - j) + (width - 1 - i) * height);
      else // model() == PO3030K_V1
        index = 4 * ((width - 1 - i) + j * width);

      red    =  rawImage[counter  ] & 0xf8;
      green  = (rawImage[counter++]         << 5);
      green += (rawImage[counter  ] & 0xf8) >> 3;
      blue   =  rawImage[counter++]         << 3;

      bgraImage[index    ] = blue;
      bgraImage[index + 1] = green;
      bgraImage[index + 2] = red;
      bgraImage[index + 3] = 0xFF;
    }
  }

  return true;
}
