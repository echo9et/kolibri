import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

// Виджет отображения логов сервера или клиента
Item {
    id: root
    property var models: null
    anchors.fill: parent

    ListView {
        id: logView
        anchors.fill: parent
        model: root.models
        clip: true
        spacing: 2

        delegate: Item {
            width: ListView.view.width
            height: row.implicitHeight + 8

            RowLayout {
                id: row
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.margins: 4
                spacing: 12

                Text {
                    text: model.severity
                    font.bold: true
                    font.family: "monospace"
                    font.pixelSize: 13
                    color: {
                        switch(model.severity) {
                            case "ERROR":   return "red"
                            case "WARRING": return "orange"
                            case "INFO":    return "green"
                            default:        return "black"
                        }
                    }
                    Layout.minimumWidth: 80
                }

                Text {
                    text: model.message
                    font.family: "monospace"
                    font.pixelSize: 13
                    wrapMode: Text.Wrap
                    Layout.fillWidth: true
                }
            }

            Rectangle {
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                height: 1
                color: "#333333"
            }
        }

        onCountChanged: {
            if (logView.contentHeight > logView.height &&
                logView.visibleArea.yPosition + logView.visibleArea.heightRatio >= 0.95) {
                positionViewAtEnd()
            }
        }

    }
}
