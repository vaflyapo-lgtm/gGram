#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QToolButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QCheckBox>
#include <QSlider>
#include <QLabel>
#include <QGroupBox>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QScrollBar>
#include <QMenu>
#include <QInputDialog>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QSvgRenderer>
#include <QDateTime>
#include <QHash>
#include <QMap>
#include <QFile>
#include <QPointer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTabWidget>
#include <QComboBox>
#include <QFormLayout>
#include <QMessageBox>
#include <QFrame>
#include <functional>
#include <thread>
#include <atomic>
#include <td/telegram/td_json_client.h>

static qint64 jsonToInt64(const QJsonValue& val) {
    if (val.isDouble()) return static_cast<qint64>(val.toDouble());
    if (val.isString()) return val.toString().toLongLong();
    return val.toVariant().toLongLong();
}

namespace SvgData {
    const char* CHATS = R"(<svg viewBox="0 0 24 24" fill="none" stroke="#38bdf8" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M21 15a2 2 0 0 1-2 2H7l-4 4V5a2 2 0 0 1 2-2h14a2 2 0 0 1 2 2z"/></svg>)";
    const char* SETTINGS = R"(<svg viewBox="0 0 24 24" fill="none" stroke="#38bdf8" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><circle cx="12" cy="12" r="3"/><path d="M19.4 15a1.65 1.65 0 0 0 .33 1.82l.06.06a2 2 0 0 1 0 2.83 2 2 0 0 1-2.83 0l-.06-.06a1.65 1.65 0 0 0-1.82-.33 1.65 1.65 0 0 0-1 1.51V21a2 2 0 0 1-2 2 2 2 0 0 1-2-2v-.09A1.65 1.65 0 0 0 9 19.4a1.65 1.65 0 0 0-1.82.33l-.06.06a2 2 0 0 1-2.83 0 2 2 0 0 1 0-2.83l.06-.06a1.65 1.65 0 0 0 .33-1.82 1.65 1.65 0 0 0-1.51-1H3a2 2 0 0 1-2-2 2 2 0 0 1 2-2h.09A1.65 1.65 0 0 0 4.6 9a1.65 1.65 0 0 0-.33-1.82l-.06-.06a2 2 0 0 1 0-2.83 2 2 0 0 1 2.83 0l.06.06a1.65 1.65 0 0 0 1.82.33H9a1.65 1.65 0 0 0 1-1.51V3a2 2 0 0 1 2-2 2 2 0 0 1 2 2v.09a1.65 1.65 0 0 0 1 1.51 1.65 1.65 0 0 0 1.82-.33l.06-.06a2 2 0 0 1 2.83 0 2 2 0 0 1 0 2.83l-.06.06a1.65 1.65 0 0 0-.33 1.82V9a1.65 1.65 0 0 0 1.51 1H21a2 2 0 0 1 2 2 2 2 0 0 1-2 2h-.09a1.65 1.65 0 0 0-1.51 1z"/></svg>)";
    const char* TRASH = R"(<svg viewBox="0 0 24 24" fill="none" stroke="#ef4444" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><polyline points="3 6 5 6 21 6"/><path d="M19 6v14a2 2 0 0 1-2 2H7a2 2 0 0 1-2-2V6m3 0V4a2 2 0 0 1 2-2h4a2 2 0 0 1 2 2v2"/></svg>)";
    const char* EDIT = R"(<svg viewBox="0 0 24 24" fill="none" stroke="#38bdf8" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M11 4H4a2 2 0 0 0-2 2v14a2 2 0 0 0 2 2h14a2 2 0 0 0 2-2v-7"/><path d="M18.5 2.5a2.121 2.121 0 0 1 3 3L12 15l-4 1 1-4 9.5-9.5z"/></svg>)";
    const char* LIKE = R"(<svg viewBox="0 0 24 24" fill="none" stroke="#38bdf8" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M14 9V5a3 3 0 0 0-3-3l-4 9v11h11.28a2 2 0 0 0 2-1.7l1.38-9a2 2 0 0 0-2-2.3zM7 22H4a2 2 0 0 1-2-2v-7a2 2 0 0 1 2-2h3"/></svg>)";
    const char* FIRE = R"(<svg viewBox="0 0 24 24" fill="none" stroke="#f97316" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M8.5 14.5A2.5 2.5 0 0 0 11 12c0-1.38-.5-2-1-3-1.072-2.143-.224-4.054 2-6 .5 2.5 2 4.9 4 6.5 2 1.6 3 3.5 3 5.5a7 7 0 1 1-14 0c0-1.153.433-2.294 1-3a2.5 2.5 0 0 0 2.5 3.5z"/></svg>)";
    const char* HEART = R"(<svg viewBox="0 0 24 24" fill="none" stroke="#ec4899" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M20.84 4.61a5.5 5.5 0 0 0-7.78 0L12 5.67l-1.06-1.06a5.5 5.5 0 0 0-7.78 7.78l1.06 1.06L12 21.23l8.78-8.78 1.06-1.06a5.5 5.5 0 0 0 0-7.78z"/></svg>)";
    const char* CHECK_DOUBLE = R"(<svg viewBox="0 0 24 24" fill="none" stroke="#38bdf8" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M7 12l5 5L22 4"/><path d="M2 12l5 5L12 12"/></svg>)";
    const char* REPLY = R"(<svg viewBox="0 0 24 24" fill="none" stroke="#38bdf8" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><polyline points="9 17 4 12 9 7"/><path d="M20 18v-2a4 4 0 0 0-4-4H4"/></svg>)";
    const char* BURGER = R"(<svg viewBox="0 0 24 24" fill="none" stroke="#e2e8f0" stroke-width="2.2" stroke-linecap="round"><line x1="4" y1="7" x2="20" y2="7"/><line x1="4" y1="12" x2="20" y2="12"/><line x1="4" y1="17" x2="20" y2="17"/></svg>)";
    // Логотип мода gGramo (бумеранг-стрелка)
    const char* LOGO = R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 1280 1280">
  <g transform="translate(-52 -69)">
    <path d="M 1170 150 C 1148 148, 1118 151, 1090 160 L 205 446 C 166 459, 139 483, 134 520 C 130 551, 140 582, 168 608 C 185 625, 205 641, 232 657 L 408 777 C 465 816, 510 857, 548 913 L 626 1040 L 754 1222 C 768 1244, 793 1264, 821 1268 C 850 1272, 878 1263, 898 1244 C 907 1235, 915 1222, 921 1205 L 1130 622 L 779 622 L 779 770 L 927 770 L 845 1035 L 732 873 C 697 822, 661 785, 620 748 C 579 710, 535 674, 485 640 L 378 578 L 1058 341 L 1020 525 L 1167 534 L 1248 287 C 1257 258, 1253 226, 1235 201 C 1219 178, 1197 160, 1170 150 Z"
      fill="#DCEAF7" stroke="#DCEAF7" stroke-width="4" stroke-linejoin="round" stroke-linecap="round"/>
  </g>
</svg>)";
}

// Иконка-папка (chat folder), рисуется программно
static QPixmap makeFolderIconPixmap(bool active, int size = 22) {
    QPixmap pm(size, size);
    pm.fill(Qt::transparent);
    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing);
    QColor c = active ? QColor("#38bdf8") : QColor("#94a3b8");
    QPainterPath path;
    qreal s = size / 24.0;
    path.moveTo(3 * s, 7 * s);
    path.cubicTo(3 * s, 5.9 * s, 3.9 * s, 5 * s, 5 * s, 5 * s);
    path.lineTo(9.2 * s, 5 * s);
    path.lineTo(11 * s, 7 * s);
    path.lineTo(19 * s, 7 * s);
    path.cubicTo(20.1 * s, 7 * s, 21 * s, 7.9 * s, 21 * s, 9 * s);
    path.lineTo(21 * s, 17 * s);
    path.cubicTo(21 * s, 18.1 * s, 20.1 * s, 19 * s, 19 * s, 19 * s);
    path.lineTo(5 * s, 19 * s);
    path.cubicTo(3.9 * s, 19 * s, 3 * s, 18.1 * s, 3 * s, 17 * s);
    path.closeSubpath();
    p.fillPath(path, active ? QColor(56, 189, 248, 40) : QColor(148, 163, 184, 25));
    p.setPen(QPen(c, 1.6 * s));
    p.drawPath(path);
    return pm;
}

static QIcon makeFolderIcon(bool active, int size = 22) {
    return QIcon(makeFolderIconPixmap(active, size));
}

// ===== Splash-экран: SVG-логотип + анимация названия мода =====
class SplashWidget : public QWidget {
    Q_OBJECT
    Q_PROPERTY(qreal logoOpacity READ getLogoOpacity WRITE setLogoOpacity)
    Q_PROPERTY(qreal logoScale READ getLogoScale WRITE setLogoScale)
    Q_PROPERTY(qreal textOpacity READ getTextOpacity WRITE setTextOpacity)
    Q_PROPERTY(int textShift READ getTextShift WRITE setTextShift)
public:
    explicit SplashWidget(QWidget* parent = nullptr) : QWidget(parent) {
        setAttribute(Qt::WA_TranslucentBackground);
        renderer_ = new QSvgRenderer(QByteArray(SvgData::LOGO), this);
        setLogoOpacity(0.0);
        setLogoScale(0.55);
        setTextOpacity(0.0);
        setTextShift(30);

        titleFont_.setFamily("Inter, 'JetBrains Mono', sans-serif");
        titleFont_.setPointSizeF(34);
        titleFont_.setBold(true);

        auto mkAnim = [this](const char* prop, qreal from, qreal to, int dur, int start) {
            QPropertyAnimation* a = new QPropertyAnimation(this, prop, this);
            a->setDuration(dur);
            a->setStartValue(from);
            a->setEndValue(to);
            a->setStartDelay(start);
            a->setEasingCurve(QEasingCurve::OutCubic);
            seq_.addAnimation(a);
        };
        // 1) Логотип плавно появляется и "вырастает"
        mkAnim("logoOpacity", 0.0, 1.0, 700, 150);
        mkAnim("logoScale", 0.55, 1.0, 900, 150);
        // 2) Затем появляется название мода (typewriter-эффект через timer ниже)
        mkAnim("textOpacity", 0.0, 1.0, 400, 1100);
        mkAnim("textShift", 30, 0, 600, 1100);

        connect(&seq_, &QSequentialAnimationGroup::finished, this, &SplashWidget::finished);
        seq_.start();

        typeTimer_.setInterval(55);
        connect(&typeTimer_, &QTimer::timeout, this, [this]() {
            if (typedLen_ < fullTitle_.size()) {
                typedLen_++;
                update();
            } else {
                typeTimer_.stop();
            }
        });
        typeTimer_.start();
    }

