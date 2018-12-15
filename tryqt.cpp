#include "tryqt.h"

MainAppWindow::MainAppWindow()
    :
     m_scene(),
     m_view(&m_scene)
{
    QMenuBar* mainmenu = menuBar();
    QMenu* filemenu = mainmenu->addMenu(tr("&File"));
    QAction* exitaction = filemenu->addAction("E&xit");
    QObject::connect(exitaction, &QAction::triggered,
                     this, &MainAppWindow::on_close);

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

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    //QPushButton* button = new QPushButton("Hello, World!");
    //button->show();
    MainAppWindow window;
    return app.exec();
}
