#ifndef YLW_MAIN_WIDGET_H
#define YLW_MAIN_WIDGET_H

#include <QComboBox>
#include <QFrame>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QWidget>

#include "YLW_paint_area.h"

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = 0);
    ~MainWidget();

private slots:
    void slotChangeShap(int);
    void slotChangePenColor();
    void slotChangePenWidth(int);
    void slotChangePenStyle(int);
    void slotChangePenCap(int);
    void slotChangePenJoin(int);
    void slotChangeFillRule();
    void slotChangeSpreadStyle();
    void slotChangeBrushColor();
    void slotChangeBrushStyle(int);

private:
    PaintArea *m_pPaintArea;
    QLabel *m_pLabelShap;
    QLabel *m_pLabelPenColor;
    QLabel *m_pLabelPenWidth;
    QLabel *m_pLabelPenStyle;
    QLabel *m_pLabelPenCap;
    QLabel *m_pLabelPenJoin;
    QLabel *m_pLabelFillRule;
    QLabel *m_pLabelSpreadStyle;
    QLabel *m_pLabelBrushColor;
    QLabel *m_pLabelBrushStyle;

    QComboBox *m_pComboBoxShap;
    QFrame *m_pFramePenColor;
    QPushButton *m_pButtonPenColor;
    QSpinBox *m_pSpinBoxPenWidth;
    QComboBox *m_pComboBoxPenStyle;
    QComboBox *m_pComboBoxPenCap;
    QComboBox *m_pComboBoxPenJoin;
    QComboBox *m_pComboBoxFillRule;
    QComboBox *m_pComboBoxSpreadStyle;
    QFrame *m_pFrameBrushColor;
    QPushButton *m_pButtonBrushColor;
    QComboBox *m_pComboBoxBrushStyle;

    QGradient::Spread m_spread;

    QHBoxLayout *m_pHLayout;
    QGridLayout *m_pGridLayout;
};

#endif // YLW_MAIN_WIDGET_H
