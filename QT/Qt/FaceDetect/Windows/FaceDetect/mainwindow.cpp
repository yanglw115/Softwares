
#include <QFileDialog>
#include <QMessageBox>

#include "mainwindow.h"
#include "vs_charset.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_paramsChanges(0x0)
    , m_paramDetected(0x0)
{
    this->setFixedSize(900, 600);
    this->setAttribute(Qt::WA_DeleteOnClose);
    initWidgets();
}

MainWindow::~MainWindow()
{

}

void MainWindow::slotSelectNewPhoto()
{
    QString strFilePath = QFileDialog::getOpenFileName(this, tr("选择图片"), "Desktop", "Images(*.jpg *.png)");

    if (!strFilePath.isEmpty()) {
        m_pLabelImage->changePixmap(strFilePath);
    }
}

void MainWindow::initWidgets()
{
    m_pSplitter = new QSplitter(this);
    m_pWidgetLeft = new QWidget(this);
    m_pWidgetRight = new QWidget(this);
    m_pWidgetLeft->setFixedWidth(300);
    m_pSplitter->addWidget(m_pWidgetLeft);
    m_pSplitter->addWidget(m_pWidgetRight);
    m_pSplitter->setLineWidth(2);

    /**
     * photo
     *****************************************************/
    m_pLabelImage = new CLabelImageDrag(this);
    m_pButtonBrowse = new QPushButton(tr("浏览"), this);
    m_pEditPhoto = new QLineEdit(this);
    m_pVLayoutLeft = new QVBoxLayout;
    m_pHLayoutBrowse = new QHBoxLayout;

    connect(m_pLabelImage, SIGNAL(sigPixmapChanged(QString)),
            m_pEditPhoto, SLOT(setText(QString)));
    connect(m_pButtonBrowse, SIGNAL(clicked(bool)), this, SLOT(slotSelectNewPhoto()));
    m_pLabelImage->setFixedWidth(300);
    m_pLabelImage->setFixedHeight(500);
    m_pLabelImage->changePixmap(":/images/photo.jpg");
    m_pHLayoutBrowse->addWidget(m_pButtonBrowse);
    m_pHLayoutBrowse->addWidget(m_pEditPhoto);
    m_pVLayoutLeft->addWidget(m_pLabelImage);
    m_pVLayoutLeft->addLayout(m_pHLayoutBrowse);
    m_pVLayoutLeft->addSpacing(100);
    m_pWidgetLeft->setLayout(m_pVLayoutLeft);

    /**
     * All
     *****************************************************/
    m_pGroupAll = new QGroupBox(tr("所有特征"), this);
    m_pButtonResetAll = new QPushButton(tr("恢复所有参数值"), this);
    connect(m_pButtonResetAll, SIGNAL(clicked(bool)),
            this, SLOT(slotResetAllParameters()));
    m_pButtonDetectAll = new QPushButton(tr("开始检测"), this);
    connect(m_pButtonDetectAll, SIGNAL(clicked(bool)), this, SLOT(slotDetectAll()));
    m_pHLayoutAll = new QHBoxLayout;
    m_pHLayoutAll->addWidget(m_pButtonResetAll);
    m_pHLayoutAll->addWidget(m_pButtonDetectAll);
    m_pHLayoutAll->addStretch(1);
    m_pGroupAll->setLayout(m_pHLayoutAll);

    /**
     * pumples
     *****************************************************/
    m_pGroupPimples = new QGroupBox(tr("痘痘"), this);
    m_pGridPimples = new QGridLayout;
    m_pLabelMinSizePimple = new QLabel(tr("最小面积:"), this);
    m_pLabelMaxSizePimple = new QLabel(tr("最大面积:"), this);
    m_pLabelMinColorPimple = new QLabel(tr("L最小值(HLS):"), this);
    m_pLabelMinColorDiffPimple = new QLabel(tr("最小RG差值(BGR):"), this);
    m_pLabelMinRatioPimple = new QLabel(tr("最小长宽比:"), this);
    m_pLabelMaxRatioPimple = new QLabel(tr("最大长宽比:"), this);
    m_pEditMinSizePimple = new QLineEdit(this);
    m_pEditMaxSizePimple = new QLineEdit(this);
    m_pEditMinColorPimple = new QLineEdit(this);
    m_pEditMinRGDiffPimple = new QLineEdit(this);
    m_pEditMinRatioPimple = new QLineEdit(this);
    m_pEditMaxRatioPimple = new QLineEdit(this);
    connect(m_pEditMinSizePimple, SIGNAL(textChanged(QString)), this, SLOT(slotParamsChangedPimples()));
    connect(m_pEditMaxSizePimple, SIGNAL(textChanged(QString)), this, SLOT(slotParamsChangedPimples()));
    connect(m_pEditMinColorPimple, SIGNAL(textChanged(QString)), this, SLOT(slotParamsChangedPimples()));
    connect(m_pEditMinRGDiffPimple, SIGNAL(textChanged(QString)), this, SLOT(slotParamsChangedPimples()));
    connect(m_pEditMinRatioPimple, SIGNAL(textChanged(QString)), this, SLOT(slotParamsChangedPimples()));
    connect(m_pEditMaxRatioPimple, SIGNAL(textChanged(QString)), this, SLOT(slotParamsChangedPimples()));
    m_pButtonResetPimples = new QPushButton(tr("恢复默认"), this);
    m_pButtonDetectPimples = new QPushButton(tr("开始检测"), this);
    connect(m_pButtonResetPimples, SIGNAL(clicked(bool)), this, SLOT(slotResetPimplesParas()));
    connect(m_pButtonDetectPimples, SIGNAL(clicked(bool)), this, SLOT(slotDetectPimples()));
    m_pGridPimples->addWidget(m_pLabelMinSizePimple, 0, 0, Qt::AlignRight);
    m_pGridPimples->addWidget(m_pLabelMaxSizePimple, 1, 0, Qt::AlignRight);
    m_pGridPimples->addWidget(m_pEditMinSizePimple, 0, 1, Qt::AlignRight);
    m_pGridPimples->addWidget(m_pEditMaxSizePimple, 1, 1, Qt::AlignRight);

    m_pGridPimples->addWidget(m_pLabelMinColorPimple, 0, 2, Qt::AlignRight);
    m_pGridPimples->addWidget(m_pLabelMinColorDiffPimple, 1, 2, Qt::AlignRight);
    m_pGridPimples->addWidget(m_pEditMinColorPimple, 0, 3, Qt::AlignRight);
    m_pGridPimples->addWidget(m_pEditMinRGDiffPimple, 1, 3, Qt::AlignRight);

    m_pGridPimples->addWidget(m_pLabelMinRatioPimple, 0, 4, Qt::AlignRight);
    m_pGridPimples->addWidget(m_pLabelMaxRatioPimple, 1, 4, Qt::AlignRight);
    m_pGridPimples->addWidget(m_pEditMinRatioPimple, 0, 5, Qt::AlignRight);
    m_pGridPimples->addWidget(m_pEditMaxRatioPimple, 1, 5, Qt::AlignRight);

    m_pGridPimples->addWidget(m_pButtonResetPimples, 0, 6, 2, 1);
    m_pGridPimples->addWidget(m_pButtonDetectPimples, 0, 7, 2, 1);
    m_pGroupPimples->setLayout(m_pGridPimples);

    /**
     * blackheads
     *****************************************************/
    m_pGroupBlackheads = new QGroupBox(tr("黑头"), this);
    m_pGridBlackheads = new QGridLayout;
    m_pLabelMaxSizeBlackheads = new QLabel(tr("最大面积:"), this);
    m_pLabelMaxColorBlackheads = new QLabel(tr("最大颜色值:"), this);
    m_pLabelMinRatioBlackheads = new QLabel(tr("最小长宽比:"), this);
    m_pLabelMaxRatioBlackheads = new QLabel(tr("最大长宽比:"), this);
    m_pEditMaxSizeBlackheads = new QLineEdit(this);
    m_pEditMaxColorBlackheads = new QLineEdit(this);
    m_pEditMinRatioBlackheads = new QLineEdit(this);
    m_pEditMaxRatioBlackheads = new QLineEdit(this);
    connect(m_pEditMaxSizeBlackheads, SIGNAL(textChanged(QString)), this, SLOT(slotParamsChangedBlackheads()));
    connect(m_pEditMaxColorBlackheads, SIGNAL(textChanged(QString)), this, SLOT(slotParamsChangedBlackheads()));
    connect(m_pEditMinRatioBlackheads, SIGNAL(textChanged(QString)), this, SLOT(slotParamsChangedBlackheads()));
    connect(m_pEditMaxRatioBlackheads, SIGNAL(textChanged(QString)), this, SLOT(slotParamsChangedBlackheads()));
    m_pButtonResetBlackheads = new QPushButton(tr("恢复默认"), this);
    m_pButtonDetectBlackheads = new QPushButton(tr("开始检测"), this);
    connect(m_pButtonResetBlackheads, SIGNAL(clicked(bool)), this, SLOT(slotResetBlackheadsParas()));
    connect(m_pButtonDetectBlackheads, SIGNAL(clicked(bool)), this, SLOT(slotDetectBlackheads()));
    m_pGridBlackheads->addWidget(m_pLabelMaxSizeBlackheads, 0, 0, 2, 1, Qt::AlignRight);
    m_pGridBlackheads->addWidget(m_pEditMaxSizeBlackheads, 0, 1, 2, 1, Qt::AlignRight);

    m_pGridBlackheads->addWidget(m_pLabelMaxColorBlackheads, 0, 2, 2, 1, Qt::AlignRight);
    m_pGridBlackheads->addWidget(m_pEditMaxColorBlackheads, 0, 3, 2, 1, Qt::AlignRight);

    m_pGridBlackheads->addWidget(m_pLabelMinRatioBlackheads, 0, 4, Qt::AlignRight);
    m_pGridBlackheads->addWidget(m_pLabelMaxRatioBlackheads, 1, 4, Qt::AlignRight);
    m_pGridBlackheads->addWidget(m_pEditMinRatioBlackheads, 0, 5, Qt::AlignRight);
    m_pGridBlackheads->addWidget(m_pEditMaxRatioBlackheads, 1, 5, Qt::AlignRight);

    m_pGridBlackheads->addWidget(m_pButtonResetBlackheads, 0, 6, 2, 1);
    m_pGridBlackheads->addWidget(m_pButtonDetectBlackheads, 0, 7, 2, 1);
    m_pGroupBlackheads->setLayout(m_pGridBlackheads);


    /**
     * FaceColor
     *****************************************************/
    m_pGroupFaceColor = new QGroupBox(tr("肤色"), this);
    m_pGridFaceColor = new QGridLayout;
    m_pLabelTouBai = new QLabel(tr("透白"), this);
    m_pLabelBaiXi = new QLabel(tr("白皙"), this);
    m_pLabelZiRan = new QLabel(tr("自然"), this);
    m_pLabelXiaoMai = new QLabel(tr("小麦"), this);
    m_pLabelAnHei = new QLabel(tr("暗黑"), this);
    m_pLabelYouHei = new QLabel(tr("黝黑"), this);
    m_pEditTouBai = new QLineEdit(this);
    m_pEditBaiXi = new QLineEdit(this);
    m_pEditZiRan = new QLineEdit(this);
    m_pEditXiaoMai = new QLineEdit(this);
    m_pEditAnChen = new QLineEdit(this);
    m_pEditYouHei = new QLineEdit(this);
    connect(m_pEditTouBai, SIGNAL(textChanged(QString)), this, SLOT(slotParamsChangedFaceColor()));
    connect(m_pEditBaiXi, SIGNAL(textChanged(QString)), this, SLOT(slotParamsChangedFaceColor()));
    connect(m_pEditZiRan, SIGNAL(textChanged(QString)), this, SLOT(slotParamsChangedFaceColor()));
    connect(m_pEditXiaoMai, SIGNAL(textChanged(QString)), this, SLOT(slotParamsChangedFaceColor()));
    connect(m_pEditAnChen, SIGNAL(textChanged(QString)), this, SLOT(slotParamsChangedFaceColor()));
    connect(m_pEditYouHei, SIGNAL(textChanged(QString)), this, SLOT(slotParamsChangedFaceColor()));
    m_pButtonResetFaceColor = new QPushButton(tr("恢复默认"), this);
    m_pButtonDetectFaceColor = new QPushButton(tr("开始检测"), this);
    connect(m_pButtonResetFaceColor, SIGNAL(clicked(bool)), this, SLOT(slotResetFaceColoreParas()));
    connect(m_pButtonDetectFaceColor, SIGNAL(clicked(bool)), this, SLOT(slotDetectFaceColor()));
    m_pGridFaceColor->addWidget(m_pLabelTouBai, 0, 0, 1, 1, Qt::AlignCenter);
    m_pGridFaceColor->addWidget(m_pEditTouBai, 1, 0, 1, 1, Qt::AlignCenter);
    m_pGridFaceColor->addWidget(m_pLabelBaiXi, 0, 1, 1, 1, Qt::AlignCenter);
    m_pGridFaceColor->addWidget(m_pEditBaiXi, 1, 1, 1, 1, Qt::AlignCenter);
    m_pGridFaceColor->addWidget(m_pLabelZiRan, 0, 2, 1, 1, Qt::AlignCenter);
    m_pGridFaceColor->addWidget(m_pEditZiRan, 1, 2, 1, 1, Qt::AlignCenter);
    m_pGridFaceColor->addWidget(m_pLabelXiaoMai, 0, 3, 1, 1, Qt::AlignCenter);
    m_pGridFaceColor->addWidget(m_pEditXiaoMai, 1, 3, 1, 1, Qt::AlignCenter);
    m_pGridFaceColor->addWidget(m_pLabelAnHei, 0, 4, 1, 1, Qt::AlignCenter);
    m_pGridFaceColor->addWidget(m_pEditAnChen, 1, 4, 1, 1, Qt::AlignCenter);
    m_pGridFaceColor->addWidget(m_pLabelYouHei, 0, 5, 1, 1, Qt::AlignCenter);
    m_pGridFaceColor->addWidget(m_pEditYouHei, 1, 5, 1, 1, Qt::AlignCenter);
    m_pGridFaceColor->addWidget(m_pButtonResetFaceColor, 0, 6, 2, 1, Qt::AlignCenter);
    m_pGridFaceColor->addWidget(m_pButtonDetectFaceColor, 0, 7, 2, 1, Qt::AlignCenter);
    m_pGroupFaceColor->setLayout(m_pGridFaceColor);

    /**
     * Pore
     *****************************************************/
    m_pGroupPore = new QGroupBox(tr("毛孔粗大度"), this);
    m_pGridPore = new QGridLayout;
    m_pLabelRoughPore = new QLabel(tr("粗大>="), this);
    m_pLabelNormalPore = new QLabel(tr(">一般>="), this);
    m_pLabelSmoothPore = new QLabel(tr(">细腻"), this);
    m_pEditRoughPore = new QLineEdit(this);
    m_pEditNormalPore = new QLineEdit(this);
    connect(m_pEditRoughPore, SIGNAL(textChanged(QString)), this, SLOT(slotParamsChangedPore()));
    connect(m_pEditNormalPore, SIGNAL(textChanged(QString)), this, SLOT(slotParamsChangedPore()));
    m_pButtonResetPore = new QPushButton(tr("恢复默认"), this);
    m_pButtonDetectPore = new QPushButton(tr("开始检测"), this);
    connect(m_pButtonResetPore, SIGNAL(clicked(bool)), this, SLOT(slotResetPoreParas()));
    connect(m_pButtonDetectPore, SIGNAL(clicked(bool)), this, SLOT(slotDetectPore()));
    m_pGridPore->setSpacing(5);
    m_pGridPore->addWidget(m_pLabelRoughPore, 0, 0, Qt::AlignRight);
    m_pGridPore->addWidget(m_pEditRoughPore, 0, 1, Qt::AlignCenter);
    m_pGridPore->addWidget(m_pLabelNormalPore, 0, 2, Qt::AlignCenter);
    m_pGridPore->addWidget(m_pEditNormalPore, 0, 3, Qt::AlignCenter);
    m_pGridPore->addWidget(m_pLabelSmoothPore, 0, 4, Qt::AlignLeft);
    m_pGridPore->addWidget(m_pButtonResetPore, 0, 5, Qt::AlignCenter);
    m_pGridPore->addWidget(m_pButtonDetectPore, 0, 6, Qt::AlignCenter);
    m_pGroupPore->setLayout(m_pGridPore);

    /**
     * Coarseness
     *****************************************************/
    m_pGroupCoarseness = new QGroupBox(tr("皮肤光滑度"), this);
    m_pGridCoarse = new QGridLayout;
    m_pLabelRoughCoarse = new QLabel(tr("粗糙>="), this);
    m_pLabelNormalCoarse = new QLabel(tr(">一般>="), this);
    m_pLabelSmoothCoarse = new QLabel(tr(">光滑"), this);
    m_pEditRoughCoarse = new QLineEdit(this);
    m_pEditNormalCoarse = new QLineEdit(this);
    connect(m_pEditRoughCoarse, SIGNAL(textChanged(QString)), this, SLOT(slotParamsChangedCoarse()));
    connect(m_pEditNormalCoarse, SIGNAL(textChanged(QString)), this, SLOT(slotParamsChangedCoarse()));
    m_pButtonResetCoarse = new QPushButton(tr("恢复默认"), this);
    m_pButtonDetectCoarse = new QPushButton(tr("开始检测"), this);
    connect(m_pButtonResetCoarse, SIGNAL(clicked(bool)), this, SLOT(slotResetCoarseparas()));
    connect(m_pButtonDetectCoarse, SIGNAL(clicked(bool)), this, SLOT(slotDetectCoarseness()));
    m_pGridCoarse->setSpacing(5);
    m_pGridCoarse->addWidget(m_pLabelRoughCoarse, 0, 0, Qt::AlignRight);
    m_pGridCoarse->addWidget(m_pEditRoughCoarse, 0, 1, Qt::AlignCenter);
    m_pGridCoarse->addWidget(m_pLabelNormalCoarse, 0, 2, Qt::AlignCenter);
    m_pGridCoarse->addWidget(m_pEditNormalCoarse, 0, 3, Qt::AlignCenter);
    m_pGridCoarse->addWidget(m_pLabelSmoothCoarse, 0, 4, Qt::AlignLeft);
    m_pGridCoarse->addWidget(m_pButtonResetCoarse, 0, 5, Qt::AlignCenter);
    m_pGridCoarse->addWidget(m_pButtonDetectCoarse, 0, 6, Qt::AlignCenter);
    m_pGroupCoarseness->setLayout(m_pGridCoarse);

    m_pVLayoutRight = new QVBoxLayout;
    m_pVLayoutRight->addWidget(m_pGroupAll);
    m_pVLayoutRight->addWidget(m_pGroupPimples);
    m_pVLayoutRight->addWidget(m_pGroupBlackheads);
    m_pVLayoutRight->addWidget(m_pGroupFaceColor);
    m_pVLayoutRight->addWidget(m_pGroupPore);
    m_pVLayoutRight->addWidget(m_pGroupCoarseness);
    m_pWidgetRight->setLayout(m_pVLayoutRight);

    this->setCentralWidget(m_pSplitter);
    slotResetAllParameters();

}

