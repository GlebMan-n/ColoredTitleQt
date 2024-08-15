#include "mytitlebar.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QFile>
#include <QMouseEvent>
#include <QDebug>

#define  BUTTON_HEIGHT 30 // Button height;
#define  BUTTON_WIDTH 30 // Button width;
#define  TITLE_HEIGHT 50 // Title bar height;

MyTitleBar::MyTitleBar(QWidget *parent)
    : QWidget(parent)
    , m_colorR(67)
    , m_colorG(125)
    , m_colorB(212)
    , m_isPressed(false)
    , m_buttonType(ONLY_CLOSE_BUTTON)
    , m_windowBorderWidth(0)
    , m_isTransparent(false)
{
    // Initialization;
    initControl();
    initConnections();
    // Load the local style MyTitle.css file;
    loadStyleSheet("MyTitle");
    QColor color = Qt::white;
    QPalette palette = m_pTitleContent->palette();
    palette.setColor(QPalette::WindowText, color);
    m_pTitleContent->setPalette(palette);
}

MyTitleBar::~MyTitleBar()
{
qDebug() << "MyTitleBar::~MyTitleBar()";
}

// Initialize the control;
void MyTitleBar::initControl()
{
    qDebug() << "MyTitleBar::initControl()";
    m_pIcon = new QLabel(this);
    m_pTitleContent = new QLabel(this);

    m_pButtonMin = new QPushButton(this);
    m_pButtonRestore = new QPushButton(this);
    m_pButtonMax = new QPushButton(this);
    m_pButtonClose = new QPushButton(this);

    QString styleSheet = "QPushButton:hover:pressed { \
            padding-right: 15px; /* make room for the arrows */ \
            border-image: url(:/close_pressed.png); \
             border-width: 3;} \
          \
        QPushButton{ \
             padding-right: 15px; /* make room for the arrows */ \
            border-image: url(:/close.png); \
             border-width: 3;} \
         \
         QPushButton:hover { \
            padding-right: 15px; /* make room for the arrows */ \
                border-image: url(:/close_hover.png);  \
             border-width: 3;}";
        m_pButtonMin->setStyleSheet(styleSheet);
        m_pButtonRestore->setStyleSheet(styleSheet);
        m_pButtonMax->setStyleSheet(styleSheet);
        m_pButtonClose->setStyleSheet(styleSheet);

    connect(m_pButtonMin, SIGNAL(clicked()), this, SLOT(onButtonMinClicked()));
    connect(m_pButtonRestore, SIGNAL(clicked()), this, SLOT(onButtonRestoreClicked()));
    connect(m_pButtonMax, SIGNAL(clicked()), this, SLOT(onButtonMaxClicked()));
    connect(m_pButtonClose, SIGNAL(clicked()), this, SLOT(onButtonCloseClicked()));

    m_pButtonMin->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
    m_pButtonRestore->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
    m_pButtonMax->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
    m_pButtonClose->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));

    m_pTitleContent->setObjectName("TitleContent");
    m_pButtonMin->setObjectName("ButtonMin");
    m_pButtonRestore->setObjectName("ButtonRestore");
    m_pButtonMax->setObjectName("ButtonMax");
    m_pButtonClose->setObjectName("ButtonClose");

    m_pButtonMin->setToolTip(QStringLiteral("minimize"));
    m_pButtonRestore->setToolTip(QStringLiteral("reduction"));
    m_pButtonMax->setToolTip(QStringLiteral("maximize"));
    m_pButtonClose->setToolTip(QStringLiteral("shut down"));
    QVBoxLayout* vLayout = new QVBoxLayout(this);
    QHBoxLayout* mylayout = new QHBoxLayout();
    mylayout->addWidget(m_pIcon);
    mylayout->addWidget(m_pTitleContent);

    mylayout->addWidget(m_pButtonMin);
    mylayout->addWidget(m_pButtonRestore);
    mylayout->addWidget(m_pButtonMax);
    mylayout->addWidget(m_pButtonClose);
    mylayout->addSpacing(25);

    mylayout->setContentsMargins(0, 0, 0, 0);
    mylayout->setSpacing(75);
    vLayout->addLayout(mylayout);
    //vLayout->addSpacing(10);

    m_pTitleContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setFixedHeight(TITLE_HEIGHT);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

// Binding of signal slots;
void MyTitleBar::initConnections()
{
    qDebug() << "MyTitleBar::initConnections()";

}

// Set the background color of the title bar and draw the background color of the title bar in the paintEvent event;
// The default value is given in the constructor, you can set the color value externally to change the background color of the title bar;
void MyTitleBar::setBackgroundColor(int r, int g, int b, bool isTransparent)
{
    qDebug() << "MyTitleBar::setBackgroundColor()";
    m_colorR = r;
    m_colorG = g;
    m_colorB = b;
    m_isTransparent = isTransparent;
    // Repaint (call paintEvent event);
    update();
}

// Set the title bar icon;
void MyTitleBar::setTitleIcon(QString filePath, QSize IconSize)
{
    qDebug() << "MyTitleBar::setTitleIcon()";
    QPixmap titleIcon(filePath);
    m_pIcon->setPixmap(titleIcon.scaled(IconSize));
}

// Set the title content;
void MyTitleBar::setTitleContent(QString titleContent, int titleFontSize)
{
    qDebug() << "MyTitleBar::setTitleContent()";
    // Set the title font size;
    QFont font = m_pTitleContent->font();
    font.setPointSize(titleFontSize);
    m_pTitleContent->setFont(font);
    // Set the title content;
    m_pTitleContent->setText(titleContent);
    m_titleContent = titleContent;
}

