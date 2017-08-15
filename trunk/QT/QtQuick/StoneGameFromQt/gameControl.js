.import QtQuick.LocalStorage 2.0 as Sql
.import QtQuick 2.5 as QtQuick

var widthPerStone = 40
var maxPerRow = 20
var maxPerColumn = 40
var arrayComponent = new Array(maxPerRow * maxPerColumn)
var arrayChecked = null
var sameNumber = 0
var gameDuration = 0
var bFirstStart = true

function startGame() {

    for (var i = 0; i < maxPerRow * maxPerColumn; ++i) {
        arrayComponent[i] = null
    }
    maxPerRow = Math.floor(idBackgroud.width / widthPerStone) //idRealPlayArea.width / widthPerStone
    maxPerColumn = Math.floor(idBackgroud.height / widthPerStone) //idRealPlayArea.height / widthPerStone

    console.log("maxPerRow:", maxPerRow)
    console.log("maxPerColumn", maxPerColumn)

    arrayComponent = new Array(maxPerRow * maxPerColumn)

    var component = Qt.createComponent("Stone.qml")
    if (component.status === QtQuick.Component.Ready) {
        for (var j = 0; j < maxPerColumn; ++j) {
            for (var k = 0; k < maxPerRow; ++k) {
                var object = component.createObject(idRealPlayArea)
                object.type = Math.round(Math.random() * 3)
                object.x = k * widthPerStone
                object.y = j * widthPerStone
                object.birthing = true
                arrayComponent[j * maxPerRow + k] = object
            }
        }
        if (bFirstStart) {
            bFirstStart = false
            getRecord(0, "")
            labelScore.goal = 800
            labelScore.score = 0
            dialogInputUser.visible = true
        }
    } else {
        console.log("Create component failed:", component.errorString())
    }
}

function getIndex(row, volumn)
{
    return maxPerRow * row + volumn
}

function handleClicked(x, y)
{
    var objectRow = Math.floor(y / widthPerStone)
    var objectVolumn = Math.floor(x / widthPerStone)
    removeSameObjects(objectRow, objectVolumn, -1)
    removeEmptyObjectsInterval();
    if (sameNumber > 0)
        labelScore.score += (sameNumber - 1) * (sameNumber - 1)
    sameNumber = 0
    if (labelScore.score > labelScore.playerRecord) {
        labelScore.playerRecord = labelScore.score
    }

    if (labelScore.score > labelScore.gameRecord) {
//        bBreakRecord = true
//        labelScore.gameRecord = labelScore.score
    }
    checkGameOver()
}

function removeSameObjects(row, volumn, type)
{
    if (row < 0 || volumn < 0 || row >= maxPerColumn || volumn >= maxPerRow) {
        return;
    }

    if (arrayComponent[getIndex(row, volumn)] === null) {
        arrayChecked[getIndex(row, volumn)] = 1
        return
    }

    var firstStart = false
    if (-1 === type) {
        firstStart = true
        type = arrayComponent[getIndex(row, volumn)].type
        if (arrayChecked == null) {
            arrayChecked = new Array(maxPerRow * maxPerColumn)
        }
        for (var i = 0; i < maxPerColumn * maxPerRow; ++i)
            arrayChecked[i] = 0
    }

    if (arrayChecked[getIndex(row, volumn)] === 1) {
        return
    }

    if (!firstStart) {
        if (type !== arrayComponent[getIndex(row, volumn)].type) {
            arrayChecked[getIndex(row, volumn)] = 1
            return
        }
    }

    //    if (arrayComponent[getIndex(row, volumn)] === null || arrayChecked[getIndex(row, volumn)] === 1
    //            || (!firstStart && type !== arrayComponent[getIndex(row, volumn)].type)) {
    //        console.log("Removed? or type is not the same?", row, volumn)
    //        return
    //    }

    arrayChecked[getIndex(row, volumn)] = 1

    removeSameObjects(row + 1, volumn, type)
    removeSameObjects(row - 1, volumn, type)
    removeSameObjects(row, volumn + 1, type)
    removeSameObjects(row, volumn - 1, type)

    if (firstStart && sameNumber < 1) {
        return
    }

    arrayComponent[getIndex(row, volumn)].dying = true
    arrayComponent[getIndex(row, volumn)] = null
    sameNumber++
}

function removeEmptyObjectsInterval()
{
    //to down
    for (var i = 0; i < maxPerRow; ++i) {
        var intervalH = 0
        for (var j = maxPerColumn - 1; j >= 0; --j) {
            if (arrayComponent[getIndex(j, i)] === null) {
                ++intervalH
            } else if (intervalH > 0) {
                var obj = arrayComponent[getIndex(j, i)];
                obj.y += intervalH * widthPerStone
                arrayComponent[getIndex(j + intervalH, i)] = obj
                arrayComponent[getIndex(j, i)] = null
            }
        }
    }

    //to left
    var intervalV = 0
    for (i = 0; i < maxPerRow; ++i) {
        if (arrayComponent[getIndex(maxPerColumn - 1, i)] === null) {
            ++intervalV
        } else if (intervalV > 0) {
            for (j = 0; j < maxPerColumn; ++j) {
                obj = arrayComponent[getIndex(j, i)];
                if (obj !== null) {
                    obj.x -= intervalV * widthPerStone
                    arrayComponent[getIndex(j, i - intervalV)] = obj
                    arrayComponent[getIndex(j, i)] = null
                }
            }
        }
    }
}

