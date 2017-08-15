#include <QPixmap>
#include <QtDebug>

#include "pallete.h"

Pallete::Pallete(QWidget *parent)
    : QDialog(parent)
    , m_pFrameCtrl(NULL)
    , m_pFrameContent(NULL)
    , m_pLabelWindow(NULL)
    , m_pLabelWindowText(NULL)
    , m_pLabelButton(NULL)
    , m_pLabelButtonText(NULL)
    , m_pLabelBase(NULL)
    , m_pComboBoxWindown(NULL)
    , m_pComboBoxWindownText(NULL)
    , m_pComboBoxButton(NULL)
    , m_pComboBoxButtonText(NULL)
    , m_pComboBoxBase(NULL)
    , m_pLabelValue(NULL)
    , m_pLabelInput(NULL)
    , m_pComboBoxValue(NULL)
    , m_pLineEdit(NULL)
    , m_pTextEdit(NULL)
    , m_pButtonOk(NULL)
    , m_pButtonCancel(NULL)
    , m_pMainLayout(NULL)
{
    m_pMainLayout = new QGridLayout(this);
    createCtrlFrame();
    createContentFrame();

    m_pMainLayout->addWidget(m_pFrameCtrl, 0, 0);
    m_pMainLayout->addWidget(m_pFrameContent, 0, 1);
    m_pMainLayout->setSpacing(10);
    m_pMainLayout->setMargin(10);
}


Pallete::~Pallete()
{

}

void Pallete::slotShowWindow()
{
    /* 返回QT所知道的颜色列表 */
    QStringList colorList = QColor::colorNames();
    QColor color = colorList[m_pComboBoxWindown->currentIndex()];
    qDebug() << "show window, current index:" << m_pComboBoxWindown->currentIndex();
    /* 返回widget的palette */
    QPalette palette = m_pFrameContent->palette();
    /* 修改widget的palette */
    palette.setColor(QPalette::Window, color);
    /* 重新设置widget的Palette */
    m_pFrameContent->setPalette(palette);

    m_pFrameContent->update();
}

void Pallete::slotShowWindowText()
{
    QStringList colorList = QColor::colorNames();
    QColor color = colorList[m_pComboBoxWindownText->currentIndex()];
    QPalette palette = m_pFrameContent->palette();
    palette.setColor(QPalette::WindowText, color);
    m_pFrameContent->setPalette(palette);

    m_pFrameContent->update();
}

void Pallete::slotShowButton()
{
    QStringList colorList = QColor::colorNames();
    QColor color = colorList[m_pComboBoxButton->currentIndex()];
    QPalette palette = m_pFrameContent->palette();
    palette.setColor(QPalette::Button, color);
    m_pFrameContent->setPalette(palette);

    m_pFrameContent->update();
}

void Pallete::slotShowButtonText()
{
    QStringList colorList = QColor::colorNames();
    QColor color = colorList[m_pComboBoxButtonText->currentIndex()];
    QPalette palette = m_pFrameContent->palette();
    palette.setColor(QPalette::ButtonText, color);
    m_pFrameContent->setPalette(palette);

    m_pFrameContent->update();

}

void Pallete::slotShowBase()
{
    QStringList colorList = QColor::colorNames();
    QColor color = colorList[m_pComboBoxBase->currentIndex()];
    QPalette palette = m_pFrameContent->palette();
    palette.setColor(QPalette::Base, color);
    m_pFrameContent->setPalette(palette);

    m_pFrameContent->update();

}

