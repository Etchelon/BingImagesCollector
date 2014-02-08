import QtQuick 2.2

Item {
	function change_image(newSource, slideDirection)
	{
		// Turn on/off images
		if (image1.active && !image2.active)
		{
			// Adjust sliding direction
			if (slideDirection == "left")
			{
				// Set destination for current image to slide out
				image1.manualX = -width;
				// Move next image to the right so it can slide left into
				image2.move_x(+width);
			}
			else if (slideDirection == "right")
			{
				// Set destination for current image to slide out
				image1.manualX = +width;
				// Move next image to the left so it can slide right into
				image2.move_x(-width);
			}

			image1.fade();
			image2.show(newSource);
		}
		else if (!image1.active && image2.active)
		{
			// Adjust sliding direction
			if (slideDirection == "left")
			{
				// Set destination for current image to slide out
				image2.manualX = -width;
				// Move next image to the right so it can slide left into
				image1.move_x(+width);
			}
			else if (slideDirection == "right")
			{
				// Set destination for current image to slide out
				image2.manualX = +width;
				// Move next image to the left so it can slide right into
				image1.move_x(-width);
			}

			image2.fade();
			image1.show(newSource);
		}
		else if (!image1.active && !image2.active)
		{
//			image1.move_y(-height);
			image1.show(newSource);
		}
	}

	property int slideDuration: 1000
	property var status: image1.active ? image1.status : image2.status
	property int fillMode: Image.PreserveAspectFit

	SlidingImage {
		id: image1

		width: parent.width
		height: parent.height
		fillMode: parent.fillMode
		slideDuration: parent.slideDuration
	}

	SlidingImage {
		id: image2

		width: parent.width
		height: parent.height
		fillMode: parent.fillMode
		slideDuration: parent.slideDuration
	}
}
