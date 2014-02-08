import QtQuick 2.2

Image {
	function fade()
	{
		active = false;
		fadeTimer.start();
	}
	function show(url)
	{
		source = url;
		active = true;
	}
	function move_x(newX)
	{
		xAnimation.duration = 0
		manualX = newX;
		xAnimation.duration = slideDuration
	}
	function move_y(newY)
	{
		yAnimation.duration = 0
		manualY = newY;
		yAnimation.duration = slideDuration
	}

	width: parent.width
	height: parent.height

	source: ""
	property bool active: false
	property real manualX: 0
	property real manualY: 0
	property int slideDuration: 1000

	x: active ? 0 : manualX
	Behavior on x {
		NumberAnimation {
			id: xAnimation

			duration: slideDuration
			easing.type: Easing.InOutQuad
		}
	}

	y: active ? 0 : manualY
	Behavior on y {
		NumberAnimation {
			id: yAnimation

			duration: slideDuration
			easing.type: Easing.InOutQuad
		}
	}

	Timer {
		id: fadeTimer

		interval: slideDuration
		repeat: false
		triggeredOnStart: false
		onTriggered: source = "";
	}
}

