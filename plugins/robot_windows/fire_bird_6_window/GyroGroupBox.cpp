#include "GyroGroupBox.hpp"
#include "FireBird6Representation.hpp"

#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

// components
enum {X, Y, Z};

// defining the prefixes and the colors of the tree mLabels
static const QString prefixes[3] = { QString("x = "), QString("y = "), QString("z = ") };
static const QString colorLabels[4] = { QString("redLabel"), QString("greenLabel"), QString("blueLabel"), QString("blackLabel") };

// constructor
GyroGroupBox::GyroGroupBox(QWidget *parent):
  QGroupBox(parent)
{
  // create and set the mLabels
  mVBox = new QVBoxLayout;
  for (int i=0; i<3; i++) {
    mLabels[i] = new QLabel;
    mLabels[i]->setObjectName(colorLabels[i]);
    mVBox->addWidget(mLabels[i]);
  }
  setLayout(mVBox);

  // set the title
  setTitle("Gyro");
}

// destructor
GyroGroupBox::~GyroGroupBox() {
  delete mVBox;
  for (int i=0; i<3; i++)
    delete mLabels[i];
}

// update
void GyroGroupBox::updateValues() {
  FireBird6Representation *firebird6 = FireBird6Representation::instance();

  bool enable = firebird6->isGyroEnabled();
  setEnabled(enable);
  if (!enable)
    return;

  const double *gyroValues = firebird6->gyroValues();
  if (gyroValues) {
    for (int i=0; i<3; i++)
      mLabels[i]->setText(prefixes[i] + QString::number(gyroValues[i],'f',4));
  } else {
    mLabels[X]->setText("Disabled");
    mLabels[Y]->setText("");
    mLabels[Z]->setText("");
  }
}
