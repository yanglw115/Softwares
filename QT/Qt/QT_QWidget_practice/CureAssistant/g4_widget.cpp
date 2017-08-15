
#include <QStandardPaths>
#include <QFileDialog>
#include <QtDebug>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QSettings>
#include <QStringList>
#include <QProcess>

#include "g4_widget.h"
#include "read_write_interface.h"
#include "vs_char_set.h"

const static QString g_strStartUpDir        = "C:/Documents and Settings/Administrator/「开始」菜单/程序/启动/";
const static QString g_strRasphoneFileDir   = "C:/Documents and Settings/All Users/Application Data/Microsoft/Network/Connections/Pbk/rasphone.pbk";
//const static QString g_strRasphoneFileDir   = "D:/rasphone.pbk";
static QString g_strCMDSetDialupHead        = "REG ADD HKLM\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run /v \"4g卡拨号\" /f /d ";
static QString g_strCMDDelDialup            = "REG DELETE HKLM\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run /v 4g卡拨号 /f";
static QString g_strCMDSetDialupValue       = "C:\\Windows\\System32\\rasphone.exe";

G4Widget::G4Widget(QWidget *parent)
    : QWidget(parent)
    , m_strDestLTEFilePath(QString(""))
{
    // TLE USB MODEM
    m_pGroupBoxLTEUSB =  new QGroupBox(tr("LTE USB Modem"), this);
    m_pLabelApplication = new QLabel(tr("应用程序:"), this);
    m_pLineEditFile = new FileLineEdit(this);
    m_pButtonBrowser = new QPushButton(tr("浏览"), this);
    m_pButtonSetLTE = new QPushButton(tr("设置LET USB Modem开机自启动"), this);
    m_pButtonCancelLTE = new QPushButton(tr("取消LET USB Modem开机自启动"), this);
    m_pButtonSetLTE->setEnabled(false);
    m_pButtonCancelLTE->setEnabled(false);
    connect(m_pLineEditFile, SIGNAL(textChanged(QString)), this, SLOT(slotLETFileChanged(QString)));
    connect(m_pButtonBrowser, SIGNAL(clicked(bool)), this, SLOT(slotSelectLTEApplication()));
    connect(m_pButtonSetLTE, SIGNAL(clicked(bool)), this, SLOT(slotSetLTEAutoStarUp()));
    connect(m_pButtonCancelLTE, SIGNAL(clicked(bool)), this, SLOT(slotCancelLTEAutoStartUp()));

    QHBoxLayout *pHLayoutFileSelect = new QHBoxLayout;
    pHLayoutFileSelect->addWidget(m_pLabelApplication);
    pHLayoutFileSelect->addWidget(m_pLineEditFile, 2);
    pHLayoutFileSelect->addWidget(m_pButtonBrowser, 1);
    QVBoxLayout *pVLayoutLTE = new QVBoxLayout;
    pVLayoutLTE->addLayout(pHLayoutFileSelect);
    pVLayoutLTE->addWidget(m_pButtonSetLTE, 0, Qt::AlignHCenter);
    pVLayoutLTE->addWidget(m_pButtonCancelLTE, 0, Qt::AlignHCenter);
    m_pGroupBoxLTEUSB->setLayout(pVLayoutLTE);

    // 4G dailup
    m_pGroupBoxDialup = new QGroupBox(tr("4G上网卡拔号"), this);
    m_pTextDialupNote = new QTextEdit(this);
    m_pButtonSetDialup = new QPushButton(tr("设置开机自动拔号"), this);
    m_pButtonCancelDialup = new QPushButton(tr("取消开机自动拔号"), this);
    connect(m_pButtonSetDialup, SIGNAL(clicked(bool)), this, SLOT(slotSetAutoDialup()));
    connect(m_pButtonCancelDialup, SIGNAL(clicked(bool)), this, SLOT(slotCancelAutoDialup()));
    QVBoxLayout *pVLayoutDialup = new QVBoxLayout;
    pVLayoutDialup->addWidget(m_pTextDialupNote, 0, Qt::AlignJustify);
    pVLayoutDialup->addWidget(m_pButtonSetDialup, 0, Qt::AlignHCenter);
    pVLayoutDialup->addWidget(m_pButtonCancelDialup, 0, Qt::AlignHCenter);
    m_pGroupBoxDialup->setLayout(pVLayoutDialup);

    m_pMainLayout = new QVBoxLayout(this);
    m_pMainLayout->addWidget(m_pGroupBoxLTEUSB, 1);
    m_pMainLayout->addWidget(m_pGroupBoxDialup, 1);

    initDialupNote();
}

