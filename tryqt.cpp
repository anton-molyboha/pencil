#include "tryqt.h"

MainAppWindow::MainAppWindow()
    :m_button("Push me!", this),
      pressed(false)
{
    QMenuBar* mainmenu = menuBar();
    QMenu* filemenu = mainmenu->addMenu(tr("&File"));
    QAction* exitaction = filemenu->addAction("E&xit");
    QObject::connect(exitaction, &QAction::triggered,
                     this, &MainAppWindow::on_close);
    //emit test_signal();
    //QObject::connect(this, &MainAppWindow::test_signal,
    //                 this, &MainAppWindow::on_close);

    //// Mouse tracking
    m_button.resize(2 * m_button.width(), m_button.height());
    setMouseTracking(true);
    QObject::connect(&m_button, &QPushButton::clicked,
                     this, &MainAppWindow::on_button_press);

    show();
}

void MainAppWindow::mouseMoveEvent(QMouseEvent *event)
{
    if( ! pressed )
    {
        m_button.move(event->x() - m_button.width() / 2, event->y() - m_button.height() / 2);
    }
}

void MainAppWindow::on_button_press()
{
    pressed = true;
    m_button.setText("Yay!!! Thank you!");
    m_button.setEnabled(false);
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    //QPushButton* button = new QPushButton("Hello, World!");
    //button->show();
    MainAppWindow window;
    return app.exec();
}
