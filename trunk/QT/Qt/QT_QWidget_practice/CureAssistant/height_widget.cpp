#include <QSerialPortInfo>
#include <QList>
#include <QtDebug>
#include <QMessageBox>

#include "height_widget.h"
#include "utils.h"
#include "vs_char_set.h"

const int g_leftMargin = 10;
const float g_fDeviation = 1.0; //cm


HeightWidget::HeightWidget(QWidget *parent)
    : QWidget(parent)
{
    m_pHeightDevice = new CHeightDeviceThread(this);

    /* 串口打开操作 */
    m_pGroupBoxOpen = new QGroupBox(tr("串口选择"), this);
    m_pComboBoxSerialPort = new QComboBox(this);
    QList<QSerialPortInfo> listSerialPorts = QSerialPortInfo::availablePorts();
    if (!listSerialPorts.isEmpty()) {
        int i = 0;
        foreach (QSerialPortInfo info, listSerialPorts) {
            m_pComboBoxSerialPort->addItem(info.portName());
            if (info.portName().toLower() == "com1") {
                m_pComboBoxSerialPort->setCurrentIndex(i); // COM1
            }
            ++i;
        }
    }
    m_pComboBoxSerialPort->setEnabled(false);
    m_pButtonOpenSerial = new QPushButton(tr("打开串口"), this);
    connect(m_pButtonOpenSerial, SIGNAL(clicked(bool)), this, SLOT(slotOpenHeightDevice()));

    QHBoxLayout *pHLayoutSerial = new QHBoxLayout;
    pHLayoutSerial->addSpacing(g_leftMargin);
    pHLayoutSerial->addWidget(m_pComboBoxSerialPort);
    pHLayoutSerial->addWidget(m_pButtonOpenSerial);
    pHLayoutSerial->addStretch(1);
    pHLayoutSerial->setStretch(0, 4);
    pHLayoutSerial->setStretch(1, 2);

    m_pGroupBoxOpen->setLayout(pHLayoutSerial);

    /*### 身高测量操作 */
    m_pGroupBoxMeasure = new QGroupBox(tr("身高测量"), this);
    m_pButtonStartHeight = new QPushButton(tr("开始测量身高"), this);
    m_pLabelHeightValue = new QLabel(tr("身高测量结果:"), this);
    m_pLineEditHeightValue = new QLineEdit(this);
    m_pLabelHeightUnit = new QLabel(tr("cm(厘米)"), this);
    /* 只有串口成功打开才使能身高测量按钮 */
    m_pButtonStartHeight->setEnabled(false);
    m_pLineEditHeightValue->setReadOnly(true);
    connect(m_pButtonStartHeight, SIGNAL(clicked(bool)), this, SLOT(slotPrepareMeasure()));

    QHBoxLayout *pHLayoutHeightMeasure = new QHBoxLayout;
    pHLayoutHeightMeasure->addSpacing(g_leftMargin);
    pHLayoutHeightMeasure->addWidget(m_pButtonStartHeight);
    pHLayoutHeightMeasure->addStretch(1);
    pHLayoutHeightMeasure->setStretch(1, 1);
    QHBoxLayout *pHLayoutHeightValue = new QHBoxLayout;
    pHLayoutHeightValue->addSpacing(g_leftMargin);
    pHLayoutHeightValue->addWidget(m_pLabelHeightValue);
    pHLayoutHeightValue->addWidget(m_pLineEditHeightValue);
    pHLayoutHeightValue->addWidget(m_pLabelHeightUnit);
    pHLayoutHeightValue->addStretch(1);
    pHLayoutHeightValue->setStretch(1, 1);
    pHLayoutHeightValue->setStretch(2, 1);
    pHLayoutHeightValue->setStretch(3, 1);

    QVBoxLayout *pVLayoutMeasure = new QVBoxLayout;
    pVLayoutMeasure->addLayout(pHLayoutHeightMeasure);
    pVLayoutMeasure->addLayout(pHLayoutHeightValue);
    m_pGroupBoxMeasure->setLayout(pVLayoutMeasure);

    /*### 身高校正操作 */
    m_pGroupBoxAdjust = new QGroupBox(tr("身高校正"), this);
    m_pLabelIncrease = new QLabel(tr("增加:"), this);
    m_pLabelDecrease = new QLabel(tr("减小:"), this);
    m_pSpinBoxIncrease = new QSpinBox(this);
    m_pSpinBoxDecrease = new QSpinBox(this);
    m_pLabelIncreaseUnit = new QLabel(tr("mm(毫米)"), this);
    m_pLabelDecreaseUnit = new QLabel(tr("mm(毫米)"), this);
    m_pButtonSetIncrease = new QPushButton(tr("确定"), this);
    m_pButtonSetDecrease = new QPushButton(tr("确定"), this);

    m_pSpinBoxIncrease->setEnabled(false);
    m_pSpinBoxDecrease->setEnabled(false);
    m_pButtonSetIncrease->setEnabled(false);
    m_pButtonSetDecrease->setEnabled(false);
    m_pSpinBoxIncrease->setRange(1, 1000);
    m_pSpinBoxDecrease->setRange(1, 1000);
    connect(m_pButtonSetIncrease, SIGNAL(clicked(bool)), this, SLOT(slotAdjustIncrease()));
    connect(m_pButtonSetDecrease, SIGNAL(clicked(bool)), this, SLOT(slotAdjustDecrease()));

    QHBoxLayout *pHLayoutIncrease = new QHBoxLayout;
    pHLayoutIncrease->addSpacing(g_leftMargin);
    pHLayoutIncrease->addWidget(m_pLabelIncrease);
    pHLayoutIncrease->addWidget(m_pSpinBoxIncrease);
    pHLayoutIncrease->addWidget(m_pLabelIncreaseUnit);
    pHLayoutIncrease->addWidget(m_pButtonSetIncrease);
    pHLayoutIncrease->setStretch(2, 1);
    pHLayoutIncrease->setStretch(4, 1);
    QHBoxLayout *pHLayoutDecrease = new QHBoxLayout;
    pHLayoutDecrease->addSpacing(g_leftMargin);
    pHLayoutDecrease->addWidget(m_pLabelDecrease);
    pHLayoutDecrease->addWidget(m_pSpinBoxDecrease);
    pHLayoutDecrease->addWidget(m_pLabelDecreaseUnit);
    pHLayoutDecrease->addWidget(m_pButtonSetDecrease);
    pHLayoutDecrease->setStretch(2, 1);
    pHLayoutDecrease->setStretch(4, 1);

    QVBoxLayout *pVLayoutAdjust = new QVBoxLayout;
    pVLayoutAdjust->addLayout(pHLayoutIncrease);
    pVLayoutAdjust->addLayout(pHLayoutDecrease);
    m_pGroupBoxAdjust->setLayout(pVLayoutAdjust);

    /*### 主窗口Layout */
    m_pMainLayout = new QVBoxLayout(this);
    m_pMainLayout->addWidget(m_pGroupBoxOpen);
    m_pMainLayout->addWidget(m_pGroupBoxMeasure);
    m_pMainLayout->addWidget(m_pGroupBoxAdjust);

    m_pMeasureWidget = NULL;
    initHeightDevice();

}

