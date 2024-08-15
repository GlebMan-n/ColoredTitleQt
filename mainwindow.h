#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mytitlebar.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum WINDOW_SIDE : int {
    UNKNOWN = -1, LEFT = 1, TOP = 2, BOTTOM = 3, RIGHT = 4, TOP_LEFT = 5, TOP_RIGHT = 6, BOTTOM_LEFT = 7, BOTTOM_RIGHT = 8
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    void initTitleBar();
   // void paintEvent(QPaintEvent *event);
    void loadStyleSheet(const QString &sheetName);
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void moveEvent(QMoveEvent *ev) override;
    virtual bool event(QEvent *ev) override;

    WINDOW_SIDE whatPartOfTheWindow(const QPointF& point);
    void setCursor(WINDOW_SIDE side);
private slots:
    void onButtonMinClicked();
    void onButtonRestoreClicked();
    void onButtonMaxClicked();
    void onButtonCloseClicked();

protected:
    bool m_isPressed;
    WINDOW_SIDE m_iResizeMode = WINDOW_SIDE::UNKNOWN;
    QPoint m_startMovePos;
    QPointF m_resize;
    MyTitleBar* m_titleBar;
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
