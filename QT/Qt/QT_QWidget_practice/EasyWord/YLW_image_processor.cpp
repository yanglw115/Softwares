#include "YLW_image_processor.h"

#include <QApplication>
#include <QFile>
#include <QRect>
#include <QFileDialog>
#include <QPrintDialog>
#include <QColorDialog>
#include <QPrinter>
#include <QtDebug>
#include <QFont>
#include <QTextListFormat>
#include <QTextBlockFormat>
#include <QTextList>

ImgProcessor::ImgProcessor(QWidget *parent)
    : QMainWindow(parent)
    , m_pCentralWidget(NULL)
    , m_pMenuFile(NULL)
    , m_pMenuZoom(NULL)
    , m_pMenuRotate(NULL)
    , m_pMenuMirror(NULL)
    , m_pLabelFont(NULL)
    , m_pLabelSize(NULL)
    , m_pComboBoxFont(NULL)
    , m_pComboBoxSize(NULL)
    , m_pToolButtonBold(NULL)
    , m_pToolButtonItalic(NULL)
    , m_pToolButtonUnderline(NULL)
    , m_pToolButtonColor(NULL)
    , m_pLabelList(NULL)
    , m_pComboBoxList(NULL)
    , m_pActGroup(NULL)
    , m_pActLeft(NULL)
    , m_pActRight(NULL)
    , m_pActCenter(NULL)
    , m_pActJustify(NULL)
    , m_pActNewFile(NULL)
    , m_pActOpenFile(NULL)
    , m_pActPrintText(NULL)
    , m_pActPrintImg(NULL)
    , m_pActExit(NULL)
    , m_pActCopy(NULL)
    , m_pActCut(NULL)
    , m_pActPaste(NULL)
    , m_pActZoomIn(NULL)
    , m_pActZoomOut(NULL)
    , m_pActRotate90(NULL)
    , m_pActRotate180(NULL)
    , m_pActRotate270(NULL)
    , m_pToolFile(NULL)
    , m_pToolZoom(NULL)
    , m_pToolRotate(NULL)
    , m_pToolMirror(NULL)
    , m_pToolFont(NULL)
    , m_pToolList(NULL)
    , m_pToolDo(NULL)
    , m_windowCount(0)
{
    m_pCentralWidget = new CCentralWidget(this);
    /* 当光标位置改变的时候，根据光标位置处理的字体，改变ToolButton的状态 */
    connect(m_pCentralWidget->m_pTextEdit, SIGNAL(cursorPositionChanged()), this, SLOT(slotCursorPositionChanged()));

    m_pLabelFont = new QLabel(tr("字体:"), this);
    m_pLabelSize = new QLabel(tr("字号:"), this);

    m_pComboBoxFont = new QFontComboBox(this);
    m_pComboBoxFont->setFontFilters(QFontComboBox::ScalableFonts);
    connect(m_pComboBoxFont, SIGNAL(activated(QString)), this, SLOT(slotSetFont(QString)));

    m_pComboBoxSize = new QComboBox(this);
    QFontDatabase db;
    foreach (int size, db.standardSizes()) {
        m_pComboBoxSize->addItem(QString::number(size));
    }
    connect(m_pComboBoxSize, SIGNAL(activated(QString)), this, SLOT(slotSetFontSize(QString)));

    m_pToolButtonBold = new QToolButton(this);
    m_pToolButtonBold->setIcon(QIcon(":img/bold.png"));
    /* setCheckable是使toolButton显示是否被使用 */
    m_pToolButtonBold->setCheckable(true);
    connect(m_pToolButtonBold, SIGNAL(clicked()), this, SLOT(slotSetBold()));

    m_pToolButtonItalic = new QToolButton(this);
    m_pToolButtonItalic->setIcon(QIcon(":img/italic.png"));
    m_pToolButtonItalic->setCheckable(true);
    connect(m_pToolButtonItalic, SIGNAL(clicked()), this, SLOT(slotSetItalic()));

    m_pToolButtonUnderline = new QToolButton(this);
    m_pToolButtonUnderline->setIcon(QIcon(":img/underLine.png"));
    m_pToolButtonUnderline->setCheckable(true);
    connect(m_pToolButtonUnderline, SIGNAL(clicked()), this, SLOT(slotSetUnderLine()));

    m_pToolButtonColor = new QToolButton(this);
    m_pToolButtonColor->setIcon(QIcon(":img/color.png"));
    //    m_pToolButtonColor->setCheckable(true);
    connect(m_pToolButtonColor, SIGNAL(clicked()), this, SLOT(slotSetColor()));

    /* 排版 */
    m_pLabelList = new QLabel(tr("List:"), this);
    m_pComboBoxList = new QComboBox(this);
    m_pComboBoxList->addItem("Standard");
    m_pComboBoxList->addItem("QTextListFormat::ListDisc");
    m_pComboBoxList->addItem("QTextListFormat::ListCircle");
    m_pComboBoxList->addItem("QTextListFormat::ListSquare");
    m_pComboBoxList->addItem("QTextListFormat::ListDecimal");
    m_pComboBoxList->addItem("QTextListFormat::ListLowerAlpha");
    m_pComboBoxList->addItem("QTextListFormat::ListUpperAlpha");
    m_pComboBoxList->addItem("QTextListFormat::ListLowerRoman");
    m_pComboBoxList->addItem("QTextListFormat::ListUpperRoman");
    connect(m_pComboBoxList, SIGNAL(activated(int)), this, SLOT(slotList(int)));

            createAction();
    createMenuBar();
    createToolBar();
#ifdef WITH_LABEL_IMAGE
    QString strPath = QApplication::applicationDirPath() + "/" + "2.png";
    if (m_image.load(strPath)) {
        m_pCentralWidget->m_pLabel->setPixmap(QPixmap::fromImage(m_image));
    }
#endif // WITH_LABEL_IMAGE
    this->setCentralWidget(m_pCentralWidget);
}

