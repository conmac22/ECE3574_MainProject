#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

//System includes
#include <string>

//QT includes
#include <QWidget>

//Module includes
#include "qt_interpreter.hpp"

class MainWindow: public QWidget
{
  Q_OBJECT

public:
  MainWindow(QWidget* parent = nullptr);
  MainWindow(std::string filename, QWidget* parent = nullptr);

private:
  QtInterpreter* interp;
};

#endif
