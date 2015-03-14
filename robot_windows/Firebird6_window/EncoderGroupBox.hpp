/*
 * File:         EncoderGroupBox.hpp
 * Date:         September 2010
 * Description:  Class defining the encoder group box
 * Author:       fabien.rohrer@cyberbotics.com
 * Modifications:
 */

#ifndef ENCODER_GROUP_BOX_HPP
#define ENCODER_GROUP_BOX_HPP

#include <QtWidgets/QGroupBox>

class QVBoxLayout;
class QLabel;
class EncoderGroupBox : public QGroupBox
{
public:
  EncoderGroupBox(QWidget *parent=0);
  virtual ~EncoderGroupBox();
  void updateValues();

protected:
  QVBoxLayout *mVBox;
  QLabel *mLabels[2];
};

#endif // ENCODER_GROUP_BOX_HPP