ImgProcessor::~ImgProcessor()
{

}

void ImgProcessor::slotDecWindowCount()
{
    qDebug() << "Some new file window destroyed.";
    //    m_windowCount--;
}

void ImgProcessor::slotNewFile()
{
    if (m_windowCount++ > 10) {
        m_windowCount = 1;
    }
    ImgProcessor *processor = new ImgProcessor;
    QRect rect =  this->rect();
    processor->move(rect.x() + 10 * m_windowCount, rect.y() + 10 * m_windowCount);
    processor->setAttribute(Qt::WA_DeleteOnClose);
    connect(processor, SIGNAL(destroyed(QObject*)), SLOT(slotDecWindowCount()));
    processor->show();
}

void ImgProcessor::slotOpenFile()
{
    QString strFile = QFileDialog::getOpenFileName(this, tr("请打开新的文件"));
    if (!strFile.isEmpty()) {
        if (m_pCentralWidget->m_pTextEdit->document()->isEmpty()) {
            loadFile(strFile);
        } else {
            if (m_windowCount++ > 10) {
                m_windowCount = 1;
            }
            ImgProcessor *processor = new ImgProcessor;
            QRect rect =  this->rect();
            processor->move(rect.x() + 10 * m_windowCount, rect.y() + 10 * m_windowCount);
            processor->setAttribute(Qt::WA_DeleteOnClose);
            connect(processor, SIGNAL(destroyed(QObject*)), SLOT(slotDecWindowCount()));
            processor->loadFile(strFile);
            processor->show();
        }
    }
}

void ImgProcessor::slotPrintText()
{
    QPrinter printer;
    QPrintDialog *pPrintDialog = new QPrintDialog(&printer, this);
    if (pPrintDialog->exec()) {
        QTextDocument *pDocument = m_pCentralWidget->m_pTextEdit->document();
        pDocument->print(&printer);
    }
}

void ImgProcessor::slotZoomIn()
{
    QMatrix matrix;
    matrix.scale(2, 2);
#ifdef WITH_LABEL_IMAGE
    if (m_image.isNull()) {
        qWarning() << "The image is null, cannot zoom in.";
        return;
    }
    m_image = m_image.transformed(matrix);
    m_pCentralWidget->m_pLabel->setPixmap(QPixmap::fromImage(m_image));
#else
    m_pCentralWidget->m_pImgItem->setTransform(QTransform(matrix), true);
#endif // WITH_LABEL_IMAGE
}

void ImgProcessor::slotZoomOut()
{
    QMatrix matrix;
    matrix.scale(0.5, 0.5);
#ifdef WITH_LABEL_IMAGE
    if (m_image.isNull()) {
        qWarning() << "The image is null, cannot zoom in.";
        return;
    }
    m_image = m_image.transformed(matrix);
    m_pCentralWidget->m_pLabel->setPixmap(QPixmap::fromImage(m_image));
#else
    m_pCentralWidget->m_pImgItem->setTransform(QTransform(matrix), true);
#endif // WITH_LABEL_IMAGE
}

