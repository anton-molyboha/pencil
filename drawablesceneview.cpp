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
    setAttribute(Qt::WA_AcceptTouchEvents);
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
    setAttribute(Qt::WA_AcceptTouchEvents);
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

bool DrawableSceneView::event(QEvent *event)
{
    switch(event->type())
    {
    case QEvent::TouchBegin:
        return touchBeginEvent(static_cast<QTouchEvent *>(event));
    case QEvent::TouchUpdate:
        return touchUpdateEvent(static_cast<QTouchEvent *>(event));
    case QEvent::TouchEnd:
        return touchEndEvent(static_cast<QTouchEvent *>(event));
    default:
        return QGraphicsView::event(event);
    }
}

void DrawableSceneView::mouseMoveEvent(QMouseEvent *event)
{
//    m_cursor_follower.setPos(mapToScene(event->x(), event->y()));
    if( m_current_path_item != nullptr ) {
        //std::cerr << "Drawing..." << std::endl;
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

bool DrawableSceneView::touchBeginEvent(QTouchEvent *event)
{
    QList<QTouchEvent::TouchPoint> touchPoints = event->touchPoints();
    foreach (const QTouchEvent::TouchPoint &touchPoint, touchPoints) {
        if(touchPoint.state() != Qt::TouchPointStationary) {
            QPointF pos = mapToScene(touchPoint.pos().toPoint());
            scene()->addEllipse(pos.x() - 5, pos.y() - 5, 10, 10);
        }
    }
    return true;
}

bool DrawableSceneView::touchUpdateEvent(QTouchEvent *event)
{
    QList<QTouchEvent::TouchPoint> touchPoints = event->touchPoints();
    foreach (const QTouchEvent::TouchPoint &touchPoint, touchPoints) {
        if(touchPoint.state() != Qt::TouchPointStationary) {
            QPointF pos = mapToScene(touchPoint.pos().toPoint());
            scene()->addEllipse(pos.x() - 5, pos.y() - 5, 10, 10);
        }
    }
    return true;
}

bool DrawableSceneView::touchEndEvent(QTouchEvent *event)
{
    return true;
}

DrawableSceneView::~DrawableSceneView()
{
    //std::cerr << "Destroying DrawableSceneView" << std::endl;
}