    void paintEvent(QPaintEvent*) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        p.setRenderHint(QPainter::SmoothPixmapTransform);

        // фон-затемнение
        p.fillRect(rect(), QColor(8, 12, 24, 235));

        const int logoSize = qMin(width(), height()) / 3;
        const qreal sc = 0.55 + logoScale_ * 0.45;
        const int sz = int(logoSize * sc);
        QRect logoRect((width() - sz) / 2, (height() - sz) / 2 - 60, sz, sz);

        p.setOpacity(logoOpacity_);
        renderer_->render(&p, logoRect);

        // Название мода с typewriter-эффектом
        p.setOpacity(textOpacity_);
        p.setFont(titleFont_);
        QFontMetrics fm(titleFont_);
        QString shown = fullTitle_.left(typedLen_);
        int tw = fm.horizontalAdvance(fullTitle_);
        int x = (width() - tw) / 2;
        int y = logoRect.bottom() + 70 + textShift_;
        p.setPen(QColor("#DCEAF7"));
        p.drawText(x, y, shown);

        // светящийся "курсор" при печати
        if (typedLen_ < fullTitle_.size()) {
            int cw = fm.horizontalAdvance(shown);
            p.fillRect(x + cw + 6, y - fm.ascent() + 6, 4, fm.height() - 12, QColor("#38bdf8"));
        }
    }

    qreal getLogoOpacity() const { return logoOpacity_; }
    void setLogoOpacity(qreal v) { logoOpacity_ = v; update(); }
    qreal getLogoScale() const { return logoScale_; }
    void setLogoScale(qreal v) { logoScale_ = v; update(); }
    qreal getTextOpacity() const { return textOpacity_; }
    void setTextOpacity(qreal v) { textOpacity_ = v; update(); }
    int getTextShift() const { return textShift_; }
    void setTextShift(int v) { textShift_ = v; update(); }

signals:
    void finished();

private:
    QSvgRenderer* renderer_;
    QSequentialAnimationGroup seq_;
    QTimer typeTimer_;
    QString fullTitle_ = "gGramo";
    int typedLen_ = 0;
    QFont titleFont_;
    qreal logoOpacity_{0}, logoScale_{0}, textOpacity_{0};
    int textShift_{0};
};

// Плавная анимированная прокрутка QListWidget
class SmoothScroller : public QObject {
    Q_OBJECT
    Q_PROPERTY(qreal progress READ progress WRITE setProgress)
public:
    explicit SmoothScroller(QAbstractScrollArea* area, QObject* parent = nullptr)
        : QObject(parent), area_(area) {}

    void scrollToValue(int target) {
        QScrollBar* sb = area_->verticalScrollBar();
        startValue_ = sb->value();
        targetValue_ = qBound(sb->minimum(), target, sb->maximum());
        progress_ = 0.0;
        anim_.stop();
        anim_.start();
    }

    qreal progress() const { return progress_; }
    void setProgress(qreal p) {
        progress_ = p;
        double e = 1.0 - (1.0 - p) * (1.0 - p); // easeOutQuad
        area_->verticalScrollBar()->setValue(static_cast<int>(startValue_ + (targetValue_ - startValue_) * e));
    }

private:
    QAbstractScrollArea* area_;
    QPropertyAnimation anim_{this, "progress"};
    int startValue_{0};
    int targetValue_{0};
    qreal progress_{0.0};
};

static QPixmap renderSvgPixmap(const char* svgStr, int w, int h) {
    QByteArray ba(svgStr);
    QSvgRenderer renderer(ba);
    QPixmap pm(w, h);
    pm.fill(Qt::transparent);
    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing);
    renderer.render(&p);
    return pm;
}

static QIcon renderSvgIcon(const char* svgStr, int size = 20) {
    return QIcon(renderSvgPixmap(svgStr, size, size));
}

static QPixmap makeCircularPixmap(const QPixmap& src, int size) {
    if (src.isNull()) return QPixmap();
    QPixmap scaled = src.scaled(size, size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    QPixmap result(size, size);
    result.fill(Qt::transparent);
    QPainter painter(&result);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addEllipse(0, 0, size, size);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, scaled);
    return result;
}

static QPixmap createLetterAvatar(const QString& title, int size) {
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    uint hash = qHash(title);
    QColor colors[] = {
        QColor("#0ea5e9"), QColor("#0284c7"), QColor("#2563eb"),
        QColor("#0d9488"), QColor("#059669"), QColor("#6366f1")
    };
    painter.setBrush(colors[hash % 6]);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(0, 0, size, size);

    painter.setPen(Qt::white);
    QFont font("Sans-Serif", qMax(10, size / 3), QFont::Bold);
    painter.setFont(font);

    QString letter = title.isEmpty() ? "?" : title.left(1).toUpper();
    painter.drawText(QRect(0, 0, size, size), Qt::AlignCenter, letter);

    return pixmap;
}

static QString hexToRgbaStr(const QString& hex, int alpha) {
    QColor c(hex);
    return QString("%1, %2, %3, %4").arg(c.red()).arg(c.green()).arg(c.blue()).arg(alpha);
}

class TdClient : public QObject {
    Q_OBJECT
public:
    TdClient() {
        client_ = td_json_client_create();
        running_ = true;
        thread_ = std::thread([this]() {
            while (running_) {
                const char* res = td_json_client_receive(client_, 1.0);
                if (res) {
                    QString jsonStr = QString::fromUtf8(res);
                    QMetaObject::invokeMethod(this, "onResponse", Qt::QueuedConnection, Q_ARG(QString, jsonStr));
                }
            }
        });
    }

    ~TdClient() {
        running_ = false;
        if (thread_.joinable()) thread_.join();
        td_json_client_destroy(client_);
    }

    void send(const QJsonObject& obj) {
        QJsonDocument doc(obj);
        QByteArray bytes = doc.toJson(QJsonDocument::Compact);
        td_json_client_send(client_, bytes.constData());
    }

signals:
    void updateReceived(const QJsonObject& obj);

private slots:
    void onResponse(const QString& jsonStr) {
        QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8());
        if (doc.isObject()) {
            emit updateReceived(doc.object());
        }
    }

private:
    void* client_;
    std::atomic<bool> running_{false};
    std::thread thread_;
};

class MessageWidget : public QWidget {
    Q_OBJECT
public:
    MessageWidget(qint64 msgId, const QString& senderName, const QString& text, const QString& time, bool isOutgoing, QWidget* parent = nullptr)
        : QWidget(parent), msgId_(msgId), isOutgoing_(isOutgoing) {
        
        QHBoxLayout* mainLayout = new QHBoxLayout(this);
        mainLayout->setContentsMargins(6, 4, 6, 4);
        mainLayout->setSpacing(6);

        avatarLabel = new QLabel();
        avatarLabel->setFixedSize(30, 30);
        avatarLabel->setVisible(!isOutgoing);

        bubble = new QWidget();
        bubble->setObjectName("msgBubble");

        QVBoxLayout* bLayout = new QVBoxLayout(bubble);
        bLayout->setContentsMargins(10, 8, 10, 8);
        bLayout->setSpacing(4);

        if (!isOutgoing && !senderName.isEmpty()) {
            senderLbl = new QLabel(senderName);
            senderLbl->setStyleSheet("color: #38bdf8; font-weight: bold; font-size: 11px;");
            bLayout->addWidget(senderLbl);
        }

        replyContainer = new QWidget();
        replyContainer->setStyleSheet("background-color: rgba(56, 189, 248, 0.15); border-left: 2px solid #38bdf8; border-radius: 4px;");
        QVBoxLayout* rLayout = new QVBoxLayout(replyContainer);
        rLayout->setContentsMargins(6, 4, 6, 4);
        rLayout->setSpacing(1);
        replyTitleLbl = new QLabel();
        replyTitleLbl->setStyleSheet("color: #38bdf8; font-weight: bold; font-size: 10px;");
        replyTextLbl = new QLabel();
        replyTextLbl->setStyleSheet("color: #cbd5e1; font-size: 11px;");
        rLayout->addWidget(replyTitleLbl);
        rLayout->addWidget(replyTextLbl);
        replyContainer->setVisible(false);
        bLayout->addWidget(replyContainer);

        mediaLabel = new QLabel();
        mediaLabel->setAlignment(Qt::AlignCenter);
        mediaLabel->setStyleSheet("border-radius: 8px;");
        mediaLabel->setVisible(false);
        bLayout->addWidget(mediaLabel);

        txtLbl = new QLabel(text);
        txtLbl->setWordWrap(true);
        txtLbl->setStyleSheet("color: #f1f5f9; font-size: 13px; line-height: 1.2;");
        txtLbl->setVisible(!text.isEmpty());
        bLayout->addWidget(txtLbl);

        deletedContainer = new QWidget();
        QHBoxLayout* delLayout = new QHBoxLayout(deletedContainer);
        delLayout->setContentsMargins(0, 2, 0, 0);
        delLayout->setSpacing(4);
        
        QLabel* trashIconLbl = new QLabel();
        trashIconLbl->setPixmap(renderSvgPixmap(SvgData::TRASH, 14, 14));
        
        QLabel* delTextLbl = new QLabel("Удалено");
        delTextLbl->setStyleSheet("color: #ef4444; font-weight: bold; font-size: 10px;");

        delLayout->addWidget(trashIconLbl);
        delLayout->addWidget(delTextLbl);
        delLayout->addStretch();
        deletedContainer->setVisible(false);
        bLayout->addWidget(deletedContainer);

        QHBoxLayout* bottomMeta = new QHBoxLayout();
        bottomMeta->setContentsMargins(0, 2, 0, 0);
        bottomMeta->setSpacing(4);

        reactionsLbl = new QLabel();
        reactionsLbl->setStyleSheet("background-color: rgba(0, 0, 0, 0.25); border-radius: 8px; padding: 2px 6px; color: #f59e0b; font-size: 11px; font-weight: bold;");
        reactionsLbl->setVisible(false);

        timeLbl = new QLabel(time);
        timeLbl->setStyleSheet("color: #64748b; font-size: 10px;");

        bottomMeta->addWidget(reactionsLbl);
        bottomMeta->addStretch();
        bottomMeta->addWidget(timeLbl);

        if (isOutgoing) {
            QLabel* checkLbl = new QLabel();
            checkLbl->setPixmap(renderSvgPixmap(SvgData::CHECK_DOUBLE, 12, 12));
            bottomMeta->addWidget(checkLbl);
        }

        bLayout->addLayout(bottomMeta);

        updateBubbleStyle();

        if (isOutgoing) {
            mainLayout->addStretch();
            mainLayout->addWidget(bubble, 0);
        } else {
            mainLayout->addWidget(avatarLabel, 0, Qt::AlignBottom);
            mainLayout->addWidget(bubble, 0);
            mainLayout->addStretch();
        }
    }