void ImgProcessor::slotRotate90()
{
    QMatrix matrix;
    matrix.rotate(90);
#ifdef WITH_LABEL_IMAGE
    if (m_image.isNull()) {
        qWarning() << "The image is null, cannot zoom in.";
        return;
    }
    m_image = m_image.transformed(matrix);
    m_pCentralWidget->m_pLabel->setPixmap(QPixmap::fromImage(m_image));
#else
    /*
     * 这里可以直接使用setRotation，但是这样使用只能旋转一次，不能一直旋转
     * 如果使用setTransform则能够在上一次旋转的基础上再次进行旋转
     */
    //    m_pCentralWidget->m_pImgItem->setRotation(90);
    m_pCentralWidget->m_pImgItem->setTransform(QTransform(matrix), true);
#endif // WITH_LABEL_IMAGE
}

void ImgProcessor::slotMirrorH()
{
#ifdef WITH_LABEL_IMAGE
    if (m_image.isNull()) {
        qWarning() << "The image is null, cannot zoom in.";
        return;
    }
    m_image = m_image.mirrored(true, false);
    m_pCentralWidget->m_pLabel->setPixmap(QPixmap::fromImage(m_image));
#else
    /* 使用QGraphicsView没有找到很好地进行镜像操作的方法 */
    m_pCentralWidget->m_pImgItem->setScale(-1);
#endif // WITH_LABEL_IMAGE
}

void ImgProcessor::slotMirrorV()
{
#ifdef WITH_LABEL_IMAGE
    if (m_image.isNull()) {
        qWarning() << "The image is null, cannot zoom in.";
        return;
    }
    m_image = m_image.mirrored(false, true);
    m_pCentralWidget->m_pLabel->setPixmap(QPixmap::fromImage(m_image));
#else
    /* 使用QGraphicsView没有找到很好地进行镜像操作的方法 */
    m_pCentralWidget->m_pImgItem->setScale(-1);
#endif // WITH_LABEL_IMAGE
}

void ImgProcessor::slotSetFont(QString strFont)
{
    QTextCharFormat format;
    format.setFontFamily(strFont);
    mergeFormat(format);
}

void ImgProcessor::slotSetFontSize(QString fontSize)
{
    QTextCharFormat format;
    format.setFontPointSize(fontSize.toFloat());
    mergeFormat(format);
}

void ImgProcessor::slotSetBold()
{
    QTextCharFormat format;
    /* 这里使用了QFont::Weight枚举值 */
    format.setFontWeight(m_pToolButtonBold->isChecked()? QFont::Bold: QFont::Normal);
    mergeFormat(format);
}

void ImgProcessor::slotSetItalic()
{
    QTextCharFormat format;
    format.setFontItalic(m_pToolButtonItalic->isChecked());
    mergeFormat(format);
}

void ImgProcessor::slotSetUnderLine()
{
    QTextCharFormat format;
    format.setFontUnderline(m_pToolButtonUnderline->isChecked());
    mergeFormat(format);
}

void ImgProcessor::slotSetColor()
{
    QColor color = QColorDialog::getColor(Qt::red, this);
    if (color.isValid()) {
        QTextCharFormat format;
        format.setForeground(color);
        mergeFormat(format);
    }
}

void ImgProcessor::slotAlignment(QAction *pAction)
{
    /* 这里使用的是QActionGroup，在group中的Action同一时间只能有一个激活 */
    if (pAction == m_pActLeft) {
        m_pCentralWidget->m_pTextEdit->setAlignment(Qt::AlignLeft);
    } else if (pAction == m_pActRight) {
        m_pCentralWidget->m_pTextEdit->setAlignment(Qt::AlignRight);
    } else if (pAction == m_pActCenter) {
        m_pCentralWidget->m_pTextEdit->setAlignment(Qt::AlignCenter);
    } else if (pAction == m_pActJustify) {
        m_pCentralWidget->m_pTextEdit->setAlignment(Qt::AlignJustify);
    }
}

