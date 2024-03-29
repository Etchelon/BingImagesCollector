import QtQuick 2.2
import Logic 1.0

Rectangle {
	id: listdelegate
	width: GridView.view.width
	height: GridView.view.height
	visible: GridView.isCurrentItem

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
		visible: height > 1

		Text {
			anchors {
				left: parent.left
				leftMargin: 20
				verticalCenter: parent.verticalCenter
			}

			text: copyright
			font.pixelSize: 20
			renderType: Text.NativeRendering
			color: "white"

			visible: !listdelegate.GridView.view.isSliding && caption.visible
		}
	}

	Rectangle {
		width: text.implicitWidth + 30
		height: text.implicitHeight + 6
		anchors.top: parent.top
		anchors.topMargin: width * Math.sin(Math.PI * rotation/180) / 2 + 5
		anchors.right: parent.right
		anchors.rightMargin: 0
		rotation: 30

		border {
			color: "red"
			width: 2
		}
		color: "yellow"
		smooth: true
		antialiasing: true

		Text {
			id: text

			anchors.centerIn: parent
			text: qsTr("Already in collection!")
			font.pixelSize: 20
			style: Text.Sunken
			color: "red"
		}

		visible: duplicate
	}
}
