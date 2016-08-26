#include "CustomGraphicsView.h"


CustomGraphicsView::CustomGraphicsView(QWidget *parent) : QGraphicsView(parent)
{

}


CustomGraphicsView::~CustomGraphicsView()
{

}


void CustomGraphicsView::mousePressEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton)
		emit mousePressedInView(this->mapToScene(event->pos()), 0);
	else if(event->button() == Qt::RightButton)
		emit mousePressedInView(this->mapToScene(event->pos()), 1);
}
