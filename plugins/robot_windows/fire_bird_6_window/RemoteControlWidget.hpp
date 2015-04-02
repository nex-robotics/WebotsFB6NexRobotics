/*
 * File:          RemoteControlWidget.hpp
 * Date:          September 2010
 * Description:   Class defining the remote control interface group box
 * Author:        fabien.rohrer@cyberbotics.com
 * Modifications: - Stefania Pedrazzi - September 2012
 *                  Add upload function
 *                - Fabien Rohrer - November 2012
 *                  Add a button to enable all the sensors
 */

#ifndef REMOTE_CONTROL_GROUP_BOX_HPP
#define REMOTE_CONTROL_GROUP_BOX_HPP

#include <QtCore/QFutureWatcher>
#include <QtWidgets/QWidget>

class QComboBox;
class QPushButton;
class QGridLayout;
class QProgressDialog;
class UploaderData;

class RemoteControlWidget : public QWidget
{
  Q_OBJECT

public:
  static RemoteControlWidget *instance();
  static void clear();
  static void setParentInstance(QWidget *parent) { cParent = parent; }
  void updateValues();

public slots:
  void portsUpdated(int index);
  void populatePorts();
  void enableAllSensors();

signals:
  void progressChanged(int type, int newValue);

private:
  static RemoteControlWidget *cInstance;
  static QWidget *cParent;

  RemoteControlWidget(QWidget *parent = NULL);
  ~RemoteControlWidget();
  
  QGridLayout *mLayout;
  QComboBox *mPortsComboBox;
  QPushButton *mRefreshPortsButton;
  QPushButton *mEnableButton;
  QStringList mPorts;
  QString mPortName;
  QString mHexFileName;
  QProgressDialog *mProgressDialog;
  QProgressDialog *mPressButtonDialog;
  QProgressDialog *mConnectProgressDialog;
  QFutureWatcher<int> *mConnectFutureWatcher;
  QFutureWatcher<int> *mUploadFutureWatcher;
  
};

#endif
