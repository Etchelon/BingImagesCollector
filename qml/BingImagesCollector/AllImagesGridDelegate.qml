import QtQuick 2.2
import QtQuick.Layouts 1.1
import Logic 1.0

Flipable {
	id: griddelegate
	width: GridView.view.cellWidth
	height: GridView.view.cellHeight

	z: fullScreenTransition.running ? 100 : 0

	property bool flipped: false

	front: Item {
		anchors.fill: parent

		Image {
			id: image
			width: parent.width
			height: parent.height

			scale: status == Image.Ready ? 1 : 0
			Behavior on scale {
				NumberAnimation { duration: griddelegate.GridView.view.animationTime; easing.type: Easing.InOutQuad }
			}

			fillMode: Image.PreserveAspectFit

			asynchronous: true

			source: "file:///" + path

			GridViewIcon {
				id: fullScreenButton
				anchors {
					right: image.right
					rightMargin: 10 + (image.width - image.paintedWidth) / 2
					top: image.top
					topMargin: 10 + (image.height - image.paintedHeight) / 2
				}

				scale: 0.9
				source: image.state == "fullScreen" ? "exitfullscreen-icon.png" : "enterfullscreen-icon.png"
				rotation: 90
				visible: griddelegate.GridView.view.still

				clickable.onClicked: image.state = image.state == "" ? "fullScreen" : "";
			}

			states: [
				State {
					name: "fullScreen"
					ParentChange {
						target: image
						parent: views
						width: parent.width
						height: parent.height
						x: 0
						y: 0
					}

					PropertyChanges {
						target: views
						fullScreenMode: true && !fullScreenTransition.running
					}
				}
			]

			transitions: [
				Transition {
					id: fullScreenTransition
					ParentAnimation {
						NumberAnimation {
							properties: "x, y, width, height"
							duration: griddelegate.GridView.view.transitionTime
							easing.type: Easing.InQuad
						}
					}
				}
			]

			Row {
				id: buttons
				anchors {
					left: parent.left
					leftMargin: 10
					bottom: parent.bottom
					bottomMargin: 10
				}

				spacing: 5
				visible: griddelegate.GridView.view.still && !fullScreenTransition.running && image.state == ""

				GridViewIcon {
					scale: favorite ? 1.0 : 0.8
					Behavior on scale {
						NumberAnimation { duration: 500; easing.type: Easing.InOutBack }
					}

					opacity: image.status == Image.Ready && (clickable.containsMouse || favorite) ? 1 : 0.2
					source: favorite ? "favorite2-icon.png" : "favorite2-icon.png"

					clickable.onClicked: favorite = !favorite;
				}

				GridViewIcon {
					scale: 0.9
					source: "info-icon.png"
					clickable.onClicked: griddelegate.flipped = true;
				}

				GridViewIcon {
					source: "desktop-icon.png"
					clickable.onClicked: griddelegate.GridView.view.model.get(index).set_as_wallpaper();
				}

				ToolBarSeparator { }

				GridViewIcon {
					source: "trash-icon.png"
					clickable.onClicked: griddelegate.GridView.view.confirm_deletion(index);
				}
			}
		}
	}

	back: Rectangle {
		id: info
		anchors.fill: parent
		color: "black"

		Text {
			anchors {
				top: parent.top
				topMargin: 10
				left: parent.left
				leftMargin: 10
				right: parent.right
				rightMargin: 10
			}

			text: copyright
			wrapMode: Text.WrapAtWordBoundaryOrAnywhere
			font.pixelSize: 24
			renderType: Text.NativeRendering
			color: "white"
		}

		GridViewIcon {
			anchors {
				bottom: parent.bottom
				bottomMargin: 10
				left: parent.left
				leftMargin: 10
			}

			source: "undo-icon.png"
			opacity: 1
			clickable.onClicked: griddelegate.flipped = false;
			clickable.preventStealing: true
		}
	}

	//	// Manual highlight
	//	Item {
	//		anchors.fill: parent

	//		opacity: 0.5
	//		visible: griddelegate.GridView.isCurrentItem && image.ready

	//		Rectangle {
	//			anchors.left: parent.left
	//			width: 5
	//			height: parent.height
	//			color: "yellow"
	//		}
	//		Rectangle {
	//			anchors.top: parent.top
	//			width: parent.width
	//			height: 5
	//			color: "yellow"
	//		}
	//		Rectangle {
	//			anchors.right: parent.right
	//			width: 5
	//			height: parent.height
	//			color: "yellow"
	//		}
	//		Rectangle {
	//			anchors.bottom: parent.bottom
	//			width: parent.width
	//			height: 5
	//			color: "yellow"
	//		}
	//	}

	// Vertical flipping
	transform: Rotation {
		id: rotation

		origin.x: griddelegate.width / 2
		origin.y: griddelegate.height / 2
		axis.x: 1; axis.y: 0; axis.z: 0

		angle: 0
		Behavior on angle {
			NumberAnimation { duration: griddelegate.GridView.view.animationTime; easing.type: Easing.InOutBack }
		}
	}

	states: State {
		name: "back"
		when: griddelegate.flipped

		PropertyChanges { target: rotation; angle: 180 }
	}
}
