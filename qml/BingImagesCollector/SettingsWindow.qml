import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Dialogs 1.1
import QtQuick.Window 2.1
import QtQuick.Layouts 1.1

ApplicationWindow {
	id: settingsWindow

	title: "Preferences"

	width: grid.implicitWidth + defaultSpacing * 2
	height: col.implicitHeight + defaultSpacing * 2
	property real defaultSpacing: 10

	modality: Qt.ApplicationModal

	function setupValues()
	{
		// Resolution
		if (settings.resolution == "1920x1200")
			resolutionCbox.currentIndex = 0;
		else if (settings.resolution == "1920x1080")
			resolutionCbox.currentIndex = 1;
		else
			resolutionCbox.currentIndex = 2;
	}

	ColumnLayout {
		id: col

		anchors.fill: parent
		anchors.margins: settingsWindow.defaultSpacing
		spacing: 10
		property real cellWidth: col.width / 3 - spacing

		Grid {
			id: grid

			columns: 2
			spacing: settingsWindow.defaultSpacing
			width: parent.width

			Text { text: qsTr("Resolution for downloaded images: ") }
			ComboBox {
				id: resolutionCbox
				model: ["1920x1200", "1920x1080", "1366x768"]
			}
		}

		RowLayout {
			id: row

			width: parent.width
			spacing: settingsWindow.defaultSpacing

			Item { Layout.fillWidth: true }
			Item {
				width: parent.width
				height: embedLogoCbox.implicitHeight > txt.implicitHeight ? embedLogoCbox.implicitHeight : txt.implicitHeight

				CheckBox {
					id: embedLogoCbox

					anchors.centerIn: parent

					text: qsTr("Embed Bing logo in images")
					visible: resolutionCbox.currentIndex != 0

					Binding {
						target: embedLogoCbox
						property: "checked"
						value: settings.embedLogo
					}
				}
				Text {
					id: txt

					anchors.centerIn: parent

					text: qsTr("1920x1200 images already have the bing logo embedded")
					wrapMode: Text.WordWrap
					visible: resolutionCbox.currentIndex == 0
				}
			}
			Item { Layout.fillWidth: true }
		}

		RowLayout {
			id: buttons

			width: parent.width
			spacing: settingsWindow.defaultSpacing

			Item { Layout.fillWidth: true }
			Button {
				id: saveSettingsButton

				text: qsTr("Save")
				onClicked:
				{
					settings.resolution = resolutionCbox.currentText;
					settings.save_settings();

					settingsWindow.close();
				}
			}

			Button {
				id: discardChangesButton

				text: qsTr("Cancel")
				onClicked: {
					settingsWindow.setupValues();
					settingsWindow.close();
				}
			}
			Item { Layout.fillWidth: true }
		}
	}

	Component.onCompleted: setupValues();
}