void ImgProcessor::slotList(int index)
{
    QTextCursor cursor = m_pCentralWidget->m_pTextEdit->textCursor();
    if (0 != index) {
        QTextListFormat::Style style = QTextListFormat::ListDisc;

        switch (index) {
        default:
        case 1:
            style = QTextListFormat::ListDisc;
            break;
        case 2:
            style = QTextListFormat::ListCircle;
            break;
        case 3:
            style = QTextListFormat::ListSquare;
            break;
        case 4:
            style = QTextListFormat::ListDecimal;
            break;
        case 5:
            style = QTextListFormat::ListLowerAlpha;
            break;
        case 6:
            style = QTextListFormat::ListUpperAlpha;
            break;
        case 7:
            style = QTextListFormat::ListLowerRoman;
            break;
        case 8:
            style = QTextListFormat::ListUpperRoman;
            break;
        }

        cursor.beginEditBlock(); /* 与后台的end相应，目的是在回退的时候一起回退 */

        QTextBlockFormat blockFormat = cursor.blockFormat(); /* 段落格式 */
        QTextListFormat listFormat; /* 排序格式 */
        if (cursor.currentList()) {
            listFormat = cursor.currentList()->format();
        } else {
            listFormat.setIndent(blockFormat.indent() + 1); /* 设置缩进值 */
            blockFormat.setIndent(0); /* 重新设置段落缩进，因为前面添加了序号，所以这里设置为0 */
            cursor.setBlockFormat(blockFormat);
        }
        listFormat.setStyle(style);
        cursor.createList(listFormat);

        cursor.endEditBlock();
    } else {
        QTextBlockFormat blockFormat;
        blockFormat.setObjectIndex(-1); /* -1的时候format object is invalid */
        cursor.mergeBlockFormat(blockFormat);
    }
}

/* 当光标位置变化时，改变对应光标处所对应的工具栏工具状态 */
void ImgProcessor::slotCursorPositionChanged()
{
    /* QTextEidt直接提供了接口可以获取其当前段落或文本字体是否加粗、斜体、下划线、左对齐、右对齐等状态 */
    /* 斜体工具栏状态恢复 */
    m_pToolButtonItalic->setChecked(m_pCentralWidget->m_pTextEdit->fontItalic());
    /* 加粗状态栏恢复 */
    switch (m_pCentralWidget->m_pTextEdit->fontWeight()) {
    case QFont::Normal:
        m_pToolButtonBold->setChecked(false);
        break;
    case QFont::Bold:
        m_pToolButtonBold->setChecked(true);
        break;
    default:
        break;
    }
    /* 下划线状态栏恢复 */
    m_pToolButtonUnderline->setChecked(m_pCentralWidget->m_pTextEdit->fontUnderline());

    /* 当前字体 */
    m_pComboBoxFont->setCurrentIndex(m_pComboBoxFont->findText(m_pCentralWidget->m_pTextEdit->currentFont().family()));
    m_pComboBoxSize->setCurrentIndex(m_pComboBoxSize->findText(QString("%1").arg(m_pCentralWidget->m_pTextEdit->currentFont().pointSize())));

    /* 对齐状态栏恢复 */
    switch (m_pCentralWidget->m_pTextEdit->alignment()) {
    case Qt::AlignLeft:
        m_pActLeft->setChecked(true);
        break;
    case Qt::AlignRight:
        m_pActRight->setChecked(true);
        break;
    case Qt::AlignCenter:
        m_pActCenter->setChecked(true);
        break;
    case Qt::AlignJustify:
        m_pActJustify->setChecked(true);
        break;
    default:
        /* 下面的操作并不需要，因为这里使用的是QActionGroup，在group中的Action同一时间只能有一个激活 */
        //        m_pActLeft->setChecked(false);
        //        m_pActRight->setChecked(false);
        //        m_pActCenter->setChecked(false);
        //        m_pActJustify->setChecked(false);
        break;
    }
}

void ImgProcessor::mergeFormat(QTextCharFormat format)
{
    /* 获取光标副本 */
    QTextCursor cursor = m_pCentralWidget->m_pTextEdit->textCursor();
    /* 光标是否有选择，是否选中文本 */
    if (!cursor.hasSelection()) {
        qDebug() << "The current cursor has no selection...";
        /* WordUnderCursor以光标所在处的单词作为选中对象，以标点符号作为单词分隔 */
        cursor.select(QTextCursor::WordUnderCursor);
    }
    /* 将新的格式与原格式进行合并 */
    cursor.mergeCharFormat(format);
    /* 将新格式设置到当前文本,前面cursor只是一个副本 */
    m_pCentralWidget->m_pTextEdit->mergeCurrentCharFormat(format);
}

