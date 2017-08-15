#include <QUrl>
#include <QtDebug>
#include <QDesktopWidget>
#include <QApplication>

#include "measure_widget.h"
#include "utils.h"
#include "vs_char_set.h"

static const QString g_strMediaPlayState[]           = {"停止播放", "正在播放", "暂停播放"};

static const QString g_strMeasureHStartGif           = "Resource/MeasureHeightStart.gif";
static const QString g_strMeasureHDingPng            = "Resource/MeasureHeightDoing.png";
static const QString g_strMeasureHFinishPng          = "Resource/MeasureHeightFinish.png";
static const QString g_strMeasureHTimeoutPng         = "Resource/MeasureHeightTimeout.png";
static const QString g_strMeasureHTooLowPng          = "Resource/MeasureHeightToolow.png";

static const QString g_strLocalHeightStartWav        = "Resource/MeasureHeightStart.wav";
static const QString g_strLocalHeightDoingWav        = "Resource/MeasureHeightDoing.wav";
static const QString g_strLocalHeightFinishWav       = "Resource/MeasureHeightFinish.wav";

static const QString g_strMeasureFStartGif           = "Resource/MeasureBodyFatStart.gif";
static const QString g_strMeasureFDingPng            = "Resource/MeasureBodyFatDoing.png";
static const QString g_strMeasureFFinishPng          = "Resource/MeasureBodyFatFinish.png";

static const QString g_strLocalFatStartWav           = "Resource/MeasureBodyFatStart.wav";
static const QString g_strLocalFatDoingWav           = "Resource/MeasureBodyFatDoing.wav";
static const QString g_strLocalFatFinishWav          = "Resource/MeasureBodyFatFinish.wav";

static const int g_nMeasureTimeout                   = 20000; // 20秒超时


MeasureWidget::MeasureWidget(QWidget *parent)
    : QWidget(parent)
    , m_stage(OFF_STAGE)
    , m_state(OFF_STATE)
{
    QDesktopWidget *desktop = QApplication::desktop();
    if (NULL == desktop) {
        qCritical() << "获取屏幕桌面信息失败，窗口初始化失败！";
        return;
    }
    QRect rect = desktop->screenGeometry(desktop->primaryScreen());
    this->resize(rect.width() * 0.8, rect.height() * 0.8);
    this->setWindowFlags(Qt::FramelessWindowHint);

    this->hide();
    m_pPlayerAudio = new QMediaPlayer(this);
    connect(m_pPlayerAudio, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(slotAudioStateChanged(QMediaPlayer::State)));

    m_pMainLabel = new QLabel(this);
    m_pMainLabel->resize(this->size());
    m_pMovie = new QMovie(this);

    m_pTimer = new QTimer(this);
    m_pTimer->setSingleShot(true);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(slotMeasureTimeout()));
}

MeasureWidget::~MeasureWidget()
{
}

void MeasureWidget::prepareMeasureHeight()
{
    m_stage = MEASURE_HEIGHT_STAGE;
    m_state = PRE_STATE;
    showMeasureGIF(g_strMeasureHStartGif);
    playMeasureAudio(g_strLocalHeightStartWav);
    this->show();
}

void MeasureWidget::prepareMeasureFat()
{
    m_stage = MEASURE_WEIGHT_STAGE;
    m_state = PRE_STATE;
    showMeasureGIF(g_strMeasureFStartGif);
    playMeasureAudio(g_strLocalFatStartWav);
    this->show();
}

void MeasureWidget::slotGetHeightData()
{
    m_state = FINISH_STATE;
    m_pTimer->stop();
    showMeasurePNG(g_strMeasureHFinishPng);
    playMeasureAudio(g_strLocalHeightFinishWav);
}

void MeasureWidget::slotAudioStateChanged(QMediaPlayer::State newState)
{
    qDebug() << "音频播放状态转换为：" << g_strMediaPlayState[newState];

    if (QMediaPlayer::StoppedState == newState) {
        if (MEASURE_HEIGHT_STAGE == m_stage) {
            switch (m_state) {
            case PRE_STATE:
                showMeasurePNG(g_strMeasureHDingPng);
                playMeasureAudio(g_strLocalHeightDoingWav);
                m_state = ON_STATE;
                break;
            case ON_STATE:
                m_pTimer->start(g_nMeasureTimeout);
                emit sigMeasureHeightStart();
                break;
            case FINISH_STATE:
                this->hide(); // hide main label
                emit sigMeasureHeightFinish();
                break;
            default:
                break;
            }
        } else if (MEASURE_WEIGHT_STAGE == m_stage) {
            switch (m_state) {
            case PRE_STATE:
                showMeasurePNG(g_strMeasureFDingPng);
                playMeasureAudio(g_strLocalFatDoingWav);
                m_state = ON_STATE;
                break;
            case ON_STATE:
                m_pTimer->start(g_nMeasureTimeout);
                emit sigMeasureFatStart();
                break;
            case FINISH_STATE:
                this->hide(); // hide main label
                emit sigMeasureFatFinish();
                break;
            default:
                break;
            }
        }
    }
}

void MeasureWidget::slotMeasureTimeout()
{
    if (MEASURE_HEIGHT_STAGE == m_stage) {
        if (FINISH_STATE != m_state) {
            emit sigMeasureHeightTimeout();
        }
    } else if (MEASURE_WEIGHT_STAGE == m_stage) {
        if (FINISH_STATE != m_state) {
            emit sigMeasureFatTimeout();
        }
    }
}

void MeasureWidget::slotMeasureFatFinish()
{
    m_state = FINISH_STATE;
    m_pTimer->stop();
    this->hide();
}

void MeasureWidget::showMeasurePNG(const QString &strFileName)
{
    QString strFilePath = CUtils::getFileFullPath(strFileName);
    qDebug() << "将要展示的图片：" << strFilePath;
    m_pixmapMain.load(strFilePath);
    m_pMainLabel->setPixmap(m_pixmapMain);
    m_pMainLabel->setScaledContents(true);
    if (QMovie::Running == m_pMovie->state()) {
        m_pMovie->stop();
    }
    m_pMainLabel->show();
}

void MeasureWidget::showMeasureGIF(const QString &strFileName)
{
    QString strFilePath = CUtils::getFileFullPath(strFileName);
    qDebug() << "将要播放的GIF文件：" << strFilePath;
    m_pMovie->setFileName(strFilePath);
    m_pMainLabel->setMovie(m_pMovie);
    m_pMainLabel->setScaledContents(true);
    /* GIF播放的时候测试需要这样先stop */
    m_pMovie->stop();
    m_pMovie->start();
    m_pMainLabel->show();
}

void MeasureWidget::playMeasureAudio(const QString &strFileName)
{
    QString strFilePath = CUtils::getFileFullPath(strFileName);
    if (QMediaPlayer::StoppedState != m_pPlayerAudio->state())
        m_pPlayerAudio->stop();
    qDebug() << "将要播放的音频文件:" << strFilePath;
    m_pPlayerAudio->setMedia(QUrl::fromLocalFile(strFilePath));
    m_pPlayerAudio->play();
}

