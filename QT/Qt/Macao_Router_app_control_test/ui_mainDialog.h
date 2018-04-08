/********************************************************************************
** Form generated from reading UI file 'mainDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINDIALOG_H
#define UI_MAINDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *m_pComboBox;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *m_pEncryptIn;
    QPushButton *m_pButtonEncrypt;
    QLineEdit *m_pEncryptOut;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QLineEdit *m_pDecryptIn;
    QPushButton *m_pButtonDecrypt;
    QLineEdit *m_pDecryptOut;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label_6;
    QLineEdit *m_pServerIP;
    QPushButton *m_pButtonSend;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit *m_pTextSend;
    QVBoxLayout *verticalLayout_11;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_5;
    QVBoxLayout *verticalLayout_12;
    QLineEdit *m_pTextReceive;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QStringLiteral("Dialog"));
        Dialog->resize(627, 263);
        verticalLayout_2 = new QVBoxLayout(Dialog);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupBox = new QGroupBox(Dialog);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout_4 = new QVBoxLayout(groupBox);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        m_pComboBox = new QComboBox(groupBox);
        m_pComboBox->setObjectName(QStringLiteral("m_pComboBox"));

        horizontalLayout->addWidget(m_pComboBox);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_3->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        m_pEncryptIn = new QLineEdit(groupBox);
        m_pEncryptIn->setObjectName(QStringLiteral("m_pEncryptIn"));

        horizontalLayout_2->addWidget(m_pEncryptIn);

        m_pButtonEncrypt = new QPushButton(groupBox);
        m_pButtonEncrypt->setObjectName(QStringLiteral("m_pButtonEncrypt"));

        horizontalLayout_2->addWidget(m_pButtonEncrypt);

        m_pEncryptOut = new QLineEdit(groupBox);
        m_pEncryptOut->setObjectName(QStringLiteral("m_pEncryptOut"));

        horizontalLayout_2->addWidget(m_pEncryptOut);


        verticalLayout_3->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_3->addWidget(label_3);

        m_pDecryptIn = new QLineEdit(groupBox);
        m_pDecryptIn->setObjectName(QStringLiteral("m_pDecryptIn"));

        horizontalLayout_3->addWidget(m_pDecryptIn);

        m_pButtonDecrypt = new QPushButton(groupBox);
        m_pButtonDecrypt->setObjectName(QStringLiteral("m_pButtonDecrypt"));

        horizontalLayout_3->addWidget(m_pButtonDecrypt);

        m_pDecryptOut = new QLineEdit(groupBox);
        m_pDecryptOut->setObjectName(QStringLiteral("m_pDecryptOut"));

        horizontalLayout_3->addWidget(m_pDecryptOut);


        verticalLayout_3->addLayout(horizontalLayout_3);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_5->addWidget(label_4);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_3);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_5->addWidget(label_6);

        m_pServerIP = new QLineEdit(groupBox);
        m_pServerIP->setObjectName(QStringLiteral("m_pServerIP"));

        horizontalLayout_5->addWidget(m_pServerIP);

        m_pButtonSend = new QPushButton(groupBox);
        m_pButtonSend->setObjectName(QStringLiteral("m_pButtonSend"));

        horizontalLayout_5->addWidget(m_pButtonSend);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_2);


        verticalLayout_3->addLayout(horizontalLayout_5);

        m_pTextSend = new QLineEdit(groupBox);
        m_pTextSend->setObjectName(QStringLiteral("m_pTextSend"));

        verticalLayout_3->addWidget(m_pTextSend);

        verticalLayout_11 = new QVBoxLayout();
        verticalLayout_11->setSpacing(6);
        verticalLayout_11->setObjectName(QStringLiteral("verticalLayout_11"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));

        verticalLayout_11->addLayout(horizontalLayout_4);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QStringLiteral("label_5"));

        verticalLayout_11->addWidget(label_5);


        verticalLayout_3->addLayout(verticalLayout_11);

        verticalLayout_12 = new QVBoxLayout();
        verticalLayout_12->setSpacing(6);
        verticalLayout_12->setObjectName(QStringLiteral("verticalLayout_12"));

        verticalLayout_3->addLayout(verticalLayout_12);


        verticalLayout_4->addLayout(verticalLayout_3);

        m_pTextReceive = new QLineEdit(groupBox);
        m_pTextReceive->setObjectName(QStringLiteral("m_pTextReceive"));

        verticalLayout_4->addWidget(m_pTextReceive);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer);


        verticalLayout->addWidget(groupBox);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", 0));
        groupBox->setTitle(QApplication::translate("Dialog", "APP\346\216\247\345\210\266\344\272\221\347\242\237\346\265\213\350\257\225", 0));
        label->setText(QApplication::translate("Dialog", "\345\215\217\350\256\256\347\261\273\345\236\213\357\274\232", 0));
        m_pComboBox->clear();
        m_pComboBox->insertItems(0, QStringList()
         << QApplication::translate("Dialog", "UDP", 0)
         << QApplication::translate("Dialog", "TCP", 0)
        );
        label_2->setText(QApplication::translate("Dialog", "Key\345\212\240\345\257\206\357\274\232", 0));
        m_pButtonEncrypt->setText(QApplication::translate("Dialog", "\345\212\240\345\257\206", 0));
        label_3->setText(QApplication::translate("Dialog", "Key\350\247\243\345\257\206\357\274\232", 0));
        m_pButtonDecrypt->setText(QApplication::translate("Dialog", "\350\247\243\345\257\206", 0));
        label_4->setText(QApplication::translate("Dialog", "\345\217\221\351\200\201\346\266\210\346\201\257", 0));
        label_6->setText(QApplication::translate("Dialog", "\346\234\215\345\212\241\345\231\250IP:", 0));
        m_pButtonSend->setText(QApplication::translate("Dialog", "\347\202\271\345\207\273\345\217\221\351\200\201", 0));
        label_5->setText(QApplication::translate("Dialog", "\346\216\245\346\224\266\346\266\210\346\201\257", 0));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINDIALOG_H