void ImgProcessor::loadFile(QString strFile)
{
    QFile file(strFile);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream textStream(&file);
        while (!textStream.atEnd()) {
            m_pCentralWidget->m_pTextEdit->append(textStream.readLine());
        }
    }
}

void ImgProcessor::createAction()
{
    m_pActOpenFile = new QAction(QIcon(":img/openFile.png"), tr("打开文件"), this);
    /* 设置快捷键之后，会在菜单下拉列表后面显示快捷键 */
    m_pActOpenFile->setShortcut(tr("Ctrl+o"));
    m_pActOpenFile->setStatusTip(tr("打开一个文件"));
    connect(m_pActOpenFile, SIGNAL(triggered(bool)), this, SLOT(slotOpenFile()));

    m_pActNewFile = new QAction(QIcon(":img/newFile.png"), tr("新建文件"), this);
    connect(m_pActNewFile, SIGNAL(triggered(bool)), this, SLOT(slotNewFile()));

    m_pActPrintText = new QAction(QIcon(":img/printText.png"), tr("打印文本"), this);
    connect(m_pActPrintText, SIGNAL(triggered(bool)), this, SLOT(slotPrintText()));

    m_pActPrintImg = new QAction(QIcon(":img/printImg.png"), tr("打印图片"), this);
    m_pActExit = new QAction(QIcon(":img/exit.png"), tr("退出"), this);
    m_pActCopy = new QAction(QIcon(":img/copy.png"), tr("复制"), this);
    m_pActCut = new QAction(QIcon(":img/cut.png"), tr("剪切"), this);
    m_pActPaste = new QAction(QIcon(":img/paste.png"), tr("粘贴"), this);

    m_pActAbout = new QAction(QIcon(":img/about.png"), tr("关于"), this);

    m_pActZoomIn = new QAction(QIcon(":img/zoomIn.png"), tr("放大"), this);
    connect(m_pActZoomIn, SIGNAL(triggered(bool)), this, SLOT(slotZoomIn()));

    m_pActZoomOut = new QAction(QIcon(":img/zoomOut.png"), tr("缩小"), this);
    connect(m_pActZoomOut, SIGNAL(triggered(bool)), this, SLOT(slotZoomOut()));

    m_pActRotate90 = new QAction(QIcon(":img/rotate90.png"), tr("旋转90度"), this);
    connect(m_pActRotate90, SIGNAL(triggered(bool)), this, SLOT(slotRotate90()));

    m_pActRotate180 = new QAction(QIcon(":img/rotate180.png"), tr("旋转180度"), this);
    m_pActRotate270 = new QAction(QIcon(":img/rotate270.png"), tr("旋转270度"), this);
    m_pActMirrorHorizental = new QAction(QIcon(":img/mirrorH.png"), tr("水平镜像"), this);
    connect(m_pActMirrorHorizental, SIGNAL(triggered(bool)), this, SLOT(slotMirrorH()));

    m_pActMirrorVertical = new QAction(QIcon(":img/mirrorV.png"), tr("垂直镜像"), this);
    m_pActUndo = new QAction(QIcon(":img/undo.png"), tr("撤销"), this);
    m_pActRedo = new QAction(QIcon(":img/redo.png"), tr("重做"), this);

    /* 这里使用的是QActionGroup，在group中的Action同一时间只能有一个激活 */
    m_pActGroup = new QActionGroup(this);
    connect(m_pActGroup, SIGNAL(triggered(QAction*)), this, SLOT(slotAlignment(QAction*)));
    m_pActLeft = new QAction(QIcon(":img/alignLeft.png"), tr("左对齐"), m_pActGroup);
    m_pActLeft->setCheckable(true);
    m_pActRight = new QAction(QIcon(":img/alignRight.png"), tr("右对齐"), m_pActGroup);
    m_pActRight->setCheckable(true);
    m_pActCenter = new QAction(QIcon(":img/alignCenter.png"), tr("中间对齐"), m_pActGroup);
    m_pActCenter->setCheckable(true);
    m_pActJustify = new QAction(QIcon(":img/alignAll.png"), tr("两边对齐"), m_pActGroup);
    m_pActJustify->setCheckable(true);
}

