#ifndef DESDECRYPTWIDGET_H
#define DESDECRYPTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <QLayout>

class DESDecryptWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DESDecryptWidget(QWidget *parent = 0);
    virtual ~DESDecryptWidget();

signals:
private slots:
    void slotKeyChanged(const QString &strKey);
    void slotKeyRest();
    void slotKeyClear();
    void slotCipherChanged(const QString &strCipher);
    void slotCipherCopy();
    void slotCipherClear();
    void slotPlainCopy();
    void slotPlainClear();
    void slotDesDecrypt();
    void slotClearAll();
public slots:
private:
    QLabel *m_pLabelKey;
    QLineEdit *m_pLineEditKey;
    QPushButton *m_pButtonKeyReset;
    QPushButton *m_pButtonKeyClear;

    QLabel *m_pLabelCipherInput;
    QLineEdit *m_pLineEditCipher;
    QPushButton *m_pButtonCipherCopy;
    QPushButton *m_pButtonCipherClear;

    QLabel *m_pLabelPlainOutput;
    QLineEdit *m_pLineEditPlain;
    QPushButton *m_pButtonPlainCopy;
    QPushButton *m_pButtonPlainClear;

    QPushButton *m_pButtonDecrypt;
    QPushButton *m_pButtonClearAll;

    QGridLayout *m_pGridLayout;
};

#endif // DESDECRYPTWIDGET_H
