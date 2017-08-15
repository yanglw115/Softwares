#include <QFileDialog>

#include "cbasicinfo.h"

CBasicInfo::CBasicInfo(QWidget *parent) : QWidget(parent)
{
    /* 一个QLable和一个QLineEdit构成一组 */
    m_pLabelNetName = new QLabel(tr("用户名:"), this);
    m_pLineEditNetName = new QLineEdit(this);

    m_pLabelRealName = new QLabel(tr("姓名:"), this);
    m_pLineEditRealName = new QLineEdit(this);

    m_pLabelSex = new QLabel(tr("性别:"), this);
    /* QComboBox是可以下拉的复选框 */
    m_pComboxSex = new QComboBox(this);
    m_pComboxSex->insertItem(0, tr("男"));
    m_pComboxSex->insertItem(1, tr("女"));

    m_pLabelDepartment = new QLabel(tr("部门:"), this);
    m_pTextEditDep = new QTextEdit(this);

    m_pLabelAge = new QLabel(tr("年龄:"), this);
    m_pLineEditAge = new QLineEdit(this);

    m_pLabelNoteInfo = new QLabel(tr("备注:"), this);
    m_pLabelNoteInfo->setFrameStyle(QFrame::WinPanel | QFrame::Sunken);

    /* 使用QGridLayout是为了使QLabel与QLineEdit分别对齐 */
    m_pGridLayoutLeft = new QGridLayout;
    m_pGridLayoutLeft->addWidget(m_pLabelNetName, 0, 0);
    m_pGridLayoutLeft->addWidget(m_pLineEditNetName, 0, 1);
    m_pGridLayoutLeft->addWidget(m_pLabelRealName, 1, 0);
    m_pGridLayoutLeft->addWidget(m_pLineEditRealName, 1, 1);
    m_pGridLayoutLeft->addWidget(m_pLabelSex, 2, 0);
    m_pGridLayoutLeft->addWidget(m_pComboxSex, 2, 1);
    m_pGridLayoutLeft->addWidget(m_pLabelDepartment, 3, 0);
    m_pGridLayoutLeft->addWidget(m_pTextEditDep, 3, 1);
    m_pGridLayoutLeft->addWidget(m_pLabelAge, 4, 0);
    m_pGridLayoutLeft->addWidget(m_pLineEditAge, 4, 1);
    /**
     * void QGridLayout::addWidget(QWidget * widget, int fromRow, int fromColumn, int rowSpan, int columnSpan, Qt::Alignment alignment = 0)
     * 下面addWidget函数，调用了多个参数，代表其所占用的位置，这里从第5行，第0列开始，占用1行，2列的空间
     */
    m_pGridLayoutLeft->addWidget(m_pLabelNoteInfo, 5, 0, 1, 2);

    /* 设置边距 */
    m_pGridLayoutLeft->setMargin(10);
    /* 设置第1列和第二列拉伸时所占的比例 */
    m_pGridLayoutLeft->setRowStretch(0, 1);
    m_pGridLayoutLeft->setRowStretch(1, 3);

    m_pLabelHead = new QLabel(tr("头像:"), this);
    m_pLabelHeadImg = new QLabel(this);
    QPixmap pixMap(":defaultHead.png");
    m_pLabelHeadImg->setPixmap(pixMap);
    m_pLabelHeadImg->resize(20, 20);
    m_pLabelHeadImg->setScaledContents(true);
    m_pButtonUpdateHeadImg = new QPushButton(tr("更新"), this);
    connect(m_pButtonUpdateHeadImg, SIGNAL(clicked(bool)), this, SLOT(slotChangeHeadImg()));
    m_pHBoxHead = new QHBoxLayout;
    m_pHBoxHead->addWidget(m_pLabelHead);
    m_pHBoxHead->addWidget(m_pLabelHeadImg);
    m_pHBoxHead->addWidget(m_pButtonUpdateHeadImg);
    /* 使头像的更改靠左排列，右边再插入一个space */
    m_pHBoxHead->addStretch(1);

    m_pLabelPersonalInfo = new QLabel(tr("个人说明:"), this);
    m_pTextEditPerinfo = new QTextEdit(this);

    m_pVBoxRight = new QVBoxLayout;
    m_pVBoxRight->addLayout(m_pHBoxHead);
    m_pVBoxRight->addWidget(m_pLabelPersonalInfo);
    m_pVBoxRight->addWidget(m_pTextEditPerinfo);

    m_pGridLayoutAll = new QGridLayout;
    m_pGridLayoutAll->setMargin(10);
    m_pGridLayoutAll->addLayout(m_pGridLayoutLeft, 0, 0);
    m_pGridLayoutAll->addLayout(m_pVBoxRight, 0, 1);
//    m_pGridLayoutAll->setRowStretch(0, 1);
//    m_pGridLayoutAll->setRowStretch(1, 1);
    m_pGridLayoutAll->setColumnStretch(0, 1);
    m_pGridLayoutAll->setColumnStretch(1, 1);

//    m_pGridLayoutAll->setSizeConstraint(QLayout::SetFixedSize);
    this->setLayout(m_pGridLayoutAll);
}

CBasicInfo::~CBasicInfo()
{

}

void CBasicInfo::slotChangeHeadImg()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open file dialog"), "/", tr("Picture files(*.png *.jpeg)"));
    if (NULL != filePath) {
        QPixmap pixMap(filePath);
        m_pLabelHeadImg->setPixmap(pixMap);
    }
}