HeightWidget::~HeightWidget()
{
    if (m_pMeasureWidget) {
        m_pMeasureWidget->deleteLater();
        m_pMeasureWidget = NULL;
    }
}

void HeightWidget::slotDeviceOpenSuccess()
{
    qDebug() << "打开身高测量设备成功!";
    m_pButtonOpenSerial->setText(tr("关闭串口"));
    m_pButtonStartHeight->setEnabled(true);

    m_pSpinBoxDecrease->setEnabled(true);
    m_pSpinBoxIncrease->setEnabled(true);
    m_pButtonSetIncrease->setEnabled(true);
    m_pButtonSetDecrease->setEnabled(true);
}

void HeightWidget::slotDeviceOpenFailed(const QString &strError)
{
    qDebug() << "打开身高测量设备失败!";
    m_pButtonOpenSerial->setText(tr("打开串口"));
    QMessageBox::critical(this, tr("严重错误"), tr("串口打开失败, ") + strError);
}

void HeightWidget::slotOpenHeightDevice()
{
    if (m_pButtonOpenSerial->text() == tr("打开串口")) {
        qDebug() << "\r\n";
        qDebug() << "打开身高测量设备...";
        m_pHeightDevice->openDevice();
    } else {
        /* 关闭串口 */
        qDebug() << "关闭身高测量设备...";
        m_pHeightDevice->closeDevice();
        m_pButtonOpenSerial->setText(tr("打开串口"));
        m_pButtonStartHeight->setEnabled(false);

        m_pSpinBoxDecrease->setEnabled(false);
        m_pSpinBoxIncrease->setEnabled(false);
        m_pButtonSetIncrease->setEnabled(false);
        m_pButtonSetDecrease->setEnabled(false);
    }
}

void HeightWidget::slotPrepareMeasure()
{
    if (m_pButtonStartHeight->text() == tr("开始测量身高")) {
        m_pButtonStartHeight->setText("终止测量身高");

        m_pMeasureWidget = new MeasureWidget;
        connect(m_pMeasureWidget, SIGNAL(sigMeasureHeightStart()), this, SLOT(slotStartMeasure()));
        connect(m_pMeasureWidget, SIGNAL(sigMeasureHeightFinish()), this, SLOT(slotFinishMeasure()));
        connect(this, SIGNAL(sigGetDataFinish()), m_pMeasureWidget, SLOT(slotGetHeightData()));
        connect(m_pMeasureWidget, SIGNAL(sigMeasureHeightTimeout()), this, SLOT(slotMeasureTimeout()));
        m_pLineEditHeightValue->clear();
        m_measureTimes = 0;
        memset(m_fHTemp, 0, MEASURE_TIMES);

        m_pMeasureWidget->prepareMeasureHeight();
    } else {
        m_pButtonStartHeight->setText("开始测量身高");
        m_pMeasureWidget->deleteLater();
        m_pMeasureWidget = NULL;
        /* 避免串口忘记关闭，每次测量结束就自动关闭串口 */
        slotOpenHeightDevice();
    }
}

