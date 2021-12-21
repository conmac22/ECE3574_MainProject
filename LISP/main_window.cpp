//System includes
#include <iostream>

//QT includes
#include <QVBoxLayout>

//Module includes
#include "main_window.hpp"
#include "message_widget.hpp"
#include "canvas_widget.hpp"
#include "repl_widget.hpp"
#include "interpreter_semantic_error.hpp"

//The main GUI window that contains all widgets
MainWindow::MainWindow(QWidget* parent): MainWindow("", parent)
{
}

//The main GUI window that contains all widgets
MainWindow::MainWindow(std::string filename, QWidget* parent): QWidget(parent)
{
  //Instantiate appropriate widgets
  MessageWidget* message_widget = new MessageWidget();
  CanvasWidget* canvas_widget = new CanvasWidget();
  REPLWidget* repl_widget = new REPLWidget();
  interp = new QtInterpreter();

  //Connect input signal from REPL to the slot for interpretation
  QObject::connect(repl_widget, SIGNAL(lineEntered(QString, bool)), interp, SLOT(parseAndEvaluate(QString, bool)));
  //Connect info and error signals from the QtInterpreter
  //to the info and error slots in the MessageWidget
  QObject::connect(interp, SIGNAL(info(QString)), message_widget, SLOT(info(QString)));
  QObject::connect(interp, SIGNAL(error(QString)), message_widget, SLOT(error(QString)));
  //Connect the drawGraphic signal from QtInterpreter to the
  //addGraphic slot in CanvasWidget
  QObject::connect(interp, SIGNAL(drawGraphic(QGraphicsItem*)), canvas_widget, SLOT(addGraphic(QGraphicsItem*)));

  //Pass the program to the interpreter
  if (!filename.empty())
  {
    QString str = QString::fromStdString(filename);
    interp->parseAndEvaluate(str, true);
  }

  //Set layout in a vertical manner
  QVBoxLayout* layout = new QVBoxLayout();

  layout->addWidget(message_widget);
  layout->addWidget(canvas_widget);
  layout->addWidget(repl_widget);

  setLayout(layout);
  setWindowTitle("sldraw");
}
