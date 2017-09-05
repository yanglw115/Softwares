#ifndef YLW_MYCOMBOBOX_H
#define YLW_MYCOMBOBOX_H

#include <QComboBox>

class CMyComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit CMyComboBox(QWidget *parent = Q_NULLPTR);
    ~CMyComboBox();
};

#endif // YLW_MYCOMBOBOX_H
