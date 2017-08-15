#ifndef HEIGHTWIDGET_H
#define HEIGHTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QGroupBox>

#include "height_device_thread.h"
#include "measure_widget.h"

enum {
    MEASURE_TIMES  = 3
};

class HeightWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HeightWidget(QWidget *parent = 0);
    virtual ~HeightWidget();

    enum enumAdjustType {
        TYPE_ADJUST_NONE,
        TYPE_ADJUST_INCREASE,
        TYPE_ADJUST_DECREASE
    };

signals:
    void sigGetDataFinish();

public slots:
    void slotDeviceOpenSuccess();
    void slotDeviceOpenFailed(const QString &);
    void slotOpenHeightDevice();
    void slotPrepareMeasure();
    void slotStartMeasure();
    void slotFinishMeasure();
    void slotMeasureTimeout();
    void slotHandleMeasureData(const float &);
    void slotAdjustIncrease();
    void slotAdjustDecrease();
    void slotGetStandardData(const int &);
    void slotSetStandardSucess();

private:
    void initHeightDevice();

private:
//    QLabel *m_pLabelSerial;
    QGroupBox *m_pGroupBoxOpen;
    QComboBox *m_pComboBoxSerialPort;
    QPushButton *m_pButtonOpenSerial;

//    QLabel *m_pLabelHeightMeasure;
    QGroupBox *m_pGroupBoxMeasure;
    QPushButton *m_pButtonStartHeight;
    QLabel *m_pLabelHeightValue;
    QLineEdit *m_pLineEditHeightValue;
    QLabel *m_pLabelHeightUnit;

//    QLabel *m_pLabelHeightAdjust;
    QGroupBox *m_pGroupBoxAdjust;
    QLabel *m_pLabelIncrease;
    QLabel *m_pLabelDecrease;
    QSpinBox *m_pSpinBoxIncrease;
    QSpinBox *m_pSpinBoxDecrease;
    QLabel *m_pLabelIncreaseUnit;
    QLabel *m_pLabelDecreaseUnit;
    QPushButton *m_pButtonSetIncrease;
    QPushButton *m_pButtonSetDecrease;

    QVBoxLayout *m_pMainLayout;
    CHeightDeviceThread *m_pHeightDevice;
    MeasureWidget *m_pMeasureWidget;
    int m_measureTimes;
    float m_fHTemp[MEASURE_TIMES];
    enumAdjustType m_typeAdjust;
};

#endif // HEIGHTWIDGET_H
