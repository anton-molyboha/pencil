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
    setSceneRect(0, 0, 800, 600);
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
    m_current_path_item = new QGraphicsPathItem();
    m_current_curve.reset(new QPainterPath());
    scene()->addItem(m_current_path_item);
    trace.clear();
    //
    std::vector<QTouchEvent::TouchPoint> touch;
    QList<QTouchEvent::TouchPoint> touchPoints = event->touchPoints();
    foreach (const QTouchEvent::TouchPoint &touchPoint, touchPoints) {
        touch.push_back(touchPoint);
    }
    trace.push_back(touch);
    updateCurve();
    return true;
}

bool DrawableSceneView::touchUpdateEvent(QTouchEvent *event)
{
    assert(m_current_path_item);
    std::vector<QTouchEvent::TouchPoint> touch;
    QList<QTouchEvent::TouchPoint> touchPoints = event->touchPoints();
    foreach (const QTouchEvent::TouchPoint &touchPoint, touchPoints) {
        touch.push_back(touchPoint);
    }
    trace.push_back(touch);
    updateCurve();
    return true;
}

bool DrawableSceneView::touchEndEvent(QTouchEvent *event)
{
    m_current_path_item = nullptr;
    return true;
}

void DrawableSceneView::updateCurve()
{
    bool draw = true;
    for( std::vector<QTouchEvent::TouchPoint> &touch : trace )
    {
        if( touch.size() > 1 )
        {
            draw = false;
        }
    }
    QPainterPath curve;
    {
        bool first = true;
        for( std::vector<QTouchEvent::TouchPoint> &touch : trace )
        {
            QPointF pos(0, 0);
            int cnt = 0;
            for( QTouchEvent::TouchPoint &pt : touch )
            {
                pos += pt.pos();
                ++cnt;
            }
            pos /= cnt;
            QPointF point = mapToScene(pos.toPoint());
            if( first )
            {
                curve.moveTo(point);
                first = false;
            }
            else
            {
                curve.lineTo(point);
            }
        }
    }
    if( draw )
    {
        m_current_path_item->setPen(QPen(QBrush(Qt::black), 1));
    }
    else
    {
        m_current_path_item->setPen(QPen(QBrush(Qt::white), 40));
    }
    m_current_path_item->setPath(curve);
}

DrawableSceneView::~DrawableSceneView()
{
    //std::cerr << "Destroying DrawableSceneView" << std::endl;
}
