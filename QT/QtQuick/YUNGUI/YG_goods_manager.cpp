#include <QtDebug>

#include "YG_goods_manager.h"

CGoodsManager::CGoodsManager(QObject *parent)
    : QObject(parent)

{
    /* 通过获取商品信息实例指针的方式，对商品信息进行访问。所以程序需要确保商品信息在之前已经成功实例化 */
    m_pGoodsInfo = getGoodsInfoInstance();
}

CGoodsManager::~CGoodsManager()
{

}

/**************************************************************************
 * Function: getGoodsListSize
 * Description: 获取商品列表大小，即可选购商品的种数
 * Input:无
 * Output: 无
 * Return: 可选购商品种数
 *************************************************************************/
int CGoodsManager::getGoodsListSize()
{
    int size = 0;

    m_pGoodsInfo->m_mutex.lock();
    size = m_pGoodsInfo->m_mapGoods.count();
    m_pGoodsInfo->m_mutex.unlock();

    return size;
}

/**************************************************************************
 * Function: bIsGoodsInfoExist
 * Description: 判断对应位置的商品是否存在
 * Input: 商品在柜架上的位置
 * Output: 无
 * Return: 存在返回true,不存在返回false
 *************************************************************************/
bool CGoodsManager::bIsGoodsInfoExist(int position)
{
    bool ret = false;

    m_pGoodsInfo->m_mutex.lock();
    if (m_pGoodsInfo->m_mapGoods.contains(position)) {
        ret = true;
    }
    m_pGoodsInfo->m_mutex.unlock();

    return ret;
}

/**************************************************************************
 * Function: bIsGoodsEmpty
 * Description: 判断对应位置的商品是否还有剩余
 * Input: 商品在柜架上的位置
 * Output: 无
 * Return: 有剩余返回false,无剩余返回true
 *************************************************************************/
bool CGoodsManager::bIsGoodsEmpty(int position)
{
    bool ret = true;

    m_pGoodsInfo->m_mutex.lock();
    if (m_pGoodsInfo->m_mapGoods[position].nCount > 0) {
        ret = false;
    }
    m_pGoodsInfo->m_mutex.unlock();

    return ret;
}

/**************************************************************************
 * Function: getGoodsName
 * Description: 获取对应位置商品名称
 * Input: 商品在柜架上的位置
 * Output: 无
 * Return: 商品名称
 *************************************************************************/
QString CGoodsManager::getGoodsName(int position)
{
    QString name = "";

    m_pGoodsInfo->m_mutex.lock();
    name = m_pGoodsInfo->m_mapGoods[position].strName;
    m_pGoodsInfo->m_mutex.unlock();

    qDebug() << "Get the name:" << name;
    return name;
}

/**************************************************************************
 * Function: getGoodsPicPath
 * Description: 获取对应位置商品图片所在本地存放路径
 * Input: 商品在柜架上的位置
 * Output: 无
 * Return: 商品图片本地所在路径
 *************************************************************************/
QString CGoodsManager::getGoodsPicPath(int position)
{
    QString strPath = "file:///";

    m_pGoodsInfo->m_mutex.lock();
    strPath = m_pGoodsInfo->m_mapGoods[position].strPicturePath;
    m_pGoodsInfo->m_mutex.unlock();

    qDebug() << "Get the pic path:" << strPath;
    return strPath;
}

/**************************************************************************
 * Function: getGoodsCount
 * Description: 获取对应位置商品剩余数量
 * Input: 商品在柜架上的位置
 * Output: 无
 * Return: 商品剩余数量
 *************************************************************************/
QString CGoodsManager::getGoodsCount(int position)
{
    int count = 0;

    m_pGoodsInfo->m_mutex.lock();
    count = m_pGoodsInfo->m_mapGoods[position].nCount;
    m_pGoodsInfo->m_mutex.unlock();


    return QString("").setNum(count);
}

/**************************************************************************
 * Function: getGoodsPrice
 * Description: 获取对应位置商品单价
 * Input: 商品在柜架上的位置
 * Output: 无
 * Return: 商品单价
 *************************************************************************/
