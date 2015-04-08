/*
 * File:         Gui.hpp
 * Date:         September 2010
 * Description:  Class defining the robot window of the Firebird6
 * Author:       fabien.rohrer@cyberbotics.com
 * Modifications:
 */

#ifndef GUI_HPP
#define GUI_HPP

#include <gui/MainWindow.hpp>

class QGridLayout;
class QWidget;
class RemoteControlWidget;
class MainWidget;
class AccelerometerGroupBox;
class GyroGroupBox;
class MagnetometerGroupBox;
class EncoderGroupBox;

class Gui : public webotsQtUtils::MainWindow
{
public:
  Gui();
  virtual ~Gui();
  virtual void updateValues();

private:
  QGridLayout *mLayout;
  MainWidget *mMainWidget;
  RemoteControlWidget *mRemoteControlWidget;
  AccelerometerGroupBox *mAccelerometerGroupBox;
  GyroGroupBox *mGyroGroupBox;
  MagnetometerGroupBox *mMagnetometerGroupBox;
  EncoderGroupBox *mEncoderGroupBox;
};

#endif // GUI_HPP
