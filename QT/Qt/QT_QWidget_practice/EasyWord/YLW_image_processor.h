#ifndef YLW_IMAGE_PROCESSOR_H
#define YLW_IMAGE_PROCESSOR_H

#include <QComboBox>
#include <QFontComboBox>
#include <QMenuBar>
#include <QAction>
#include <QToolBar>
#include <QToolButton>
#include <QMainWindow>

#include "YLW_central_widget.h"

class ImgProcessor : public QMainWindow
{
    Q_OBJECT

public:
    ImgProcessor(QWidget *parent = 0);
    ~ImgProcessor();

private slots:
    void slotDecWindowCount();
    void slotNewFile();
    void slotOpenFile();
    void slotPrintText();
    void slotZoomIn();
    void slotZoomOut();
    void slotRotate90();
    void slotMirrorH();
    void slotMirrorV();
    void slotSetFont(QString);
    void slotSetFontSize(QString);
    void slotSetBold();
    void slotSetItalic();
    void slotSetUnderLine();
    void slotSetColor();
    void slotAlignment(QAction *);
    void slotList(int);
    void slotCursorPositionChanged();

private:
    void mergeFormat(QTextCharFormat);
    void loadFile(QString strFile);
    void createAction();
    void createMenuBar();
    void createToolBar();

private:
    QImage m_image;
    CCentralWidget *m_pCentralWidget;
    /* 菜单栏中的菜单,具体功能通过QAction实现 */
    QMenu *m_pMenuFile;
    QMenu *m_pMenuZoom;
    QMenu *m_pMenuRotate;
    QMenu *m_pMenuMirror;

    QLabel *m_pLabelFont;
    QLabel *m_pLabelSize;
    QFontComboBox *m_pComboBoxFont;
    QComboBox *m_pComboBoxSize;
    QToolButton *m_pToolButtonBold;
    QToolButton *m_pToolButtonItalic;
    QToolButton *m_pToolButtonUnderline;
    QToolButton *m_pToolButtonColor;

    QLabel *m_pLabelList; /* 对齐 */
    QComboBox *m_pComboBoxList;
    QActionGroup *m_pActGroup;
    QAction *m_pActLeft;
    QAction *m_pActRight;
    QAction *m_pActCenter;
    QAction *m_pActJustify;

    /* 各动作按钮 */
    QAction *m_pActNewFile; /* 新建 */
    QAction *m_pActOpenFile; /* 打开文件 */
    //QAction *m_pActSaveFile;
    QAction *m_pActPrintText; /* 打印文本 */
    QAction *m_pActPrintImg; /* 打印图片 */
    QAction *m_pActExit; /* 退出程序 */

    QAction *m_pActCopy; /* 复制 */
    QAction *m_pActCut;
    QAction *m_pActPaste;
    QAction *m_pActAbout;
    QAction *m_pActZoomIn;
    QAction *m_pActZoomOut;

    QAction *m_pActRotate90; /* 旋转 */
    QAction *m_pActRotate180;
    QAction *m_pActRotate270;

    QAction *m_pActMirrorVertical; /* 水平镜像 */
    QAction *m_pActMirrorHorizental;

    QAction *m_pActUndo;
    QAction *m_pActRedo;

    /* 多个工具栏 */
    QToolBar *m_pToolFile;
    QToolBar *m_pToolZoom;
    QToolBar *m_pToolRotate;
    QToolBar *m_pToolMirror;
    QToolBar *m_pToolFont; /* 文本编辑 */
    QToolBar *m_pToolList; /* 排版 */

    QToolBar *m_pToolDo;

    int m_windowCount;
};

#endif // YLW_IMAGE_PROCESSOR_H