void ImgProcessor::createMenuBar()
{
    /**
     * 1.QMainWindow自带菜单栏，所以可以直接使用其指针或引用添加详细菜单
     * 2.如果在QAction实例化的时候，没有指定icon,则在菜单栏图标的空间会是空白，同时工具栏会显示工具的name,如果有图标，则工具栏显示工具图标而不是名称 */
    m_pMenuFile = this->menuBar()->addMenu(tr("文件"));
    m_pMenuFile->addAction(m_pActOpenFile);
    m_pMenuFile->addAction(m_pActNewFile);
    m_pMenuFile->addAction(m_pActPrintText);
    m_pMenuFile->addAction(m_pActPrintImg);
    m_pMenuFile->addSeparator();
    m_pMenuFile->addAction(m_pActExit);

    m_pMenuZoom = this->menuBar()->addMenu(tr("编辑"));
    m_pMenuZoom->addAction(m_pActCopy);
    m_pMenuZoom->addAction(m_pActCut);
    m_pMenuZoom->addAction(m_pActPaste);
    m_pMenuZoom->addAction(m_pActAbout);
    m_pMenuZoom->addSeparator();
    m_pMenuZoom->addAction(m_pActZoomIn);
    m_pMenuZoom->addAction(m_pActZoomOut);

    m_pMenuRotate = this->menuBar()->addMenu(tr("旋转"));
    m_pMenuRotate->addAction(m_pActRotate90);
    m_pMenuRotate->addAction(m_pActRotate180);
    m_pMenuRotate->addAction(m_pActRotate270);

    m_pMenuMirror = this->menuBar()->addMenu(tr("镜像"));
    m_pMenuMirror->addAction(m_pActMirrorHorizental);
    m_pMenuMirror->addAction(m_pActMirrorVertical);
}

void ImgProcessor::createToolBar()
{
    m_pToolFile = this->addToolBar(tr("文件"));
    m_pToolFile->addAction(m_pActOpenFile);
    m_pToolFile->addAction(m_pActNewFile);
    m_pToolFile->addAction(m_pActPrintText);
    m_pToolFile->addAction(m_pActPrintImg);
    /* 设置工具条是否可以移动 */
    m_pToolFile->setMovable(false);

    m_pToolZoom = this->addToolBar(tr("编辑"));
    m_pToolZoom->addAction(m_pActCopy);
    m_pToolZoom->addAction(m_pActCut);
    m_pToolZoom->addAction(m_pActPaste);
    m_pToolZoom->addAction(m_pActZoomIn);
    m_pToolZoom->addAction(m_pActZoomOut);
    m_pToolZoom->setMovable(false);

    m_pToolRotate = this->addToolBar(tr("旋转"));
    m_pToolRotate->addAction(m_pActRotate90);
    m_pToolRotate->addAction(m_pActRotate180);
    m_pToolRotate->addAction(m_pActRotate270);
    m_pToolRotate->setMovable(false);

    /* 设置工具栏能够停靠的位置，默认是所有位置 */
    /* 第一次只是创建一个toolBar的对象及其窗口标题 */
    m_pToolMirror = this->addToolBar(tr("镜像"));
    /* 第二次利用第一次创建的对象，指定其位置，默认是Qt::TopToolBarArea */
    this->addToolBar(Qt::BottomToolBarArea, m_pToolMirror);
    m_pToolMirror->addAction(m_pActMirrorHorizental);
    m_pToolMirror->addAction(m_pActMirrorVertical);

    m_pToolFont = this->addToolBar(tr("字体"));
    m_pToolFont->addWidget(m_pLabelFont);
    m_pToolFont->addWidget(m_pComboBoxFont);
    m_pToolFont->addWidget(m_pLabelSize);
    m_pToolFont->addWidget(m_pComboBoxSize);
    m_pToolFont->addSeparator();
    m_pToolFont->addWidget(m_pToolButtonBold);
    m_pToolFont->addWidget(m_pToolButtonItalic);
    m_pToolFont->addWidget(m_pToolButtonUnderline);
    m_pToolFont->addSeparator();
    m_pToolFont->addWidget(m_pToolButtonColor);

    m_pToolList = this->addToolBar(tr("对齐"));
    m_pToolList->addWidget(m_pLabelList);
    m_pToolList->addWidget(m_pComboBoxList);
    m_pToolList->addSeparator();
    m_pToolList->addActions(m_pActGroup->actions());
}
