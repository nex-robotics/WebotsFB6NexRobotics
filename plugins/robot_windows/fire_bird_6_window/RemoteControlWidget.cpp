#include "RemoteControlWidget.hpp"
#include "FireBird6Representation.hpp"

#include <webots/robot.h>
#include <webots/remote_control.h>

#include <QtConcurrent/QtConcurrent>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QProgressDialog>
#include <QtWidgets/QProgressBar>

#include <vector>

using namespace std;

RemoteControlWidget *RemoteControlWidget::cInstance = NULL;
QWidget *RemoteControlWidget::cParent = NULL;

RemoteControlWidget *RemoteControlWidget::instance() {
  if (! cInstance)
    cInstance = new RemoteControlWidget(cParent);

  return cInstance;
}

void RemoteControlWidget::clear() {
  delete cInstance;
  cInstance = NULL;
}

// constructor
RemoteControlWidget::RemoteControlWidget(QWidget *parent) :
  QWidget(parent),
  mProgressDialog(NULL),
  mPressButtonDialog(NULL),
  mConnectProgressDialog(NULL)
{
  mConnectFutureWatcher = new QFutureWatcher<int>(this);
  mUploadFutureWatcher = new QFutureWatcher<int>(this);
  mHexFileName = "";

  // create and set the combo box into this widget
  mLayout = new QGridLayout();
  mPortsComboBox = new QComboBox();
  mRefreshPortsButton = new QPushButton("Refresh ports");
  populatePorts();
  mEnableButton = new QPushButton("Enable all");
  mEnableButton->setToolTip("Enable all the sensors");

  mLayout->addWidget(mPortsComboBox, 0, 0);
  mLayout->addWidget(mRefreshPortsButton, 0, 1);
  mLayout->addWidget(mEnableButton, 1, 1);
  setLayout(mLayout);

  // connect the signals with their respective slots
  connect(mPortsComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(portsUpdated(int)));
  connect(mRefreshPortsButton, SIGNAL(clicked()), this, SLOT(populatePorts()));
  connect(mEnableButton, SIGNAL(clicked()), this, SLOT(enableAllSensors()));

}

// destructor
RemoteControlWidget::~RemoteControlWidget() {
  delete mPortsComboBox;
  delete mRefreshPortsButton;
  delete mLayout;
  delete mConnectFutureWatcher;
  delete mUploadFutureWatcher;
  delete mConnectProgressDialog;
  delete mProgressDialog;
  delete mPressButtonDialog;

}

// populate the ports combo box
void RemoteControlWidget::populatePorts() {
  mPortsComboBox->blockSignals(true);
  mRefreshPortsButton->blockSignals(true);

  mPorts.clear();
  mPortsComboBox->clear();

  mPortsComboBox->addItem("Simulation");
  mPortsComboBox->setCurrentIndex(0);

  void *ptr = wb_remote_control_custom_function(NULL);
  if (ptr) {
    const vector<string> *comPorts = static_cast<const vector<string> *>(ptr);
    vector<string>::const_iterator it;
    for (it=comPorts->begin() ; it < comPorts->end(); it++) {
      const string &s = *it;
      mPorts << QString::fromStdString(s);
      mPortsComboBox->addItem(QString(s.c_str()));
    }
  }

  mPortsComboBox->blockSignals(false);
  mRefreshPortsButton->blockSignals(false);
}

// update
void RemoteControlWidget::updateValues() {
  // check that the remote control is still active
  if (wb_robot_get_mode() == 0 && mPortsComboBox->currentIndex() != 0)
    mPortsComboBox->setCurrentIndex(0);
}

// handle the combo box selection
void RemoteControlWidget::portsUpdated(int index) {
  if (index == 0)
    wb_robot_set_mode(WB_MODE_SIMULATION, NULL);
  else {
    const string &port = mPortsComboBox->currentText().toStdString();
    printf("test 1 \n");
    wb_robot_set_mode(WB_MODE_REMOTE_CONTROL, (void *) &port);
    printf("test 2 \n");
  }
}

void RemoteControlWidget::enableAllSensors() {
  FireBird6Representation::instance()->enableAllSensors();
  mEnableButton->setEnabled(false);
}
