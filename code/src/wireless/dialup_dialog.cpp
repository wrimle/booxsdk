
#include "onyx/wireless/dialup_dialog.h"
#include "onyx/screen/screen_proxy.h"
#include "onyx/sys/sys.h"
#include "onyx/data/network_types.h"

namespace ui
{
static const int ITEM_HEIGHT = 64;
static const QString BUTTON_STYLE =    "\
QPushButton                             \
{                                       \
    background: transparent;            \
    font-size: 14px;                    \
    border-width: 1px;                  \
    border-color: transparent;          \
    border-style: solid;                \
    color: black;                       \
    padding: 0px;                       \
}                                       \
QPushButton:pressed                     \
{                                       \
    padding-left: 0px;                  \
    padding-top: 0px;                   \
    border-color: black;                \
    background-color: black;            \
}                                       \
QPushButton:checked                     \
{                                       \
    padding-left: 0px;                  \
    padding-top: 0px;                   \
    color: white;                       \
    border-color: black;                \
    background-color: black;            \
}                                       \
QPushButton:disabled                    \
{                                       \
    padding-left: 0px;                  \
    padding-top: 0px;                   \
    border-color: dark;                 \
    color: dark;                        \
    background-color: white;            \
}";

struct APN
{
    QString apn;
    QString username;
    QString password;
    QString peer;
};

static const APN APNS[] =
{
    {"Mts", "mts", "mts", "mts"},
    {"Megafon", "gdata", "gdata", "megafon"},
    {"Beeline", "beeline", "beeline", "beeline"},
    {"telenor.mbb", "telenor.mbb", "", ""},
    {"movistar.es", "movistar", "movistar", "movistar"},
    {"China Unicom", "", "", "unicom"},
    {"China Telecom", "", "", "telecom"},
};
static const int APNS_COUNT = sizeof(APNS)/sizeof(APNS[0]);

DialUpDialog::DialUpDialog(QWidget *parent, SysStatus & sys)
#ifndef Q_WS_QWS
    : QDialog(parent, 0)
#else
    : QDialog(parent, Qt::FramelessWindowHint)
#endif
    , big_box_(this)
    , title_widget_(this)
    , title_vbox_(&title_widget_)
    , title_hbox_(0)
    , content_layout_(0)
    , network_label_(0)
    , state_widget_(0)
    , input_layout_(0)
    , disconnect_button_(tr("Disconnect"), this)
    , top_label_(0)
    , title_icon_label_(this)
    , title_text_label_(tr("3G Connection"), this)
    , close_button_("", this)
    , sys_(sys)
    , timer_(0)
    , connecting_(false)
{
    setAutoFillBackground(false);
    createLayout();

    timer_.setInterval(1500);
    loadConf();
}

DialUpDialog::~DialUpDialog()
{

}

QString DialUpDialog::address()
{
    QString result;
    QList<QNetworkInterface> all = QNetworkInterface::allInterfaces();
    foreach(QNetworkInterface ni, all)
    {
        qDebug("interface name %s", qPrintable(ni.name()));
        QList<QNetworkAddressEntry> addrs = ni.addressEntries();
        foreach(QNetworkAddressEntry entry, addrs)
        {
            if (ni.name().compare("ppp0", Qt::CaseInsensitive) == 0)
            {
                result = entry.ip().toString();
            }
            qDebug("ip address %s", qPrintable(entry.ip().toString()));
        }
    }
    return result;
}

void DialUpDialog::loadConf()
{
    sys::SystemConfig conf;
    DialupProfiles all;
    conf.loadDialupProfiles(all);
    if (all.size() > 0)
    {
        profile_ = all.front();
    }
}

void DialUpDialog::saveConf()
{
    sys::SystemConfig conf;
    DialupProfiles all;
    all.push_back(profile_);
    conf.saveDialupProfiles(all);
}

int  DialUpDialog::popup()
{
    showMaximized();
    onyx::screen::instance().flush(this, onyx::screen::ScreenProxy::GC);
    return exec();
}

void DialUpDialog::keyPressEvent(QKeyEvent *ke)
{
    if (ke->key() == Qt::Key_Escape)
    {
        ke->accept();
        reject();
    }

    // Disable the parent widget to update screen.

    // Can not use flush here, could be caused by the keyboard event.
    /*
    onyx::screen::instance().enableUpdate(false);
    QApplication::processEvents();
    onyx::screen::instance().enableUpdate(true);
    onyx::screen::instance().updateWidget(this, onyx::screen::ScreenProxy::DW);
    */
}

void DialUpDialog::keyReleaseEvent(QKeyEvent *ke)
{
    ke->accept();
}

bool DialUpDialog::event(QEvent * e)
{
    bool ret = QDialog::event(e);
    if (e->type() == QEvent::UpdateRequest)
    {
        onyx::screen::instance().updateWidget(this, onyx::screen::ScreenProxy::GU);
        e->accept();
        return true;
    }
    return ret;
}

void DialUpDialog::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QPainterPath path;
    path.addRoundedRect(content_layout_.contentsRect().adjusted(-2, -2, 2, 2), 8, 8, Qt::AbsoluteSize);
    painter.fillPath(path, QBrush(QColor(220, 220, 220)));
    painter.setPen(QColor(Qt::black));
    painter.drawPath(path);
}

