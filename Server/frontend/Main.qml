import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Window {
    width: 1200
    height: 480
    visible: true
    title: qsTr("Clients table")
    // visibility: Window.FullScreen  // <-- Окно на весь экран

    Logs {
        id: logs
        models: logsModel
        anchors.fill: parent
        visible: false

        onClickClose: function () {
            logs.visible = false
            tableClients.visible = true
        }

        onClickShowServerLogs: function () {
            logsModel.showServerLogs()
        }


    }

    TableClients {
        id: tableClients
        models: clientsModel
        anchors.fill: parent
        onClickRow : function (idx) {
            logsModel.setLog(clientsModel.getLog(idx))
            logs.visible = true
            tableClients.visible = false
        }
    }

    Button {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 16
        visible: !logs.visible
        text: qsTr("Журнал")
        highlighted: true
        onClicked: {
            logsModel.showServerLogs()
            logs.visible = true
            tableClients.visible = false
        }
    }

}
