#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QPainter>
#include <QFile>
#include <QScreen>
#include <QDebug>
#include <QSizeGrip>
#include <QGridLayout>
#include <QMouseEvent>


QDebug operator<<(QDebug debug, WINDOW_SIDE side) {
    switch (side)
    {
        case WINDOW_SIDE::TOP:
            debug  << "WINDOW_SIDE::TOP";
            break;
        case WINDOW_SIDE::LEFT:
            debug  << "WINDOW_SIDE::LEFT";
            break;
        case WINDOW_SIDE::BOTTOM:
            debug  << "WINDOW_SIDE::BOTTOM";
            break;
        case WINDOW_SIDE::RIGHT:
            debug  << "WINDOW_SIDE::RIGHT";
            break;
        case WINDOW_SIDE::TOP_LEFT:
            debug  << "WINDOW_SIDE::TOP_LEFT";
            break;
        case WINDOW_SIDE::TOP_RIGHT:
            debug  << "WINDOW_SIDE::TOP_RIGHT";
            break;
        case WINDOW_SIDE::BOTTOM_LEFT:
            debug  << "WINDOW_SIDE::BOTTOM_LEFT";
            break;
        case WINDOW_SIDE::BOTTOM_RIGHT:
            debug  << "WINDOW_SIDE::BOTTOM_RIGHT";
            break;
        case WINDOW_SIDE::UNKNOWN:
        default:
            debug  << "WINDOW_SIDE::UNKNOWN";
            break;
    }
    return debug;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setParent(nullptr);
    using namespace Qt;
    move(156,156);

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    //setWindowModality(Qt::NonModal);
   // setWindowState(WindowNoState);
    //setAttribute(WA_AlwaysStackOnTop,false);
  //  this->setWindowTitle(" ");

   // this->setWindowFlags((windowFlags() & ~Qt::WindowMinMaxButtonsHint));
   // this->setWindowFlags((windowFlags() & ~Qt::WindowShadeButtonHint));
    //auto flags = this->windowFlags();
    //flags = flags & (~WindowStaysOnTopHint);

    //this->setWindowFlags(flags);
    //this->setWindowFlags(windowFlags() & ~Qt::FramelessWindowHint);
//this->setWindowFlags(Qt::Window  | Qt::FramelessWindowHint);
    //this->setWindowFlags(Qt::SubWindow);

    QSizeGrip * sizeGrip = new QSizeGrip(this);

    QGridLayout * layout = new QGridLayout(this);
    layout->addWidget(sizeGrip, 0,0,1,1,Qt::AlignBottom | Qt::AlignRight);

    ui->setupUi(this);
    initTitleBar();
    this->setWindowTitle(" ");
    //this->setStyleSheet("QMainWindow{border:8px solid black;}");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initTitleBar()
{
         // Set the marquee effect of the title bar, you don't need to set it;
    m_titleBar = new MyTitleBar(this);
    m_titleBar->move(0, 0);

    connect(m_titleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
    connect(m_titleBar, SIGNAL(signalButtonRestoreClicked()), this, SLOT(onButtonRestoreClicked()));
    connect(m_titleBar, SIGNAL(signalButtonMaxClicked()), this, SLOT(onButtonMaxClicked()));
    connect(m_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));

   // m_titleBar->setTitleRoll();

    m_titleBar->setTitleIcon(":/odt_48_8.png");
    m_titleBar->setTitleContent(QStringLiteral("Без имени 1 - AlterOffice AText"));
    m_titleBar->setButtonType(ONLY_CLOSE_BUTTON);
    m_titleBar->setTitleWidth(this->width());
}
/*
void MainWindow::paintEvent(QPaintEvent* event)
{
    //Set the background color;
    QPainter painter(this);
    QPainterPath pathBack;
    pathBack.setFillRule(Qt::WindingFill);
    pathBack.addRoundedRect(QRect(0, 0, this->width(), this->height()), 3, 3);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.fillPath(pathBack, QBrush(QColor(255, 255, 255)));

    return QWidget::paintEvent(event);
}
*/
void MainWindow::loadStyleSheet(const QString &sheetName)
{
    qDebug() << "MainWindow::loadStyleSheet()";
    QFile file(":/Resources/" + sheetName + ".css");
    file.open(QFile::ReadOnly);
    if (file.isOpen())
    {
        QString styleSheet = this->styleSheet();
        styleSheet += QLatin1String(file.readAll());
        this->setStyleSheet(styleSheet);
    }
}

void MainWindow::onButtonMinClicked()
{
    qDebug() << "MainWindow::onButtonMinClicked()";
    showMinimized();
}

void MainWindow::onButtonRestoreClicked()
{
    qDebug() << "MainWindow::onButtonRestoreClicked()";
    QPoint windowPos;
    QSize windowSize;
    m_titleBar->getRestoreInfo(windowPos, windowSize);
    this->setGeometry(QRect(windowPos, windowSize));
}

void MainWindow::onButtonMaxClicked()
{
    qDebug() << "MainWindow::onButtonMaxClicked()";
    if (this->width() >= this->screen()->geometry().width()) {
        qDebug() <<"width max"; //уменьшается
        onButtonRestoreClicked();
        return;
    }
    else {
        qDebug() <<"width not max"; //увеличивается

    }
    m_titleBar->saveRestoreInfo(this->pos(), QSize(this->width(), this->height()));
    QScreen *screen = QGuiApplication::screens().first();
    QRect desktopRect = screen->availableGeometry();
    QRect FactRect = QRect(desktopRect.x() - 3, desktopRect.y() - 3, desktopRect.width() + 6, desktopRect.height() + 6);
    setGeometry(FactRect);
}

void MainWindow::onButtonCloseClicked()
{
    qDebug() << "MainWindow::onButtonCloseClicked()";
    close();
}

// Double-click to respond to events, mainly to achieve double-clicking the title bar to maximize and minimize operations;
void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if(this->isMaximized())
        this->showNormal();
    else
        this->showMaximized();

}

