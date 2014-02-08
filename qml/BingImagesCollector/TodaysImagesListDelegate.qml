import QtQuick 2.2
import Logic 1.0

Rectangle {
	id: listdelegate
	width: GridView.view.cellWidth
	height: GridView.view.cellHeight

	color: "black"

	Image {
		id: img

		anchors.fill: parent
		source: "file:///" + path
	}

	Rectangle {
		id: caption

		anchors.bottom: parent.bottom
		width: parent.width
		height: parent.height / 10

		color: "black"

		Text {
			anchors {
				left: parent.left
				leftMargin: 20
				verticalCenter: parent.verticalCenter
			}

			text: copyright
			font.pixelSize: 20
			wrapMode: Text.Wrap
			color: "white"
		}
	}
}
