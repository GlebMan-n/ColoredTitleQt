#ifndef MYTITLEBAR_H
#define MYTITLEBAR_H


#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QMainWindow>

enum ButtonType
{
    MIN_BUTTON = 0,         // Minimize and close buttons;
    MIN_MAX_BUTTON ,        // Minimize, maximize and close buttons;
    ONLY_CLOSE_BUTTON       // Only close button;
};

class MyTitleBar : public QWidget
{
    Q_OBJECT

public:
    MyTitleBar(QWidget *parent);
    //Here parent does not give the default value of NULL, to ensure that the parent pointer must be assigned when the MyTitleBar object is created; and the assignment is not NULL;
    ~MyTitleBar();

    // Set the background color of the title bar and whether to set the background color of the title bar to be transparent;
    void setBackgroundColor(int r, int g, int b , bool isTransparent = false);
    // Set the title bar icon;
    void setTitleIcon(QString filePath , QSize IconSize = QSize(25 , 25));
    // Set the title content;
    void setTitleContent(QString titleContent , int titleFontSize = 9);
    // Set the length of the title bar;
    void setTitleWidth(int width);
    // Set the button type on the title bar;
    void setButtonType(ButtonType buttonType);
    // Set whether the title in the title bar will scroll; you can see the effect for details;
    void setTitleRoll();
    // Set the width of the window border;
    void setWindowBorderWidth(int borderWidth);

    // Save/Get the position and size of the window before maximizing;
    void saveRestoreInfo(const QPoint point, const QSize size);
    void getRestoreInfo(QPoint& point, QSize& size);
    int whatPartOfTheWindow(const QPointF& point);

public:
    void paintEvent(QPaintEvent *event) override;
    /*void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;*/

    QMainWindow* GetTopLevelWindow();
    // Initialize the control;
    void initControl();
    // Binding of signal slots;
    void initConnections();
    // Load the style file;
    void loadStyleSheet(const QString &sheetName);

signals:
    // The signal triggered by the button;
    void signalButtonMinClicked();
    void signalButtonRestoreClicked();
    void signalButtonMaxClicked();
    void signalButtonCloseClicked();

private slots:
         // The slot triggered by the button;
    void onButtonMinClicked();
    void onButtonRestoreClicked();
    void onButtonMaxClicked();
    void onButtonCloseClicked();
    void onRollTitle();

private:
    QLabel* m_pIcon;                    // title bar icon;
    QLabel* m_pTitleContent;            // Title bar content;
    QPushButton* m_pButtonMin;          // Minimize button;
    QPushButton* m_pButtonRestore;      // Maximize restore button;
    QPushButton* m_pButtonMax;          // Maximize button;
    QPushButton* m_pButtonClose;        // Close button;


    QPushButton* test;

    // The background color of the title bar;
    int m_colorR;
    int m_colorG;
    int m_colorB;

    // maximize and minimize variables;
    QPoint m_restorePos;
    QSize m_restoreSize;
    // Variables of moving window;
    bool m_isPressed;
    QPoint m_startMovePos;
    // marquee effect clock in title bar;
    QTimer m_titleRollTimer;
    // Title bar content;
    QString m_titleContent;
    // Button type;
    ButtonType m_buttonType;
    // width of window border;
    int m_windowBorderWidth;
    // Whether the title bar is transparent;
    bool m_isTransparent;
};


#endif // MYTITLEBAR_H
