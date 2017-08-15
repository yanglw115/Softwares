#include <QMessageBox>
#include <QSerialPortInfo>
#include <QList>

#include "fat_widget.h"
#include "utils.h"
#include "vs_char_set.h"

const QString g_strMeasureSuccessPic   = "Resource/success.png";
const QString g_strMeasureFailedPic    = "Resource/failed.png";
const QString g_strMeasureWaitPic      = "Resource/waiting.png";

const int g_leftMargin = 10;

FatWidget::FatWidget(QWidget *parent)
    : QWidget(parent)
{
    m_pWeightDevice = new CWeightDeviceThread(this);

    /* 串口打开操作 */
    m_pGroupBoxOpen = new QGroupBox(tr("串口选择"), this);
    m_pComboBoxSerialPort = new QComboBox(this);
    QList<QSerialPortInfo> listSerialPorts = QSerialPortInfo::availablePorts();
    if (!listSerialPorts.isEmpty()) {
        int i = 0;
        foreach (QSerialPortInfo info, listSerialPorts) {
            m_pComboBoxSerialPort->addItem(info.portName());
            if (info.portName().toLower() == "com2") {
                m_pComboBoxSerialPort->setCurrentIndex(i); // COM2
            }
            ++i;
        }
    }
    m_pComboBoxSerialPort->setEnabled(false);
    m_pButtonOpenSerial = new QPushButton(tr("打开串口"), this);
    connect(m_pButtonOpenSerial, SIGNAL(clicked(bool)), this, SLOT(slotOpenWeightDevice()));

    QHBoxLayout *pHLayoutSerial = new QHBoxLayout;
    pHLayoutSerial->addSpacing(g_leftMargin);
    pHLayoutSerial->addWidget(m_pComboBoxSerialPort);
    pHLayoutSerial->addWidget(m_pButtonOpenSerial);
//    pHLayoutSerial->addStretch(1);
    pHLayoutSerial->setStretch(0, 4);
    pHLayoutSerial->setStretch(1, 2);

    m_pGroupBoxOpen->setLayout(pHLayoutSerial);

    /*### 体脂测量 */
    m_pGroupBoxMeasure = new QGroupBox(tr("体脂测量"), this);
    m_pButtonStartMeasure = new QPushButton(tr("开始测量体脂"), this);
    m_pButtonStartMeasure->setEnabled(false);
    connect(m_pButtonStartMeasure, SIGNAL(clicked(bool)), this, SLOT(slotPrepareMeasure()));

    m_pLabelPic = new QLabel(this);
    m_pLabelPic->resize(this->size().height() * 0.8, this->size().height() * 0.8);
//    m_pLabelPic->resize(30, 30);
    m_pLabelPic->setScaledContents(true);

    QVBoxLayout *pVLayoutMeasure = new QVBoxLayout;
    pVLayoutMeasure->addWidget(m_pButtonStartMeasure, 1, Qt::AlignHCenter);
    pVLayoutMeasure->addWidget(m_pLabelPic, 4, Qt::AlignHCenter);
    pVLayoutMeasure->addStretch(1);
    m_pGroupBoxMeasure->setLayout(pVLayoutMeasure);

    /*###主窗口布局 */
    m_pMainLayout = new QVBoxLayout(this);
    m_pMainLayout->addWidget(m_pGroupBoxOpen);
    m_pMainLayout->addWidget(m_pGroupBoxMeasure);

    m_pMeasureWidget = NULL;
    initWeightDevice();
}

FatWidget::~FatWidget()
{
    slotDeleteMeasureWidget();
}

void FatWidget::initWeightDevice()
{
    m_pWeightDevice->init();
    m_pWeightDevice->setSerialPortName(m_pComboBoxSerialPort->currentText());
    connect(m_pWeightDevice, SIGNAL(signalSuccess()), this, SLOT(slotDeviceOpenSuccess()));
    connect(m_pWeightDevice, SIGNAL(signalError(QString)), this, SLOT(slotDeviceOpenFailed(QString)));
    connect(m_pWeightDevice, SIGNAL(newWeightData(enumWeightDataType, struWeightData)), this, SLOT(slotHandleReceiveData(enumWeightDataType, struWeightData)));
}