// Set the length of the title bar;
void MyTitleBar::setTitleWidth(int width)
{
    qDebug() << "MyTitleBar::setTitleWidth()";
    this->setFixedWidth(width);
}

// Set the button type on the title bar;
// Since the buttons on the title bar of different windows are different, you can customize the buttons in the title bar;
// Four buttons are provided here, which are minimize, restore, maximize, and close buttons. If you need other buttons, you can add settings by yourself;
void MyTitleBar::setButtonType(ButtonType buttonType)
{
    qDebug() << "MyTitleBar::setButtonType()";
    m_buttonType = buttonType;

   switch (buttonType)
    {
    case MIN_BUTTON:
        {
            m_pButtonRestore->setVisible(false);
            m_pButtonMax->setVisible(false);
        }
        break;
    case MIN_MAX_BUTTON:
        {
            m_pButtonRestore->setVisible(false);
        }
        break;
    case ONLY_CLOSE_BUTTON:
        {
            m_pButtonMin->setVisible(false);
            m_pButtonRestore->setVisible(false);
            m_pButtonMax->setVisible(false);
        }
        break;
    default:
        break;
    }
}

// Set whether the title in the title bar will automatically scroll, the effect of the marquee;
// Under normal circumstances, the title content in the title bar is not scrollable, but since you customize it, it depends on your needs. You can do O(∩_∩)O if you want to design it!
void MyTitleBar::setTitleRoll()
{
    qDebug() << "MyTitleBar::setTitleRoll()";
    connect(&m_titleRollTimer, SIGNAL(timeout()), this, SLOT(onRollTitle()));
    m_titleRollTimer.start(200);
}

// Set the width of the window border;
void MyTitleBar::setWindowBorderWidth(int borderWidth)
{
    qDebug() << "MyTitleBar::setWindowBorderWidth()";
    m_windowBorderWidth = borderWidth;
}

// Save the position and size of the window before the window is maximized;
void MyTitleBar::saveRestoreInfo(const QPoint point, const QSize size)
{
    qDebug() << "MyTitleBar::saveRestoreInfo()";
    m_restorePos = point;
    m_restoreSize = size;
}

// Get the position and size of the window before the window is maximized;
void MyTitleBar::getRestoreInfo(QPoint& point, QSize& size)
{
    qDebug() << "MyTitleBar::saveRestoreInfo()";
    point = m_restorePos;
    size = m_restoreSize;
}

// Draw the background color of the title bar;
void MyTitleBar::paintEvent(QPaintEvent *event)
{
    // Whether to set the title transparent;
    if (!m_isTransparent)
    {
        //Set the background color;
        QPainter painter(this);
        QPainterPath pathBack;
        pathBack.setFillRule(Qt::WindingFill);
        pathBack.addRoundedRect(QRect(0, 0, this->width(), this->height()), 3, 3);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.fillPath(pathBack, QBrush(QColor(m_colorR, m_colorG, m_colorB)));
    }

    // When the window is maximized or restored, the length of the window changes, and the length of the title bar should be changed together;
    // The m_windowBorderWidth is subtracted here because the window may have a border of different width;
    // If the window has a border, you need to set the value of m_windowBorderWidth, otherwise m_windowBorderWidth defaults to 0;
    if (this->width() != (this->parentWidget()->width() - m_windowBorderWidth))
    {
        this->setFixedWidth(this->parentWidget()->width() - m_windowBorderWidth);
    }
    QWidget::paintEvent(event);
}

QMainWindow* MyTitleBar::GetTopLevelWindow()
{
    QMainWindow* res = dynamic_cast<QMainWindow*>(parent());
    return res;
}

void MyTitleBar::loadStyleSheet(const QString &sheetName)
{
    qDebug() << "MyTitleBar::loadStyleSheet()";
    QFile file(":/Resources/" + sheetName + ".css");
    file.open(QFile::ReadOnly);
    if (file.isOpen())
    {
        QString styleSheet = this->styleSheet();
        styleSheet += QLatin1String(file.readAll());
        this->setStyleSheet(styleSheet);
    }
}

// The following is the slot for button operation response;
void MyTitleBar::onButtonMinClicked()
{
    qDebug() << "MyTitleBar::onButtonMinClicked()";
    emit signalButtonMinClicked();
}

void MyTitleBar::onButtonRestoreClicked()
{
    qDebug() << "MyTitleBar::onButtonRestoreClicked()";
    m_pButtonRestore->setVisible(false);
    m_pButtonMax->setVisible(true);
    emit signalButtonRestoreClicked();
}

void MyTitleBar::onButtonMaxClicked()
{
    qDebug() << "MyTitleBar::onButtonMaxClicked()";
    //m_pButtonMax->setVisible(false);
    //m_pButtonRestore->setVisible(true);
    emit signalButtonMaxClicked();
}

void MyTitleBar::onButtonCloseClicked()
{
    qDebug() << "MyTitleBar::onButtonCloseClicked()";
    emit signalButtonCloseClicked();
}

// This method is mainly to display the title in the title bar as a scrolling effect;
void MyTitleBar::onRollTitle()
{
    static int nPos = 0;
    QString titleContent = m_titleContent;
    // When the intercepted position is longer than the string, start from the beginning;
    if (nPos > titleContent.length())
        nPos = 0;

    m_pTitleContent->setText(titleContent.mid(nPos));
    nPos++;
}
