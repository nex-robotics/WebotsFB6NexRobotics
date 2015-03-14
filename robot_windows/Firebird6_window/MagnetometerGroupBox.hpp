/*
 * File:         MagnetometerGroupBox.hpp
 * Date:         September 2010
 * Description:  Class defining the accelerometer group box
 * Author:       fabien.rohrer@cyberbotics.com
 * Modifications:
 */

#ifndef MAGNETOMETER_GROUP_BOX_HPP
#define MAGNETOMETER_GROUP_BOX_HPP

#include <QtWidgets/QGroupBox>

class QVBoxLayout;
class QLabel;
class QColor;
class MagnetometerGroupBox : public QGroupBox
{
public:
  MagnetometerGroupBox(QWidget *parent=0);
  virtual ~MagnetometerGroupBox();
  void updateValues();

protected:
  QVBoxLayout *mVBox;
  QLabel *mLabels[3];
};

#endif // MAGNETOMETER_GROUP_BOX_HPP
