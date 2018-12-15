#ifndef DRAWABLESCENEVIEW_H
#define DRAWABLESCENEVIEW_H

#include <QtGui>
#include <QtWidgets/QtWidgets>

class DrawableSceneView : public QGraphicsView
{
public:
    DrawableSceneView();
    DrawableSceneView(QGraphicsScene* scene);
    void setScene(QGraphicsScene* scene);
private:
    QGraphicsPathItem m_cursor_follower;
protected:
    virtual void mouseMoveEvent(QMouseEvent *event);
};

#endif // DRAWABLESCENEVIEW_H
