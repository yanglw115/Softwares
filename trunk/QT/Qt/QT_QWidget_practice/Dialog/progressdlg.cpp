#include "progressdlg.h"

ProgressDlg::ProgressDlg(QWidget *parent)
    : QDialog(parent)
    , m_pLabelFileNum(NULL)
    , m_pEditFileNum(NULL)
    , m_pLabelType(NULL)
    , m_pComboBox(NULL)
    , m_pProgressBar(NULL)
    , m_pButtonStart(NULL)
    , m_pProgressDlg(NULL)
    , m_pGridLayout(NULL)
{
    m_pLabelFileNum = new QLabel(tr("文件数目"), this);
    m_pEditFileNum = new QLineEdit(this);
    m_pEditFileNum->setText("10000");
    m_pLabelType = new QLabel(tr("显示类型"), this);

    m_pComboBox = new QComboBox(this);
    m_pComboBox->addItem(tr("Progress bar"));
    m_pComboBox->addItem(tr("Progress dialog"));

    m_pProgressBar = new QProgressBar(this);
    m_pButtonStart = new QPushButton(tr("开始"), this);
    connect(m_pButtonStart, SIGNAL(clicked()), this, SLOT(slotStartProgress()));

    m_pGridLayout = new QGridLayout(this);

    m_pGridLayout->addWidget(m_pLabelFileNum, 0, 0);
    m_pGridLayout->addWidget(m_pEditFileNum, 0, 1);
    m_pGridLayout->addWidget(m_pLabelType, 1, 0);
    m_pGridLayout->addWidget(m_pComboBox, 1, 1);
    m_pGridLayout->addWidget(m_pProgressBar, 2, 0, 1, 2);
    /* 这里不能插入一个空的widget在button的左边，所以通过设置button的位置来使其在右边 */
    m_pGridLayout->addWidget(m_pButtonStart, 3, 1, 1, 1);

    m_pGridLayout->setSpacing(10);
    m_pGridLayout->setMargin(10);
}


ProgressDlg::~ProgressDlg()
{

}

void ProgressDlg::slotStartProgress()
{
    int fileNumber = m_pEditFileNum->text().toInt();
    if (m_pComboBox->currentIndex() == 0) {
        m_pProgressBar->setRange(0, fileNumber);
        /* 这里最后的取值要到fileNumber大小，否则最后进度条不满 */
        for (int i = 0; i <= fileNumber; ++i) {
            /* 这里通过QProgressBar来展示进度条，相当于一个静态的widget，只需要在适当的时候设置其值即可 */
            m_pProgressBar->setValue(i);
        }
    } else if (m_pComboBox->currentIndex() == 1) {
        if (m_pProgressDlg != NULL) {
            delete m_pProgressDlg;
        }
        /* 通过一个新的Dialog Widget来展示进度 */
        m_pProgressDlg = new QProgressDialog(this);
        /* 设置为模态模式,如果是非模态模式，则是不阻塞的，这时需要通过定时器来更新进度 */
        m_pProgressDlg->setWindowModality(Qt::WindowModal);
        /* 设置最小时间(ms),如果超时该时间，才展示进度条对话框 */
        m_pProgressDlg->setMinimumDuration(5);
        m_pProgressDlg->setWindowTitle(tr("Please wait......"));
        m_pProgressDlg->setLabelText(tr("Copying ..."));
        m_pProgressDlg->setCancelButtonText(tr("Cancel"));
        m_pProgressDlg->setRange(0, fileNumber);
        for (int i = 0; i <= fileNumber; ++i) {
            m_pProgressDlg->setValue(i);
            if (m_pProgressDlg->wasCanceled()) {
                return;
            }
        }
    }
}