void MainWindow::startDetectItems(const enumItemType type)
{
    /**
     * 1、同一张图片，检测结果由用户关闭窗口进行关闭；
     * 2、同一张图片，参数改变之后，重新检测时结果在原检测结果窗口基础上进行刷新；
     * 3、更换图片之后，检测时重新生成一个新的检测结果窗口；
     */
    if ((m_pEditPhoto->text() != m_strDetectedImage) || !m_pCurDetecter) {
        m_paramsChanges = 0x0;
        m_paramDetected = 0x0;
        m_strDetectedImage = m_pEditPhoto->text();
        CFaceDetecter *pDetecter = new CFaceDetecter;
        pDetecter->setAttribute(Qt::WA_DeleteOnClose);
        m_pCurDetecter = pDetecter;
        connect(m_pCurDetecter, SIGNAL(destroyed(QObject*)),
                this, SLOT(slotCurDetecterDestroyed()));
        connect(this, SIGNAL(destroyed(QObject*)), pDetecter, SLOT(deleteLater()));
        setObjResultParamValue(m_pCurDetecter->getObjResultRef());
        m_pCurDetecter->startDetect(m_strDetectedImage, type, this);
        m_paramDetected |= type;
    } else {
        if ((m_paramsChanges & type) || !(m_paramDetected & type)) { // 参数改变了或者还没有检测过
            //setObjResultParamValue(m_pCurDetecter->getObjResultRef());
            m_pCurDetecter->startDetect(m_strDetectedImage, type, this);
            m_paramsChanges &= (~type); // 检测过的不再检测
            m_paramDetected |= type;
            return;
        }
        QMessageBox::warning(this, tr("面部特征检测"),
                             tr("图片和对应检测参数均未改变，无需重新检测!"));
    }
}

