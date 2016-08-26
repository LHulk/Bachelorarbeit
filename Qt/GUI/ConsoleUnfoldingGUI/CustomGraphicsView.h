#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include <QObject>
#include <QGraphicsView>
#include <QMouseEvent>

class CustomGraphicsView : public QGraphicsView
{
	Q_OBJECT

public:
	CustomGraphicsView(QWidget *parent = 0);
	~CustomGraphicsView();
protected:
	virtual void mousePressEvent(QMouseEvent *event);
signals:
	void mousePressedInView(QPointF coords, int button);

};

#endif // CUSTOMGRAPHICSVIEW_H
