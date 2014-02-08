import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Dialogs 1.1
import QtQuick.Window 2.1
import QtQuick.Layouts 1.1

import Logic 1.0

ToolBar {
	id: toolbar

	function shift_currentIndex(index)
	{
		var count = listview.model.count

		// If the last image is removed, move the index backward; otherwise, the image stays the same and the next image falls backward
		if (index == count - 1)
			listview.currentIndex = index - 1;
		// Force refresh of marketLabel's text with a dirty trick
		else
		{
			listview.currentIndex++;
			listview.currentIndex--;
		}
	}

	Action {
		id: keepImageAction

		iconSource: "check-icon.png"
		iconName: "keep-image"
		text: qsTr("&Add to collection")
		tooltip: qsTr("Add this image to your collection")
		onTriggered:
		{
			var index = listview.currentIndex;
			shift_currentIndex(index);
			imageLoader.keep_image(index);
		}
	}

	Action {
		id: discardImageAction

		iconSource: "delete-icon.png"
		iconName: "close-image"
		text: qsTr("&Discard image")
		tooltip: qsTr("Discard the image")
		onTriggered:
		{
			var index = listview.currentIndex;
			shift_currentIndex(index);
			imageLoader.todaysImages.remove(index, true);
		}
	}

	Action {
		id: setAsWallpaperAction

		iconSource: "desktop-icon.png"
		iconName: "set-wallpaper"
		text: qsTr("&Set as desktop wallpaper")
		tooltip: qsTr("Set as desktop wallpaper")
		onTriggered:
		{
			var index = listview.currentIndex;
			var image = imageLoader.todaysImages.get(index);
			imageLoader.set_as_wallpaper(image);
		}
	}

	RowLayout {
		id: leftRowLayout
		anchors.left: parent.left

		ToolButton {
			id: keepImageButton
			height: parent.height

			action: keepImageAction
		}

		ToolButton {
			id: discardImageButton
			height: parent.height

			action: discardImageAction
		}

		ToolButton {
			id: setWallpaperButton
			height: parent.height

			action: setAsWallpaperAction
		}
	}

	RowLayout {
		id: middleRowLayout
		anchors.centerIn: parent

		ToolButton {
			anchors.right: marketLabel.left
			height: parent.height

			rotation: 90
			iconSource: "arrowleft-icon.png"
			iconName: "Previous market"
			tooltip: "View the image from the previous market"
			onClicked: listview.moveCurrentIndexUp();
		}
		Item {
			id: marketLabel
			width: 75
			height: parent.height
			anchors.horizontalCenter: parent.horizontalCenter

			Label {
				id: marketNameLabel
				anchors.centerIn: parent

				Binding {
					target: marketNameLabel
					property: "text"
					value: listview.model.get(listview.currentIndex).market
					when: listview.visible
				}
			}
		}
		ToolButton {
			anchors.left: marketLabel.right
			height: parent.height

			rotation: 90
			iconSource: "arrowright-icon.png"
			iconName: "Next market"
			tooltip: "View the image from the next market"
			onClicked: listview.moveCurrentIndexDown();
		}
	}

	RowLayout {
		id: rightRowLayout
		anchors.right: parent.right

		ToolButton {
			id: openSettingsButton
			height: parent.height

			action: openSettingsAction
		}

		ToolBarSeparator { }

		ToolButton {
			id: closeListButton
			height: parent.height

			iconSource: "arrowdown-icon.png"
			iconName: "close-list"
			tooltip: "Close this list and go back to the grid"
			onClicked: listview.close();
		}
	}
}
