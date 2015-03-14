/*
 * File:         GroundSensorGroupBox.hpp
 * Date:         September 2010
 * Description:  Class defining the ground sensors group box
 * Author:       fabien.rohrer@cyberbotics.com
 * Modifications:
 */

#ifndef GROUND_SENSOR_GROUP_BOX_HPP
#define GROUND_SENSOR_GROUP_BOX_HPP

#include <QtWidgets/QGroupBox>

class QHBoxLayout;
class QLabel;
class GroundSensorGroupBox : public QGroupBox
{
public:
  GroundSensorGroupBox(QWidget *parent=0);
  virtual ~GroundSensorGroupBox();
  void updateValues();

protected:
  QHBoxLayout *mHBox;
  QLabel *mLabels[8];
};

#endif // GROUND_SENSOR_GROUP_BOX_HPP
