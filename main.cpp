#include "qtquick2controlsapplicationviewer.h"
#include <QtQml/qqml.h>

#include "SettingsHandler.hpp"
#include "BingImage.hpp"
#include "BingImagesModel.hpp"
#include "ImageLoader.hpp"

int main(int argc, char *argv[])
{
	qmlRegisterType<SettingsHandler>("Logic", 1, 0, "SettingsHandler");
	qmlRegisterType<BingImage>("Logic", 1, 0, "BingImage");
	qmlRegisterType<BingImagesModel>("Logic", 1, 0, "BingImagesModel");
	qmlRegisterType<ImageLoader>("Logic", 1, 0, "ImageLoader");

	Application app(argc, argv);

	QtQuick2ControlsApplicationViewer viewer;
	viewer.setMainQmlFile(QStringLiteral("qml/BingImagesCollector/main.qml"));
	viewer.show();

	return app.exec();
}
