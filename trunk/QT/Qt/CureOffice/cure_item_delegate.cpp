#include "cure_item_delegate.h"

#include <QCheckBox>
#include <QStyledItemDelegate>
#include <QtDebug>
#include <QApplication>
#include <QMouseEvent>

CureCheckDelegate::CureCheckDelegate(QObject * parent)
    : QStyledItemDelegate(parent)
{

}

CureCheckDelegate::~CureCheckDelegate()
{

}

static QRect CheckBoxRect(const QStyleOptionViewItem &viewItemStyleOptions)/*const*/
{
    //绘制按钮所需要的参数
    QStyleOptionButton checkBoxStyleOption;
    //按照给定的风格参数 返回元素子区域
    QRect checkBoxRect = QApplication::style()->subElementRect( QStyle::SE_CheckBoxIndicator, &checkBoxStyleOption);
    //返回QCheckBox坐标
    QPoint checkBoxPoint(viewItemStyleOptions.rect.x() + viewItemStyleOptions.rect.width() / 2 - checkBoxRect.width() / 2,
                         viewItemStyleOptions.rect.y() + viewItemStyleOptions.rect.height() / 2 - checkBoxRect.height() / 2);
    //返回QCheckBox几何形状
    return QRect(checkBoxPoint, checkBoxRect.size());
}

QWidget *CureCheckDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QCheckBox *pCheckBox = new QCheckBox(parent);
    return pCheckBox;
}

void CureCheckDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QCheckBox *pCheckBox = dynamic_cast<QCheckBox*>(editor);
    if (pCheckBox) {
        QString strData = index.model()->data(index, Qt::UserRole).toString();
        if (strData.toInt() > 0) {
            pCheckBox->setChecked(true);
        } else {
            pCheckBox->setChecked(false);
        }
    } else {
        qDebug() << "Dynamic cast QCheckBox failed!";
    }
}

void CureCheckDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QCheckBox *pCheckBox = dynamic_cast<QCheckBox*>(editor);
    if (pCheckBox) {
        if (pCheckBox->isChecked()) {
            model->setData(index, QString("1"), Qt::UserRole);
        } else {
            model->setData(index, QString("0"), Qt::UserRole);
        }
    } else {
        qDebug() << "Dynamic cast QCheckBox failed!";
    }
}

void CureCheckDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

#if 0
void CureCheckDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    bool bChecked = index.model()->data(index, Qt::UserRole).toBool();

    if (1) {//(0 == index.row()){
#if 0
        QStyleOptionButton checkBoxStyleOption;
        checkBoxStyleOption.state |= QStyle::State_Enabled;
        checkBoxStyleOption.state |= bChecked? QStyle::State_On: QStyle::State_Off;
        checkBoxStyleOption.rect = CheckBoxRect(option);

        QApplication::style()->drawControl(QStyle::CE_CheckBox,&checkBoxStyleOption, painter);
#else
        QStyleOptionViewItem viewOption(option);
        initStyleOption(&viewOption, index);
        if (option.state.testFlag(QStyle::State_HasFocus))
            viewOption.state = viewOption.state ^ QStyle::State_HasFocus;

        QStyledItemDelegate::paint(painter, viewOption, index);

        if (1) //(index.column() == CHECK_BOX_COLUMN)
        {
            bool data = index.model()->data(index, Qt::UserRole).toBool();

            QStyleOptionButton checkBoxStyle;
            checkBoxStyle.state = data ? QStyle::State_On : QStyle::State_Off;
            checkBoxStyle.state |= QStyle::State_Enabled;
            checkBoxStyle.iconSize = QSize(20, 20);
            checkBoxStyle.rect = option.rect;

            QCheckBox checkBox;
            QApplication::style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &checkBoxStyle, painter, &checkBox);
        }
#endif
    } else{
        QStyledItemDelegate::paint(painter, option, index);
    }
}


bool CureCheckDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (1) {//(0 == index.row()) {
#if 0
        if ((event->type() == QEvent::MouseButtonRelease) ||
                (event->type() == QEvent::MouseButtonDblClick)){
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() != Qt::LeftButton ||
                    !CheckBoxRect(option).contains(mouseEvent->pos())){
                return true;
            }
            if(event->type() == QEvent::MouseButtonDblClick){
                return true;
            }
        } else if(event->type() == QEvent::KeyPress) {
            if(static_cast<QKeyEvent*>(event)->key() != Qt::Key_[Space &&
                    static_cast<QKeyEvent*>(event)->key() != Qt::Key_Select){
                return false;
            }
        } else{
            return false;
        }

        bool bChecked = index.model()->data(index, Qt::UserRole).toBool();
        return model->setData(index, !bChecked, Qt::UserRole);
#else
        QRect decorationRect = option.rect;

        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (event->type() == QEvent::MouseButtonPress && decorationRect.contains(mouseEvent->pos()))
        {
            if (1) //(index.column() == CHECK_BOX_COLUMN)
            {
                bool data = model->data(index, Qt::UserRole).toBool();
                model->setData(index, !data, Qt::UserRole);
            }
        }

        return QStyledItemDelegate::editorEvent(event, model, option, index);
#endif
    }else{
        return QStyledItemDelegate::editorEvent(event, model, option, index);
    }
}

#endif
