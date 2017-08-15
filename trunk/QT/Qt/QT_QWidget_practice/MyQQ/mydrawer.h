#ifndef MYDRAWER_H
#define MYDRAWER_H

#include <QGroupBox>
#include <QToolBox>
#include <QToolButton>
#include <QVBoxLayout>

class MyDrawer : public QToolBox
{
public:
    MyDrawer(QWidget *parent = 0);
    ~MyDrawer();

private:
    QGroupBox *m_pGroupBoxFriend;
    QGroupBox *m_pGroupBoxStranger;
    QGroupBox *m_pGroupBoxBlackList;

    QVBoxLayout *m_pVBoxFriend;
    QVBoxLayout *m_pVBoxStranger;
    QVBoxLayout *m_pVBoxBlackList;

    QToolButton *m_pTButtonFriend01;
    QToolButton *m_pTButtonFriend02;
    QToolButton *m_pTButtonFriend03;
    QToolButton *m_pTButtonFriend04;
    QToolButton *m_pTButtonFriend05;
    QToolButton *m_pTButtonFriend10;
    QToolButton *m_pTButtonFriend11;
    QToolButton *m_pTButtonFriend20;
    QToolButton *m_pTButtonFriend21;
};

#endif // MYDRAWER_H
