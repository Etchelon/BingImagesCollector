#ifndef IMAGELOADER_HPP
#define IMAGELOADER_HPP

#include <QNetworkAccessManager>
#include <QObject>
#include <QQmlListProperty>
#include "SettingsHandler.hpp"
#include "BingImage.hpp"
#include "BingImagesModel.hpp"

class ImageLoader : public QObject
{
	// QObject related metadata
	Q_OBJECT
	Q_PROPERTY(SettingsHandler* settings READ settings CONSTANT)
	Q_PROPERTY(BingImagesModel* todaysImages READ todaysImages CONSTANT)
	Q_PROPERTY(BingImagesModel* allImages READ allImages CONSTANT)
	Q_PROPERTY(bool downloading READ downloading NOTIFY downloadingChanged)
	Q_PROPERTY(double downloadProgress READ downloadProgress NOTIFY downloadProgressChanged)

	// Members
	const QString m_requestUrl = "http://www.bing.com/HPImageArchive.aspx?format=xml&idx=0&n=1&mkt=";
	const QString m_bingLogo = "qml/BingImagesCollector/bing_new_logo.png";
	QNetworkAccessManager m_xml_manager;
	QNetworkAccessManager m_image_manager;
	QStringList m_hashCodes;

	SettingsHandler* m_settings = new SettingsHandler{ this };
	BingImagesModel* m_todaysImages = new BingImagesModel{ this };
	BingImagesModel* m_allImages = new BingImagesModel{ this };
	bool m_downloading = false;
	int m_downloadProgress = 0;

public:
	// Constructors
	explicit ImageLoader(QObject* parent = nullptr);
	~ImageLoader();

	// Getters
	SettingsHandler* settings() const;
	BingImagesModel* todaysImages() const;
	BingImagesModel* allImages() const;
	bool downloading() const;
	double downloadProgress() const;

Q_SIGNALS:
	void problems(QString message);
	void downloadFinished();
	void downloadingChanged();
	void downloadProgressChanged();

private:
	bool must_download() const;
	void download_images();
	void load_images_from_database();
	void save_images_to_database();
	bool clear_temp_folder();

	// Private setter
	void set_downloading(bool b);
	void set_downloadProgress(int n);

public Q_SLOTS:
	// Force a refresh of the image URL
	void load_images(bool forceDownload = false);

	// Set the current image as the desktop wallpaper
	void keep_image(int i);

private slots:
	void xml_ready(QNetworkReply* reply);
	void image_ready(QNetworkReply* reply);
};

#endif // IMAGELOADER_HPP
