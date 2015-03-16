#include "AccelerometerGroupBox.hpp"
#include "FireBird6Representation.hpp"

#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

// components
enum {X, Y, Z};

// defining the prefixes and the colors of the tree mLabels
static const QString prefixes[3] = { QString("x = "), QString("y = "), QString("z = ") };
static const QString colorLabels[4] = { QString("redLabel"), QString("greenLabel"), QString("blueLabel"), QString("blackLabel") };

// constructor
AccelerometerGroupBox::AccelerometerGroupBox(QWidget *parent):
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
  setTitle("Accelerometer");
}

// destructor
AccelerometerGroupBox::~AccelerometerGroupBox() {
  delete mVBox;
  for (int i=0; i<3; i++)
    delete mLabels[i];
}

// update
void AccelerometerGroupBox::updateValues() {
  FireBird6Representation *firebird6 = FireBird6Representation::instance();
  
  bool enable = firebird6->isAccelerometerEnabled();
  setEnabled(enable);
  if (!enable)
    return;
  
  const double *accValues = firebird6->accelerometerValues();
  if (accValues) {
    for (int i=0; i<3; i++)
      mLabels[i]->setText(prefixes[i] + QString::number(accValues[i],'f',4));
  } else {
    mLabels[X]->setText("Disabled");
    mLabels[Y]->setText("");
    mLabels[Z]->setText("");
  }
}

