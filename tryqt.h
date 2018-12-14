#ifndef TRYQT_H
#define TRYQT_H

#include <QtGui>
#include <QtWidgets/QtWidgets>

class MainAppWindow: public QMainWindow
{
    Q_OBJECT
public:
    MainAppWindow();
    virtual ~MainAppWindow(){}
signals:
    //void test_signal();
public slots:
    void on_close()
    {
        close();
    }
};

#endif // TRYQT_H
