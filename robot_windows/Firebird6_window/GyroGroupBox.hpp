/*
 * File:         GyroGroupBox.hpp
 * Date:         September 2010
 * Description:  Class defining the accelerometer group box
 * Author:       fabien.rohrer@cyberbotics.com
 * Modifications:
 */

#ifndef GYRO_GROUP_BOX_HPP
#define GYRO_GROUP_BOX_HPP

#include <QtWidgets/QGroupBox>

class QVBoxLayout;
class QLabel;
class QColor;
class GyroGroupBox : public QGroupBox
{
public:
  GyroGroupBox(QWidget *parent=0);
  virtual ~GyroGroupBox();
  void updateValues();

protected:
  QVBoxLayout *mVBox;
  QLabel *mLabels[3];
};

#endif // GYRO_GROUP_BOX_HPP