void HeightWidget::slotStartMeasure()
{
    m_pHeightDevice->sendMeasureData();
//    this->setEnabled(false);
}

void HeightWidget::slotFinishMeasure()
{
    m_pButtonStartHeight->setText("开始测量身高");
    m_pButtonStartHeight->setEnabled(true);
    m_pMeasureWidget->deleteLater();
    m_pMeasureWidget = NULL;
    /* 避免串口忘记关闭，每次测量结束就自动关闭串口 */
    slotOpenHeightDevice();
    //    this->setEnabled(true);
}

void HeightWidget::slotMeasureTimeout()
{
    slotFinishMeasure();
    qDebug() << "身高测量超时!";
    QMessageBox::information(this, tr("身高测量"), tr("测量超时，请上称!"));
}

void HeightWidget::slotHandleMeasureData(const float &fHeight)
{
    if (m_measureTimes >= (MEASURE_TIMES -1)) {
        if ((MEASURE_TIMES -1) == m_measureTimes) {
            m_fHTemp[m_measureTimes++] = fHeight;
        } else {
            CUtils::insertData(m_fHTemp, MEASURE_TIMES, fHeight);
        }
        /* 检查测量的身高数据是否稳定 */
        bool isStable = CUtils::testDataStable(m_fHTemp, MEASURE_TIMES, g_fDeviation);
        if (isStable) {
            float h = 0;
            m_measureTimes = 0;
            for (int i = 0; i < MEASURE_TIMES; ++i) {
                h += m_fHTemp[i];
            }
            QString strMeasurData = QString("").setNum(h / MEASURE_TIMES, 'f', 1);
            qDebug() << "测量得到稳定的身高数据为:" << strMeasurData;
            m_pLineEditHeightValue->setText(strMeasurData);
            emit sigGetDataFinish(); /* 通知界面提示用户测量完毕 */
            return ;
        } else {
            qDebug("身高数据不稳定，继续进行测量...");
        }
    } else {
        m_fHTemp[m_measureTimes++] = fHeight;
    }
    m_pHeightDevice->sendMeasureData();
}

void HeightWidget::slotAdjustIncrease()
{
    qDebug() << "开始身高测量校正: +" << m_pSpinBoxIncrease->value() << "mm" ;
    m_typeAdjust = TYPE_ADJUST_INCREASE;
    /* 为了避免连续点击设置，在设置完毕之前，关闭设置功能。完成之后需要打开 */
    m_pButtonSetIncrease->setEnabled(false);
    m_pButtonSetDecrease->setEnabled(false);
    m_pSpinBoxIncrease->setEnabled(false);

    m_pHeightDevice->getStandardData();
}

void HeightWidget::slotAdjustDecrease()
{
    qDebug() << "开始身高测量校正: -" << m_pSpinBoxIncrease->value() << "mm" ;
    m_typeAdjust = TYPE_ADJUST_DECREASE;
    m_pButtonSetIncrease->setEnabled(false);
    m_pButtonSetDecrease->setEnabled(false);
    m_pSpinBoxDecrease->setEnabled(false);

    m_pHeightDevice->getStandardData();
}

void HeightWidget::slotGetStandardData(const int &data)
{
    int changedValue = data;
    switch (m_typeAdjust) {
    case TYPE_ADJUST_INCREASE:
        changedValue += m_pSpinBoxIncrease->value();
        break;
    case TYPE_ADJUST_DECREASE:
        changedValue -= m_pSpinBoxDecrease->value();
        break;
    default:
        break;
    }
    m_pHeightDevice->setStandardData(changedValue);
}

void HeightWidget::slotSetStandardSucess()
{
//    m_pButtonSetIncrease->setEnabled(true);
//    m_pButtonSetDecrease->setEnabled(true);
//    m_pSpinBoxIncrease->setEnabled(true);
//    m_pSpinBoxDecrease->setEnabled(true);

    /* 标高设置完成之后，关闭串口 */
    slotOpenHeightDevice();
    /* 提示设置成功 */
    QMessageBox::information(this, tr("提示"), tr("设置标高成功！"));
}

void HeightWidget::initHeightDevice()
{
    m_pHeightDevice->init();
    m_pHeightDevice->setSerialPortName(m_pComboBoxSerialPort->currentText());
    connect(m_pHeightDevice, SIGNAL(signalSuccess()), this, SLOT(slotDeviceOpenSuccess()));
    connect(m_pHeightDevice, SIGNAL(signalError(QString)), this, SLOT(slotDeviceOpenFailed(QString)));
    connect(m_pHeightDevice, SIGNAL(sigNewHeightData(float)), this, SLOT(slotHandleMeasureData(float)));
    connect(m_pHeightDevice, SIGNAL(sigGetStandardHeight(int)), this, SLOT(slotGetStandardData(int)));
    connect(m_pHeightDevice, SIGNAL(sigSetStandardSuccess()), this, SLOT(slotSetStandardSucess()));
}


