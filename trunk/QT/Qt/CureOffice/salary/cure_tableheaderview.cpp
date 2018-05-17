#include "cure_tableheaderview.h"

#include <QApplication>
#include <QDebug>
#include <QTimer>

TableHeaderView::~TableHeaderView()
{

}

TableHeaderView::TableHeaderView(Qt::Orientation orientation, QWidget *parent)
    : QHeaderView(orientation, parent),
      m_bPressed(false),
      m_bChecked(false),
      m_bTristate(false)
{
    // setStretchLastSection(true);
    setHighlightSections(false);
    setMouseTracking(true);

    // 响应鼠标
    setSectionsClickable(true);
}

// 槽函数，用于更新复选框状态
void TableHeaderView::slotCheckStateChanged(int state)
{
//    qDebug() << "Header view get new state: " << state;
    if (state == Qt::PartiallyChecked) {
        if (!m_bTristate) {
            m_bTristate = true;
        }
    } else {
        m_bTristate = false;
    }

    m_bChecked = (state == Qt::Checked);
    /* 经过调试，以下几种方法均可行，但是update()有时不生效 */
//    repaint();
//    updateSection(CHECK_BOX_COLUMN);
    /* 以下是针对数据状态改变，但是表头数据未能立即刷新过来的解决方法。其中200毫秒是经验值 */
    headerDataChanged(Qt::Horizontal, 0, 0);
    QTimer::singleShot(200, this, SLOT(slotRefreshHeader()));
}

void TableHeaderView::slotRefreshHeader()
{
    headerDataChanged(Qt::Horizontal, 0, 0);
}

// 绘制复选框
void TableHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    painter->save();
    QHeaderView::paintSection(painter, rect, logicalIndex);
    painter->restore();

    if (logicalIndex == CHECK_BOX_COLUMN) {
        QStyleOptionButton option;
        option.initFrom(this);

        if (m_bTristate)
            option.state |= QStyle::State_Sunken;

        if (m_bTristate)
            option.state |= QStyle::State_NoChange;
        else
            option.state |= m_bChecked ?
                        QStyle::State_On : QStyle::State_Off;

        QCheckBox checkBox;
        option.iconSize = QSize(20, 20);
        option.rect = rect;
        style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &option, painter, &checkBox);
        //style()->drawItemPixmap(painter, rect, Qt::AlignCenter, QPixmap(":/images/checkBoxChecked"));
        //style()->drawControl(QStyle::CE_CheckBox, &option, painter, this);
    }
}

// 鼠标按下表头
void TableHeaderView::mousePressEvent(QMouseEvent *event)
{
    int nColumn = logicalIndexAt(event->pos());
    if ((event->buttons() & Qt::LeftButton) && (nColumn == CHECK_BOX_COLUMN)) {
        m_bPressed = true;
    } else {
        QHeaderView::mousePressEvent(event);
    }
}

// 鼠标从表头释放。发送信号，更新model数据
void TableHeaderView::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_bPressed) {
        if (m_bTristate) {
            m_bChecked = true;
            m_bTristate = false;
        } else {
            m_bChecked = !m_bChecked;
            m_bTristate = false;
        }

        update();

        Qt::CheckState state = m_bChecked ? Qt::Checked : Qt::Unchecked;

        emit sigCheckStateChanged(state);
    } else {
        QHeaderView::mouseReleaseEvent(event);
    }

    m_bPressed = false;
}

// 鼠标滑过、离开，更新复选框状态
bool TableHeaderView::event(QEvent *event)
{
    return QHeaderView::event(event);
    if (event->type() == QEvent::Enter || event->type() == QEvent::Leave) {
        QMouseEvent *pEvent = static_cast<QMouseEvent *>(event);
        int nColumn = logicalIndexAt(pEvent->x());
        if (nColumn == CHECK_BOX_COLUMN) {
            update();
//            updateSection(CHECK_BOX_COLUMN);
            return true;
        }
    }

    return QHeaderView::event(event);
}


/***********************************************************/


CheckBoxDelegate::CheckBoxDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

CheckBoxDelegate::~CheckBoxDelegate()
{

}

QWidget *CheckBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    QCheckBox *pCheckBox = new QCheckBox(parent);
    return pCheckBox;
}

void CheckBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QCheckBox *pCheckBox = dynamic_cast<QCheckBox*>(editor);
    if (pCheckBox) {
        if (CHECK_BOX_COLUMN == index.column()) {
            bool bState = index.model()->data(index, Qt::UserRole).toBool();
//            qDebug() << "setEditorData: (" << index.row() << ", " << index.column() << "): " << bState;
            pCheckBox->setChecked(bState);
        }
    } else {
        qDebug() << "Dynamic cast QCheckBox failed!";
    }
}

void CheckBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QCheckBox *pCheckBox = dynamic_cast<QCheckBox*>(editor);
    if (pCheckBox) {
        if (CHECK_BOX_COLUMN == index.column()) {
            if (pCheckBox->isChecked()) {
//                qDebug() << "setModelData: (" << index.row() << ", " << index.column() << "): " << pCheckBox->isChecked();
                model->setData(index, true, Qt::UserRole);
            } else {
//                qDebug() << "setModelData: (" << index.row() << ", " << index.column() << "): " << pCheckBox->isChecked();
                model->setData(index, false, Qt::UserRole);
            }
        }
    } else {
        qDebug() << "Dynamic cast QCheckBox failed!";
    }
}

void CheckBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)
    editor->setGeometry(option.rect);
}

// 绘制复选框
void CheckBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);
    if (option.state.testFlag(QStyle::State_HasFocus))
        viewOption.state = viewOption.state ^ QStyle::State_HasFocus;

    QStyledItemDelegate::paint(painter, viewOption, index);

    if (index.column() == CHECK_BOX_COLUMN) {
        bool data = index.model()->data(index, Qt::UserRole).toBool();

        QStyleOptionButton checkBoxStyle;
        checkBoxStyle.state = data ? QStyle::State_On : QStyle::State_Off;
        checkBoxStyle.state |= QStyle::State_Enabled;
        checkBoxStyle.iconSize = QSize(20, 20);
        checkBoxStyle.rect = option.rect;

        QCheckBox checkBox;
        QApplication::style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &checkBoxStyle, painter, &checkBox);
    }
}

// 响应鼠标事件，更新数据
bool CheckBoxDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    QRect decorationRect = option.rect;

    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    if (event->type() == QEvent::MouseButtonPress && decorationRect.contains(mouseEvent->pos())) {
        if (index.column() == CHECK_BOX_COLUMN) {
            bool data = model->data(index, Qt::UserRole).toBool();
//            qDebug() << "setModelData: (" << index.row() << ", " << index.column() << "): " << !data;
            model->setData(index, !data, Qt::UserRole);
        }
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