void MainWindow::setObjResultParamValue(CObjectResult &obj)
{
    obj.m_objPimples.m_dMinAreaSize = m_pEditMinSizePimple->text().toDouble();
    obj.m_objPimples.m_dMaxAreaSize = m_pEditMaxSizePimple->text().toDouble();
    obj.m_objPimples.m_nMinLColorValue = m_pEditMinColorPimple->text().toInt();
    obj.m_objPimples.m_nMinRGDiffValue = m_pEditMinRGDiffPimple->text().toInt();
    obj.m_objPimples.m_dMinRatio = m_pEditMinRatioPimple->text().toDouble();
    obj.m_objPimples.m_dMaxRatio = m_pEditMaxRatioPimple->text().toDouble();

    obj.m_objBlackheads.m_dMaxAreaSize = m_pEditMaxSizeBlackheads->text().toDouble();
    obj.m_objBlackheads.m_nMaxColor = m_pEditMaxColorBlackheads->text().toInt();
    obj.m_objBlackheads.m_dMinRatio = m_pEditMinRatioBlackheads->text().toDouble();
    obj.m_objBlackheads.m_dMaxRatio = m_pEditMaxRatioBlackheads->text().toDouble();

    obj.m_objFaceColor.m_nTouBai = m_pEditTouBai->text().toInt();
    obj.m_objFaceColor.m_nBaiXi = m_pEditBaiXi->text().toInt();
    obj.m_objFaceColor.m_nZiRan = m_pEditZiRan->text().toInt();
    obj.m_objFaceColor.m_nXiaoMai = m_pEditXiaoMai->text().toInt();
    obj.m_objFaceColor.m_nAnChen = m_pEditAnChen->text().toInt();
    obj.m_objFaceColor.m_nYouHei = m_pEditYouHei->text().toInt();

    obj.m_objPore.m_nRough = m_pEditRoughPore->text().toInt();
    obj.m_objPore.m_nNormal = m_pEditNormalPore->text().toInt();

    obj.m_objCoarse.m_nRough = m_pEditRoughCoarse->text().toInt();
    obj.m_objCoarse.m_nNormal = m_pEditNormalCoarse->text().toInt();
}

