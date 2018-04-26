#include <QFileDialog>
#include <memory>
#include <iostream>
#include <QtDebug>
#include <QMessageBox>
#include <QHeaderView>

#include "cure_salary.h"
#include "msvs_charset.h"
#include "cure_item_delegate.h"

using namespace std;

static const QString g_strOpenExcel("打开文件");
static const QString g_strCloseExcel("关闭");

CureSalary::CureSalary(QWidget *parent)
    : QWidget(parent)
    , m_pLabelOpenExcel(NULL)
    , m_pEditOpenedExcel(NULL)
    , m_pButtonOpenExcel(NULL)
    , m_pTableExcel(NULL)
    , m_pXlsxDoc(NULL)
    , m_pVLayoutMain(NULL)
    , m_pHLayoutOpenExcel(NULL)
    , m_bStateOpenExcel(false)
{
    initWidgets();
}

CureSalary::~CureSalary()
{

}

void CureSalary::slotOpenExcel()
{
    if (!m_bStateOpenExcel) {
        QString strFilePath = QFileDialog::getOpenFileName(this, tr("Excel文件选择"), "Desktop", "*.xlsx");
        if (!strFilePath.isEmpty()) {
            m_pEditOpenedExcel->setText(strFilePath);
            freeXlsxDocument();
            m_pXlsxDoc = new Document(strFilePath);
            if (m_pXlsxDoc) {
                QString strSheetName = m_pXlsxDoc->sheetNames()[0];
                Worksheet *pWorksheet = dynamic_cast<Worksheet*>(m_pXlsxDoc->sheet(strSheetName));
                if (pWorksheet) {
                    int nStartRow = 5;
                    m_pTableExcel->setModel(new SheetModel(pWorksheet, m_pTableExcel, nStartRow));
                    foreach (CellRange range, pWorksheet->mergedCells())
                        m_pTableExcel->setSpan(range.firstRow()-1, range.firstColumn()-1, range.rowCount(), range.columnCount());
                    m_bStateOpenExcel = true;
                }
            } else {
                qCritical() << "New xlsx document failed!";
            }
        }
    } else {
        freeXlsxDocument();
        m_bStateOpenExcel = false;
    }

    if (m_bStateOpenExcel) {
        m_pButtonOpenExcel->setText(g_strCloseExcel);
    } else {
        m_pButtonOpenExcel->setText(g_strOpenExcel);
    }
}

void CureSalary::initWidgets()
{
    m_pLabelOpenExcel = new QLabel(tr("请打开.xlsx格式的工资表: "), this);
    m_pEditOpenedExcel = new QLineEdit(this);
    m_pEditOpenedExcel->setReadOnly(true);
    m_pButtonOpenExcel = new QPushButton(g_strOpenExcel, this);
    connect(m_pButtonOpenExcel, SIGNAL(clicked(bool)), this, SLOT(slotOpenExcel()));
    m_pTableExcel = new QTableView(this);

    m_pVLayoutMain = new QVBoxLayout;
    m_pHLayoutOpenExcel = new QHBoxLayout;

    this->setLayout(m_pVLayoutMain);
    m_pHLayoutOpenExcel->addWidget(m_pLabelOpenExcel);
    m_pHLayoutOpenExcel->addWidget(m_pEditOpenedExcel);
    m_pHLayoutOpenExcel->addWidget(m_pButtonOpenExcel);

    m_pVLayoutMain->addLayout(m_pHLayoutOpenExcel);
    m_pVLayoutMain->addWidget(m_pTableExcel);
}

void CureSalary::freeXlsxDocument()
{
    if (m_pXlsxDoc) {
        SheetModel *pModel = dynamic_cast<SheetModel*>(m_pTableExcel->model());
        if (pModel) {
            m_pTableExcel->clearSpans();
            m_pTableExcel->setModel(NULL);
            delete pModel;
            pModel = NULL;
        } else {
            qWarning() << "Get SheetModel failed from tableView!";
            //QMessageBox::warning(this, tr(""), tr(""));
            return;
        }
        delete m_pXlsxDoc;
        m_pXlsxDoc = NULL;
    }
}

/* 根据导入的数据模板，提取邮件发送的excel头部信息。也可以直接使用邮件发送模板，就不需要这样提取了。 */
void CureSalary::saveSalaryExcelHead(Worksheet *pWorksheet, const int nStartRow)
{
    QString strSaveFileName = "Salary_201808.xlsx";
    Document *pDoc = new Document;
    pDoc->addSheet(tr("工资明细"));

    Format format;
    format.setFont(QFont("宋体", 10, QFont::Bold));
    format.setPatternBackgroundColor(Qt::yellow);
    format.setTextWarp(true); // 自动换行
    format.setVerticalAlignment(Format::AlignVCenter);
    format.setHorizontalAlignment(Format::AlignHCenter);
    format.setBorderStyle(Format::BorderThin); // 边框为实线

    for (int i = 0; i < pWorksheet->dimension().lastColumn(); ++i) {
        Cell *pCell = pWorksheet->cellAt(nStartRow - 1, i + 1);
        if (!pCell->value().isNull()) {
            pDoc->write(1, i + 1, pCell->value(), format);
        }
    }
    if (QFile::exists(strSaveFileName)) {
        QFile::remove(strSaveFileName);
    }
    pDoc->saveAs(strSaveFileName);
    delete pDoc;
    pDoc = NULL;
}

