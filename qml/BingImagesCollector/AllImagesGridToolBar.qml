import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

import Logic 1.0

ToolBar {
	id: toolbar

	property alias progressBar: progressBar

	Action {
		id: forceDownloadAction

		iconSource: "world-icon.png"
		iconName: "force-download"
		text: qsTr("&Download images")
		tooltip: qsTr("Force a new download from the server")
		onTriggered:
		{
			listview.model = null;
			imageLoader.load_images(true);
		}
	}

	RowLayout {
		id: leftRowLayout
		anchors.left: parent.left

		ToolButton {
			id: openListButton
			height: parent.height

			iconSource: "arrowup-icon.png"
			tooltip: qsTr("Open the list of today's images")

			onClicked:
			{
				if (listview.model != null)
					listview.open();
				else
					imageLoader.load_images(true);
			}
		}

		ToolButton {
			id: downloadImagesButton
			height: parent.height

			action: forceDownloadAction
		}

		ToolBarSeparator { }

		ToolButton {
			id: grid2by2
			height: parent.height

			iconSource: settings.gridCellsPerPage == 3 ? "grid-2by2.png" : "grid-3by3.png"
			tooltip: settings.gridCellsPerPage == 3 ? qsTr("2x2 grid") : qsTr("3x3 grid")
			onClicked: settings.gridCellsPerPage = settings.gridCellsPerPage == 3 ? 2 : 3;
		}
	}

	RowLayout {
		id: rightRowLayout
		anchors.right: parent.right

		ProgressBar {
			id: progressBar

			indeterminate: false
			value: imageLoader.downloadProgress
			visible: imageLoader.downloading

			Text {
				anchors.centerIn: parent

				text: qsTr("Downloading")
				font.italic: true
			}
		}

		Item {
			id: spacing2
			width: openSettingsButton.width
		}

		ToolButton {
			id: openSettingsButton
			height: parent.height

			action: openSettingsAction
		}

		ToolBarSeparator { }

		ToolButton {
			id: quitAppButton
			height: parent.height

			action: quitAppAction
		}
	}
}