G4Widget::~G4Widget()
{

}

void G4Widget::initDialupNote()
{
    m_pTextDialupNote->setReadOnly(true);
    m_pTextDialupNote->setFontWeight(QFont::Bold);
    m_pTextDialupNote->setTextColor(Qt::red);
    m_pTextDialupNote->append(tr("说明:"));

    m_pTextDialupNote->setFontWeight(QFont::Medium);
    m_pTextDialupNote->setTextColor(Qt::black);
    m_pTextDialupNote->append(tr("    在进行拔号开机自启动设置时，请一定先运行LTE USB Modem应用程序，"
                                 "并手动进行一次网络连接，然后断开连接，否则将会导致后面开机自动拔号设置失败。"));
}

bool G4Widget::initRasphone()
{
    bool bRet = false;
    if (QFile::exists(g_strRasphoneFileDir)) {
        QSettings::Format format = QSettings::registerFormat("pbk", IniReadFunc, IniWriteFunc);
        if (format == QSettings::InvalidFormat) {
            qWarning() << "Register format failed!";
            return false;
        }
        setConfigFilePath(g_strRasphoneFileDir);
        QSettings *pSettings = new QSettings(g_strRasphoneFileDir, format, this);
        QStringList strListGroups = pSettings->childGroups();
        qDebug() << "配置组有:";
        foreach (QString strGroup, strListGroups) {
            qDebug() << strGroup;
        }
        bool bOK = false;
        for (int i = 0; (i < strListGroups.length()) && !bOK; ++i) {
            QString strOldGroupName = strListGroups[i];
            QString strNewGroupName = strOldGroupName;
            qDebug() << "解析当前配置组: " << strOldGroupName;
            pSettings->beginGroup(strOldGroupName);
            if (pSettings->contains("Type")) {
                int nType = pSettings->value("Type").toInt();
                qDebug() << "Type:" << nType;
                /* 下面结合了QSettings与自己封装的一些方法，是为了解决QSettings不区分大小写及封装的接口放入QMap之后会排序也导致配置文件不能用的问题 */
                if (1 == nType) { // Type为1表示4G拔号，5为宽带拔号
                    if (strOldGroupName.contains(' ')) { // 包含空格
                        strNewGroupName = strNewGroupName.replace(" ", ""); // 去掉空格
                        changeGroupName(strOldGroupName, strNewGroupName);
                        qDebug() << "拔号名称带空格，将去除! 去掉空格后的组名: " << strNewGroupName;

                    }
                    pSettings->endGroup();
                    m_strTrimmedDialUpName = strNewGroupName;

                    /* 下面修改group内的数据 */
                    /* 这里结合了QSettings和自己封装的接口 */
                    setCurrentGroup(m_strTrimmedDialUpName);
                    setValueNotBySettings("DataEncryption", "8");
                    setValueNotBySettings("RedialAttempts", "25");
                    setValueNotBySettings("RedialSeconds", "3");
                    setValueNotBySettings("RedialOnLinkFailure", "1");
                    setValueNotBySettings("SharedPhoneNumbers", "1");
                    setValueNotBySettings("PreviewUserPw", "0");
                    setValueNotBySettings("PreviewPhoneNumber", "0");
                    setValueNotBySettings("ms_psched", "1");
                    writeDataToConfigFile();
                    bOK = true;
                    bRet = true;
                    m_bInitDialup = true;

                }
            } else {
                qWarning() << "该组不包含Type键!";
                pSettings->endGroup();
            }
        }
        return bRet;

    } else {
        qWarning() << "拔号配置文件不存在: " << g_strRasphoneFileDir;
        return bRet;
    }
}

void G4Widget::slotSelectLTEApplication()
{
#ifdef Q_OS_UNIX
    QString strFilePath = QFileDialog::getOpenFileName(this, tr("选择LTE USB Modem应用程序"), "/", "file(*.*)");
#else
    QString strFilePath = QFileDialog::getOpenFileName(this, tr("选择LTE USB Modem应用程序"), QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), "file(*.*)");
#endif
    if (!strFilePath.isEmpty()) {
        m_pLineEditFile->setText(strFilePath);
    }
}