void Pallete::createCtrlFrame()
{
    m_pFrameCtrl = new QFrame(this);
    /* 注意：这里一定要设置自动填充背景，否则程序不生效 */
    m_pFrameCtrl->setAutoFillBackground(true);

    /* 窗口背景颜色 */
    m_pLabelWindow = new QLabel(tr("QPallete::Window:"), this);
    m_pComboBoxWindown = new QComboBox(this);
    fillColorList(m_pComboBoxWindown);
    connect(m_pComboBoxWindown, SIGNAL(activated(int)), this, SLOT(slotShowWindow()));

    /* 窗口字体颜色，比如会影响Label的颜色等 */
    m_pLabelWindowText = new QLabel(tr("QPallete::WindowText:"), this);
    m_pComboBoxWindownText = new QComboBox(this);
    fillColorList(m_pComboBoxWindownText);
    connect(m_pComboBoxWindownText, SIGNAL(activated(int)), this, SLOT(slotShowWindowText()));

    /* 按钮颜色，会影响按钮背景的颜色等 */
    m_pLabelButton = new QLabel(tr("QPallete::Button:"), this);
    m_pComboBoxButton = new QComboBox(this);
    fillColorList(m_pComboBoxButton);
    connect(m_pComboBoxButton, SIGNAL(activated(int)), this, SLOT(slotShowButton()));

    /* 按钮上的字体颜色 */
    m_pLabelButtonText = new QLabel(tr("QPallete::ButtonText:"), this);
    m_pComboBoxButtonText = new QComboBox(this);
    fillColorList(m_pComboBoxButtonText);
    connect(m_pComboBoxButtonText, SIGNAL(activated(int)), this, SLOT(slotShowButtonText()));

    /* Base颜色，会影响文本实体窗口的颜色，比如TextEdit等 */
    m_pLabelBase = new QLabel(tr("QPallete::Base:"), this);
    m_pComboBoxBase = new QComboBox(this);
    fillColorList(m_pComboBoxBase);
    connect(m_pComboBoxBase, SIGNAL(activated(int)), this, SLOT(slotShowBase()));

    QGridLayout *gridLayoutCtrl = new QGridLayout(m_pFrameCtrl);
    gridLayoutCtrl->addWidget(m_pLabelWindow, 0, 0);
    gridLayoutCtrl->addWidget(m_pComboBoxWindown, 0, 1);
    gridLayoutCtrl->addWidget(m_pLabelWindowText, 1, 0);
    gridLayoutCtrl->addWidget(m_pComboBoxWindownText, 1, 1);
    gridLayoutCtrl->addWidget(m_pLabelButton, 2, 0);
    gridLayoutCtrl->addWidget(m_pComboBoxButton, 2, 1);
    gridLayoutCtrl->addWidget(m_pLabelButtonText, 3, 0);
    gridLayoutCtrl->addWidget(m_pComboBoxButtonText, 3, 1);
    gridLayoutCtrl->addWidget(m_pLabelBase, 4, 0);
    gridLayoutCtrl->addWidget(m_pComboBoxBase, 4, 1);
}

void Pallete::createContentFrame()
{
    m_pFrameContent = new QFrame(this);
    m_pFrameContent->setAutoFillBackground(true);

    m_pLabelValue = new QLabel(tr("请选择一个值:"), this);
    m_pComboBoxValue = new QComboBox(this);
    m_pLabelInput = new QLabel(tr("请输入字符串"), this);
    m_pLineEdit = new QLineEdit(tr("输入内容为"), this);

    m_pTextEdit = new QTextEdit(tr("中华人民共和国万岁"), this);

    QVBoxLayout *vLayout = new QVBoxLayout(m_pFrameContent);
    QGridLayout *gridLayout = new QGridLayout;
    QHBoxLayout *hLayout = new QHBoxLayout;
    vLayout->addLayout(gridLayout);
    vLayout->addLayout(hLayout);

    gridLayout->addWidget(m_pLabelValue, 0, 0);
    gridLayout->addWidget(m_pComboBoxValue, 0, 1);
    gridLayout->addWidget(m_pLabelInput, 1, 0);
    gridLayout->addWidget(m_pLineEdit, 1, 1);
    gridLayout->addWidget(m_pTextEdit, 2, 0, 1, 2);

    gridLayout->setSpacing(10);
    gridLayout->setMargin(10);

    m_pButtonOk = new QPushButton(tr("确认"), this);
    m_pButtonCancel = new QPushButton(tr("取消"), this);
    hLayout->addStretch(1);
    hLayout->addWidget(m_pButtonOk);
    hLayout->addWidget(m_pButtonCancel);

}

void Pallete::fillColorList(QComboBox *comboBox)
{
    QStringList strList = QColor::colorNames();
    foreach (QString str, strList) {
        QPixmap pixMap(QSize(70, 20));
        pixMap.fill(QColor(str));
        comboBox->addItem(QIcon(pixMap), NULL);
        comboBox->setIconSize(QSize(70, 20));
        comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    }
}
