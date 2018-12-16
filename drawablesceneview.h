#ifndef DRAWABLESCENEVIEW_H
#define DRAWABLESCENEVIEW_H

#include <QtGui>
#include <QtWidgets/QtWidgets>

class DrawableSceneView : public QGraphicsView
{
public:
    DrawableSceneView();
    DrawableSceneView(QGraphicsScene* scene);
    virtual ~DrawableSceneView();
    void setScene(QGraphicsScene* scene);
private:
    QGraphicsPathItem m_cursor_follower;
    std::unique_ptr<QPainterPath> m_current_curve;
    QGraphicsPathItem* m_current_path_item;    // Owned by the scene at all times
protected:
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
};

#endif // DRAWABLESCENEVIEW_H
