#include "drawablesceneview.h"
#include <iostream>

DrawableSceneView::DrawableSceneView()
    :m_current_path_item()
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
    :m_current_path_item()
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
//    if( this->scene() != nullptr )
//    {
//        this->scene()->removeItem(&m_cursor_follower);
//    }
    QGraphicsView::setScene(scene);
//    this->scene()->addItem(&m_cursor_follower);
}

void DrawableSceneView::mouseMoveEvent(QMouseEvent *event)
{
//    m_cursor_follower.setPos(mapToScene(event->x(), event->y()));
    if( m_current_path_item != nullptr ) {
        std::cerr << "Drawing..." << std::endl;
        if( m_current_curve ) {
            m_current_curve->lineTo(mapToScene(event->x(), event->y()));
        } else {
            m_current_curve.reset(new QPainterPath());
            m_current_curve->moveTo(mapToScene(event->x(), event->y()));
        }
        m_current_path_item->setPath(*m_current_curve);
    }
}

void DrawableSceneView::mousePressEvent(QMouseEvent *event)
{
    if( event->button() == Qt::LeftButton ) {
        m_current_curve.reset(new QPainterPath());
        m_current_curve->moveTo(mapToScene(event->x(), event->y()));
        m_current_path_item = new QGraphicsPathItem(*m_current_curve);
        this->scene()->addItem(m_current_path_item);
    }
}

void DrawableSceneView::mouseReleaseEvent(QMouseEvent *event)
{
    if( event->button() == Qt::LeftButton ) {
        m_current_path_item = nullptr;
        m_current_curve.release();
    }
}

DrawableSceneView::~DrawableSceneView()
{
    //std::cerr << "Destroying DrawableSceneView" << std::endl;
}
