#ifndef FATWIDGET_H
#define FATWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#include <QLayout>

#include "weight_device_thread.h"
#include "measure_widget.h"

class FatWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FatWidget(QWidget *parent = 0);
    virtual ~FatWidget();

    void initWeightDevice();
    void showResultPicture(const QString &);
    void closeMeasureDevice();
signals:
    void sigMeasureFinish();

public slots:
    void slotOpenWeightDevice();
    void slotDeviceOpenSuccess();
    void slotDeviceOpenFailed(const QString &);
    void slotPrepareMeasure();
    void slotStartMeasure();
    void slotFinishMeasure();
    void slotHandleReceiveData(const enumWeightDataType &type, const struWeightData &data);
    void slotDeleteMeasureWidget();
    void slotMeasureTimeout();
private:
    QGroupBox *m_pGroupBoxOpen;
    QComboBox *m_pComboBoxSerialPort;
    QPushButton *m_pButtonOpenSerial;

    QGroupBox *m_pGroupBoxMeasure;
    QPushButton *m_pButtonStartMeasure;
    QLabel *m_pLabelPic;
    QPixmap m_pixmap;

    QVBoxLayout *m_pMainLayout;
    CWeightDeviceThread *m_pWeightDevice;
    MeasureWidget *m_pMeasureWidget;

};

#endif // FATWIDGET_H
