import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

// Основной виджет
// пробрасываются модели и добавляются функции обработчики событий
Window {
    width: 1200
    height: 480
    visible: true
    title: qsTr("Clients table")
    minimumWidth: 1200
    minimumHeight: 480

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
        text: "Журнал"
        highlighted: true
        onClicked: {
            logsModel.showServerLogs()
            logs.visible = true
            tableClients.visible = false
        }
    }

}