    void setReply(const QString& title, const QString& text) {
        replyTitleLbl->setText(title);
        replyTextLbl->setText(text);
        replyContainer->setVisible(true);
        emit sizeChanged();
    }

    void setMediaPixmap(const QPixmap& pix) {
        if (pix.isNull()) return;
        QPixmap scaled = pix.scaledToWidth(260, Qt::SmoothTransformation);
        mediaLabel->setPixmap(scaled);
        mediaLabel->setVisible(true);
        emit sizeChanged();
    }

    void setReactions(const QString& reactionsText) {
        if (reactionsText.isEmpty()) {
            reactionsLbl->setVisible(false);
        } else {
            reactionsLbl->setText(reactionsText);
            reactionsLbl->setVisible(true);
        }
        emit sizeChanged();
    }

    void updateText(const QString& newText, bool edited = true) {
        txtLbl->setText(newText + (edited ? " (изм.)" : ""));
        txtLbl->setVisible(!newText.isEmpty());
        emit sizeChanged();
    }

    void markAsDeleted() {
        isDeleted_ = true;
        deletedContainer->setVisible(true);
        updateBubbleStyle();
        emit sizeChanged();
    }

    qint64 msgId() const { return msgId_; }
    bool isOutgoing() const { return isOutgoing_; }
    QString text() const { return txtLbl->text(); }

    QLabel* avatarLabel;
    QLabel* mediaLabel;
    int photoFileId{0};

signals:
    void sizeChanged();

private:
    void updateBubbleStyle() {
        if (isDeleted_) {
            bubble->setStyleSheet("QWidget#msgBubble { background-color: rgba(239, 68, 68, 0.15); border: 1px solid rgba(239, 68, 68, 0.35); border-radius: 12px; }");
        } else if (isOutgoing_) {
            bubble->setStyleSheet("QWidget#msgBubble { background-color: rgba(14, 165, 233, 0.22); border: 1px solid rgba(56, 189, 248, 0.3); border-radius: 12px; }");
        } else {
            bubble->setStyleSheet("QWidget#msgBubble { background-color: rgba(30, 41, 59, 0.85); border: 1px solid rgba(255, 255, 255, 0.06); border-radius: 12px; }");
        }
    }

    qint64 msgId_;
    bool isOutgoing_;
    bool isDeleted_{false};

    QWidget* bubble;
    QLabel* senderLbl{nullptr};
    QWidget* replyContainer;
    QLabel* replyTitleLbl;
    QLabel* replyTextLbl;
    QLabel* txtLbl;
    QWidget* deletedContainer;
    QLabel* reactionsLbl;
    QLabel* timeLbl;
};

class ChatItemWidget : public QWidget {
    Q_OBJECT
public:
    ChatItemWidget(const QString& title, const QString& lastMsg, const QString& timeStr, int unread, QWidget* parent = nullptr)
        : QWidget(parent) {
        QHBoxLayout* layout = new QHBoxLayout(this);
        layout->setContentsMargins(8, 6, 8, 6);
        layout->setSpacing(10);

        avatarLabel = new QLabel();
        avatarLabel->setFixedSize(40, 40);
        layout->addWidget(avatarLabel);

        QVBoxLayout* infoLayout = new QVBoxLayout();
        infoLayout->setSpacing(2);

        QHBoxLayout* topRow = new QHBoxLayout();
        titleLbl = new QLabel(title);
        titleLbl->setStyleSheet("font-weight: bold; color: #f1f5f9; font-size: 13px;");
        timeLbl = new QLabel(timeStr);
        timeLbl->setStyleSheet("color: #64748b; font-size: 11px;");
        topRow->addWidget(titleLbl);
        topRow->addStretch();
        topRow->addWidget(timeLbl);

        QHBoxLayout* bottomRow = new QHBoxLayout();
        msgLbl = new QLabel(lastMsg);
        msgLbl->setStyleSheet("color: #94a3b8; font-size: 12px;");
        bottomRow->addWidget(msgLbl);
        bottomRow->addStretch();

        badgeLbl = new QLabel();
        badgeLbl->setStyleSheet("background-color: #0ea5e9; color: white; border-radius: 8px; padding: 1px 6px; font-weight: bold; font-size: 10px;");
        bottomRow->addWidget(badgeLbl);

        updateUnread(unread);

        infoLayout->addLayout(topRow);
        infoLayout->addLayout(bottomRow);
        layout->addLayout(infoLayout);
    }

    void updateInfo(const QString& lastMsg, const QString& timeStr, int unread) {
        msgLbl->setText(lastMsg);
        if (!timeStr.isEmpty()) timeLbl->setText(timeStr);
        updateUnread(unread);
    }

    void updateUnread(int unread) {
        if (unread > 0) {
            badgeLbl->setText(QString::number(unread));
            badgeLbl->setVisible(true);
        } else {
            badgeLbl->setVisible(false);
        }
    }

    QLabel* avatarLabel;

private:
    QLabel* titleLbl;
    QLabel* timeLbl;
    QLabel* msgLbl;
    QLabel* badgeLbl;
};

// Строка-папка чатов (chat folder) в левом краю списка
class FolderItemWidget : public QWidget {
    Q_OBJECT
public:
    FolderItemWidget(const QString& name, bool active, QWidget* parent = nullptr) : QWidget(parent) {
        QHBoxLayout* l = new QHBoxLayout(this);
        l->setContentsMargins(10, 4, 10, 4);
        l->setSpacing(8);
        QLabel* ic = new QLabel();
        ic->setPixmap(makeFolderIconPixmap(active, 20));
        QLabel* nm = new QLabel(name);
        nm->setStyleSheet(active ? "color:#38bdf8; font-weight:bold; font-size:13px;"
                                 : "color:#cbd5e1; font-size:13px;");
        l->addWidget(ic);
        l->addWidget(nm);
        l->addStretch();
    }
};

