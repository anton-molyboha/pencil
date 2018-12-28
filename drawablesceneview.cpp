#include "drawablesceneview.h"
#include <iostream>

TouchInfo::TouchInfo()
    :timestamp(std::chrono::steady_clock::now())
{}

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
    setSceneRect(-10, -10, 20, 20);
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
    case QEvent::TouchCancel:
        return touchCancelEvent(static_cast<QTouchEvent *>(event));
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
    m_original_transform = transform();
    m_original_transform_inverse = m_original_transform.inverted();
    //
    TouchInfo touch;
    QList<QTouchEvent::TouchPoint> touchPoints = event->touchPoints();
    foreach (const QTouchEvent::TouchPoint &touchPoint, touchPoints) {
        touch.touchpoints.push_back(touchPoint);
    }
    trace.push_back(touch);
    updateCurve();
    return true;
}

bool DrawableSceneView::touchUpdateEvent(QTouchEvent *event)
{
    TouchInfo touch;
    QList<QTouchEvent::TouchPoint> touchPoints = event->touchPoints();
    foreach (const QTouchEvent::TouchPoint &touchPoint, touchPoints) {
        touch.touchpoints.push_back(touchPoint);
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

bool DrawableSceneView::touchCancelEvent(QTouchEvent *event)
{
    if( m_current_path_item != nullptr ) {
        scene()->removeItem(m_current_path_item);
        delete m_current_path_item;
    }
    setTransform(m_original_transform);
    QPointF old_center = m_original_transform_inverse.map(QPointF());
    setSceneRect(old_center.x() - 10, old_center.y() - 10, 20, 20);
    return true;
}

void DrawableSceneView::updateCurve()
{
    enum class GestureType { draw, erase, scroll };
    GestureType gtype = GestureType::draw;
    for( TouchInfo &touch : trace )
    {
        if( touch.touchpoints.size() > 1 )
        {
            double diameter_sq = 0;
            for( QTouchEvent::TouchPoint &pt1 : touch.touchpoints )
            {
                for( QTouchEvent::TouchPoint &pt2 : touch.touchpoints )
                {
                    QPointF delta = pt1.pos() - pt2.pos();
                    double dist = QPointF::dotProduct(delta, delta);
                    if( dist > diameter_sq )
                    {
                        diameter_sq = dist;
                    }
                }
            }
            if( diameter_sq >= TWO_FINGERS_THRESHOLD * TWO_FINGERS_THRESHOLD )
            {
                gtype = GestureType::scroll;
            }
            else
            {
                if( gtype == GestureType::draw )
                {
                    gtype = GestureType::erase;
                }
            }
        }
    }
    QPainterPath curve;
    {
        bool first = true;
        for( TouchInfo &touch : trace )
        {
            QPointF pos(0, 0);
            int cnt = 0;
            for( QTouchEvent::TouchPoint &pt : touch.touchpoints )
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
    if( gtype == GestureType::draw )
    {
        assert(m_current_path_item);
        m_current_path_item->setPen(QPen(QBrush(Qt::black), 1));
        m_current_path_item->setPath(curve);
    }
    else if( gtype == GestureType::erase )
    {
        assert(m_current_path_item);
        m_current_path_item->setPen(QPen(QBrush(Qt::white), ERASER_SIZE));
        m_current_path_item->setPath(curve);
    }
    else if( gtype == GestureType::scroll )
    {
        // If we started drawing, remove that
        if( m_current_path_item != nullptr )
        {
            scene()->removeItem(m_current_path_item);
            delete m_current_path_item;
            m_current_path_item = nullptr;
        }
        // The point, in scene coordinates, at which we started dragging
        QPointF initial_touch;
        {
            auto first = std::find_if(trace.begin(), trace.end(), [](TouchInfo info){return info.touchpoints.size() >= 2;});
            for( QTouchEvent::TouchPoint pt : first->touchpoints )
            {
                initial_touch += pt.pos();
            }
            initial_touch /= first->touchpoints.size();
            initial_touch = m_original_transform_inverse.map(initial_touch);
        }
        // The point where we are dragging last
        QPointF current_touch;
        {
            auto last = std::find_if(trace.rbegin(), trace.rend(), [](TouchInfo info){return info.touchpoints.size() >= 2;});
            for( QTouchEvent::TouchPoint pt : last->touchpoints )
            {
                current_touch += pt.pos();
            }
            current_touch /= last->touchpoints.size();
        }
        // The transform necessary to translate the initial_touch into current_touch
        {
            QTransform transform;
            transform.translate(current_touch.x() - initial_touch.x(), current_touch.y() - initial_touch.y());
            setTransform(transform, false);
            QPointF new_center = transform.inverted().map(QPointF());
            setSceneRect(new_center.x() - 10, new_center.y() - 10, 20, 20);
        }
    }
}

DrawableSceneView::~DrawableSceneView()
{
    //std::cerr << "Destroying DrawableSceneView" << std::endl;
}

const double DrawableSceneView::ERASER_SIZE = 40;
const double DrawableSceneView::TWO_FINGERS_THRESHOLD = 150;