QString CGoodsManager::getGoodsPrice(int position)
{
    int price = 0;

    m_pGoodsInfo->m_mutex.lock();
    price = m_pGoodsInfo->m_mapGoods[position].nPrice;
    m_pGoodsInfo->m_mutex.unlock();

    return QString("").setNum(price);
}

/**************************************************************************
 * Function: setSelectedGoods
 * Description: 设置所选商品的购买数量
 * Input:
 *      position: 商品所在货架位置
 *      count: 选购数量
 *      start: 是否本次购买所有商品的开始
 *      end: 是否本次购买所有商品的结束
 * Output: 无
 * Return: 成功返回true,失败返回false
 *************************************************************************/
bool CGoodsManager::setSelectedGoods(int position, int count, bool start, bool end)
{

    return  m_pGoodsInfo->setSelectedGoods(position, count, start, end);
}

/**************************************************************************
 * Function: setPayStyle
 * Description: 设置本次购买支付方式
 * Input: 支付类型
 * Output: 无
 * Return: 成功返回true,失败返回false
 *************************************************************************/
bool CGoodsManager::setPayStyle(int style)
{
    return m_pGoodsInfo->setPayStyle(style);
}

/**************************************************************************
 * Function: startToPay
 * Description: 开始支付。打开对应支付接收设备，使其可以工作
 * Input: 无
 * Output: 无
 * Return: 成功返回true,失败返回false
 *************************************************************************/
bool CGoodsManager::startToPay()
{
    qDebug() << "Get pay style: " << m_pGoodsInfo->m_struPayInfo.payStyle;
    switch (m_pGoodsInfo->m_struPayInfo.payStyle) {
    case TYPE_PAY_STYLE_COIN:
        connect(m_pGoodsInfo->m_pCoinAcceptor, SIGNAL(sigAcceptCoin()), this, SLOT(slotHandleCoinIn()));
        if (!m_pGoodsInfo->m_pCoinAcceptor->isEnabled()) {
            qDebug() << "Coin device disabled, enable it.";
            m_pGoodsInfo->m_pCoinAcceptor->SetEnabled(true);
        }
        /* payout device operation */
        connect(m_pGoodsInfo->m_pNoteAcceptor, SIGNAL(sigNoteAccepted(uint)), this, SLOT(slotHandleNoteIn(uint)));
        //                m_pGoodsInfo->m_pNoteAcceptor->slotInit();
        m_pGoodsInfo->m_pNoteAcceptor->StartAccept();
        break;
    case TYPE_PAY_STYLE_CARD:
        /* 或者使用读卡器进行读卡支付，或者使用制卡器进行读卡支付 */
        if (PAY_WITH_CARD_READER) {
            connect(m_pGoodsInfo->m_pCardReadr, SIGNAL(sigCardIn(bool)), this, SLOT(slotHandleCardIn(bool)));
            connect(m_pGoodsInfo->m_pCardReadr, SIGNAL(sigReadBlockData(uchar*)), this, SLOT(slotHandleCardMoneyData(uchar*)));
        } else {
            connect(m_pGoodsInfo->m_pCardReleaser, SIGNAL(sigReadBlockData(uchar*)), this, SLOT(slotHandleCardMoneyData(uchar*)));
            connect(m_pGoodsInfo->m_pCardReleaser, SIGNAL(sigCardInRFPos(bool)), this, SLOT(slotHandleCardIn(bool)));
            qDebug() << "Start to pay with card releaser.";
            /* 先设置允许插卡 */
            m_pGoodsInfo->m_pCardReleaser->setEnableInCard(true);
            /* 再发起检测卡是否插入的指令，在槽函数中实现不断检测 */
            m_pGoodsInfo->m_pCardReleaser->searchRFCard();
        }
        break;
    default:
        break;
    }
    return true;
}

/**************************************************************************
 * Function: startOddChange
 * Description: 开始找零
 * Input: 找零金额
 * Output: 无
 * Return: 成功返回true,失败返回false
 *************************************************************************/
