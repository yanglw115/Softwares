#ifndef CURE_TABLEHEADERVIEW
#define CURE_TABLEHEADERVIEW

#include <QHeaderView>
#include <QCheckBox>
#include <QPainter>
#include <QMouseEvent>
#include <QEvent>
#include <QStyledItemDelegate>

#define CHECK_BOX_COLUMN 0

class TableHeaderView: public QHeaderView
{
    Q_OBJECT
public:
    TableHeaderView(Qt::Orientation, QWidget *);
    virtual ~TableHeaderView();

signals:
    void sigCheckStateChanged(int state);
protected:
    virtual void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const;
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual bool event(QEvent *event);

public slots:
    void slotCheckStateChanged(int state);

private slots:
    void slotRefreshHeader();
private:
    bool m_bPressed;
    bool m_bChecked;
    bool m_bTristate;
    bool m_bNoChange;
};


class CheckBoxDelegate: public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CheckBoxDelegate(QObject *parent = NULL);
    virtual ~CheckBoxDelegate();
    virtual QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
protected:
    virtual bool editorEvent(QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index);
private:

};

#endif // CURE_TABLEHEADERVIEW

