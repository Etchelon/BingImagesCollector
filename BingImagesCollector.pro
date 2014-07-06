TEMPLATE = app

QT += qml quick widgets

QMAKE_CXXFLAGS = -std=c++11

SOURCES += main.cpp \
	BingImage.cpp \
	BingImagesModel.cpp \
	ImageLoader.cpp \
	SettingsHandler.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

OTHER_FILES += \
	IconFile.rc \
	BingImagesCollector.ico \
	qml/BingImagesCollector/AllImagesGrid.qml \
	qml/BingImagesCollector/AllImagesGridDelegate.qml \
	qml/BingImagesCollector/AllImagesGridToolBar.qml \
	qml/BingImagesCollector/GridViewIcon.qml \
	qml/BingImagesCollector/ImageSlider.qml \
	qml/BingImagesCollector/main.qml \
	qml/BingImagesCollector/SettingsWindow.qml \
	qml/BingImagesCollector/SlidingImage.qml \
	qml/BingImagesCollector/TodaysImagesList.qml \
	qml/BingImagesCollector/TodaysImagesListDelegate.qml \
	qml/BingImagesCollector/TodaysImagesListToolBar.qml \
	qml/BingImagesCollector/ToolBarSeparator.qml \
	qml/BingImagesCollector/image_not_available.jpg \
	qml/BingImagesCollector/add-icon.png \
	qml/BingImagesCollector/arrowdown-icon.png \
	qml/BingImagesCollector/arrowleft-icon.png \
	qml/BingImagesCollector/arrowright-icon.png \
	qml/BingImagesCollector/arrowup-icon.png \
	qml/BingImagesCollector/Background.png \
	qml/BingImagesCollector/bing_new_logo.png \
	qml/BingImagesCollector/bing_new_logo_opaque.png \
	qml/BingImagesCollector/bookmark-icon.png \
	qml/BingImagesCollector/calendar-icon.png \
	qml/BingImagesCollector/check-icon.png \
	qml/BingImagesCollector/delete-icon.png \
	qml/BingImagesCollector/desktop-icon.png \
	qml/BingImagesCollector/enterfullscreen-icon.png \
	qml/BingImagesCollector/exitfullscreen-icon.png \
	qml/BingImagesCollector/exit-icon.png \
	qml/BingImagesCollector/favorite2-icon.png \
	qml/BingImagesCollector/favorite-icon.png \
	qml/BingImagesCollector/grid-2by2.png \
	qml/BingImagesCollector/grid-3by3.png \
	qml/BingImagesCollector/help-icon.png \
	qml/BingImagesCollector/info-icon.png \
	qml/BingImagesCollector/non-favorite-icon.png \
	qml/BingImagesCollector/open-icon.png \
	qml/BingImagesCollector/redo-icon.png \
	qml/BingImagesCollector/refresh-icon.png \
	qml/BingImagesCollector/save-icon.png \
	qml/BingImagesCollector/search-icon.png \
	qml/BingImagesCollector/settings-icon.png \
	qml/BingImagesCollector/trash-icon.png \
	qml/BingImagesCollector/undo-icon.png \
	qml/BingImagesCollector/world-icon.png

HEADERS += \
	BingImage.hpp \
	BingImagesModel.hpp \
	ImageLoader.hpp \
	SettingsHandler.hpp

RC_FILE = IconFile.rc
