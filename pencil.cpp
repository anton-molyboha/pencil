#include "pencil.h"
#include <iostream>
#include <QSvgGenerator>

MainAppWindow::MainAppWindow()
    :
     m_scene(),
     m_view(&m_scene)
{
    QMenuBar* mainmenu = menuBar();
    QMenu* filemenu = mainmenu->addMenu(tr("&File"));
    QAction* saveaction = filemenu->addAction("&Save");
    QAction* exitaction = filemenu->addAction("E&xit");
    QObject::connect(exitaction, &QAction::triggered,
                     this, &MainAppWindow::on_close);
    QObject::connect(saveaction, &QAction::triggered,
                     this, &MainAppWindow::on_save);

    setCentralWidget(&m_view);

    // Sample path in the scene
    m_scene.addPath(samplePath());

    //emit test_signal();
    //QObject::connect(this, &MainAppWindow::test_signal,
    //                 this, &MainAppWindow::on_close);

    //// Mouse tracking
    setMouseTracking(true);

    show();
}

QPainterPath MainAppWindow::samplePath()
{
    QPainterPath res;
    // Face
    res.moveTo(150, 200);
    res.cubicTo(125, 200, 100, 175, 100, 150);
    res.cubicTo(100, 125, 125, 150, 150, 100);
    res.cubicTo(175, 100, 200, 125, 200, 150);
    res.cubicTo(200, 175, 175, 200, 150, 200);
    // Smile
    res.moveTo(175, 175);
    res.cubicTo(160, 190, 140, 190, 125, 175);
    // Nose
    res.moveTo(150, 125);
    res.lineTo(150, 175);
    // Left eye
    res.moveTo(120, 140);
    res.cubicTo(124, 144, 126, 144, 130, 140);
    // Right eye
    res.moveTo(170, 140);
    res.cubicTo(174, 144, 176, 144, 180, 140);
    return res;
}

void MainAppWindow::mouseMoveEvent(QMouseEvent *event)
{
}

std::ostream& operator << (std::ostream& strm, const QRectF& rect)
{
    strm << "(" << rect.left() << ", " << rect.top() << ") - (" << rect.right() << ", " << rect.bottom() << ")";
    return strm;
}

std::ostream& operator << (std::ostream& strm, const QTransform& transform)
{
    return strm << "((" << transform.m11() << ", " << transform.m12() << ", " << transform.m13() << "), ("
                        << transform.m21() << ", " << transform.m22() << ", " << transform.m23() << "), ("
                        << transform.m31() << ", " << transform.m32() << ", " << transform.m33() << "))";
}

void MainAppWindow::on_save()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Image"), ".", tr("Scalable Vector Graphics (*.svg)"));
    if( fileName != "" ) {
        QSvgGenerator svg;
        svg.setFileName(fileName);
        QRectF bndRect = m_view.scene()->itemsBoundingRect();
        QRectF drawRect = bndRect;
        drawRect.moveTo(0, 0);
//        svg.setSize(drawRect.size().toSize());
        svg.setViewBox(drawRect);
        std::cerr << bndRect << std::endl;
        std::cerr << drawRect << std::endl;
        std::cerr << m_view.transform() << std::endl;
        QPainter painter;
        painter.begin(&svg);
        //m_view.scene()->render(&painter, QRectF(), bndRect.toRect());
        m_view.scene()->render(&painter, drawRect.toRect(), bndRect.toRect());
        painter.end();
    }
}

MainAppWindow::~MainAppWindow()
{
    //std::cerr << "Destroying MainAppWindow" << std::endl;
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    //QPushButton* button = new QPushButton("Hello, World!");
    //button->show();
    MainAppWindow window;
    return app.exec();
}
