/****************************************************************************
** Copyright (c) 2013-2014 Debao Zhang <hello@debao.me>
** All right reserved.
**
** Permission is hereby granted, free of charge, to any person obtaining
** a copy of this software and associated documentation files (the
** "Software"), to deal in the Software without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Software, and to
** permit persons to whom the Software is furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be
** included in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
** NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
** LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
** OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
** WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**
****************************************************************************/

#include <xlsx/xlsx_sheet_model.h>
#include "xlsx_sheet_model_p.h"
#include "xlsxworksheet.h"
#include "cure_global.h"

#include <QBrush>
#include <QCheckBox>
#include <QtDebug>

QT_BEGIN_NAMESPACE_XLSX

#define CHECK_BOX_COLUMN 0

SheetModelPrivate::SheetModelPrivate(SheetModel *p)
    :q_ptr(p)
{

}

/*!
 * \class SheetModel
 *
 *  Helper class for gui applicaiton user
 *
 *  \note SheetModel indices start from 0, while Worksheet
 *  column/row indices start from 1.
 */

/*!
 * Creates a model object with the given \a sheet and \a parent.
 */
/* 增加excel数据读取的起始行，必须要大于等于1(起始行从1计起) */
SheetModel::SheetModel(Worksheet *sheet, QObject *parent, const int nStartRow)
    :QAbstractTableModel(parent), d_ptr(new SheetModelPrivate(this))
{
    d_ptr->sheet = sheet;
    m_nStartRow = nStartRow;
    m_bEmitCheckStateChange = true;
    m_vecotrSelect.resize(this->rowCount());
//    m_vecotrSelect.fill(SALARY_CHECKED | SALARY_SEND_OK);
    m_vecotrSelect.fill(SALARY_SEND_OK);

    // connect(this, SIGNAL(sigDataChanged(QModelIndex)), this, SLOT(slotDataChanged(QModelIndex)));
}

/*!
 * Destroys the model.
 */
SheetModel::~SheetModel()
{
    delete d_ptr;
}

int SheetModel::rowCount(const QModelIndex &/*parent*/) const
{
    Q_D(const SheetModel);
    return d->sheet->dimension().lastRow() - m_nStartRow + 1;
}


int SheetModel::columnCount(const QModelIndex &/*parent*/) const
{
    Q_D(const SheetModel);
    return d->sheet->dimension().lastColumn() + 1; // with checkheader
}

Qt::ItemFlags SheetModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    if (CHECK_BOX_COLUMN == index.column()) {
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
    }
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant SheetModel::data(const QModelIndex &index, int role) const
{
    Q_D(const SheetModel);

    if (!index.isValid())
        return QVariant();

    int nColumn = index.column();

    if (CHECK_BOX_COLUMN == nColumn) {
        /* 第一列是checkbox，所以不需要返回显示数据 */
        if (role == Qt::UserRole) {
//            qDebug() << "get user role:" << m_vecotrSelect;
            return (m_vecotrSelect[index.row()] & SALARY_CHECKED);
        } else {
            return QVariant();
        }
    }

    /**
     * 从excel表中读取数据，sheet表的数据索引是从(1, 1)开始
     * 这里因为占用了第一列用作checkbox,所以sheet值从index.column(>=1)开始，否则就是index.colum+1
     */
    Cell *cell = d->sheet->cellAt(index.row() + m_nStartRow, index.column());
    if (!cell)
        return QVariant();
    QVariant userFriendlyValue = d->sheet->read(index.row() + m_nStartRow, index.column());

    switch (role) {
    case Qt::DisplayRole:
        if (cell->isDateTime())
            return userFriendlyValue;
        return cell->value();
        break;
    case Qt::EditRole:
        return userFriendlyValue;
        break;
    case Qt::TextAlignmentRole:
    {
        Qt::Alignment align;
        switch (cell->format().horizontalAlignment()) {
        case Format::AlignLeft:
            align |= Qt::AlignLeft;
            break;
        case Format::AlignRight:
            align |= Qt::AlignRight;
            break;
        case Format::AlignHCenter:
            align |= Qt::AlignHCenter;
            break;
        case Format::AlignHJustify:
            align |= Qt::AlignJustify;
            break;
        default:
            break;
        }
        switch (cell->format().verticalAlignment()) {
        case Format::AlignTop:
            align |= Qt::AlignTop;
            break;
        case Format::AlignBottom:
            align |= Qt::AlignBottom;
            break;
        case Format::AlignVCenter:
            align |= Qt::AlignVCenter;
            break;
        default:
            break;
        }
        return QVariant(align);
        break;
    }
    case Qt::FontRole:
        if (cell->format().hasFontData())
            return cell->format().font();
        break;
    case Qt::ForegroundRole:
        if (cell->format().fontColor().isValid())
            return QBrush(cell->format().fontColor());
        break;
    case Qt::BackgroundRole:
#if 0
        if (cell->format().patternBackgroundColor().isValid())
            return QBrush(cell->format().patternBackgroundColor());
#else
        if (m_vecotrSelect[index.row()] & SALARY_SEND_OK) {
            return QBrush(Qt::white);
        } else {
            return QBrush(Qt::yellow);
        }
#endif
        break;
#if 0
    case Qt::CheckStateRole:
        // 直接在TableView中设置checkbox不稳定，有时候check状态会无故改变
        if (0 == index.column()) {
            return (m_vecotrSelect[index.row()] & SALARY_CHECKED)? Qt::Checked: Qt::Unchecked;
        }
        break;
    case Qt::UserRole:
        if (nColumn == CHECK_BOX_COLUMN)
            return m_vecotrSelect[index.row()] & SALARY_CHECKED;
        break;
#endif
    default:
        break;
    }
    return QVariant();
}

