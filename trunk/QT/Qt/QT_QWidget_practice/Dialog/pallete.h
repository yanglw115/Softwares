#ifndef PALLETE_H
#define PALLETE_H

#include <QBoxLayout>
#include <QComboBox>
#include <QDialog>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>

/**
 * @brief The Pallete class
 * 《Qt5开发与实例》4.9.1 QPalette类
 * Qt5的每一个Widget都包含有一个Palette，通过palette可以改变每个widget的各种状态下各部件的颜色
 */

class Pallete : public QDialog
{
    Q_OBJECT
public:
    explicit Pallete(QWidget *parent = 0);
    virtual ~Pallete();

private slots:
    void slotShowWindow();
    void slotShowWindowText();
    void slotShowButton();
    void slotShowButtonText();
    void slotShowBase();

private:
    void createCtrlFrame();
    void createContentFrame();
    void fillColorList(QComboBox *comboBox);

private:
    QFrame *m_pFrameCtrl;
    QFrame *m_pFrameContent;
    QLabel *m_pLabelWindow;
    QLabel *m_pLabelWindowText;
    QLabel *m_pLabelButton;
    QLabel *m_pLabelButtonText;
    QLabel *m_pLabelBase;

    QComboBox *m_pComboBoxWindown;
    QComboBox *m_pComboBoxWindownText;
    QComboBox *m_pComboBoxButton;
    QComboBox *m_pComboBoxButtonText;
    QComboBox *m_pComboBoxBase;

    QLabel *m_pLabelValue;
    QLabel *m_pLabelInput;
    QComboBox *m_pComboBoxValue;
    QLineEdit *m_pLineEdit;
    QTextEdit *m_pTextEdit;
    QPushButton *m_pButtonOk;
    QPushButton *m_pButtonCancel;

    QGridLayout *m_pMainLayout;

};

#endif // PALLETE_H
