//QT includes
#include <QWidget>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <iostream>

//Module includes
#include "canvas_widget.hpp"

//Contains the canvas that points, lines, and arcs can be drawn on
CanvasWidget::CanvasWidget(QWidget* parent): QWidget(parent)
{
  //Create canvas
  scene = new QGraphicsScene(this);
  QGraphicsView* view = new QGraphicsView(scene);

  //Display canvas
  QHBoxLayout* layout = new QHBoxLayout();
  layout->addWidget(view);
  setLayout(layout);
}

//SLOT: Adds a point, line, or arc to the scene
void CanvasWidget::addGraphic(QGraphicsItem* item)
{
  scene->addItem(item);
}
