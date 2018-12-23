#ifndef DRAWABLESCENEVIEW_H
#define DRAWABLESCENEVIEW_H

#include <QtGui>
#include <QtWidgets/QtWidgets>
#include <chrono>

struct TouchInfo
{
    std::chrono::time_point<std::chrono::steady_clock> timestamp;
    std::vector<QTouchEvent::TouchPoint> touchpoints;
    TouchInfo();
};

class DrawableSceneView : public QGraphicsView
{
public:
    DrawableSceneView();
    DrawableSceneView(QGraphicsScene* scene);
    virtual ~DrawableSceneView();
    void setScene(QGraphicsScene* scene);
private:
    std::vector<TouchInfo> trace;
    QGraphicsPathItem m_cursor_follower;
    std::unique_ptr<QPainterPath> m_current_curve;
    QGraphicsPathItem* m_current_path_item;    // Owned by the scene at all times
    void updateCurve();
protected:
    virtual bool event(QEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual bool touchBeginEvent(QTouchEvent *event);
    virtual bool touchUpdateEvent(QTouchEvent *event);
    virtual bool touchEndEvent(QTouchEvent *event);
    virtual bool touchCancelEvent(QTouchEvent *event);
};

#endif // DRAWABLESCENEVIEW_H
