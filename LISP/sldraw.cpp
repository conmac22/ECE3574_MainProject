//System includes
#include <string>
#include <iostream>

//QT includes
#include <QApplication>
#include <QDebug>

//Module includes
#include "main_window.hpp"

//Driver class for Project 3
int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  std::string filename;

  if (argc == 2)
  {
    filename = argv[1];
  }
  else
  {
    filename = "noFile";
  }

  if (argc > 2)
  {
    std::cerr << "Error: invalid number of arguments to sldraw" << std::endl;
    return EXIT_FAILURE;
  }

  MainWindow w(filename);
  w.setMinimumSize(800, 600);
  w.show();

  return app.exec();
}
