//QT includes
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QKeyEvent>
#include <iostream>

//Module includes
#include "repl_widget.hpp"

//Input promt for user to enter a program
REPLWidget::REPLWidget(QWidget* parent): QWidget(parent)
{
  //Slisp prompt
  slispLabel = new QLabel("slisp>");
  //User input
  replInput = new QLineEdit();

  //Connect the key press signal to the REPL slot
  QObject::connect(replInput, SIGNAL(returnPressed()), this, SLOT(changed()));

  //Arrange in a horizontal manner
  QHBoxLayout* layout = new QHBoxLayout();
  layout->addWidget(slispLabel);
  layout->addWidget(replInput);
  setLayout(layout);
}

//SLOT: Resets the text box upon an enter or return key being pressed
//Emits the REPL signal so the message can be interpreted
void REPLWidget::changed()
{
  emit lineEntered(replInput->text(), false);
  replInput->setText("");
}