bool CGoodsManager::startOddChange(uint change)
{

    m_pGoodsInfo->m_pCoinPayout->PayoutCoins(change);
    return true;
}

/**************************************************************************
 * Function: stopToPay
 * Description: 结束支付。主要停止相关设备工作。
 * Input: 找零金额
 * Output: 无
 * Return: 成功返回true,失败返回false
 *************************************************************************/
bool CGoodsManager::stopToPay()
{
    switch (m_pGoodsInfo->m_struPayInfo.payStyle) {
    case TYPE_PAY_STYLE_COIN:
        m_pGoodsInfo->m_pCoinAcceptor->SetEnabled(false);
        this->disconnect(m_pGoodsInfo->m_pCoinAcceptor);
        //        m_pGoodsInfo->m_pCoinAcceptor->close();

        //        m_pGoodsInfo->m_pCoinPayout->SetEnabled(false);
        //        this->disconnect(m_pGoodsInfo->m_pCoinPayout);
        //        m_pGoodsInfo->m_pCoinPayout->close();

        //        m_pGoodsInfo->m_pNoteAcceptor->SetEnabled(false);
        this->disconnect(m_pGoodsInfo->m_pNoteAcceptor);
        m_pGoodsInfo->m_pNoteAcceptor->EndAccept();
        break;
    case TYPE_PAY_STYLE_CARD:
        if (!PAY_WITH_CARD_READER) {
            m_pGoodsInfo->m_pCardReleaser->popCard();
        }
        break;
    default:
        break;
    }
    return true;
}

/**************************************************************************
 * Function: refreshSoldGoodsInfoToDB
 * Description: 刷新本地已售商品信息
 * Input: 找零金额
 * Output: 无
 * Return: 成功返回true,失败返回false
 *************************************************************************/
bool CGoodsManager::refreshSoldGoodsInfoToDB()
{
    return m_pGoodsInfo->refreshSoldGoodsInfoToDB();
}

/**************************************************************************
 * Function: getTotalToPay
 * Description: 获取本次购买所应支付金额
 * Input: 找零金额
 * Output: 无
 * Return: 本次购买所应支付金额
 *************************************************************************/
QString CGoodsManager::getTotalToPay()
{
    int total = 0;
    total = m_pGoodsInfo->getTotalToPay();
    return QString("").setNum(total);
}

/**************************************************************************
 * Function: setPayResult
 * Description: 设置支付结果
 * Input: 支付成功与否
 * Output: 无
 * Return: 成功返回true,失败返回false
 *************************************************************************/
bool CGoodsManager::setPayResult(bool bIsSuccess)
{
    return true;
}

bool CGoodsManager::readValidMoneyOfCard()
{
    if (PAY_WITH_CARD_READER) {
        m_pGoodsInfo->m_pCardReadr->setSector(1);
        m_pGoodsInfo->m_pCardReadr->verifyKey();
        m_pGoodsInfo->m_pCardReadr->readBlock(0);
    } else {
        m_pGoodsInfo->m_pCardReleaser->setSector(1);
        m_pGoodsInfo->m_pCardReleaser->resetRFCard();
        m_pGoodsInfo->m_pCardReleaser->verifyKey();
        m_pGoodsInfo->m_pCardReleaser->readBlock(0, NULL);
    }
    return true;
}

bool CGoodsManager::writeValidMoneyOfCard(double money)
{
    if (money < 0) {
        qWarning() << "Invalid money data!";
        return false;
    }
    qDebug() << "Prepare to write to card money:" << money;
    QString strMoney = QString("%1").arg(money, 0, 'f', 2);
    int integral = strMoney.section('.', -2, -2).toInt();
    int fractional = strMoney.section('.', -1, -1).toInt();
    QByteArray byteArray = QByteArray("").setNum(integral, 16).rightJustified(8, '0')
            + QByteArray("").setNum(fractional, 16).rightJustified(8, '0');

    qDebug() << "Prepare to write to card data:" << byteArray;
    if (PAY_WITH_CARD_READER) {
        m_pGoodsInfo->m_pCardReadr->setSector(1);
        m_pGoodsInfo->m_pCardReadr->verifyKey();
        m_pGoodsInfo->m_pCardReadr->writeBlock(0, (const uchar*)(byteArray.data()));
    } else {
        m_pGoodsInfo->m_pCardReleaser->setSector(1);
        m_pGoodsInfo->m_pCardReleaser->resetRFCard();
        m_pGoodsInfo->m_pCardReleaser->verifyKey();
        m_pGoodsInfo->m_pCardReleaser->writeBlock(0, (const uchar*)(byteArray.data()));
    }

    return true;
}