void FatWidget::showResultPicture(const QString &strFile)
{
    QString strFilePath = CUtils::getFileFullPath(strFile);
    m_pixmap.load(strFilePath);
    m_pLabelPic->setPixmap(m_pixmap);
    m_pLabelPic->show();
}

void FatWidget::closeMeasureDevice()
{
    qDebug() << "操作完毕，关闭体脂测量设备!";
    m_pWeightDevice->closeDevice(); /* 测量完毕关闭串口 */
    m_pButtonOpenSerial->setText(tr("打开串口"));
//    m_pButtonStartMeasure->setText(tr("开始测量体脂"));
    m_pButtonStartMeasure->setEnabled(false);
    slotDeleteMeasureWidget();
}

void FatWidget::slotOpenWeightDevice()
{
    if (m_pButtonOpenSerial->text() == tr("打开串口")) {
        qDebug() << "\r\n";
        qDebug() << "打开体脂测量设备...";
        m_pWeightDevice->openDevice();
    } else {
        /* 关闭串口 */
        closeMeasureDevice();
    }
}

void FatWidget::slotDeviceOpenSuccess()
{
    m_pButtonOpenSerial->setText(tr("关闭串口"));
    m_pButtonStartMeasure->setText(tr("开始测量体脂"));
    m_pButtonStartMeasure->setEnabled(true);
    showResultPicture(g_strMeasureWaitPic); // 文本：等待测量结果
}

void FatWidget::slotDeviceOpenFailed(const QString &strError)
{
    m_pButtonOpenSerial->setText(tr("打开串口"));
    qDebug() << "体脂测量设备打开失败!";
    QMessageBox::critical(this, tr("严重错误"), tr("串口打开失败, ") + strError);
}

void FatWidget::slotPrepareMeasure()
{
    if (m_pButtonStartMeasure->text() == tr("开始测量体脂")) {
        m_pButtonStartMeasure->setText(tr("停止测量体脂"));
        slotDeleteMeasureWidget();
        m_pMeasureWidget = new MeasureWidget;
        connect(m_pMeasureWidget, SIGNAL(sigMeasureFatStart()), this, SLOT(slotStartMeasure()));
        connect(this, SIGNAL(sigMeasureFinish()), m_pMeasureWidget, SLOT(slotMeasureFatFinish()));
        connect(m_pMeasureWidget, SIGNAL(sigMeasureFatTimeout()), this, SLOT(slotMeasureTimeout()));
        m_pMeasureWidget->prepareMeasureFat();

    } else if (m_pButtonStartMeasure->text() == tr("停止测量体脂")) {
        qDebug() << "停止测量体脂!";
        m_pButtonStartMeasure->setText(tr("开始测量体脂"));
        closeMeasureDevice();

    } else {
        // 此时标签显示"测量结果"
    }
}

void FatWidget::slotStartMeasure()
{
    m_pWeightDevice->sendFatDataGetCmd();
}

void FatWidget::slotFinishMeasure()
{
}

void FatWidget::slotHandleReceiveData(const enumWeightDataType &type, const struWeightData &data)
{
    if (type == TYPE_WEIGHT_FAT_DATA) {
        /* 所有的体脂测量数据 */
        QByteArray byteArray = data.byteArryaFat;

        float fat = (float)CUtils::getHighAndLowHexValue(byteArray.at(5), byteArray.at(6)) / 10;
        if (fat > 0) {
            qDebug() << "体脂测量成功.";
            m_pButtonStartMeasure->setText(tr("体脂测量成功!"));
            showResultPicture(g_strMeasureSuccessPic);
        } else {
            qDebug() << "体脂测量失败!";
            m_pButtonStartMeasure->setText(tr("体脂测量失败!"));
            showResultPicture(g_strMeasureFailedPic);
        }
        emit sigMeasureFinish();
        closeMeasureDevice();
    }
}

void FatWidget::slotDeleteMeasureWidget()
{
    if (m_pMeasureWidget != NULL) {
        delete m_pMeasureWidget;
        m_pMeasureWidget = NULL;
    }
}

void FatWidget::slotMeasureTimeout()
{
    closeMeasureDevice();
    qDebug() << "体脂测量超时!";
    QMessageBox::information(this, tr("体脂检测"), tr("测量超时，请上称!"));
}

