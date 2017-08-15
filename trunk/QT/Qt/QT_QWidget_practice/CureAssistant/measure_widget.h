#ifndef MEASUREWIDGET_H
#define MEASUREWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QMediaPlayer>
#include <QMovie>
#include <QTimer>

class MeasureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MeasureWidget(QWidget *parent = 0);
    virtual ~MeasureWidget();

    enum state {
        OFF_STATE = 0,
        PRE_STATE,
        ON_STATE,
        FINISH_STATE,
        FAILED_STATE
    };

    enum stage {
        OFF_STAGE = 0,
        MEASURE_HEIGHT_STAGE,
        MEASURE_WEIGHT_STAGE,
    };

    void prepareMeasureHeight();
    void prepareMeasureFat();

signals:
    void sigMeasureHeightStart();
    void sigMeasureHeightFinish();
    void sigMeasureFatStart();
    void sigMeasureFatFinish();
    void sigMeasureHeightTimeout();
    void sigMeasureFatTimeout();

public slots:
    void slotGetHeightData();
    void slotAudioStateChanged(QMediaPlayer::State newState);
    void slotMeasureTimeout();
    void slotMeasureFatFinish();

private:
    void showMeasurePNG(const QString &strFileName);
    void showMeasureGIF(const QString &strFileName);
    void playMeasureAudio(const QString &strFileName);
private:
    stage m_stage;
    state m_state;
    QPixmap m_pixmapMain;
    QMediaPlayer *m_pPlayerAudio;
    QLabel *m_pMainLabel;
    QMovie *m_pMovie;
    QTimer *m_pTimer;
};

#endif // MEASUREWIDGET_H
