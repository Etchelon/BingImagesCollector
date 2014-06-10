import QtQuick 2.2
import QtQuick.Dialogs 1.1
import Logic 1.0

GridView {
	id: gridview

	property int animationTime: 750
	property bool still: !listview.isSliding

	cellWidth: width / settings.gridCellsPerPage
	Behavior on cellWidth {
		enabled: !listview.isSliding
		NumberAnimation { duration: gridview.animationTime; easing.type: Easing.InOutQuad }
	}
	cellHeight: (listview.isSliding ? height : width * 9 / 16) / settings.gridCellsPerPage
	Behavior on cellHeight {
		enabled: !listview.isSliding
		NumberAnimation { duration: gridview.animationTime; easing.type: Easing.InOutQuad }
	}

	model: null
	delegate: AllImagesGridDelegate { }

	cacheBuffer: 1000
	highlightRangeMode: GridView.StrictlyEnforceRange
	snapMode: GridView.SnapToRow

	// Transitions
	property int transitionTime: 500

	displaced: Transition {
		NumberAnimation { properties: "x, y"; duration: gridview.transitionTime }
	}

	remove: Transition {
		ParallelAnimation {
			NumberAnimation { property: "opacity"; to: 0; duration: gridview.transitionTime }
			NumberAnimation { properties: "width, height"; to: 0; duration: gridview.transitionTime }
		}
	}

	function confirm_deletion(i)
	{
		confirmDeletionDialog.imageToDelete = i;
		confirmDeletionDialog.open();
	}

	MessageDialog {
		id: confirmDeletionDialog

		property int imageToDelete: -1

		title: qsTr("Confirm deletion")
		informativeText: qsTr("Are you sure you want to delete this image from disk?")
		detailedText: qsTr("If you delete the image you won't be able to undo the operation and the file will be permanently lost")
		standardButtons: StandardButton.Yes | StandardButton.No
		icon: StandardIcon.Critical
		modality: Qt.WindowModal

		onYes: gridview.model.remove(imageToDelete, true);
		onRejected: close();
	}
}

