#include "YLW_mycombobox.h"

#include <QListView>

CMyComboBox::CMyComboBox(QWidget *parent)
    : QComboBox(parent)
{
    ((QListView *)this->view())->setSpacing(3);
}

CMyComboBox::~CMyComboBox()
{

}