void CGoodsManager::checkCardInStatus()
{
    m_pGoodsInfo->m_pCardReleaser->searchRFCard();
}

/**************************************************************************
 * Function: slotHandleCoinIn
 * Description: 投币器设备发过来的新投币接收信息处理槽函数。接收到1个硬币投进，则响应一次
 * Input: 无
 * Output: 无
 * Return: 无
 *************************************************************************/
void CGoodsManager::slotHandleCoinIn()
{
    qDebug() << "Emit one coin in signal ...";

    m_pGoodsInfo->m_struPayInfo.inputMoney += 1;
    emit sigGetMoney(1);
    if (m_pGoodsInfo->m_struPayInfo.inputMoney
            >= m_pGoodsInfo->m_struPayInfo.totalToPay) {
        /* 当总计接收到的钱币满足支付时，停止相关设备的工作 */
        m_pGoodsInfo->m_pCoinAcceptor->SetEnabled(false);
        m_pGoodsInfo->m_pNoteAcceptor->EndAccept();
    }
}

/**************************************************************************
 * Function: slotHandleNoteIn
 * Description: 纸币器设备发过来的新投币接收信息处理槽函数
 * Input: inputMoney: 本次接收到的纸币面额
 * Output: 无
 * Return: 无
 *************************************************************************/
void CGoodsManager::slotHandleNoteIn(unsigned int inputMoney)
{
    if (m_pGoodsInfo) {
        m_pGoodsInfo->m_struPayInfo.inputMoney += inputMoney;
        emit sigGetMoney(inputMoney);
        if (m_pGoodsInfo->m_struPayInfo.inputMoney
                >= m_pGoodsInfo->m_struPayInfo.totalToPay) {
            /* 当总计接收到的钱币满足支付时，停止相关设备的工作 */
            m_pGoodsInfo->m_pCoinAcceptor->SetEnabled(false);
            m_pGoodsInfo->m_pNoteAcceptor->EndAccept();
        }
    }
}

void CGoodsManager::slotHandleCardIn(bool bIsIn)
{
    qDebug() << "Card in: " << bIsIn;
    if (!PAY_WITH_CARD_READER) {
        if (bIsIn) {
            /* 当检测到卡插入之后，先复位，然后读取卡中余额数据 */
            m_pGoodsInfo->m_pCardReleaser->resetRFCard();
            readValidMoneyOfCard();
        } else {
            /* 检测到卡没有插入时，循环进行判断与检测；每次检测都能得到结果反馈。 */
            m_pGoodsInfo->m_pCardReleaser->searchRFCard();
        }
    }
    emit sigCardIn(bIsIn);
}

void CGoodsManager::slotHandleCardMoneyData(uchar *data)
{
    QByteArray byteArray;
    byteArray.append((const char*)data);
    qDebug() << "Read card acount data:" << byteArray;
    QByteArray byteArrayIntegral = byteArray.mid(0, 8);
    QByteArray byteArrayFractional = byteArray.mid(8);
    bool ok = true;
    qDebug() << "Integral: " << byteArrayIntegral;
    qDebug() << "Integral: " << byteArrayFractional;
    QString strTemp = QString("%1").arg(byteArrayFractional.toInt(&ok, 16));
    QString strMoney = QString("%1.%2").arg(byteArrayIntegral.toInt(&ok, 16)).arg(strTemp, 2, '0');
    double money = strMoney.toDouble();
    qDebug() << "Card acount: ￥" << money;
    emit sigGetCardMoney(money);
}

