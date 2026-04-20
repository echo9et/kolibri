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
    }

    TableClients {
        id: tableClients
        models: clientsModel
        anchors.fill: parent
        onClickRow : function (uuid, isOnline) {
            if (isOnline) {
                footer.chooseUUID = uuid
            } else {
                footer.chooseUUID = null;
            }

            logsModel.setLog(clientsModel.getLog(uuid))
            showLog(true)
        }
    }

    Footer {
        id: footer
        chooseUUID: null
        isLogVisible: logs.visible
        onClickDisconectedClient: function() {
            appCore.stopClient(footer.chooseUUID)
            showLog(false)
        }

        onClickClose: function () {
            showLog(false)
        }

        onClickShowServerLogs: function () {
            logsModel.showServerLogs()
            showLog(true)
        }
    }

    function showLog(status) {
        logs.visible = status
        tableClients.visible = !status
    }
}
