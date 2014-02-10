import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Dialogs 1.1
import QtQuick.Window 2.1
import QtQuick.Layouts 1.1

import Logic 1.0

ApplicationWindow {
	id: main

	property alias settings: imageLoader.settings

	ImageLoader {
		id: imageLoader

		onDownloadFinished:
		{
			listview.model = imageLoader.todaysImages
			listview.open();
		}
		onProblems: messageBar.push_error_message(message);
	}

	title: qsTr("Bing Images Collector! - ") + (listview.isOpen ? qsTr("Today's images") : qsTr("Your collection"))

	width: 1280
	height: 720 + toolbars.height
	minimumWidth: 640
	minimumHeight: 480

	Action {
		id: openSettingsAction

		iconSource: "settings-icon.png"
		iconName: "open-settings"
		text: qsTr("&Preferences")
		tooltip: qsTr("Open settings")
		onTriggered: settingsWindow.show();
	}

	Action {
		id: quitAppAction

		iconSource: "exit-icon.png"
		iconName: "quit-app"
		text: qsTr("&Quit the app")
		tooltip: qsTr("Quit the app")
		onTriggered: Qt.quit(); //quitAppDialog.open();
	}

	Item {
		id: toolbars
		anchors.top: parent.top
		width: parent.width
		height: 35
		z: 2

		AllImagesGridToolBar {
			id: gridViewToolBar
			width: parent.width
			height: parent.height
			y: listview.isOpen ? -height : 0
			Behavior on y {
				NumberAnimation { duration: listview.animationTime; easing.type: Easing.InOutSine }
			}
			z: !listview.isOpen && listview.isSliding ? 2 : 1
		}

		TodaysImagesListToolBar {
			id: listViewToolBar
			width: parent.width
			height: parent.height
			y: listview.isOpen ? 0 : -height
			Behavior on y {
				NumberAnimation { duration: listview.animationTime; easing.type: Easing.InOutSine }
			}
			z: listview.isOpen && listview.isSliding ? 2 : 1
		}
	}

	// Main content
	color: "black"
	Image {
		id: background

		anchors.fill: parent
		anchors.margins: 100
		fillMode: Image.PreserveAspectFit

		source: "bing_new_logo.png"
		antialiasing: true
		opacity: 0.75
	}

	Item {
		id: views
		anchors.top: toolbars.bottom
		anchors.bottom: parent.bottom
		width: parent.width

		AllImagesGrid {
			id: gridview

			anchors.top: parent.top
			anchors.bottom: listview.top
			width: parent.width

			model: imageLoader.allImages
		}

		TodaysImagesList {
			id: listview

			anchors.bottom: parent.bottom
			width: parent.width
		}
	}

	StatusBar {
		id: statusBar

		height: messageBar.text == "" ? 0 : toolbars.height
		width: parent.width
		anchors.bottom: parent.bottom

		Label {
			id: messageBar

			property var errorMessages: []
			function push_error_message(message)
			{
				var count = errorMessages.length;

				errorMessages.push(message);
				if (count == 0)
					text = errorMessages[0];
			}

			function pop_error_message()
			{
				errorMessages.pop()
				if (errorMessages.length == 0)
					text = "";
			}

			height: implicitHeight
			anchors.verticalCenter: parent.verticalCenter
			anchors.left: parent.left
			anchors.leftMargin: 10

			text: ""

//			Behavior on height {
//				NumberAnimation { duration: 500; easing.type: Easing.InOutQuad }
//			}
		}

		Button {
			id: dismissStatusBarButton

			anchors.verticalCenter: parent.verticalCenter
			anchors.right: parent.right
			anchors.rightMargin: 10

			text: "Dismiss"
			visible: messageBar.text != ""
			onClicked: messageBar.pop_error_message();
		}

		Behavior on height {
			NumberAnimation { duration: 500; easing.type: Easing.InOutQuad }
		}
	}

	SettingsWindow { id: settingsWindow }

	MessageDialog {
		id: aboutDialog

		title: qsTr("About")
		informativeText: qsTr("The coolest app to collect Bing images!")
		detailedText: qsTr("Developed by Etchelon")
		standardButtons: StandardButton.Ok
		icon: StandardIcon.Information
		modality: Qt.WindowModal

		onAccepted: close();
	}

	MessageDialog {
		id: quitAppDialog

		title: qsTr("Quit")
		informativeText: qsTr("Are you sure you want to quit?")
		standardButtons: StandardButton.Ok | StandardButton.Cancel
		icon: StandardIcon.Warning
		modality: Qt.ApplicationModal

		onAccepted: Qt.quit();
		onRejected: close();
	}

	Component.onCompleted: imageLoader.load_images();
}
