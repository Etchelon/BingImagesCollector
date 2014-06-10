import QtQuick 2.2
import Logic 1.0

GridView {
	id: listview

	property int animationTime: 750

	height: isOpen ? parent.height : 0
	Behavior on height {
		NumberAnimation {
			id: anim
			duration: listview.animationTime
			easing.type: Easing.InOutCubic
		}
	}

	// Force a 1x1 layout
	cellWidth: width
	cellHeight: height

	property bool isOpen: false
	property bool isSliding: anim.running

	function open()
	{
		isOpen = true;
	}
	function close()
	{
		isOpen = false;
	}

	model: null
	delegate: TodaysImagesListDelegate { }

	highlightRangeMode: GridView.StrictlyEnforceRange
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
}
