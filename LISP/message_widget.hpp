#ifndef MESSAGE_WINDOW_HPP
#define MESSAGE_WINDOW_HPP

//QT includes
#include <QString>
#include <QWidget>
#include <QLabel>

class QLineEdit;

class MessageWidget: public QWidget
{
  Q_OBJECT

public:
  MessageWidget(QWidget* parent = nullptr);

private:
  QLabel* messageLabel;
  QLineEdit* displayMessage;

public slots:
  void info(QString message);
  void error(QString message);
  void clear();
};

#endif
