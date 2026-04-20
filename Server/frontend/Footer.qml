import QtQuick
import QtQuick.Controls

Item {
    id: root
    height: 25
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    anchors.right: parent.right

    property var onClickClose : null
    property var onClickShowServerLogs : null
    property var onClickDisconectedClient: null

    property var chooseUUID: null
    property bool isLogVisible: false
    property bool isShowServerLog: false

    Button {
        visible: !isShowServerLog
        text: "Показать логи сервера"
        anchors.left: parent.left
        highlighted: true
        onClicked: {
            isShowServerLog = true
            onClickShowServerLogs()
            chooseUUID = null
        }
    }

    Button {
        visible: chooseUUID !== null
        text: "Остановить работу клиента"
        highlighted: true
        anchors.horizontalCenter: parent.horizontalCenter
        onClicked: {
            onClickDisconectedClient()
            chooseUUID = null
        }
    }

    Button {
        visible: isLogVisible
        anchors.right: parent.right
        highlighted: true
        text: "Закрыть"
        onClicked: {
            onClickClose()
            isShowServerLog = false
        }
    }
}