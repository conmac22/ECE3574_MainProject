//QT includes
#include <QLabel>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPalette>

//Module includes
#include "message_widget.hpp"

//Displays the result of the REPL input
MessageWidget::MessageWidget(QWidget* parent): QWidget(parent)
{
  //Message text
  messageLabel = new QLabel("Message:");
  //Message display
  displayMessage = new QLineEdit();
  displayMessage->setReadOnly(true);

  //Arrange in a horizontal manner
  QHBoxLayout* layout = new QHBoxLayout();
  layout->addWidget(messageLabel);
  layout->addWidget(displayMessage);
  setLayout(layout);
}

//SLOT: Displays a valid interpeter result
void MessageWidget::info(QString message)
{
  //Reset palette to default state
  QPalette pal = palette();
  displayMessage->setPalette(pal);
  //Display results
  displayMessage->setText(message);
}

//SLOT: Displays an invalid interpeter error
void MessageWidget::error(QString message)
{
  //Display results
  displayMessage->setText(message);
  //Format text to be red and highlighted
  displayMessage->selectAll();
  QPalette pal = palette();
  pal.setColor(QPalette::Background, Qt::red);
  pal.setColor(QPalette::Highlight, Qt::red);
  displayMessage->setAutoFillBackground(true);
  displayMessage->setPalette(pal);
}

void MessageWidget::clear()
{
  // TODO: your code here...
}
