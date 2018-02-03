#include "mainwindow.h"
#include "vs_charset.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setFixedSize(900, 600);
    initWidgets();
}

MainWindow::~MainWindow()
{

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
    m_pixmap.load(":/images/photo.jpg");
    m_pLabelImage->setFixedWidth(300);
    m_pLabelImage->setFixedHeight(500);
    m_pixmap = m_pixmap.scaled(m_pLabelImage->width(), m_pLabelImage->height(), Qt::KeepAspectRatio);
    //m_pLabelImage->setScaledContents(true);
    m_pLabelImage->setPixmap(m_pixmap);
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
    m_pButtonDetectAll = new QPushButton(tr("开始检测"), this);
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
    m_pLabelMaxColorDiffPimple = new QLabel(tr("最大RG差值(BGR):"), this);
    m_pLabelMinRatioPimple = new QLabel(tr("最小长宽比:"), this);
    m_pLabelMaxRatioPimple = new QLabel(tr("最大长宽比:"), this);
    m_pEditMinSizePimple = new QLineEdit(this);
    m_pEditMaxSizePimple = new QLineEdit(this);
    m_pEditMinColorPimple = new QLineEdit(this);
    m_pEditMaxColorPimple = new QLineEdit(this);
    m_pEditMinRatioPimple = new QLineEdit(this);
    m_pEditMaxRatioPimple = new QLineEdit(this);
    m_pButtonResetPimples = new QPushButton(tr("恢复默认"), this);
    m_pButtonDetectPimples = new QPushButton(tr("开始检测"), this);
    m_pGridPimples->addWidget(m_pLabelMinSizePimple, 0, 0, Qt::AlignRight);
    m_pGridPimples->addWidget(m_pLabelMaxSizePimple, 1, 0, Qt::AlignRight);
    m_pGridPimples->addWidget(m_pEditMinSizePimple, 0, 1, Qt::AlignRight);
    m_pGridPimples->addWidget(m_pEditMaxSizePimple, 1, 1, Qt::AlignRight);

    m_pGridPimples->addWidget(m_pLabelMinColorPimple, 0, 2, Qt::AlignRight);
    m_pGridPimples->addWidget(m_pLabelMaxColorDiffPimple, 1, 2, Qt::AlignRight);
    m_pGridPimples->addWidget(m_pEditMinColorPimple, 0, 3, Qt::AlignRight);
    m_pGridPimples->addWidget(m_pEditMaxColorPimple, 1, 3, Qt::AlignRight);

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
    m_pButtonResetBlackheads = new QPushButton(tr("恢复默认"), this);
    m_pButtonDetectBlackheads = new QPushButton(tr("开始检测"), this);
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
    m_pEditAnHei = new QLineEdit(this);
    m_pEditYouHei = new QLineEdit(this);
    m_pButtonResetFaceColor = new QPushButton(tr("恢复默认"), this);
    m_pButtonDetectFaceColor = new QPushButton(tr("开始检测"), this);
    m_pGridFaceColor->addWidget(m_pLabelTouBai, 0, 0, 1, 1, Qt::AlignCenter);
    m_pGridFaceColor->addWidget(m_pEditTouBai, 1, 0, 1, 1, Qt::AlignCenter);
    m_pGridFaceColor->addWidget(m_pLabelBaiXi, 0, 1, 1, 1, Qt::AlignCenter);
    m_pGridFaceColor->addWidget(m_pEditBaiXi, 1, 1, 1, 1, Qt::AlignCenter);
    m_pGridFaceColor->addWidget(m_pLabelZiRan, 0, 2, 1, 1, Qt::AlignCenter);
    m_pGridFaceColor->addWidget(m_pEditZiRan, 1, 2, 1, 1, Qt::AlignCenter);
    m_pGridFaceColor->addWidget(m_pLabelXiaoMai, 0, 3, 1, 1, Qt::AlignCenter);
    m_pGridFaceColor->addWidget(m_pEditXiaoMai, 1, 3, 1, 1, Qt::AlignCenter);
    m_pGridFaceColor->addWidget(m_pLabelAnHei, 0, 4, 1, 1, Qt::AlignCenter);
    m_pGridFaceColor->addWidget(m_pEditAnHei, 1, 4, 1, 1, Qt::AlignCenter);
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
    m_pButtonResetPore = new QPushButton(tr("恢复默认"), this);
    m_pButtonDetectPore = new QPushButton(tr("开始检测"), this);
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
    m_pButtonResetCoarse = new QPushButton(tr("恢复默认"), this);
    m_pButtonDetectCoarse = new QPushButton(tr("开始检测"), this);
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
    resetAllParameters();

}

void MainWindow::resetAllParameters()
{
    resetPimplesParas();
    resetBlackheadsParas();
    resetFaceColoreParas();
    resetPoreParas();
    resetCoarseparas();
}

void MainWindow::resetPimplesParas()
{
    m_pEditMinSizePimple->setText(QString("%1").arg(20));
    m_pEditMaxSizePimple->setText(QString("%1").arg(250));
    m_pEditMinColorPimple->setText(QString("%1").arg(20));
    m_pEditMaxColorPimple->setText(QString("%1").arg(20));
    m_pEditMinRatioPimple->setText(QString("%1").arg(0.3));
    m_pEditMaxRatioPimple->setText(QString("%1").arg(2.5));
}

void MainWindow::resetBlackheadsParas()
{
    m_pEditMaxSizeBlackheads->setText(QString("%1").arg(20));
    m_pEditMaxColorBlackheads->setText(QString("%1").arg(255));
    m_pEditMinRatioBlackheads->setText(QString("%1").arg(0.3));
    m_pEditMaxRatioBlackheads->setText(QString("%1").arg(2.5));
}

void MainWindow::resetFaceColoreParas()
{
    m_pEditTouBai->setText(QString("%1").arg(55));
    m_pEditBaiXi->setText(QString("%1").arg(45));
    m_pEditZiRan->setText(QString("%1").arg(25));
    m_pEditXiaoMai->setText(QString("%1").arg(10));
    m_pEditAnHei->setText(QString("%1").arg(-30));
    m_pEditYouHei->setText(QString("%1").arg(-255));
}

void MainWindow::resetPoreParas()
{
    m_pEditRoughPore->setText(QString("%1").arg(150));
    m_pEditNormalPore->setText(QString("%1").arg(60));
}

void MainWindow::resetCoarseparas()
{
    m_pEditRoughCoarse->setText(QString("%1").arg(33));
    m_pEditNormalCoarse->setText(QString("%1").arg(15));
}
