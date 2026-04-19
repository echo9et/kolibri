import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

// Таблица для отображеняи данных клиентов
Item {
    id: root
    anchors.fill: parent
    property var models: null
    property var onClickRow : null

    HorizontalHeaderView {
        id: horizontalHeader
        anchors.left: tableViewId.left
        anchors.right: tableViewId.right
        anchors.top: parent.top
        syncView: tableViewId
        clip: true
        onWidthChanged: tableViewId.forceLayout()
    }

    TableView {
        id: tableViewId
        anchors.left: parent.left
        anchors.top: horizontalHeader.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        columnSpacing: 1
        rowSpacing: 1
        clip: true

        model: root.models

        columnWidthProvider: function(column) { return root.width / models.columnCount() }

        delegate: Item {
            implicitHeight: 40

            Label {
                anchors.centerIn: parent
                text: model.tabledata
                visible: column !== 1
                padding: 12
                wrapMode: Text.Wrap
                elide: Text.ElideRight
            }

            Rectangle {
                visible: column === 1
                anchors.centerIn: parent
                width: 16
                height: 16
                radius: 8
                color: model.tabledata ? "green" : "red"
            }

            Rectangle {
                anchors.fill: parent
                color: "lightgray"
                z: -1
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    onClickRow(root.models.index(row,0))
                }
            }
        }
    }
}