class GGramoWindow : public QMainWindow {
    Q_OBJECT
public:
    GGramoWindow() {
        setWindowTitle("gGramo Client");
        resize(1100, 720);
        setAttribute(Qt::WA_TranslucentBackground);

        QWidget* centralWidget = new QWidget(this);
        QVBoxLayout* rootLayout = new QVBoxLayout(centralWidget);
        rootLayout->setContentsMargins(6, 6, 6, 6);

        rootStack = new QStackedWidget();

        QWidget* authContainer = new QWidget();
        QVBoxLayout* authCenterLayout = new QVBoxLayout(authContainer);
        QWidget* authCard = new QWidget();
        authCard->setFixedWidth(380);
        authCard->setObjectName("authCard");
        QVBoxLayout* cardLayout = new QVBoxLayout(authCard);
        cardLayout->setContentsMargins(20, 20, 20, 20);

        authStack = new QStackedWidget();

        QWidget* warnPage = new QWidget();
        QVBoxLayout* warnLayout = new QVBoxLayout(warnPage);
        QLabel* warnTitle = new QLabel("<h2>gGramo</h2>");
        warnTitle->setAlignment(Qt::AlignCenter);
        warnTitle->setStyleSheet("color: #38bdf8;");
        QLabel* warnText = new QLabel("Подключение к Telegram...");
        warnText->setAlignment(Qt::AlignCenter);
        QPushButton* btnWarnOk = new QPushButton("Продолжить");
        warnLayout->addWidget(warnTitle);
        warnLayout->addWidget(warnText);
        warnLayout->addSpacing(15);
        warnLayout->addWidget(btnWarnOk);

        QWidget* phonePage = new QWidget();
        QVBoxLayout* phoneLayout = new QVBoxLayout(phonePage);
        QLabel* phoneTitle = new QLabel("<b>Вход по номеру</b>");
        phoneInput = new QLineEdit();
        phoneInput->setPlaceholderText("+373...");
        QPushButton* btnSendPhone = new QPushButton("Получить код");
        phoneLayout->addWidget(phoneTitle);
        phoneLayout->addWidget(phoneInput);
        phoneLayout->addWidget(btnSendPhone);

        QWidget* codePage = new QWidget();
        QVBoxLayout* codeLayout = new QVBoxLayout(codePage);
        QLabel* codeTitle = new QLabel("<b>Код подтверждения</b>");
        codeInput = new QLineEdit();
        codeInput->setPlaceholderText("12345");
        QPushButton* btnSendCode = new QPushButton("Войти");
        codeLayout->addWidget(codeTitle);
        codeLayout->addWidget(codeInput);
        codeLayout->addWidget(btnSendCode);

        QWidget* passPage = new QWidget();
        QVBoxLayout* passLayout = new QVBoxLayout(passPage);
        QLabel* passTitle = new QLabel("<b>2FA Пароль</b>");
        passwordInput = new QLineEdit();
        passwordInput->setEchoMode(QLineEdit::Password);
        passwordInput->setPlaceholderText("Пароль...");
        QPushButton* btnSendPass = new QPushButton("Подтвердить");
        passLayout->addWidget(passTitle);
        passLayout->addWidget(passwordInput);
        passLayout->addWidget(btnSendPass);

        authStack->addWidget(warnPage);
        authStack->addWidget(phonePage);
        authStack->addWidget(codePage);
        authStack->addWidget(passPage);

        authErrorLabel = new QLabel();
        authErrorLabel->setStyleSheet("color: #ef4444; font-size: 11px;");
        authErrorLabel->setAlignment(Qt::AlignCenter);

        cardLayout->addWidget(authStack);
        cardLayout->addWidget(authErrorLabel);
        authCenterLayout->addWidget(authCard, 0, Qt::AlignCenter);

        QWidget* mainAppWidget = new QWidget();
        QVBoxLayout* mainLayout = new QVBoxLayout(mainAppWidget);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(0);

        // ===== Верхняя панель с бургер-меню (вместо широкого сайдбара) =====
        QWidget* topBar = new QWidget();
        topBar->setObjectName("topBar");
        topBar->setFixedHeight(46);
        QHBoxLayout* topLayout = new QHBoxLayout(topBar);
        topLayout->setContentsMargins(8, 4, 12, 4);

        btnBurger = new QToolButton();
        btnBurger->setIcon(renderSvgIcon(SvgData::BURGER, 22));
        btnBurger->setCursor(Qt::PointingHandCursor);
        btnBurger->setToolTip("Меню");

        QLabel* logoTop = new QLabel("<b>gGramo</b>");
        logoTop->setStyleSheet("font-size: 16px; color: #38bdf8;");

        topLayout->addWidget(btnBurger);
        topLayout->addSpacing(8);
        topLayout->addWidget(logoTop);
        topLayout->addStretch();

        appPages = new QStackedWidget();

        QWidget* chatsMasterPage = new QWidget();
        QHBoxLayout* chatsMasterLayout = new QHBoxLayout(chatsMasterPage);
        chatsMasterLayout->setContentsMargins(0, 0, 0, 0);
        chatsMasterLayout->setSpacing(6);

        // Левая колонка: папки чатов сверху + список чатов
        QWidget* chatListPanel = new QWidget();
        chatListPanel->setObjectName("chatListPanel");
        QVBoxLayout* chatPanelLayout = new QVBoxLayout(chatListPanel);
        chatPanelLayout->setContentsMargins(6, 6, 6, 6);
        chatPanelLayout->setSpacing(4);

        folderBar_ = new QListWidget();
        folderBar_->setObjectName("folderBar");
        folderBar_->setVisible(false);
        folderBar_->setFocusPolicy(Qt::NoFocus);
        connect(folderBar_, &QListWidget::currentRowChanged, this, [this](int row) {
            if (row < 0) return;
            QListWidgetItem* it = folderBar_->item(row);
            qint64 fid = jsonToInt64(it->data(Qt::UserRole));
            switchFolder(fid);
        });

        chatListWidget = new QListWidget();
        chatListWidget->setObjectName("chatList");
        chatPanelLayout->addWidget(folderBar_);
        chatPanelLayout->addWidget(chatListWidget, 1);

        chatsMasterLayout->addWidget(chatListPanel, 2);

        QWidget* chatDetailWidget = new QWidget();
        chatDetailWidget->setObjectName("chatDetail");
        QVBoxLayout* chatDetailLayout = new QVBoxLayout(chatDetailWidget);
        chatDetailLayout->setContentsMargins(6, 6, 6, 6);

        activeChatTitle = new QLabel("<b>Выберите чат</b>");
        activeChatTitle->setStyleSheet("font-size: 14px; color: #38bdf8; padding: 6px; border-bottom: 1px solid rgba(255, 255, 255, 0.05);");
        
        messageListWidget = new QListWidget();
        messageListWidget->setObjectName("messageList");
        messageListWidget->setContextMenuPolicy(Qt::CustomContextMenu);

        replyPreviewWidget = new QWidget();
        replyPreviewWidget->setStyleSheet("background-color: rgba(15, 23, 42, 0.9); border-left: 3px solid #38bdf8; border-radius: 4px; padding: 4px;");
        QHBoxLayout* rpLayout = new QHBoxLayout(replyPreviewWidget);
        rpLayout->setContentsMargins(8, 4, 8, 4);

        QVBoxLayout* rpTextLayout = new QVBoxLayout();
        replyPreviewTitle = new QLabel("Ответ на:");
        replyPreviewTitle->setStyleSheet("color: #38bdf8; font-weight: bold; font-size: 11px;");
        replyPreviewText = new QLabel();
        replyPreviewText->setStyleSheet("color: #cbd5e1; font-size: 12px;");
        rpTextLayout->addWidget(replyPreviewTitle);
        rpTextLayout->addWidget(replyPreviewText);

        QPushButton* btnCancelReply = new QPushButton("✕");
        btnCancelReply->setFixedSize(22, 22);
        btnCancelReply->setStyleSheet("background: transparent; color: #94a3b8; font-weight: bold; border: none;");

        rpLayout->addLayout(rpTextLayout);
        rpLayout->addStretch();
        rpLayout->addWidget(btnCancelReply);
        replyPreviewWidget->setVisible(false);

        QHBoxLayout* inputLayout = new QHBoxLayout();
        messageInput = new QLineEdit();
        messageInput->setPlaceholderText("Написать сообщение...");
        QPushButton* btnSendMsg = new QPushButton("Отправить");

        inputLayout->addWidget(messageInput);
        inputLayout->addWidget(btnSendMsg);

        chatDetailLayout->addWidget(activeChatTitle);
        chatDetailLayout->addWidget(messageListWidget);
        chatDetailLayout->addWidget(replyPreviewWidget);
        chatDetailLayout->addLayout(inputLayout);

        chatsMasterLayout->addWidget(chatDetailWidget, 3);

        QWidget* settingsPage = new QWidget();
        QVBoxLayout* settingsMainLayout = new QVBoxLayout(settingsPage);

        settingsTabs = new QTabWidget();
        settingsTabs->setStyleSheet("QTabBar::tab { background: rgba(30, 41, 59, 0.8); color: #cbd5e1; padding: 8px 16px; border-top-left-radius: 6px; border-top-right-radius: 6px; }"
                                     "QTabBar::tab:selected { background: #0ea5e9; color: white; font-weight: bold; }");

        QWidget* profileTab = new QWidget();
        QFormLayout* profileLayout = new QFormLayout(profileTab);
        editFirstName = new QLineEdit();
        editLastName = new QLineEdit();
        editBio = new QLineEdit();
        editUsername = new QLineEdit();
        QPushButton* btnSaveProfile = new QPushButton("Сохранить изменения профиля");
        profileLayout->addRow("Имя:", editFirstName);
        profileLayout->addRow("Фамилия:", editLastName);
        profileLayout->addRow("О себе (Bio):", editBio);
        profileLayout->addRow("Юзернейм (@):", editUsername);
        profileLayout->addRow(btnSaveProfile);

        QWidget* privacyTab = new QWidget();
        QFormLayout* privacyLayout = new QFormLayout(privacyTab);
        cmbPhonePrivacy = new QComboBox();
        cmbPhonePrivacy->addItems({"Все", "Мои контакты", "Никто"});
        cmbLastSeenPrivacy = new QComboBox();
        cmbLastSeenPrivacy->addItems({"Все", "Мои контакты", "Никто"});
        QPushButton* btnSavePrivacy = new QPushButton("Применить приватность");
        privacyLayout->addRow("Кто видит номер телефона:", cmbPhonePrivacy);
        privacyLayout->addRow("Кто видит время входа (Last Seen):", cmbLastSeenPrivacy);
        privacyLayout->addRow(btnSavePrivacy);

        QWidget* securityTab = new QWidget();
        QVBoxLayout* securityLayout = new QVBoxLayout(securityTab);
        QPushButton* btnTerminateSessions = new QPushButton("Завершить все остальные сеансы аккаунта");
        btnTerminateSessions->setStyleSheet("background-color: #ef4444; color: white;");
        QHBoxLayout* ttlLayout = new QHBoxLayout();
        QLabel* lblTtl = new QLabel("Автоудаление аккаунта если неактивен:");
        cmbAccountTTL = new QComboBox();
        cmbAccountTTL->addItem("1 месяц", 30);
        cmbAccountTTL->addItem("3 месяца", 90);
        cmbAccountTTL->addItem("6 месяцев", 180);
        cmbAccountTTL->addItem("1 год", 365);
        QPushButton* btnSaveTTL = new QPushButton("Сохранить TTL");
        ttlLayout->addWidget(lblTtl);
        ttlLayout->addWidget(cmbAccountTTL);
        ttlLayout->addWidget(btnSaveTTL);
        securityLayout->addWidget(btnTerminateSessions);
        securityLayout->addLayout(ttlLayout);
        securityLayout->addStretch();

        QWidget* appTab = new QWidget();
        QVBoxLayout* appLayout = new QVBoxLayout(appTab);
        QGroupBox* ghostGroup = new QGroupBox("Режим Призрака (G-Gramo)");
        QVBoxLayout* ghostLayout = new QVBoxLayout(ghostGroup);
        chkReadOnReply = new QCheckBox("Чтение сообщений только при ответе");
        chkAlwaysOffline = new QCheckBox("Всегда не в сети (Не отправлять онлайн статус)");
        ghostLayout->addWidget(chkReadOnReply);
        ghostLayout->addWidget(chkAlwaysOffline);

        QGroupBox* visualGroup = new QGroupBox("Интерфейс");
        QVBoxLayout* visualLayout = new QVBoxLayout(visualGroup);
        QLabel* lblRadius = new QLabel("Скругление элементов (px):");
        QSlider* sliderRadius = new QSlider(Qt::Horizontal);
        sliderRadius->setRange(0, 24);
        sliderRadius->setValue(12);
        visualLayout->addWidget(lblRadius);
        visualLayout->addWidget(sliderRadius);

        QGroupBox* folderGroup = new QGroupBox("Папки чатов");
        QHBoxLayout* folderCtlLayout = new QHBoxLayout(folderGroup);
        QPushButton* btnRefreshFolders = new QPushButton("Загрузить папки с аккаунта");
        chkShowAllInFolders = new QCheckBox("Добавить вкладку \"Все\"");
        chkShowAllInFolders->setChecked(true);
        folderCtlLayout->addWidget(btnRefreshFolders);
        folderCtlLayout->addWidget(chkShowAllInFolders);
        folderCtlLayout->addStretch();

        QGroupBox* accentGroup = new QGroupBox("Акцентный цвет");
        QHBoxLayout* accentLayout = new QHBoxLayout(accentGroup);
        QLabel* lblAccent = new QLabel("Цвет интерфейса:");
        cmbAccent = new QComboBox();
        cmbAccent->addItem("Голубой (по умолчанию)", "#38bdf8");
        cmbAccent->addItem("Фиолетовый", "#a78bfa");
        cmbAccent->addItem("Зелёный", "#34d399");
        cmbAccent->addItem("Оранжевый", "#fb923c");
        cmbAccent->addItem("Розовый", "#f472b6");
        accentLayout->addWidget(lblAccent);
        accentLayout->addWidget(cmbAccent);
        accentLayout->addStretch();

        appLayout->addWidget(ghostGroup);
        appLayout->addWidget(visualGroup);
        appLayout->addWidget(folderGroup);
        appLayout->addWidget(accentGroup);
        appLayout->addStretch();

        settingsTabs->addTab(profileTab, "Профиль TG");
        settingsTabs->addTab(privacyTab, "Приватность");
        settingsTabs->addTab(securityTab, "Безопасность");
        settingsTabs->addTab(appTab, "Клиент gGramo");

        settingsMainLayout->addWidget(settingsTabs);

        appPages->addWidget(chatsMasterPage);
        appPages->addWidget(settingsPage);

        mainLayout->addWidget(topBar);
        mainLayout->addWidget(appPages, 1);

        // ===== Бургер-меню: дефолтные настройки TG + раздел настроек мода =====
        burgerMenu_ = new QMenu(this);
        QAction* actChats = burgerMenu_->addAction(renderSvgIcon(SvgData::CHATS), "  Чаты");
        QAction* actTgSettings = burgerMenu_->addAction(renderSvgIcon(SvgData::SETTINGS), "  Настройки Telegram");
        burgerMenu_->addSeparator();
        QAction* actModSettings = burgerMenu_->addAction(makeFolderIcon(true), "  Настройки мода gGramo");
        connect(actChats, &QAction::triggered, this, [this]() { appPages->setCurrentIndex(0); });
        connect(actTgSettings, &QAction::triggered, this, [this]() {
            appPages->setCurrentIndex(1);
            settingsTabs->setCurrentIndex(0);
            requestMeProfile();
        });
        connect(actModSettings, &QAction::triggered, this, [this]() {
            appPages->setCurrentIndex(1);
            settingsTabs->setCurrentIndex(settingsTabs->count() - 1);
        });
        connect(btnBurger, &QToolButton::clicked, this, [this]() {
            QPoint p = btnBurger->mapToGlobal(QPoint(0, btnBurger->height() + 2));
            burgerMenu_->exec(p);
        });

        rootStack->addWidget(authContainer);
        rootStack->addWidget(mainAppWidget);
        setCentralWidget(centralWidget);
        rootLayout->addWidget(rootStack);

        connect(btnWarnOk, &QPushButton::clicked, [=]() { authStack->setCurrentIndex(1); });

        connect(btnSendPhone, &QPushButton::clicked, [=]() {
            QJsonObject req;
            req["@type"] = "setAuthenticationPhoneNumber";
            req["phone_number"] = phoneInput->text().trimmed();
            tdClient.send(req);
        });

        connect(btnSendCode, &QPushButton::clicked, [=]() {
            QJsonObject req;
            req["@type"] = "checkAuthenticationCode";
            req["code"] = codeInput->text().trimmed();
            tdClient.send(req);
        });

        connect(btnSendPass, &QPushButton::clicked, [=]() {
            QJsonObject req;
            req["@type"] = "checkAuthenticationPassword";
            req["password"] = passwordInput->text().trimmed();
            tdClient.send(req);
        });

        connect(btnCancelReply, &QPushButton::clicked, [=]() {
            replyToMsgId = 0;
            replyPreviewWidget->setVisible(false);
        });

        connect(chatListWidget, &QListWidget::itemClicked, [=](QListWidgetItem* item) {
            qint64 chatId = item->data(Qt::UserRole).toLongLong();
            QString title = item->data(Qt::UserRole + 1).toString();
            openChat(chatId, title);
        });

        connect(btnSendMsg, &QPushButton::clicked, this, &GGramoWindow::sendMessage);
        connect(messageInput, &QLineEdit::returnPressed, this, &GGramoWindow::sendMessage);

        connect(btnSaveProfile, &QPushButton::clicked, this, &GGramoWindow::saveTelegramProfile);
        connect(btnSavePrivacy, &QPushButton::clicked, this, &GGramoWindow::saveTelegramPrivacy);
        connect(btnTerminateSessions, &QPushButton::clicked, this, &GGramoWindow::terminateSessions);
        connect(btnSaveTTL, &QPushButton::clicked, this, &GGramoWindow::saveAccountTTL);

        connect(messageListWidget->verticalScrollBar(), &QScrollBar::valueChanged, this, [=](int value) {
            if (value == 0 && !isLoadingHistory && oldestMsgId > 0 && currentChatId != 0) {
                isLoadingHistory = true;
                QJsonObject req;
                req["@type"] = "getChatHistory";
                req["chat_id"] = currentChatId;
                req["from_message_id"] = oldestMsgId;
                req["offset"] = 0;
                req["limit"] = 50;
                req["only_local"] = false;
                req["@extra"] = QString("history_%1").arg(currentChatId);
                tdClient.send(req);
            }
        });

        connect(messageListWidget, &QListWidget::customContextMenuRequested, this, &GGramoWindow::showMessageContextMenu);
        connect(sliderRadius, &QSlider::valueChanged, this, &GGramoWindow::updateStyle);
        connect(btnRefreshFolders, &QPushButton::clicked, this, [this]() { loadFolders(); });
        connect(chkShowAllInFolders, &QCheckBox::toggled, this, [this]() { rebuildFolderBar(); });
        connect(cmbAccent, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int) {
            accentColor_ = cmbAccent->currentData().toString();
            updateStyle(currentRadius_);
        });
        connect(&tdClient, &TdClient::updateReceived, this, &GGramoWindow::handleTdUpdate);

