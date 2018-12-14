#include "tryqt.h"

MainAppWindow::MainAppWindow()
{
    QMenuBar* mainmenu = menuBar();
    QMenu* filemenu = mainmenu->addMenu(tr("&File"));
    QAction* exitaction = filemenu->addAction("E&xit");
    QObject::connect(exitaction, &QAction::triggered,
                     this, &MainAppWindow::on_close);
    //emit test_signal();
    //QObject::connect(this, &MainAppWindow::test_signal,
    //                 this, &MainAppWindow::on_close);
    show();
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    //QPushButton* button = new QPushButton("Hello, World!");
    //button->show();
    MainAppWindow window;
    return app.exec();
}