void MainWindow::slotResetAllParameters()
{
    slotResetPimplesParas();
    slotResetBlackheadsParas();
    slotResetFaceColoreParas();
    slotResetPoreParas();
    slotResetCoarseparas();
}

void MainWindow::slotResetPimplesParas()
{
    m_pEditMinSizePimple->setText(QString("%1").arg(20));
    m_pEditMaxSizePimple->setText(QString("%1").arg(250));
    m_pEditMinColorPimple->setText(QString("%1").arg(20));
    m_pEditMinRGDiffPimple->setText(QString("%1").arg(20));
    m_pEditMinRatioPimple->setText(QString("%1").arg(0.3));
    m_pEditMaxRatioPimple->setText(QString("%1").arg(2.5));
}

void MainWindow::slotResetBlackheadsParas()
{
    m_pEditMaxSizeBlackheads->setText(QString("%1").arg(20));
    m_pEditMaxColorBlackheads->setText(QString("%1").arg(255));
    m_pEditMinRatioBlackheads->setText(QString("%1").arg(0.3));
    m_pEditMaxRatioBlackheads->setText(QString("%1").arg(2.5));
}

void MainWindow::slotResetFaceColoreParas()
{
    m_pEditTouBai->setText(QString("%1").arg(55));
    m_pEditBaiXi->setText(QString("%1").arg(45));
    m_pEditZiRan->setText(QString("%1").arg(25));
    m_pEditXiaoMai->setText(QString("%1").arg(10));
    m_pEditAnChen->setText(QString("%1").arg(-30));
    m_pEditYouHei->setText(QString("%1").arg(-255));
}

