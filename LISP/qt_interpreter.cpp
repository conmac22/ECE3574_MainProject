//System includes
#include <string>
#include <sstream>
#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>

//QT includes
#include <QBrush>
#include <QDebug>
#include <QGraphicsEllipseItem>

//Module includes
#include "qt_interpreter.hpp"
#include "qgraphics_arc_item.hpp"
#include "interpreter_semantic_error.hpp"

//PI
#define PI (atan(1) * 4)

//Interpets the result of the interpeter and sends the appropriate signals
QtInterpreter::QtInterpreter(QObject* parent): QObject(parent)
{
}

//SLOT: Evaluates the program from the REPL signal or command line and emits
//an appropriate signal after parsing and evaluating
void QtInterpreter::parseAndEvaluate(QString entry, bool isFile)
{
  std::istream* program;
  std::string programStr = entry.toStdString();

  if (isFile)
  {
    program = new std::ifstream(programStr);
  }
  else
  {
    program = new std::istringstream(programStr);
  }

  //Emit error if program cannot be parsed
  if (!interp.parse(*program))
  {
    emit error("Error: Could not parse program");
  }
  else
  {
    try
    {
      Expression result = interp.eval();
      QString resultMessage;
      //Emit signal based on Expression type
      if (result.head.type == BooleanType)
      {
        if (result.head.value.bool_value)
        {
          resultMessage = "(True)";
        }
        else
        {
          resultMessage = "(False)";
        }
      }
      else if (result.head.type == NumberType)
      {
        resultMessage.append("(");
        QString numValue = QString::number(result.head.value.num_value);
        resultMessage.append(numValue);
        resultMessage.append(")");
      }
      else if (result.head.type == PointType)
      {
        //Display point in message
        resultMessage.append("(");
        QString x = QString::number(result.head.value.point_value.x);
        resultMessage.append(x);
        resultMessage.append(",");
        QString y = QString::number(result.head.value.point_value.y);
        resultMessage.append(y);
        resultMessage.append(")");
      }
      else if (result.head.type == LineType)
      {
        //Display line in message
        resultMessage.append("((");
        QString x1 = QString::number(result.head.value.line_value.first.x);
        resultMessage.append(x1);
        resultMessage.append(",");
        QString y1 = QString::number(result.head.value.line_value.first.y);
        resultMessage.append(y1);
        resultMessage.append(")(");
        QString x2 = QString::number(result.head.value.line_value.second.x);
        resultMessage.append(x2);
        resultMessage.append(",");
        QString y2 = QString::number(result.head.value.line_value.second.y);
        resultMessage.append(y2);
        resultMessage.append("))");
      }
      else if (result.head.type == ArcType)
      {
        //Display arc in message
        resultMessage.append("(");
        QString x1 = QString::number(result.head.value.arc_value.center.x);
        resultMessage.append(x1);
        resultMessage.append(",");
        QString y1 = QString::number(result.head.value.arc_value.center.y);
        resultMessage.append(y1);
        resultMessage.append(")(");
        QString x2 = QString::number(result.head.value.arc_value.start.x);
        resultMessage.append(x2);
        resultMessage.append(",");
        QString y2 = QString::number(result.head.value.arc_value.start.y);
        resultMessage.append(y2);
        resultMessage.append(") ");
        QString ang = QString::number(result.head.value.arc_value.span);
        resultMessage.append(ang);
        resultMessage.append(")");
      }
      //Draw on canvas if a draw command was entered
      std::vector<Atom> graphics = interp.getGraphics();
      if (!graphics.empty() && result.head.type == NoneType)
      {
        for (int i = 0; i < graphics.size(); i++)
        {
          if (graphics[i].type == PointType)
          {
            int diameter = 4;
            Number x = graphics[i].value.point_value.x - 2;
            Number y = graphics[i].value.point_value.y - 2;
            QGraphicsEllipseItem* circle = new QGraphicsEllipseItem(x, y,
              diameter, diameter);
            QBrush brush(Qt::black);
            circle->setBrush(brush);
            emit drawGraphic(circle);
          }
          else if (graphics[i].type == LineType)
          {
            Number x1 = graphics[i].value.line_value.first.x;
            Number y1 = graphics[i].value.line_value.first.y;
            Number x2 = graphics[i].value.line_value.second.x;
            Number y2 = graphics[i].value.line_value.second.y;
            QGraphicsLineItem* line = new QGraphicsLineItem(x1, y1, x2, y2);
            emit drawGraphic(line);
          }
          else if (graphics[i].type == ArcType)
          {
            Number centerX = graphics[i].value.arc_value.center.x;
      			Number centerY = graphics[i].value.arc_value.center.y;
      			Number startX = graphics[i].value.arc_value.start.x;
      			Number startY = graphics[i].value.arc_value.start.y;
            Number spanAng = graphics[i].value.arc_value.span;
            //Adjust span
            spanAng = 16 * spanAng * 180 / PI;
            Number startAng = -16 * atan2(startY - centerY, startX - centerX) * 180 / PI;
      			Number radius = sqrt(pow(startX - centerX, 2) + pow(startY - centerY, 2));
      			QGraphicsArcItem* arc = new QGraphicsArcItem(
              centerX - radius, centerY - radius, 2 * radius, 2 * radius);
      			arc->setStartAngle(startAng);
      			arc->setSpanAngle(spanAng);
      			emit drawGraphic(arc);
          }
        }
        resultMessage = "(None)";
      }
      emit info(resultMessage);
    }
    //Emit error if exception occurs when evaluating
    catch (InterpreterSemanticError& err)
    {
      QString errorMessage = "Error: ";
      errorMessage.append(err.what());
      emit error(errorMessage);
    }
  }
}
