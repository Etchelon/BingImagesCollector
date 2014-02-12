import QtQuick 2.2

Image {
	property alias clickable: mouseArea

	width: 35
	height: width

	antialiasing: true
	opacity: mouseArea.containsMouse ? 1 : 0.2
	Behavior on opacity {
		NumberAnimation { duration: 200 }
	}

	MouseArea {
		id: mouseArea
		anchors.fill: parent
		hoverEnabled: true
	}
}
