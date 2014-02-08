import QtQuick 2.2
import QtQuick.Dialogs 1.1
import Logic 1.0

GridView {
	id: listview

	// Enforce a 1x1 ListView layout
	cellWidth: width
	cellHeight: height

	property int animationTime: 750

	height: isOpen ? parent.height : 0
	Behavior on height {
		NumberAnimation { duration: listview.animationTime; easing.type: Easing.InOutCubic }
	}

	visible: height > 1
	property bool isOpen: false
	property bool isSliding: false

	function open()
	{
		isOpen = true;
		isSliding = true;
		slideTimer.start();
	}
	function close()
	{
		currentIndex = 0;
		isOpen = false;
		isSliding = true;
		slideTimer.start();
	}

	model: null
	delegate: TodaysImagesListDelegate { }

	highlightRangeMode: GridView.StrictlyEnforceRange
	layoutDirection: Qt.LeftToRight
	snapMode: GridView.SnapToRow

	// Transitions
	property int transitionTime: 500
	displaced: Transition {
		NumberAnimation { properties: "x, y"; duration: transitionTime }
	}

	remove: Transition {
		ParallelAnimation {
			NumberAnimation { property: "opacity"; to: 0; duration: transitionTime }
			NumberAnimation { properties: "width, height"; to: 0; duration: transitionTime }
		}
	}

	Timer {
		id: slideTimer
		repeat: false
		interval: listview.animationTime
		triggeredOnStart: false

		onTriggered: isSliding = false;
	}
}
