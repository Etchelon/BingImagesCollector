import QtQuick 2.2
import QtQuick.Dialogs 1.1
import Logic 1.0

GridView {
	id: gridview

	cellWidth: width / settings.gridCellsPerPage
	cellHeight: height / settings.gridCellsPerPage

	property int animationTime: 500

	model: null
	delegate: AllImagesGridDelegate { }

	snapMode: GridView.SnapToRow
	visible: height > 1

	// Transitions
	displaced: Transition {
		NumberAnimation { properties: "x, y"; duration: 500 }
	}

	remove: Transition {
		ParallelAnimation {
			NumberAnimation { property: "opacity"; to: 0; duration: 500 }
			NumberAnimation { properties: "width, height"; to: 0; duration: 500 }
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

