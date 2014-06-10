#include "SettingsHandler.hpp"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QVector>

// Global paths for the application
const QString SettingsHandler::ImageExtension = ".jpg";
const QString SettingsHandler::AppFolder = QDir::homePath() + "/AppData/Local/BingImagesCollector/";
const QString SettingsHandler::TempImagesFolder = AppFolder + "temp/";
const QString SettingsHandler::FavoriteFolder = AppFolder + "favorite/";
const QString SettingsHandler::ImagesFolder = AppFolder + "images/";
const QString SettingsHandler::SettingsFile = AppFolder + "config.ini";
const QString SettingsHandler::DatabaseFile = AppFolder + "database.xml";
const QStringList SettingsHandler::Markets = { "en-WW", "en-US", "en-GB", "de-DE", "en-CA", "en-AU", "fr-FR", "ja-JP", "zh-CN" };

SettingsHandler::SettingsHandler(QObject* parent)
	: QObject(parent)
{
	QDir path{ AppFolder };

	// Check for the existence of support folders; create if they don't exist
	if (!path.exists())
		path.mkpath(".");

	if (!path.exists("temp/"))
		path.mkdir("temp/");

	if (!path.exists("images/"))
		path.mkdir("images/");

	if (!path.exists("favorite/"))
		path.mkdir("favorite/");

	// Settings file
	QFile settingsFile{ SettingsFile };

	// If the config file doesn't exist, create it with default settings
	if (!settingsFile.exists())
	{
		settingsFile.open(QIODevice::WriteOnly);

		QTextStream os{ &settingsFile };
		os << "width: 1920; height: 1080; add logo: 1; cells per page: 3; show only new images: 1; lastDownload_ mer gen 22 22:34:17 2014 GMT";

		settingsFile.close();
	}
	// Otherwise read previously saved settings
	else
	{
		// Read settings
		settingsFile.open(QIODevice::ReadOnly);

		QTextStream is{ &settingsFile };
		QStringList settings = is.readLine().split(';', QString::SkipEmptyParts);

		m_imagesWidth  = settings[0].split(":")[1].toInt();
		m_imagesHeight = settings[1].split(":")[1].toInt();
		m_embedLogo    = static_cast<bool>(settings[2].split(":")[1].toInt());
		m_gridCellsPerPage = settings[3].split(":")[1].toInt();
		m_showOnlyNewImages = static_cast<bool>(settings[4].split(":")[1].toInt());
		m_lastDownload = QDateTime::fromString(settings[5].split("_")[1]/*, "yyyy-MM-dd_HH-mm"*/);

		settingsFile.close();
	}

	// Database file
	QFile databaseFile{ DatabaseFile };

	// If the config file doesn't exist, create it with default settings
	if (!databaseFile.exists())
	{
		databaseFile.open(QIODevice::WriteOnly);

		QTextStream os{ &databaseFile };
		os.setCodec("UTF-8");
		os << R"(<?xml version="1.0" encoding="UTF-8"?>\n<Images>\n</Images>)";

		databaseFile.close();
	}
}

SettingsHandler::~SettingsHandler()
{
	// Upon destruction, make sure to save settings
	save_settings();
}

// Getters
bool SettingsHandler::embedLogo() const
{
	// 1920x1200 images already come with the bing logo embedded
	if (m_imagesHeight == 1200)
		return false;

	return m_embedLogo;
}

QString SettingsHandler::resolution() const
{
	return QString::number(m_imagesWidth) + "x" + QString::number(m_imagesHeight);
}

QDateTime SettingsHandler::lastDownload() const
{
	return m_lastDownload;
}

int SettingsHandler::gridCellsPerPage() const
{
	return m_gridCellsPerPage;
}

bool SettingsHandler::showOnlyNewImages() const
{
	return m_showOnlyNewImages;
}

// Setters
void SettingsHandler::set_embedLogo(bool b)
{
	if (m_embedLogo != b)
	{
		m_embedLogo = b;
		emit embedLogoChanged();
	}
}

void SettingsHandler::set_resolution(QString res)
{
	auto list = res.split('x');
	int w = list[0].toInt();
	int h = list[1].toInt();

	m_imagesWidth  = w;
	m_imagesHeight = h;
	if (w != m_imagesWidth || h != m_imagesHeight)
		emit resolutionChanged();
}

void SettingsHandler::set_lastDownload(QDateTime ld)
{
	if (ld.isValid() && m_lastDownload != ld )
	{
		m_lastDownload = ld;
		save_settings();
	}
}

void SettingsHandler::set_gridCellsPerPage(int c)
{
	if (m_gridCellsPerPage != c)
	{
		m_gridCellsPerPage = c;
		emit gridCellsPerPageChanged();
	}
}

void SettingsHandler::set_showOnlyNewImages(bool b)
{
	if (m_showOnlyNewImages != b)
	{
		m_showOnlyNewImages = b;
		emit showOnlyNewImagesChanged();
	}
}

// Methods
void SettingsHandler::save_settings() const
{
	QFile settingsFile{ SettingsFile };

	if (settingsFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
	{
		QTextStream os{ &settingsFile };

		os << "width: " + QString::number(m_imagesWidth) + "; ";
		os << "height: " + QString::number(m_imagesHeight) + "; ";
		os << "add logo: " + QString::number(static_cast<int>(m_embedLogo)) + "; ";
		os << "cells per page: " + QString::number(m_gridCellsPerPage) + "; ";
		os << "show only new images: " + QString::number(static_cast<int>(m_showOnlyNewImages)) + "; ";
		os << "lastDownload_ " + m_lastDownload.toString() + ";";

		settingsFile.close();
	}
}
