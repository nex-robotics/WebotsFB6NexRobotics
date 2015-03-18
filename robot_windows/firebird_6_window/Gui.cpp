#include "Gui.hpp"
#include "RemoteControlWidget.hpp"
#include "MainWidget.hpp"
#include "AccelerometerGroupBox.hpp"
#include "GyroGroupBox.hpp"
#include "MagnetometerGroupBox.hpp"
#include "EncoderGroupBox.hpp"
#include "FireBird6Representation.hpp"

#include <webots/robot.h>

#include <QtWidgets/QGridLayout>

using namespace webotsQtUtils;

// constructor
Gui::Gui() :
  MainWindow()
{
  // create the widgets
  QWidget *containerWidget = new QWidget(this);
  RemoteControlWidget::setParentInstance(this);
  mRemoteControlWidget = RemoteControlWidget::instance();
  mAccelerometerGroupBox = new AccelerometerGroupBox(this);
  mEncoderGroupBox = new EncoderGroupBox(this);
  mGyroGroupBox = new GyroGroupBox(this);
  mMagnetometerGroupBox = new MagnetometerGroupBox(this);
  
  int rowsNumber = 2;

  mMainWidget = new MainWidget(this);
  
  // place them into the grid mLayout
  mLayout = new QGridLayout;
  mLayout->addWidget(mMainWidget, 0, 0, rowsNumber, 1);
  mLayout->addWidget(mRemoteControlWidget, rowsNumber, 0);
  mLayout->addWidget(mEncoderGroupBox, 2, 1);
  mLayout->addWidget(mAccelerometerGroupBox, 0, 2);
  mLayout->addWidget(mGyroGroupBox, 1, 2);
  mLayout->addWidget(mMagnetometerGroupBox, 2, 2);

  containerWidget->setLayout(mLayout);
  
  setCentralWidget(containerWidget);
  
  // set the title
  QString title("Firebird 6 viewer (");
  title += wb_robot_get_name();
  title += ")";
  setWindowTitle(title);
  
  // defining the style sheet
  setStyleSheet(
    "QGroupBox {"
    "  border: 2px groove darkGray;"
    "  border-radius: 8px;"
    "  margin-top: 1ex;"
    "}"
    "QGroupBox::title {"
    "  subcontrol-origin: margin;"
    "  subcontrol-position: top center;"
    "}"
    "QLabel#redLabel {"
    "  color: #C80000;"
    "}"
    "QLabel#greenLabel {"
    "  color: #00C800;"
    "}"
    "QLabel#blueLabel {"
    "  color: #0000C8;"
    "}"
    "QLabel#blackLabel {"
    "  color: black;"
    "}"
    "QLabel#redLabel:disabled, "
    "QLabel#greenLabel:disabled, "
    "QLabel#blueLabel:disabled, "
    "QLabel#blackLabel:disabled "
    "{"
    "  color: gray;"
    "}"
  );
}

// destructor
Gui::~Gui() {
  RemoteControlWidget::clear();
}

// update the Gui
void Gui::updateValues() {
  mMainWidget->updateValues();
  mRemoteControlWidget->updateValues();
  mAccelerometerGroupBox->updateValues();
  mGyroGroupBox->updateValues();
  mMagnetometerGroupBox->updateValues();
  mEncoderGroupBox->updateValues();
}
