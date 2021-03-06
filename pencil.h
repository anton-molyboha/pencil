#ifndef TRYQT_H
#define TRYQT_H

#include <QtGui>
#include <QtWidgets/QtWidgets>
#include "drawablesceneview.h"

class MainAppWindow: public QMainWindow
{
    Q_OBJECT
public:
    MainAppWindow();
    virtual ~MainAppWindow();
private:
    // The declaration order is significant, don't change it
    QGraphicsScene m_scene;
    DrawableSceneView m_view;
private:
    QPainterPath samplePath();
protected:
    virtual void mouseMoveEvent(QMouseEvent *event);
signals:
    //void test_signal();
public slots:
    void on_close()
    {
        close();
    }
    void on_save();
};

#endif // TRYQT_H
