#ifndef REPL_WIDGET_HPP
#define REPL_WIDGET_HPP

//QT includes
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QString>
#include <QVector>

class REPLWidget: public QWidget
{
Q_OBJECT

public:
  REPLWidget(QWidget* parent = nullptr);

private:
  QLabel* slispLabel;
  QLineEdit* replInput;

signals:
  void lineEntered(QString entry, bool isFile);

private slots:
  void changed();
};

#endif