/*
 * Copy from xlsxutility.cpp, so this example don't depend on the xlsx-private
 * This function should be removed once this class moved to the xlsx library.
 */
static QString col_to_name(int col_num)
{
    QString col_str;

    int remainder;
    while (col_num) {
        remainder = col_num % 26;
        if (remainder == 0)
            remainder = 26;
        col_str.prepend(QChar('A'+remainder-1));
        col_num = (col_num - 1) / 26;
    }

    return col_str;
}

QVariant SheetModel::headerData(int section, Qt::Orientation orientation, int role) const
{
#if 0
    /* A-Z,AA-AZ…… */
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal)
            return col_to_name(section + 1);
        else
            return QString::number(section + 1);
    }
    return QVariant();
#else
    Q_D(const SheetModel);
    /* 读取excel */
    switch (role) {
    case Qt::DisplayRole:
        if (orientation == Qt::Horizontal) {
            if (section != CHECK_BOX_COLUMN) {
                /* 这里因为使用了header中的checkbox，单独占用一列，所以下面为section, 否则为section + 1 */
                Cell *cell = d->sheet->cellAt(m_nStartRow - 1, section);
                return cell->value();
            } else {
                return QVariant();
            }
        } else
            return QString::number(section + 1);
        break;
    default:
        break;
    }
    return QVariant();
#endif
}

bool SheetModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_D(const SheetModel);

    if (!index.isValid())
        return false;

    int nColumn = index.column();
    bool bReturn = false;
    switch (role) {
    case Qt::EditRole:
        /* 这里因为使用了header的checkbox，所以下面的sheet取值列为index.column() + 2，否则为index.column() + 1 */
        if (d->sheet->write(index.row() + m_nStartRow, index.column() + 2, value) == 0)
            bReturn = true;
        break;
    case Qt::CheckStateRole:
        /* checkstate用于非自定义的控件 */
        break;
    case Qt::UserRole:
        /* 直接在数据列的checkbox上面进行操作，使用的是UserRole */
        if (nColumn == CHECK_BOX_COLUMN) {
            if (value.toBool()) {
                m_vecotrSelect[index.row()] |= SALARY_CHECKED;
            } else {
                m_vecotrSelect[index.row()] &= (~SALARY_CHECKED);
            }
            slotDataChanged(index);
            if (m_bEmitCheckStateChange) {
                checkStateChanged();
            }
            bReturn = true;
        }
        break;
    default:
        break;
    }

    return bReturn;
}

/*!
 * Returns the sheet object.
 */
Worksheet *SheetModel::sheet() const
{
    Q_D(const SheetModel);
    return d->sheet;
}

QVector<int> & SheetModel::getCheckStateVector()
{
    return m_vecotrSelect;
}

void SheetModel::checkStateChanged()
{
    Qt::CheckState state = Qt::Unchecked;
    int nCount = m_vecotrSelect.size();
    int nSelectedCount = 0;

    for (int i = 0; i < nCount; ++i) {
        if (m_vecotrSelect[i] & SALARY_CHECKED)
            ++nSelectedCount;
    }

    if (nSelectedCount >= nCount) {
        state = Qt::Checked;
    } else if (nSelectedCount > 0) {
        state = Qt::PartiallyChecked;
    }

//    qDebug() << "Header checkbox new state: " << state;
    emit sigCheckStateChanged(state);
}

void SheetModel::slotCheckStateChanged(int state)
{
    QModelIndex index;
    for (int i = 0; i < rowCount(); ++i) {
        index = this->index(i, CHECK_BOX_COLUMN);
        m_bEmitCheckStateChange = false;
        setData(index, state == Qt::Checked, Qt::UserRole);
        m_bEmitCheckStateChange = true;
    }
}

void SheetModel::slotDataChanged(const QModelIndex &index)
{
    /* 必须执行reset,否则包括头部的checkbox状态会有问题 */
    Q_UNUSED(index)
    beginResetModel();
    endResetModel();
}

QT_END_NAMESPACE_XLSX