        updateStyle(12);
    }

private slots:
    void openChat(qint64 chatId, const QString& title) {
        currentChatId = chatId;
        oldestMsgId = 0;
        isLoadingHistory = true;
        replyToMsgId = 0;
        replyPreviewWidget->setVisible(false);

        activeChatTitle->setText("<b>" + title + "</b>");
        messageListWidget->clear();
        msgWidgetsMap.clear();

        QJsonObject openReq;
        openReq["@type"] = "openChat";
        openReq["chat_id"] = chatId;
        tdClient.send(openReq);

        QJsonObject req;
        req["@type"] = "getChatHistory";
        req["chat_id"] = chatId;
        req["from_message_id"] = 0;
        req["offset"] = 0;
        req["limit"] = 50;
        req["only_local"] = false;
        req["@extra"] = QString("history_%1").arg(chatId);
        tdClient.send(req);
    }

    void sendMessage() {
        QString text = messageInput->text().trimmed();
        if (text.isEmpty() || currentChatId == 0) return;

        if (chkReadOnReply->isChecked()) {
            QJsonObject readReq;
            readReq["@type"] = "viewMessages";
            readReq["chat_id"] = currentChatId;
            readReq["force_read"] = true;
            tdClient.send(readReq);
        }

        QJsonObject req;
        req["@type"] = "sendMessage";
        req["chat_id"] = currentChatId;
        
        if (replyToMsgId > 0) {
            QJsonObject replyTo;
            replyTo["@type"] = "inputMessageReplyToMessage";
            replyTo["message_id"] = replyToMsgId;
            req["reply_to"] = replyTo;
        }

        QJsonObject content;
        content["@type"] = "inputMessageText";
        QJsonObject formattedText;
        formattedText["text"] = text;
        content["text"] = formattedText;
        
        req["input_message_content"] = content;
        tdClient.send(req);

        messageInput->clear();
        replyToMsgId = 0;
        replyPreviewWidget->setVisible(false);
    }

    void requestMeProfile() {
        QJsonObject req;
        req["@type"] = "getMe";
        tdClient.send(req);
    }

    void saveTelegramProfile() {
        QJsonObject nameReq;
        nameReq["@type"] = "setName";
        nameReq["first_name"] = editFirstName->text().trimmed();
        nameReq["last_name"] = editLastName->text().trimmed();
        tdClient.send(nameReq);

        QJsonObject bioReq;
        bioReq["@type"] = "setBio";
        bioReq["bio"] = editBio->text().trimmed();
        tdClient.send(bioReq);

        QJsonObject unameReq;
        unameReq["@type"] = "setUsername";
        unameReq["username"] = editUsername->text().trimmed();
        tdClient.send(unameReq);

        QMessageBox::information(this, "Telegram", "Запросы на изменение профиля отправлены на сервер!");
    }

    void saveTelegramPrivacy() {
        auto makeRules = [](int mode) {
            QJsonObject rule;
            if (mode == 0) rule["@type"] = "userPrivacySettingRuleAllowAll";
            else if (mode == 1) rule["@type"] = "userPrivacySettingRuleAllowUserContacts";
            else rule["@type"] = "userPrivacySettingRuleRestrictAll";
            QJsonArray arr;
            arr.append(rule);
            QJsonObject rulesObj;
            rulesObj["rules"] = arr;
            return rulesObj;
        };

        QJsonObject pReq;
        pReq["@type"] = "setPrivacySetting";
        QJsonObject phoneSetting;
        phoneSetting["@type"] = "userPrivacySettingShowPhoneNumber";
        pReq["setting"] = phoneSetting;
        pReq["rules"] = makeRules(cmbPhonePrivacy->currentIndex());
        tdClient.send(pReq);

        QJsonObject sReq;
        sReq["@type"] = "setPrivacySetting";
        QJsonObject statusSetting;
        statusSetting["@type"] = "userPrivacySettingShowStatus";
        sReq["setting"] = statusSetting;
        sReq["rules"] = makeRules(cmbLastSeenPrivacy->currentIndex());
        tdClient.send(sReq);

        QMessageBox::information(this, "Telegram", "Настройки приватности обновлены!");
    }

    void terminateSessions() {
        if (QMessageBox::question(this, "Сеансы", "Завершить все остальные сеансы аккаунта?") == QMessageBox::Yes) {
            QJsonObject req;
            req["@type"] = "terminateAllOtherSessions";
            tdClient.send(req);
            QMessageBox::information(this, "Telegram", "Сеансы успешно завершены!");
        }
    }

    void saveAccountTTL() {
        int days = cmbAccountTTL->currentData().toInt();
        QJsonObject req;
        req["@type"] = "setAccountTtl";
        QJsonObject ttlObj;
        ttlObj["days"] = days;
        req["ttl"] = ttlObj;
        tdClient.send(req);
        QMessageBox::information(this, "Telegram", "Период автоудаления обновлен!");
    }

    void showMessageContextMenu(const QPoint& pos) {
        QListWidgetItem* item = messageListWidget->itemAt(pos);
        if (!item) return;

        MessageWidget* widget = qobject_cast<MessageWidget*>(messageListWidget->itemWidget(item));
        if (!widget) return;

        QMenu menu(this);
        menu.setStyleSheet("QMenu { background-color: #0f172a; border: 1px solid #38bdf8; border-radius: 8px; color: #f8fafc; padding: 4px; }"
                           "QMenu::item:selected { background-color: #0ea5e9; }");

        QAction* actReply = menu.addAction(renderSvgIcon(SvgData::REPLY), " Ответить");
        QAction* actLike = menu.addAction(renderSvgIcon(SvgData::LIKE), " Поставить лайк");
        QAction* actFire = menu.addAction(renderSvgIcon(SvgData::FIRE), " Поставить огонь");
        QAction* actHeart = menu.addAction(renderSvgIcon(SvgData::HEART), " Поставить сердце");
        
        QAction* actEdit = nullptr;
        if (widget->isOutgoing()) {
            actEdit = menu.addAction(renderSvgIcon(SvgData::EDIT), " Редактировать");
        }

        QAction* selected = menu.exec(messageListWidget->mapToGlobal(pos));
        if (!selected) return;

        if (selected == actReply) {
            replyToMsgId = widget->msgId();
            replyPreviewText->setText(widget->text().isEmpty() ? "[Медиа]" : widget->text());
            replyPreviewWidget->setVisible(true);
            messageInput->setFocus();
        } else if (selected == actEdit) {
            bool ok;
            QString newText = QInputDialog::getText(this, "Редактирование", "Новый текст:", QLineEdit::Normal, widget->text(), &ok);
            if (ok && !newText.trimmed().isEmpty()) {
                QJsonObject req;
                req["@type"] = "editMessageText";
                req["chat_id"] = currentChatId;
                req["message_id"] = widget->msgId();
                QJsonObject content;
                content["@type"] = "inputMessageText";
                QJsonObject formatted;
                formatted["text"] = newText.trimmed();
                content["text"] = formatted;
                req["input_message_content"] = content;
                tdClient.send(req);
            }
        } else if (selected == actLike || selected == actFire || selected == actHeart) {
            QString emoji = "👍";
            if (selected == actFire) emoji = "🔥";
            if (selected == actHeart) emoji = "❤️";

            QJsonObject req;
            req["@type"] = "addMessageReaction";
            req["chat_id"] = currentChatId;
            req["message_id"] = widget->msgId();
            QJsonObject reactionType;
            reactionType["@type"] = "reactionTypeEmoji";
            reactionType["emoji"] = emoji;
            req["reaction_type"] = reactionType;
            req["is_big"] = false;
            tdClient.send(req);
        }
    }

    void requestFileDownload(int fileId) {
        if (fileId <= 0 || filePaths.contains(fileId)) return;
        QJsonObject req;
        req["@type"] = "downloadFile";
        req["file_id"] = fileId;
        req["priority"] = 1;
        req["offset"] = 0;
        req["limit"] = 0;
        req["synchronous"] = false;
        tdClient.send(req);
    }

    void applyAvatarToLabel(QLabel* label, int fileId, const QString& fallbackTitle, int size) {
        if (!label) return;
        label->setProperty("file_id", fileId);
        label->setProperty("fallback_title", fallbackTitle);
        label->setProperty("avatar_size", size);

        if (fileId > 0 && filePaths.contains(fileId) && QFile::exists(filePaths[fileId])) {
            QPixmap pix(filePaths[fileId]);
            label->setPixmap(makeCircularPixmap(pix, size));
        } else {
            label->setPixmap(createLetterAvatar(fallbackTitle, size));
            if (fileId > 0) {
                pendingAvatars[fileId].append(QPointer<QLabel>(label));
                requestFileDownload(fileId);
            }
        }
    }

    void handleTdUpdate(const QJsonObject& obj) {
        QString type = obj["@type"].toString();

        if (type == "updateAuthorizationState") {
            QJsonObject authState = obj["authorization_state"].toObject();
            QString stateType = authState["@type"].toString();

            if (stateType == "authorizationStateWaitTdlibParameters") {
                QJsonObject params;
                params["@type"] = "setTdlibParameters";
                params["database_directory"] = "ggramo_db";
                params["use_message_database"] = true;
                params["use_secret_chats"] = true;
                params["api_id"] = 22197286;
                params["api_hash"] = "bb9b42aff3becf7f4abd62a0970a27d0";
                params["system_language_code"] = "ru";
                params["device_model"] = "Desktop";
                params["system_version"] = "Arch Linux";
                params["application_version"] = "1.0";
                tdClient.send(params);
            } else if (stateType == "authorizationStateWaitPhoneNumber") {
                rootStack->setCurrentIndex(0);
            } else if (stateType == "authorizationStateWaitCode") {
                rootStack->setCurrentIndex(0);
                authStack->setCurrentIndex(2);
            } else if (stateType == "authorizationStateWaitPassword") {
                rootStack->setCurrentIndex(0);
                authStack->setCurrentIndex(3);
            } else if (stateType == "authorizationStateReady") {
                rootStack->setCurrentIndex(1);
                
                if (chkAlwaysOffline && chkAlwaysOffline->isChecked()) {
                    QJsonObject opt;
                    opt["@type"] = "setOption";
                    opt["name"] = "online";
                    QJsonObject val;
                    val["@type"] = "optionValueBoolean";
                    val["value"] = false;
                    opt["value"] = val;
                    tdClient.send(opt);
                }

                QJsonObject req;
                req["@type"] = "loadChats";
                req["limit"] = 100;
                tdClient.send(req);

                // Папки чатов с аккаунта
                QJsonObject fr;
                fr["@type"] = "getChatFolders";
                fr["@extra"] = "folders_list";
                tdClient.send(fr);
            }
        } else if (type == "user" || type == "updateUser") {
            QJsonObject uObj = (type == "user") ? obj : obj["user"].toObject();
            qint64 uId = jsonToInt64(uObj["id"]);
            QString fName = uObj["first_name"].toString();
            QString lName = uObj["last_name"].toString();
            QString fullName = (fName + " " + lName).trimmed();
            if (!fullName.isEmpty()) userNames[uId] = fullName;

            if (uObj.contains("profile_photo")) {
                QJsonObject photo = uObj["profile_photo"].toObject();
                if (photo.contains("small")) {
                    QJsonObject small = photo["small"].toObject();
                    int fileId = small["id"].toInt();
                    userPhotos[uId] = fileId;
                    if (small["local"].toObject()["is_downloading_completed"].toBool()) {
                        filePaths[fileId] = small["local"].toObject()["path"].toString();
                    }
                }
            }

            if (type == "user") {
                editFirstName->setText(fName);
                editLastName->setText(lName);
                editUsername->setText(uObj["username"].toString());
            }
        } else if (type == "updateFile") {
            QJsonObject file = obj["file"].toObject();
            int fileId = file["id"].toInt();
            QJsonObject local = file["local"].toObject();
            if (local["is_downloading_completed"].toBool()) {
                QString path = local["path"].toString();
                filePaths[fileId] = path;

                if (pendingAvatars.contains(fileId)) {
                    QPixmap pix(path);
                    for (const QPointer<QLabel>& lbl : pendingAvatars[fileId]) {
                        if (lbl) {
                            int size = lbl->property("avatar_size").toInt();
                            if (size <= 0) size = 36;
                            lbl->setPixmap(makeCircularPixmap(pix, size));
                        }
                    }
                    pendingAvatars.remove(fileId);
                }

                if (pendingMediaWidgets.contains(fileId)) {
                    QPixmap pix(path);
                    for (const QPointer<MessageWidget>& msgW : pendingMediaWidgets[fileId]) {
                        if (msgW) {
                            msgW->setMediaPixmap(pix);
                        }
                    }
                    pendingMediaWidgets.remove(fileId);
                }
            }
        } else if (type == "updateNewChat") {
            QJsonObject chat = obj["chat"].toObject();
            qint64 chatId = jsonToInt64(chat["id"]);
            QString title = chat["title"].toString();
            int unread = chat["unread_count"].toInt();
            chatUnreadMap[chatId] = unread;

            int fileId = 0;
            if (chat.contains("photo")) {
                QJsonObject photo = chat["photo"].toObject();
                if (photo.contains("small")) {
                    QJsonObject small = photo["small"].toObject();
                    fileId = small["id"].toInt();
                    if (small["local"].toObject()["is_downloading_completed"].toBool()) {
                        filePaths[fileId] = small["local"].toObject()["path"].toString();
                    }
                }
            }

            QJsonObject lastMsgObj = chat["last_message"].toObject();
            QString lastMsgText = formatMsgContent(lastMsgObj["content"].toObject());
            qint64 dateSecs = jsonToInt64(lastMsgObj["date"]);
            QString timeStr = dateSecs > 0 ? QDateTime::fromSecsSinceEpoch(dateSecs).toString("hh:mm") : "";

            ChatItemWidget* widget = new ChatItemWidget(title, lastMsgText, timeStr, unread);
            applyAvatarToLabel(widget->avatarLabel, fileId, title, 40);

            QListWidgetItem* item = new QListWidgetItem();
            item->setSizeHint(QSize(0, 56));
            item->setData(Qt::UserRole, chatId);
            item->setData(Qt::UserRole + 1, title);

            chatListWidget->addItem(item);
            chatListWidget->setItemWidget(item, widget);
            chatItemsMap[chatId] = item;
            if (chat.contains("folder_id")) applyChatFolder(chatId, chat["folder_id"].toObject());
            refreshChatListVisibility();
        } else if (type == "updateChatFolderId" || type == "updateChatFolder") {
            qint64 chatId = jsonToInt64(obj["chat_id"]);
            if (obj.contains("folder_id")) applyChatFolder(chatId, obj["folder_id"].toObject());
        } else if (type == "updateChatUnreadCount") {
            qint64 chatId = jsonToInt64(obj["chat_id"]);
            int unread = obj["unread_count"].toInt();
            chatUnreadMap[chatId] = unread;
            if (chatItemsMap.contains(chatId)) {
                ChatItemWidget* w = qobject_cast<ChatItemWidget*>(chatListWidget->itemWidget(chatItemsMap[chatId]));
                if (w) w->updateUnread(unread);
            }
        } else if (type == "chatFolders") {
            folders_.clear();
            QJsonArray arr = obj["folders"].toArray();
            for (auto v : arr) {
                QJsonObject f = v.toObject();
                FolderInfo fi;
                fi.id = jsonToInt64(f["id"]);
                fi.name = f["title"].toString();
                if (fi.id > 0 && !fi.name.isEmpty()) folders_.append(fi);
            }
            foldersLoaded_ = true;
            rebuildFolderBar();
            loadFolders();
            refreshChatListVisibility();
        } else if (type == "chats") {
            QString extra = obj["@extra"].toString();
            if (extra.startsWith("folder")) {
                qint64 fid = 0;
                if (extra == "folder_main") fid = 0;
                else fid = extra.section('_', 1).toLongLong();
                QJsonArray arr = obj["chats"].toArray();
                for (auto v : arr) {
                    qint64 cid = jsonToInt64(v);
                    if (fid == 0) { chatsInMainFolder.insert(cid); chatFolderMap[cid] = 0; }
                    else chatFolderMap[cid] = fid;
                }
                refreshChatListVisibility();
            }
        } else if (type == "messages") {
            QString extra = obj["@extra"].toString();
            if (extra != QString("history_%1").arg(currentChatId)) {
                return;
            }

            QJsonArray msgs = obj["messages"].toArray();
            int prevScrollVal = messageListWidget->verticalScrollBar()->value();
            int prevCount = messageListWidget->count();

            for (int i = 0; i < msgs.size(); ++i) {
                QJsonObject msg = msgs[i].toObject();
                qint64 mId = jsonToInt64(msg["id"]);
                if (oldestMsgId == 0 || mId < oldestMsgId) {
                    oldestMsgId = mId;
                }
                addMessageToView(msg, true);
            }

            isLoadingHistory = false;
            if (prevCount == 0) {
                messageListWidget->scrollToBottom();
            } else {
                messageListWidget->verticalScrollBar()->setValue(prevScrollVal + (messageListWidget->count() - prevCount) * 45);
            }
        } else if (type == "updateNewMessage") {
            QJsonObject msg = obj["message"].toObject();
            qint64 chatId = jsonToInt64(msg["chat_id"]);

            if (chatId == currentChatId) {
                addMessageToView(msg, false);
                messageListWidget->scrollToBottom();
            }
        } else if (type == "updateMessageContent") {
            qint64 chatId = jsonToInt64(obj["chat_id"]);
            qint64 msgId = jsonToInt64(obj["message_id"]);
            if (chatId == currentChatId && msgWidgetsMap.contains(msgId)) {
                QString newText = formatMsgContent(obj["new_content"].toObject());
                msgWidgetsMap[msgId]->updateText(newText, true);
            }
        } else if (type == "updateMessageInteractionInfo") {
            qint64 chatId = jsonToInt64(obj["chat_id"]);
            qint64 msgId = jsonToInt64(obj["message_id"]);
            if (chatId == currentChatId && msgWidgetsMap.contains(msgId)) {
                QJsonObject info = obj["interaction_info"].toObject();
                QJsonObject reactionsObj = info["reactions"].toObject();
                QJsonArray reactionsArr = reactionsObj["reactions"].toArray();
                QString str;
                for (auto rVal : reactionsArr) {
                    QJsonObject r = rVal.toObject();
                    QString emoji = r["type"].toObject()["emoji"].toString();
                    int count = r["count"].toInt();
                    if (!emoji.isEmpty()) {
                        str += emoji + " " + QString::number(count) + "  ";
                    }
                }
                msgWidgetsMap[msgId]->setReactions(str.trimmed());
            }
        } else if (type == "updateDeleteMessages") {
            if (obj["from_recall"].toBool()) {
                qint64 chatId = jsonToInt64(obj["chat_id"]);
                if (chatId == currentChatId) {
                    QJsonArray ids = obj["message_ids"].toArray();
                    for (auto val : ids) {
                        qint64 mId = jsonToInt64(val);
                        if (msgWidgetsMap.contains(mId)) {
                            msgWidgetsMap[mId]->markAsDeleted();
                        }
                    }
                }
            }
        } else if (type == "error") {
            authErrorLabel->setText(obj["message"].toString());
        }
    }