void DialUpDialog::resizeEvent(QResizeEvent *)
{
}

void DialUpDialog::mousePressEvent(QMouseEvent *)
{
}

void DialUpDialog::mouseReleaseEvent(QMouseEvent *)
{
}

void DialUpDialog::createLayout()
{
    const int SPACING = 2;
    const int WIDGET_HEIGHT = 50;
    big_box_.setSizeConstraint(QLayout::SetMaximumSize);
    big_box_.setSpacing(SPACING);
    big_box_.setContentsMargins(SPACING, SPACING, SPACING, SPACING);

    // title widget.
    title_widget_.setAutoFillBackground(true);
    title_widget_.setBackgroundRole(QPalette::Dark);
    title_widget_.setContentsMargins(0, 0, 0, 0);
    title_widget_.setFixedHeight(WIDGET_HEIGHT + SPACING * 2);
    big_box_.addWidget(&title_widget_);

    // content layout.
    const int MARGINS = 10;
    big_box_.addLayout(&content_layout_);
    content_layout_.setContentsMargins(MARGINS, 6, MARGINS, 6);

    // Status.
    state_widget_.setWordWrap(true);
    state_widget_.setAlignment(Qt::AlignLeft);
    state_widget_.setContentsMargins(MARGINS, 0, MARGINS, 0);

    network_label_.setWordWrap(true);
    network_label_.setAlignment(Qt::AlignLeft);
    network_label_.setContentsMargins(MARGINS, 0, MARGINS, 0);

    content_layout_.addWidget(&network_label_);
    content_layout_.addWidget(&state_widget_);
    content_layout_.addSpacing(MARGINS);

    // top_label_
    title_vbox_.setSpacing(0);
    title_vbox_.setContentsMargins(SPACING, 0, SPACING, 0);
    title_vbox_.addWidget(&top_label_);
    top_label_.setFixedHeight(2);
    top_label_.setFrameShape(QFrame::HLine);
    top_label_.setAutoFillBackground(true);
    top_label_.setBackgroundRole(QPalette::Base);

    // title hbox.
    title_vbox_.addLayout(&title_hbox_);
    title_hbox_.addWidget(&title_icon_label_, 0, Qt::AlignVCenter);
    title_hbox_.addSpacing(SPACING * 2);
    title_hbox_.addWidget(&title_text_label_, 0, Qt::AlignVCenter);
    title_hbox_.addStretch(0);
    title_hbox_.addWidget(&close_button_);
    title_hbox_.setContentsMargins(2 * SPACING, SPACING, 2 * SPACING, SPACING);
    title_icon_label_.setPixmap(QPixmap(":/images/network_connection.png"));
    title_text_label_.setAlignment(Qt::AlignVCenter);
    title_icon_label_.setFixedHeight(WIDGET_HEIGHT);
    title_text_label_.useTitleBarStyle();
    title_text_label_.setFixedHeight(WIDGET_HEIGHT);

    close_button_.setStyleSheet(BUTTON_STYLE);
    QPixmap close_pixmap(":/images/close.png");
    close_button_.setIconSize(close_pixmap.size());
    close_button_.setIcon(QIcon(close_pixmap));
    close_button_.setFocusPolicy(Qt::NoFocus);
    QObject::connect(&close_button_, SIGNAL(clicked()), this, SLOT(onCloseClicked()));

    // ap layout.
    content_layout_.addLayout(&input_layout_);

    input_layout_.setContentsMargins(MARGINS, 0, MARGINS, 0);
    input_layout_.setSpacing(10);
    // number_label_.setText(tr("Dial-Up Number: "));
    // input_layout_.addWidget(&number_label_, 0, 0);
    // input_layout_.addWidget(&number_edit_, 0, 1);

    for(int i = 0; i < APNS_COUNT; ++i)
    {
        OnyxPushButton *btn = new OnyxPushButton(APNS[i].apn, 0);
        btn->setAutoExclusive(true);
        btn->setCheckable(true);
        btn->setData(i);
        buttons_.push_back(btn);
        input_layout_.addWidget(btn, i, 0);
        QObject::connect(btn, SIGNAL(clicked(bool)), this, SLOT(onApnClicked(bool)));
    }

    input_layout_.addWidget(&disconnect_button_);
    disconnect_button_.setAutoExclusive(true);
    disconnect_button_.setCheckable(true);
    QObject::connect(&disconnect_button_, SIGNAL(clicked(bool)), this, SLOT(onDisconnectClicked(bool)));
    content_layout_.addStretch(0);

    // QObject::connect(&number_edit_, SIGNAL(getFocus(NabooLineEdit *)), this, SLOT(onGetFocus(NabooLineEdit *)));
    QObject::connect(&sys_, SIGNAL(pppConnectionChanged(const QString &, int)),
                     this, SLOT(onPppConnectionChanged(const QString &, int)));
    QObject::connect(&sys_, SIGNAL(report3GNetwork(const int, const int, const int)),
                     this, SLOT(onReport3GNetwork(const int, const int, const int)));
}

