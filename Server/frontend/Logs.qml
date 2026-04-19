import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: root
    property var models: null
    property var onClickClose : null
    property var onClickShowServerLogs : null
    Item {
        id: areaShowLogs
        anchors.top: parent.top
        anchors.bottom: footer.top
        anchors.left: parent.left
        anchors.right: parent.right

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

    Item {
        id: footer
        height: 25
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        Button {
            text: "Close"
            anchors.right: parent.right
            highlighted: true
            onClicked: {
                onClickClose()
            }
        }

        Button {
            text: "Показать логи сервера"
            anchors.left: parent.left
            highlighted: true
            onClicked: {
                onClickShowServerLogs()
            }
        }
    }

}
