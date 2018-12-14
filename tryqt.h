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
private:
    QPushButton m_button;
    bool pressed;
protected:
    virtual void mouseMoveEvent(QMouseEvent *event);
signals:
    //void test_signal();
public slots:
    void on_close()
    {
        close();
    }
    void on_button_press();
};

#endif // TRYQT_H