// The following three events, mousePressEvent, mouseMoveEvent, and mouseReleaseEvent, realize the effect of dragging the title bar with the mouse to move the window;
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "MyTitleBar::mousePressEvent()";
        QGuiApplication::setOverrideCursor(QCursor(Qt::ClosedHandCursor));
        m_isPressed = true;
        m_startMovePos = event->globalPos();
        qDebug() << event->windowPos();
    if(m_iResizeMode == -1)
        m_iResizeMode = whatPartOfTheWindow(event->windowPos());


    QWidget::mousePressEvent(event);
}

WINDOW_SIDE MainWindow::whatPartOfTheWindow(const QPointF& point)
{
    static const char BORDER_SIZE = 10.0;
    int deltaX = this->width() - point.x();
    int deltaY = this->height() - point.y();
    //left side
    if (point.x() <= BORDER_SIZE && point.y() >= BORDER_SIZE && deltaY >= BORDER_SIZE)
        return WINDOW_SIDE::LEFT;
    //top side
    if (point.y() <= BORDER_SIZE && point.x() >= BORDER_SIZE && deltaX >= BORDER_SIZE)
        return WINDOW_SIDE::TOP;
    //right side ++
    if (deltaX <= BORDER_SIZE && point.y() > BORDER_SIZE)
        return WINDOW_SIDE::RIGHT;
    //bottom side ++
    if (deltaY <= BORDER_SIZE && point.x() > BORDER_SIZE)
        return WINDOW_SIDE::BOTTOM;
    //top-left
    if (point.x() < BORDER_SIZE && point.y() < BORDER_SIZE)
        return WINDOW_SIDE::TOP_LEFT;
    //top-right
    if (deltaX < BORDER_SIZE && point.y() < BORDER_SIZE)
        return WINDOW_SIDE::TOP_RIGHT;
    //bottom-left
    if (point.x() < BORDER_SIZE && deltaY < BORDER_SIZE)
        return WINDOW_SIDE::BOTTOM_LEFT;
    //bottom-right
    if (deltaX < BORDER_SIZE*2 && deltaY < BORDER_SIZE*2)
        return WINDOW_SIDE::BOTTOM_RIGHT;
    //cant guess
    return WINDOW_SIDE::UNKNOWN;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << "MOVE!";
    if (!m_isPressed)
        return;
    QPoint movePoint = event->globalPos() - m_startMovePos;
    QPoint widgetPos = this->pos();
    m_startMovePos = event->globalPos();
    if(m_iResizeMode == WINDOW_SIDE::UNKNOWN)
    {
        this->move(widgetPos.x() + movePoint.x(), widgetPos.y() + movePoint.y());
        return;
    }
    QRect rect(this->geometry());
    switch (m_iResizeMode)
    {
        case WINDOW_SIDE::TOP:
            rect.setTopRight(QPoint(rect.topRight().x(), rect.topRight().y() + movePoint.y() ));
            this->setGeometry(rect);
            break;
        case WINDOW_SIDE::LEFT:
            rect.setTopLeft(QPoint(rect.topLeft().x() + movePoint.x(), rect.topLeft().y()));
            this->setGeometry(rect);
        break;
        case WINDOW_SIDE::BOTTOM:
            rect.setBottomRight(QPoint(rect.bottomRight().x(), rect.bottomRight().y() + movePoint.y() ));
            this->setGeometry(rect);
        break;
        case WINDOW_SIDE::RIGHT:
            rect.setTopRight(QPoint(rect.topRight().x() + movePoint.x(), rect.topRight().y()));
            this->setGeometry(rect);
        break;
        case WINDOW_SIDE::TOP_LEFT:
            rect.setTopLeft(QPoint(rect.topLeft().x() + movePoint.x(), rect.topLeft().y() + movePoint.y()));
            this->setGeometry(rect);
        break;

        case WINDOW_SIDE::TOP_RIGHT:
            rect.setTopRight(QPoint(rect.topRight().x() + movePoint.x(), rect.topRight().y() + movePoint.y()));
            this->setGeometry(rect);
        break;

        case WINDOW_SIDE::BOTTOM_LEFT:
            rect.setBottomLeft(QPoint(rect.bottomLeft().x() + movePoint.x(), rect.bottomLeft().y() + movePoint.y()));
            this->setGeometry(rect);
        break;
        case WINDOW_SIDE::BOTTOM_RIGHT:
            rect.setBottomRight(QPoint(rect.bottomRight().x() + movePoint.x(), rect.bottomRight().y() + movePoint.y()));
            this->setGeometry(rect);
        break;
    case WINDOW_SIDE::UNKNOWN:
    default:
        break;
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    m_iResizeMode = WINDOW_SIDE::UNKNOWN;
    qDebug() << "MyTitleBar::mouseReleaseEvent()";
    m_isPressed = false;
    QGuiApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
    return QWidget::mouseReleaseEvent(event);
}

void MainWindow::moveEvent(QMoveEvent *ev)
{
    Q_UNUSED(ev);
    qDebug() << "MainWindow::moveEvent";
}

void MainWindow::setCursor(WINDOW_SIDE side)
{

    switch (side) {

    case LEFT:
        QGuiApplication::setOverrideCursor(QCursor(Qt::SizeHorCursor));
        break;
    case TOP:
        QGuiApplication::setOverrideCursor(QCursor(Qt::SizeVerCursor));
        break;
    case BOTTOM:
        QGuiApplication::setOverrideCursor(QCursor(Qt::SizeVerCursor));
        break;
    case RIGHT:
        QGuiApplication::setOverrideCursor(QCursor(Qt::SizeHorCursor));
        break;
    case TOP_LEFT:
        QGuiApplication::setOverrideCursor(QCursor(Qt::SizeFDiagCursor));
        break;
    case TOP_RIGHT:
        QGuiApplication::setOverrideCursor(QCursor(Qt::SizeBDiagCursor));
        break;
    case BOTTOM_LEFT:
        QGuiApplication::setOverrideCursor(QCursor(Qt::SizeBDiagCursor));
        break;
    case BOTTOM_RIGHT:
        QGuiApplication::setOverrideCursor(QCursor(Qt::SizeFDiagCursor));
        break;
    case UNKNOWN:
    default:
        QGuiApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
        break;

    }
}

bool MainWindow::event(QEvent *ev)
{
    if (ev->type() == QEvent::HoverMove) {
        QHoverEvent* event = dynamic_cast<QHoverEvent*>(ev);
        if(event)
        {
            setCursor(whatPartOfTheWindow(event->posF()));
        }
    }
    QMainWindow::event(ev);
    return true;
}

