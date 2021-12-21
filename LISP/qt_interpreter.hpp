#ifndef QT_INTERPRETER_HPP
#define QT_INTERPRETER_HPP

//System includes
#include <string>

//QT includes
#include <QObject>
#include <QString>
#include <QGraphicsItem>

//Module includes
#include "interpreter.hpp"

class QtInterpreter: public QObject, private Interpreter
{
Q_OBJECT

public:
  QtInterpreter(QObject* parent = nullptr);

private:
  Interpreter interp;

signals:
  void drawGraphic(QGraphicsItem* item);
  void info(QString message);
  void error(QString message);

public slots:
  void parseAndEvaluate(QString entry, bool isFile);
};

#endif
