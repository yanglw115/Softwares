import QtQuick 2.4
import QtQuick.Controls 1.2
import qt.YG 1.0

/* 整个商品购买区域 */
Rectangle {
    id: idGoodsList
    width: 1024
    height: 768
    visible: true
    color: sysPalette.window
    /* 商品列表中商品所在位置索引，与货贺上的位置索引保持一致 */
    property int indexGoods

    /* 这里的参数类型是QtObject，来自Component QML Type中的例子，之前一直没有找到好的参数类型 */
    signal sigGoToPlay();
    signal sigStartToPay();

    /* 进入商品支付界面时销毁商品列表对象，该对象在main窗口中动态创建 */
    function f_destroyGoodsList()
    {
        idGoods.destroy()
    }

    function f_addGoodsToShoppingCart(count) /* 参数count是选购数量 */
    {
        console.log("indexGoods:", indexGoods)
        for (var i = 0; i < myShoppingCart.myModel.count; ++i) {
            console.log("inde:", myShoppingCart.myModel.get(i).index)
            if (myShoppingCart.myModel.get(i).index === indexGoods) {
                /* 原本这里model的值发生变化后，delegate会随之变化，可调试过程中发现有时并不会改变 */
//                myShoppingCart.myModel.get(i).selectCount = count
                return
            }
        }

        myShoppingCart.myModel.append({"name":idGridView.model.get(indexGoods).name,
                                      "image":idGridView.model.get(indexGoods).image,
                                      "price":idGridView.model.get(indexGoods).price,
                                      "selectCount":count,
                                      "leftCount":idGridView.model.get(indexGoods).leftCount,
                                      "index":indexGoods})
    }

    /* 销毁单个商品的详情对象，该详情页也是动态创建 */
    function f_destroyGoodsDetail(obj)
    {
        idGridView.opacity = 1
        idGridView.enabled = true
        obj.destroy()
    }

    /* 动态创建单个商品详情对象 */
    function f_createGoodsDetail(index)
    {
        /* 动态加载商品详细信息 */
        var component = Qt.createComponent("GoodsDetail.qml")
        if (component.status === Component.Error) {
            console.log("Error: ", component.errorString())
            return
        }
        /* 创建对象时，父object需要指定正确，因为后面会设置GridView的opacity，这里创建的object
            的父对象不能是GridView，否则GridView的opacity也将影响object的opacity */
        var goodsDetail = component.createObject(idGoodsList);

        if (goodsDetail === null) {
            console.log("Create goodsDetail object failed!")
            return
        }

        indexGoods = index

        goodsDetail.width = idGridView.width * 0.9
        goodsDetail.height = idGridView.height * 0.8
        goodsDetail.z = 2

        /* 注意这里ListModel子元素的定位方法 */
        goodsDetail.imageSource = idGridView.model.get(index).image
        goodsDetail.price = idGridView.model.get(index).price
        goodsDetail.spinMax = parseInt(idGridView.model.get(index).leftCount) + 1
        goodsDetail.leftCount = idGridView.model.get(index).leftCount

        goodsDetail.anchors.verticalCenter = idGridView.verticalCenter
        goodsDetail.anchors.horizontalCenter = idGridView.horizontalCenter
        goodsDetail.sigDestroyDetail.connect(f_destroyGoodsDetail)
        goodsDetail.sigAddToCart.connect(f_addGoodsToShoppingCart)

        /* 商品列表透明度减少，以明显单个商品的信息展示 */
        idGridView.opacity = 0.2
        idGridView.enabled = false
    }

    function f_startToPayInGoodsList()
    {
        idGoodsList.visible = false
        /* 将信号发送给main窗口 */
        idGoodsList.sigStartToPay()
        idGoodsList.destroy()
    }

    SystemPalette {
        id: sysPalette
    }

    /* 商品信息处理接口，在C++中实现 */
    GoodsManager {
        id: goodsManager
    }

    /* 商品列表的展示，使用model/view，model定义在GoodsListModel.qml中 */
    GoodsListModel {
        id: idGridModel
        Component.onCompleted: {
            for (var i = 0; i < count; ++i) {
                var obj = get(i)
                if (goodsManager.bIsGoodsInfoExist(i) && (!goodsManager.bIsGoodsEmpty(i))) {
                    obj.name = goodsManager.getGoodsName(i)
                    obj.image = "file:///" + goodsManager.getGoodsPicPath(i)
                    obj.price = goodsManager.getGoodsPrice(i)
                    obj.leftCount = goodsManager.getGoodsCount(i);
                }
            }
        }
    }

    /* 通过GridView来展示在GoodsListModel中得到的数据 */
    GridView {
        id: idGridView
        width: 600
        height: parent.height
        cellWidth: 300
        cellHeight: 240

        model: idGridModel
        /* 通过SingleGoods作为展示从model中接收数据并在GridView中展示*/
        delegate: SingleGoods {
            id: idGoods
            source: model.image
            text: model.name
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                /* 这里获取单个商品在商品列表中的位置使用了一定的技巧 */
                var goodsY = idGridView.visibleArea.yPosition * idGridView.contentHeight + mouseY
                var index = idGridView.indexAt(mouseX, goodsY)
                f_createGoodsDetail(index)
            }
        }
    }




    /* 商品列表滚动条 */
    Rectangle {
        id: goodsListScrollbar
        /* 滚动条位置是动态绑定的，也有一定的技巧 */
        anchors.left: idGridView.right
        y: idGridView.visibleArea.yPosition * idGridView.height
        width: 10
        height: idGridView.visibleArea.heightRatio * idGridView.height
        color: "#1f931d"
    }

    Timer {
        interval: 500; running: true; repeat: true
        onTriggered: {
            var date = new Date()
            var weekday = new Array(7)
            weekday[0] = "天"
            weekday[1] = "一"
            weekday[2] = "二"
            weekday[3] = "三"
            weekday[4] = "四"
            weekday[5] = "五"
            weekday[6] = "六"
            textTime.text = date.toTimeString()
            textDate.year = date.getFullYear()
            textDate.month = date.getMonth() + 1
            textDate.day = date.getDate()
            textWeek.myWeek = weekday[date.getDay()]
            delete date
        }
    }

    /* 时间展示区域 */
    Rectangle {
        id: timeAream
        width: parent.width - idGridView.width
        height: 100
        color: "#2cbbf3"
        gradient: Gradient {
            GradientStop {
                position: 0.00;
                color: "#2cbbf3";
            }
            GradientStop {
                position: 1.00;
                color: "#ffffff";
            }
        }
        anchors.top: parent.top
        anchors.left: goodsListScrollbar.right

        /* 展示当前时间 */
        Text {
            id: textTime
            color: "gold"
            font.family: "WenQuanYi Micro Hei"
            font.pointSize: 34
            font.bold: true
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.verticalCenter: parent.verticalCenter
        }

        /* 展示当前日期 */
        Text {
            id: textDate
            property int year
            property int month
            property int day
            color: "black"
            font.bold: true
            font.pointSize: 16
            font.family: "WenQuanYi Micro Hei"
            anchors.left: textTime.right
            anchors.leftMargin: 20
            anchors.top: parent.top
            anchors.topMargin: 20
            text: year.toString() + "年" + month.toString() + "月" + day.toString() + "日"
        }

        /* 展示星期 */
        Text {
            id: textWeek
            default property string myWeek: "4"
            color: "#20d140"
            font.bold: true
            font.pointSize: 26
            font.family: "WenQuanYi Micro Hei"
            anchors.left: textTime.right
            anchors.leftMargin: 20
            anchors.top: textDate.bottom
            anchors.topMargin: 10

            text: "星期" + myWeek
        }
    }

    /* 购物车内容，在ShoppingCart.qml中定义 */
    ShoppingCart {
        id: myShoppingCart
        anchors.left: goodsListScrollbar.right
        anchors.right: parent.right
        anchors.top: timeAream.bottom

        Component.onCompleted: {
            myShoppingCart.sigStartToPay.connect(f_startToPayInGoodsList)
        }
    }

    /* 返回播放界面按钮 */
    Rectangle {
        id: rectReturn
        width: 100
        height: 40
        anchors.left: idGridView.right
        anchors.leftMargin: 40
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 40
        radius: 5

        gradient: Gradient {
            GradientStop { position: 0 ; color: "#00CC6C"}
            GradientStop { position: 1 ; color: "#04FDCC"}
        }

        Text {
            anchors.centerIn: parent
            text: "<font color='black'>取消并返回播放</font>"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                idGoodsList.visible = false
                idGoodsList.sigGoToPlay()
                idGoodsList.deleteLater()
            }
        }
    }
}