private:
    void addMessageToView(const QJsonObject& msg, bool prepend) {
        qint64 msgId = jsonToInt64(msg["id"]);
        bool isOutgoing = msg["is_outgoing"].toBool();
        qint64 dateSecs = jsonToInt64(msg["date"]);
        QString timeStr = QDateTime::fromSecsSinceEpoch(dateSecs).toString("hh:mm");

        qint64 senderUserId = 0;
        QJsonObject senderIdObj = msg["sender_id"].toObject();
        if (senderIdObj["@type"].toString() == "messageSenderUser") {
            senderUserId = jsonToInt64(senderIdObj["user_id"]);
        }

        QString senderName;
        int photoFileId = 0;

        if (!isOutgoing && senderUserId > 0) {
            if (userNames.contains(senderUserId)) {
                senderName = userNames[senderUserId];
            } else {
                senderName = "ID: " + QString::number(senderUserId);
                QJsonObject req;
                req["@type"] = "getUser";
                req["user_id"] = senderUserId;
                tdClient.send(req);
            }
            photoFileId = userPhotos.value(senderUserId, 0);
        }

        MessageWidget* widget = new MessageWidget(msgId, senderName, "", timeStr, isOutgoing);
        if (!isOutgoing) {
            applyAvatarToLabel(widget->avatarLabel, photoFileId, senderName, 30);
        }

        QString text = parseContentAndMedia(msg["content"].toObject(), widget);
        widget->updateText(text, false);

        if (msg.contains("reply_to")) {
            QJsonObject replyTo = msg["reply_to"].toObject();
            if (replyTo["@type"].toString() == "messageReplyToMessage") {
                qint64 replyMsgId = jsonToInt64(replyTo["message_id"]);
                if (msgWidgetsMap.contains(replyMsgId)) {
                    MessageWidget* orig = msgWidgetsMap[replyMsgId];
                    widget->setReply("Ответ на сообщение", orig->text().isEmpty() ? "[Медиа]" : orig->text());
                } else {
                    widget->setReply("Ответ на сообщение", "...");
                }
            }
        }

        if (msg.contains("interaction_info")) {
            QJsonObject info = msg["interaction_info"].toObject();
            QJsonObject reactionsObj = info["reactions"].toObject();
            QJsonArray reactionsArr = reactionsObj["reactions"].toArray();
            QString str;
            for (auto rVal : reactionsArr) {
                QJsonObject r = rVal.toObject();
                QString emoji = r["type"].toObject()["emoji"].toString();
                int count = r["count"].toInt();
                if (!emoji.isEmpty()) {
                    str += emoji + " " + QString::number(count) + "  ";
                }
            }
            widget->setReactions(str.trimmed());
        }

        QListWidgetItem* item = new QListWidgetItem();
        
        auto recalcSize = [=]() {
            item->setSizeHint(widget->sizeHint());
        };
        connect(widget, &MessageWidget::sizeChanged, this, recalcSize);
        recalcSize();

        if (prepend) {
            messageListWidget->insertItem(0, item);
        } else {
            messageListWidget->addItem(item);
        }
        messageListWidget->setItemWidget(item, widget);
        msgWidgetsMap[msgId] = widget;
    }

    QString parseContentAndMedia(const QJsonObject& content, MessageWidget* widget) {
        if (content.isEmpty()) return "";
        QString cType = content["@type"].toString();

        if (cType == "messageText") {
            return content["text"].toObject()["text"].toString();
        } else if (cType == "messagePhoto") {
            QJsonObject photo = content["photo"].toObject();
            QJsonArray sizes = photo["sizes"].toArray();
            if (!sizes.isEmpty()) {
                QJsonObject bestSize = sizes[qMin(sizes.size() - 1, 2)].toObject();
                QJsonObject fileObj = bestSize["photo"].toObject();
                int fileId = fileObj["id"].toInt();
                if (fileId > 0) {
                    widget->photoFileId = fileId;
                    bool completed = fileObj["local"].toObject()["is_downloading_completed"].toBool();
                    QString path = fileObj["local"].toObject()["path"].toString();
                    if (completed && !path.isEmpty() && QFile::exists(path)) {
                        widget->setMediaPixmap(QPixmap(path));
                    } else {
                        pendingMediaWidgets[fileId].append(QPointer<MessageWidget>(widget));
                        requestFileDownload(fileId);
                    }
                }
            }
            return content["caption"].toObject()["text"].toString();
        } else if (cType == "messageSticker") {
            QJsonObject sticker = content["sticker"].toObject();
            QString emoji = sticker["emoji"].toString();
            QJsonObject photoObj = sticker["sticker"].toObject();
            if (!photoObj.contains("id")) {
                photoObj = sticker["thumbnail"].toObject()["file"].toObject();
            }
            int fileId = photoObj["id"].toInt();
            if (fileId > 0) {
                widget->photoFileId = fileId;
                bool completed = photoObj["local"].toObject()["is_downloading_completed"].toBool();
                QString path = photoObj["local"].toObject()["path"].toString();
                if (completed && !path.isEmpty() && QFile::exists(path)) {
                    widget->setMediaPixmap(QPixmap(path));
                } else {
                    pendingMediaWidgets[fileId].append(QPointer<MessageWidget>(widget));
                    requestFileDownload(fileId);
                }
            }
            return emoji.isEmpty() ? "[Стикер]" : emoji;
        } else if (cType == "messageAnimation") {
            QJsonObject anim = content["animation"].toObject();
            QJsonObject minithumb = anim["thumbnail"].toObject()["file"].toObject();
            int fileId = minithumb["id"].toInt();
            if (fileId > 0) {
                widget->photoFileId = fileId;
                bool completed = minithumb["local"].toObject()["is_downloading_completed"].toBool();
                QString path = minithumb["local"].toObject()["path"].toString();
                if (completed && !path.isEmpty() && QFile::exists(path)) {
                    widget->setMediaPixmap(QPixmap(path));
                } else {
                    pendingMediaWidgets[fileId].append(QPointer<MessageWidget>(widget));
                    requestFileDownload(fileId);
                }
            }
            return content["caption"].toObject()["text"].toString();
        } else if (cType == "messageVoiceNote") {
            return "[Голосовое сообщение]";
        } else if (cType == "messageDocument") {
            return "[Файл]";
        }
        return "[Сообщение]";
    }

    QString formatMsgContent(const QJsonObject& content) {
        if (content.isEmpty()) return "";
        QString cType = content["@type"].toString();
        if (cType == "messageText") return content["text"].toObject()["text"].toString();
        if (cType == "messagePhoto") return "[Фото]";
        if (cType == "messageVideo") return "[Видео]";
        if (cType == "messageSticker") return "[Стикер]";
        if (cType == "messageVoiceNote") return "[Голосовое]";
        if (cType == "messageDocument") return "[Файл]";
        return "[Сообщение]";
    }

    void updateStyle(int radius) {
        currentRadius_ = radius;
        const QString A = accentColor_;
        QString style = QString(
            "QMainWindow { background: transparent; }"
            "QWidget { color: #e2e8f0; font-family: 'Inter', sans-serif; border: none; outline: none; }"
            "#authCard { background-color: rgba(15, 23, 42, 0.95); border: 1px solid rgba(%2); border-radius: %1px; }"
            "#topBar { background-color: rgba(15, 23, 42, 0.85); border-radius: %1px; }"
            "#topBar QToolButton { background: transparent; border: none; border-radius: 6px; padding: 4px; }"
            "#topBar QToolButton:hover { background-color: rgba(%3); }"
            "#chatListPanel { background-color: rgba(15, 23, 42, 0.75); border-radius: %1px; }"
            "#folderBar { background: transparent; border: none; max-height: 40px; }"
            "#folderBar::item { border: none; background: transparent; margin-right: 4px; }"
            "#folderBar::item:selected { background-color: rgba(%3); border-radius: 14px; }"
            "#chatList { background-color: transparent; border: none; outline: none; }"
            "#messageList { background-color: rgba(15, 23, 42, 0.7); border-radius: %1px; outline: none; border: none; }"
            "#chatList::item, #messageList::item { border: none; background: transparent; outline: none; padding: 0px; margin: 2px 0px; }"
            "#chatList::item:hover { background-color: rgba(%3); border-radius: %1px; }"
            "#chatList::item:selected { background-color: rgba(%5); border-left: 3px solid %4; border-radius: %1px; }"
            "#chatDetail { background-color: rgba(15, 23, 42, 0.6); border-radius: %1px; }"
            "QLabel { background: transparent; border: none; padding: 0px; margin: 0px; }"
            "QGroupBox { font-weight: bold; color: %4; border: 1px solid rgba(%2); border-radius: %1px; margin-top: 10px; padding-top: 15px; }"
            "QPushButton { background-color: rgba(30, 41, 59, 0.8); border: 1px solid rgba(255,255,255,0.1); border-radius: %1px; padding: 8px 12px; font-weight: bold; color: #f8fafc; }"
            "QPushButton:hover { background-color: %4; color: #ffffff; }"
            "QLineEdit { background-color: rgba(15, 23, 42, 0.9); border: 1px solid rgba(%2); border-radius: %1px; padding: 8px; color: #ffffff; }"
            "QLineEdit:focus { border: 1px solid %4; }"
            "QComboBox { background-color: rgba(15, 23, 42, 0.9); border: 1px solid rgba(%2); border-radius: %1px; padding: 6px; color: #ffffff; }"
            "QCheckBox { spacing: 8px; }"
            "QCheckBox::indicator { width: 18px; height: 18px; border-radius: 4px; border: 1px solid %4; }"
            "QCheckBox::indicator:checked { background-color: %4; }"
            "QMenu { background-color: #0f172a; border: 1px solid rgba(%2); border-radius: 10px; color: #f8fafc; padding: 6px; }"
            "QMenu::item { padding: 8px 26px; border-radius: 6px; }"
            "QMenu::item:selected { background-color: rgba(%3); }"
            "QMenu::separator { height: 1px; background: rgba(255,255,255,0.08); margin: 4px 8px; }"
            "QScrollBar:vertical { background: transparent; width: 8px; }"
            "QScrollBar::handle:vertical { background: rgba(%2); border-radius: 4px; min-height: 30px; }"
            "QScrollBar::add-line, QScrollBar::sub-line { height: 0px; }"
        ).arg(radius)
         .arg(hexToRgbaStr(A, 102))   // %2 мягкие рамки
         .arg(hexToRgbaStr(A, 30))    // %3 hover-подложки
         .arg(A)                      // %4 чистый акцент
         .arg(hexToRgbaStr(A, 64));   // %5 selected
        this->setStyleSheet(style);
    }

    TdClient tdClient;
    qint64 currentChatId{0};
    qint64 oldestMsgId{0};
    qint64 replyToMsgId{0};
    bool isLoadingHistory{false};

    QMap<qint64, QListWidgetItem*> chatItemsMap;
    QMap<qint64, int> chatUnreadMap;
    QMap<qint64, MessageWidget*> msgWidgetsMap;

    QMap<qint64, QString> userNames;
    QMap<qint64, int> userPhotos;
    QMap<int, QString> filePaths;
    QMap<int, QList<QPointer<QLabel>>> pendingAvatars;
    QMap<int, QList<QPointer<MessageWidget>>> pendingMediaWidgets;

    QStackedWidget* rootStack;
    QStackedWidget* authStack;
    QStackedWidget* appPages;
    
    QListWidget* chatListWidget;
    QListWidget* messageListWidget;
    QLineEdit* messageInput;
    QLabel* activeChatTitle;

    QWidget* replyPreviewWidget;
    QLabel* replyPreviewTitle;
    QLabel* replyPreviewText;

    QLineEdit* editFirstName;
    QLineEdit* editLastName;
    QLineEdit* editBio;
    QLineEdit* editUsername;
    QComboBox* cmbPhonePrivacy;
    QComboBox* cmbLastSeenPrivacy;
    QComboBox* cmbAccountTTL;

    QLineEdit* phoneInput;
    QLineEdit* codeInput;
    QLineEdit* passwordInput;
    QLabel* authErrorLabel;

    QCheckBox* chkReadOnReply;
    QCheckBox* chkAlwaysOffline;
    QCheckBox* chkShowAllInFolders{nullptr};
    QComboBox* cmbAccent{nullptr};
    QTabWidget* settingsTabs{nullptr};
    QToolButton* btnBurger{nullptr};
    QMenu* burgerMenu_{nullptr};
    QListWidget* folderBar_{nullptr};

    QString accentColor_{"#38bdf8"};
    int currentRadius_{12};

    qint64 currentFolderId_{0}; // 0 = Все чаты
    struct FolderInfo { qint64 id; QString name; };
    QVector<FolderInfo> folders_;
    QMap<qint64, qint64> chatFolderMap;   // chatId -> folderId
    QSet<qint64> chatsInMainFolder;       // chatIds с inputChatFolderIdMain
    bool mainFolderComplete_{false};
    bool foldersLoaded_{false};

    void loadFolders() {
        // Список папок аккаунта
        QJsonObject fr;
        fr["@type"] = "getChatFolders";
        fr["@extra"] = "folders_list";
        tdClient.send(fr);
        // Чаты текущей (основной) папки
        QJsonObject cur;
        cur["@type"] = "getChats";
        QJsonObject curFolder; curFolder["@type"] = "chatFolderIdCurrent";
        cur["folder_id"] = curFolder;
        cur["limit"] = 300;
        cur["@extra"] = "folder_main";
        tdClient.send(cur);
        // Чаты каждой регулярной папки
        for (const FolderInfo& f : folders_) {
            QJsonObject r;
            r["@type"] = "getChats";
            QJsonObject fidObj; fidObj["@type"] = "chatFolderIdRegular"; fidObj["id"] = QString::number(f.id);
            r["folder_id"] = fidObj;
            r["limit"] = 300;
            r["@extra"] = QString("folder_%1").arg(f.id);
            tdClient.send(r);
        }
    }

    static qint64 folderIdFromInfo(const QJsonObject& folderInfo) {
        if (folderInfo.isEmpty()) return 0;
        QString t = folderInfo["@type"].toString();
        if (t == "chatFolderIdCurrent") return 0;
        if (t == "chatFolderIdRegular") return jsonToInt64(folderInfo["id"]);
        return 0;
    }

    void rebuildFolderBar() {
        if (!folderBar_) return;
        folderBar_->blockSignals(true);
        folderBar_->clear();
        bool hasFolders = !folders_.isEmpty();
        folderBar_->setVisible(hasFolders);

        auto addTab = [&](const QString& name, qint64 fid) {
            QListWidgetItem* it = new QListWidgetItem(name);
            it->setData(Qt::UserRole, fid);
            it->setSizeHint(QSize(0, 30));
            it->setTextAlignment(Qt::AlignCenter);
            folderBar_->addItem(it);
            if (fid == currentFolderId_) folderBar_->setCurrentItem(it);
        };
        if (chkShowAllInFolders && chkShowAllInFolders->isChecked()) addTab("Все", 0);
        for (const FolderInfo& f : folders_) addTab(f.name, f.id);
        if (hasFolders && folderBar_->currentRow() < 0) folderBar_->setCurrentRow(0);
        folderBar_->blockSignals(false);
    }

    void switchFolder(qint64 folderId) {
        currentFolderId_ = folderId;
        refreshChatListVisibility();
    }

    void refreshChatListVisibility() {
        for (auto it = chatItemsMap.begin(); it != chatItemsMap.end(); ++it) {
            QListWidgetItem* item = it.value();
            if (!item) continue;
            bool vis = true;
            if (currentFolderId_ != 0) {
                vis = chatFolderMap.value(it.key(), -1) == currentFolderId_;
            } else if (!folders_.isEmpty() && !(chkShowAllInFolders && chkShowAllInFolders->isChecked())) {
                vis = true;
            }
            item->setHidden(!vis);
        }
    }

    void applyChatFolder(qint64 chatId, const QJsonObject& folderInfo) {
        qint64 fid = folderIdFromInfo(folderInfo);
        QString t = folderInfo["@type"].toString();
        if (t == "chatFolderIdMain") {
            chatsInMainFolder.insert(chatId);
            chatFolderMap[chatId] = 0;
        } else if (t == "chatFolderIdRegular") {
            chatFolderMap[chatId] = fid;
        } else {
            chatFolderMap.remove(chatId);
        }
        refreshChatListVisibility();
    }
};