void DialUpDialog::clear()
{
}

void DialUpDialog::connect(const QString & file,
                           const QString & username,
                           const QString & password)
{
    profile_.setNumber(file);
    profile_.setUsername(username);
    profile_.setPassword(password);
    saveConf();
    if (!sys_.connect3g(file, username, password))
    {
        if (!sys_.isPowerSwitchOn())
        {
            state_widget_.setText(tr("3G Power switch is off."));
        }
    }
}

void DialUpDialog::onTimeout()
{
}

void DialUpDialog::onConnectClicked(bool)
{
    state_widget_.setText(tr("Connecting..."));
}

void DialUpDialog::onPppConnectionChanged(const QString &message, int status)
{
    if (status == TG_CHECKING_NETWORK)
    {
        state_widget_.setText(tr("Searching Network..."));
    }
    else if (status == TG_CONNECTING)
    {
        state_widget_.setText(tr("Connecting..."));
    }
    else if (status == TG_CONNECTED)
    {
        QString result("Connected. Address: %1");
        result = result.arg(qPrintable(address()));
        state_widget_.setText(result);
    }
    else if (status == TG_DISCONNECTED)
    {
        state_widget_.setText(tr("Disconnect."));
    }
}

void DialUpDialog::showDNSResult(QHostInfo info)
{
    if (info.addresses().size() > 0)
    {
        state_widget_.setText(tr("Fininshed."));
    }
    else
    {
        state_widget_.setText(info.errorString());
    }
}

void DialUpDialog::onApnClicked(bool)
{
    for(int i = 0; i < buttons_.size(); ++i)
    {
        if (buttons_.at(i)->isChecked())
        {
            connect(APNS[i].peer, APNS[i].username.toLocal8Bit().constData(), APNS[i].password.toLocal8Bit().constData());
            onConnectClicked(true);
            return;
        }
    }
}

static QString networkType(const int n)
{
    switch (n)
    {
    case NO_SERVICE:
        return "NO_SERVICE";
    case GSM_MODE:
        return "GSM";
    case GPRS_MODE:
        return "GPRS";
    case EDGE_MODE:
        return "EDGE";
    case WCDMA_MODE:
    case HSDPA_MODE:
    case HSUPA_MODE:
    case HSDPA_MODE_AND_HSUPA_MODE:
    case TD_SCDMA_MODE:
    case HSPA_PLUS_MODE:
        return "3G";
    }
    return "";
}

void DialUpDialog::onReport3GNetwork(const int signal,
                                     const int total,
                                     const int network)
{
    QString t("%1 %2%");
    t = t.arg(networkType(network)).arg(signal * 100 / total);
    network_label_.setText(t);
}

void DialUpDialog::onDisconnectClicked(bool)
{
    sys::SysStatus::instance().disconnect3g();
}

void DialUpDialog::onGetFocus(OnyxLineEdit *object)
{

}

void DialUpDialog::onCloseClicked()
{
    reject();
}

}   // namespace ui

///
/// \example wifi/dialup_main.cpp
/// This is an example of how to use the DialUpDialog class.
///