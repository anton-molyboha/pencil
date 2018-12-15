#include "drawablesceneview.h"

DrawableSceneView::DrawableSceneView()
{
    setMouseTracking(true);
    {
        QPainterPath cursor;
        cursor.addEllipse(-5, -5, 10, 10);
        m_cursor_follower.setPath(cursor);
    }
    //scene()->addItem(&m_cursor_follower);
}

DrawableSceneView::DrawableSceneView(QGraphicsScene* scene)
{
    setMouseTracking(true);
    {
        QPainterPath cursor;
        cursor.addEllipse(-5, -5, 10, 10);
        m_cursor_follower.setPath(cursor);
    }
    setScene(scene);
}


void DrawableSceneView::setScene(QGraphicsScene* scene)
{
    if( this->scene() != nullptr )
    {
        this->scene()->removeItem(&m_cursor_follower);
    }
    QGraphicsView::setScene(scene);
    this->scene()->addItem(&m_cursor_follower);
}

void DrawableSceneView::mouseMoveEvent(QMouseEvent *event)
{
    m_cursor_follower.setPos(mapToScene(event->x(), event->y()));
}