void G4Widget::slotSetLTEAutoStarUp()
{
    qDebug() << "\r\n";
    qDebug() << "设置LTE开机自启动!";
    QString strSourceFilePath = m_pLineEditFile->text();
    if (QFile::exists(strSourceFilePath)) {
        QFileInfo fileInfo(strSourceFilePath);
        QString strDestFilePath = g_strStartUpDir + fileInfo.fileName();
        qDebug() << "源文件:" << strSourceFilePath;
        qDebug() << "目的文件:" << strDestFilePath;
        if (QFile::exists(strDestFilePath)) {
            qDebug() << "无需重复设置LTE USB Modem开机自启动!";
            QMessageBox::warning(this, tr("设置LTE USB Modem"), tr("无需重复设置LTE USB Modem开机自启动!"));
            m_pButtonCancelLTE->setEnabled(true);
            m_strDestLTEFilePath = strDestFilePath;
        } else {
            if (QFile::copy(strSourceFilePath, strDestFilePath)) {
                qDebug() << "设置LTE USB Modem开机自启动成功!";
                QMessageBox::information(this, tr("设置LTE USB Modem"), tr("设置LTE USB Modem开机自启动成功!"));
                m_pButtonCancelLTE->setEnabled(true);
                m_strDestLTEFilePath = strDestFilePath;
            } else {
                qDebug() << "设置LTE USB Modem开机自启动失败: 请确保在XP系统上运行该程序!";
                QMessageBox::critical(this, tr("设置LTE USB Modem"), tr("设置LTE USB Modem开机自启动失败: 请确保在XP系统上运行该程序!"));
            }
        }
    } else {
        qDebug() << "设置LTE USB Modem开机自启动失败: 应用程序路径出错: " << strSourceFilePath;
        QString strContent = tr("设置LTE USB Modem开机自启动失败: 应用程序路径出错(") + strSourceFilePath + ")!";
        QMessageBox::critical(this, tr("设置LTE USB Modem"), strContent);
    }
}

void G4Widget::slotCancelLTEAutoStartUp()
{
    if (QFile::exists(m_strDestLTEFilePath)) {
        if (QFile::remove(m_strDestLTEFilePath)) {
            qDebug() << "取消LTE USB Modem开机自启动成功!";
            QMessageBox::information(this, tr("设置LTE USB Modem"), tr("取消LTE USB Modem开机自启动成功!"));
            m_pButtonCancelLTE->setEnabled(false);
            slotCancelAutoDialup(); // 如果取消LTE自动启动，则拔号也需要取消
        } else {
            qDebug() << "取消LTE USB Modem开机自启动失败: 请确保自启动文件可被删除!";
            QMessageBox::critical(this, tr("设置LTE USB Modem"), tr("取消LTE USB Modem开机自启动失败: 请确保自启动文件可被删除!"));
        }
    } else {
        qDebug() << "自启动文件已不复存在，无需取消!";
        QMessageBox::information(this, tr("设置LTE USB Modem"), tr("自启动文件已不复存在，无需取消!"));
        m_pButtonCancelLTE->setEnabled(false);
        slotCancelAutoDialup();
    }
}

void G4Widget::slotSetAutoDialup()
{
    qDebug() << "\r\n";
    qDebug() << "设置开机自动拔号...";
    if (!m_bInitDialup) {
        if (!initRasphone()) {
            qDebug() << "设置失败，请手动设置!";
            QMessageBox::information(this, tr("设置自动拔号"), tr("设置失败，请手动设置!"));
            return;
        }
    }
    // 注册表操作
    QString strExec = g_strCMDSetDialupHead + "\"" + g_strCMDSetDialupValue + " -d " + m_strTrimmedDialUpName + "\"";
    qDebug() << "strExec: " << strExec;
    if (QProcess::execute(strExec) >= 0) {
        qDebug() << "设置开机自动拔号成功!";
        QMessageBox::information(this, tr("设置自动拔号"), tr("设置成功! 建议立即手动禁止无线网络连接，以优化开机速度。"));
    } else {
        qDebug() << "设置开机自动拔号失败，请手动设置!";
        QMessageBox::information(this, tr("设置自动拔号"), tr("设置失败，请手动设置!"));
    }
}

void G4Widget::slotCancelAutoDialup()
{
    // 注册表删除操作
    qDebug() << "strExec: " << g_strCMDDelDialup;
    if (QProcess::execute(g_strCMDDelDialup) >= 0) {
        qDebug() << "取消开机自动拔号成功!";
        QMessageBox::information(this, tr("取消自动拔号"), tr("取消成功!"));
    } else {
        qDebug() << "取消开机自动拔号失败，请手动删除注册表配置!";
        QMessageBox::information(this, tr("取消自动拔号"), tr("取消失败，请手动删除注册表配置!"));
    }
}

void G4Widget::slotLETFileChanged(const QString &strFilePath)
{
    if (!strFilePath.isEmpty()) {
       qDebug() << "LTE应用程序:" << strFilePath;
       m_pButtonSetLTE->setEnabled(true);
       m_pButtonCancelLTE->setEnabled(false);
   }
}

