#ifndef G4WIDGET_H
#define G4WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QTextEdit>
#include <QLayout>

#include "file_line_edit.h"

class G4Widget: public QWidget
{
    Q_OBJECT
public:
    explicit G4Widget(QWidget *parent = 0);
    virtual ~G4Widget();

    void initDialupNote();
    bool initRasphone();
public slots:
    void slotSelectLTEApplication();
    void slotSetLTEAutoStarUp();
    void slotCancelLTEAutoStartUp();
    void slotSetAutoDialup();
    void slotCancelAutoDialup();
    void slotLETFileChanged(const QString &strFilePath);

private:
    QString m_strDestLTEFilePath;
    QString m_strTrimmedDialUpName;
    QGroupBox *m_pGroupBoxLTEUSB;
    QLabel *m_pLabelApplication;
    FileLineEdit *m_pLineEditFile;
    QPushButton *m_pButtonBrowser;
    QPushButton *m_pButtonSetLTE; // 设置开机自启动
    QPushButton *m_pButtonCancelLTE; // 取消开机自启动

    QGroupBox *m_pGroupBoxDialup;
    QTextEdit *m_pTextDialupNote;
    QPushButton *m_pButtonSetDialup; // 设置开机自动拔号
    QPushButton *m_pButtonCancelDialup; // 取消开机自动拔号

    QVBoxLayout *m_pMainLayout;

    bool m_bInitDialup; // 是否已经修改了拔号配置文件

};

#endif // G4WIDGET_H