void MainWindow::slotResetPoreParas()
{
    m_pEditRoughPore->setText(QString("%1").arg(150));
    m_pEditNormalPore->setText(QString("%1").arg(60));
}

void MainWindow::slotResetCoarseparas()
{
    m_pEditRoughCoarse->setText(QString("%1").arg(33));
    m_pEditNormalCoarse->setText(QString("%1").arg(15));
}

void MainWindow::slotDetectAll()
{
    startDetectItems(TYPE_ALL);
}

void MainWindow::slotDetectPimples()
{
    startDetectItems(TYPE_PIMPLES);
}

void MainWindow::slotDetectBlackheads()
{
    startDetectItems(TYPE_BLACKHEADS);
}

void MainWindow::slotDetectFaceColor()
{
    startDetectItems(TYPE_FACE_COLOR);
}

void MainWindow::slotDetectPore()
{
    startDetectItems(TYPE_PORE);
}

void MainWindow::slotDetectCoarseness()
{
    startDetectItems(TYPE_COARSENESS);
}

void MainWindow::slotParamsChangedPimples()
{
    m_paramsChanges |= TYPE_PIMPLES;
}

void MainWindow::slotParamsChangedBlackheads()
{
    m_paramsChanges |= TYPE_BLACKHEADS;
}

void MainWindow::slotParamsChangedFaceColor()
{
    m_paramsChanges |= TYPE_FACE_COLOR;
}

void MainWindow::slotParamsChangedPore()
{
    m_paramsChanges |= TYPE_PORE;
}

void MainWindow::slotParamsChangedCoarse()
{
    m_paramsChanges |= TYPE_COARSENESS;
}

void MainWindow::slotCurDetecterDestroyed()
{
    m_pCurDetecter = NULL;
}
