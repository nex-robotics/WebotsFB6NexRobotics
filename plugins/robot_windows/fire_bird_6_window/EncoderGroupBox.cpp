#include "EncoderGroupBox.hpp"
#include "FireBird6Representation.hpp"

#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

// prefixes of the mLabels
static const QString prefixes[2] = { QString("right = "), QString("left = ") };

// constructor
EncoderGroupBox::EncoderGroupBox(QWidget *parent):
  QGroupBox(parent)
{
  // set the mLabels into this widget
  mVBox = new QVBoxLayout;
  for (int i=0; i<2; i++) {
    mLabels[i] = new QLabel;
    mLabels[i]->setObjectName("blueLabel");
    mVBox->addWidget(mLabels[i]);
  }
  setLayout(mVBox);

  // set the title
  setTitle("Encoders");
}

// destructor
EncoderGroupBox::~EncoderGroupBox() {
  delete mVBox;
  for (int i=0; i<2; i++)
    delete mLabels[i];
}

// update the mLabels
void EncoderGroupBox::updateValues() {
  FireBird6Representation *fireBird6 = FireBird6Representation::instance();

  bool enable = fireBird6->areEncodersEnabled();
  setEnabled(enable);
  if (!enable)
    return;

  double encoders[] = {
    fireBird6->leftEncoderValue(),
    fireBird6->rightEncoderValue()
  };
  for (int i=0; i<2; i++)
    mLabels[i]->setText(prefixes[i] + QString::number((int) encoders[i]));
}