class SplashOverlay : public QWidget {
public:
    explicit SplashOverlay(QWidget* mainWindow) : QWidget(mainWindow->topLevelWidget()) {
        setGeometry(mainWindow->rect());
        splash_ = new SplashWidget(this);
        splash_->setGeometry(rect());
        splash_->show();
        connect(splash_, &SplashWidget::finished, this, [this]() {
            if (fadeOut_) return;
            fadeOut_ = true;
            QGraphicsOpacityEffect* eff = new QGraphicsOpacityEffect(this);
            setGraphicsEffect(eff);
            QPropertyAnimation* a = new QPropertyAnimation(eff, "opacity", this);
            a->setDuration(450);
            a->setStartValue(1.0);
            a->setEndValue(0.0);
            a->setEasingCurve(QEasingCurve::InQuad);
            connect(a, &QPropertyAnimation::finished, this, &QObject::deleteLater);
            a->start();
        });
    }
    void resizeEvent(QResizeEvent*) override {
        if (splash_) splash_->setGeometry(rect());
    }
private:
    SplashWidget* splash_;
    bool fadeOut_{false};
};

int main(int argc, char *argv[]) {
    QJsonObject logReq;
    logReq["@type"] = "setLogVerbosityLevel";
    logReq["new_verbosity_level"] = 1;
    QJsonDocument doc(logReq);
    td_json_client_execute(nullptr, doc.toJson(QJsonDocument::Compact).constData());

    QApplication app(argc, argv);

    GGramoWindow window;
    window.show();

    // Splash-экран поверх окна: SVG-логотип + анимация названия мода
    SplashOverlay* overlay = new SplashOverlay(&window);
    overlay->show();
    overlay->raise();

    return app.exec();
}

#include "main.moc"