function checkGameOver()
{
    var clearAll = true
    for (var i = 0; i < maxPerRow; ++i) {
        if (arrayComponent[getIndex(maxPerColumn - 1, i)] !== null) {
            clearAll = false;
            break;
        }
    }

    if (clearAll) {
        labelScore.score += 500
    }
    if (!checkHaveTheSameLeft(maxPerColumn - 1, 0, -1)) {
        if (labelScore.score < labelScore.goal) {
            console.log("Game Over.")
            idDialog.visible = true
            bFirstStart = true
            if (labelScore.score >= labelScore.playerRecord) {
                saveScore(labelScore.name)
            }
        } else {
            labelScore.goal += 1300
            startGame()
        }
//        if (labelScore.score > labelScore.gameRecord) {
//            idDialog.visible = true
//        }
    }
}

function checkHaveTheSameLeft(row, column, type)
{
    if (row > maxPerColumn -1 || row < 0 || column > maxPerRow || column < 0) {
        return false
    }
    if (arrayComponent[getIndex(row, column)] === null) {
        return false
    }

    if (arrayComponent[getIndex(row, column)].type === type) {
        return true
    }

    type = arrayComponent[getIndex(row, column)].type

    return checkHaveTheSameLeft(row - 1, column, type) || checkHaveTheSameLeft(row, column + 1, type)
}

function saveScore(name)
{
    var timeNow = new Date()
    console.log("timeBefore:", gameDuration)
    console.log("timeNow:", timeNow)
    gameDuration = timeNow - gameDuration // miniseconds
    console.log("gameDuration:", gameDuration)
    var db = Sql.LocalStorage.openDatabaseSync("gameDatabase", "1.0", "High Score Record" , 5000)
    var strInsert = "INSERT INTO Scores VALUES(?, ?, ?, ?)"
    var strInsertData = [name, labelScore.score, maxPerRow + "x" + maxPerColumn, gameDuration]
    db.transaction(function(tx){
        tx.executeSql('CREATE TABLE IF NOT EXISTS Scores(name TXT, score NUMBER, gridSize TXT, gameTime NUMBER)')
        tx.executeSql(strInsert, strInsertData)
        var data = tx.executeSql('SELECT * FROM Scores')
        var pr = "";
        for (var i = 0; i < data.rows.length; ++i) {
            pr += data.rows.item(i).name + ' got ' + data.rows.item(i).score + ' in ' +
                    data.rows.item(i).gridSize + ' with ' + data.rows.item(i).gameTime + '\n'
        }
        console.log("Result:", pr)
    })
}

function clearDatabase()
{
    var db = Sql.LocalStorage.openDatabaseSync("gameDatabase", "1.0", "High Score Record", 5000)
    db.transaction(function(tx){
        tx.executeSql('CREATE TABLE IF NOT EXISTS Scores(name TXT, score NUMBER, gridSize TXT, gameTime NUMBER)')
        var data = tx.executeSql('SELECT * FROM Scores')
        for (var i = 0; i < data.rows.length; ++i) {
            var strExec = "DELETE FROM Scores WHERE name='" + data.rows.item(i).name + "'";
            tx.executeSql(strExec)
        }
    })
}

function dropScoreTable()
{
    var db = Sql.LocalStorage.openDatabaseSync("gameDatabase", "1.0", "High Score Record", 5000)
    db.transaction(function(tx){
        tx.executeSql('DROP TABLE IF EXISTS Scores')
    })
}

function getAllUsersIndatabase()
{
    var list = null
    var db = Sql.LocalStorage.openDatabaseSync("gameDatabase", "1.0", "High Score Record", 5000)
    db.transaction(function(tx){
        tx.executeSql('CREATE TABLE IF NOT EXISTS Scores(name TXT, score NUMBER, gridSize TXT, gameTime NUMBER)')
        var data = tx.executeSql('SELECT * FROM Scores')
        list = new Array(data.rows.length)
        if (data.rows.length > 0) {
            for (var j = 0; j < data.rows.length; ++j) {
//                idComboBox.modelList[j] = data.rows.item(j).name
//                idComboBox.idModel.append({"name": data.rows.item(j).name})
//                console.log("", idComboBox.modelList[j])
                list[j] = data.rows.item(j).name
            }
        } else {
            console.log("Database records is empty.")
        }
    })
    return list
}

function getRecord(type, name)
{
    var db = Sql.LocalStorage.openDatabaseSync("gameDatabase", "1.0", "High Score Record", 5000)
    db.transaction(function(tx){
        tx.executeSql('CREATE TABLE IF NOT EXISTS Scores(name TXT, score NUMBER, gridSize TXT, gameTime NUMBER)')
//        var data = tx.executeSql('SELECT * FROM Scores')
        var data

        if (0 === type) { // game record
            data = tx.executeSql('SELECT name, score FROM Scores ORDER BY score DESC');
            if (data.rows.length > 0) {
                console.log("Get game record:", data.rows.item(0).name, data.rows.item(0).score)
                labelScore.recordName = data.rows.item(0).name
                labelScore.gameRecord = data.rows.item(0).score
            } else {
                console.log("Cann't find game record!")
            }
        } else {
            var strExec = "SELECT name, score FROM Scores WHERE name='" + name + "' ORDER BY score DESC"
            data = tx.executeSql(strExec);
            if (data.rows.length > 0) {
                console.log("Get user record:", data.rows.item(0).name, data.rows.item(0).score)
//                labelScore.name = data.rows.item(0).name
                labelScore.playerRecord = data.rows.item(0).score
            } else {
            console.log("Cann't find user record!")}
        }
    })
}
