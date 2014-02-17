#include "ImageLoader.hpp"

#include <QCryptographicHash>
#include <QImage>
#include <QPainter>
#include <QDir>
#include <QFile>
#include <QFileInfoList>
#include <QMap>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QThread>

#include <algorithm>
#include <stdexcept>

ImageLoader::ImageLoader(QObject* parent)
	: QObject(parent)
{
	connect(&m_xml_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(xml_ready(QNetworkReply*)));
	connect(&m_image_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(image_ready(QNetworkReply*)));

	try
	{
		load_images_from_database();

		for (const BingImage* image : *m_allImages)
			m_hashCodes.push_back(image->hash());
	}
	catch (const std::exception& ex)
	{
		emit problems(QString{ "Could not load images: " } + ex.what());
	}
}

ImageLoader::~ImageLoader()
{
	save_images_to_database();
	clear_temp_folder();
}

void ImageLoader::load_images_from_database()
{
	QFile databaseFile{ SettingsHandler::DatabaseFile };
	if (!databaseFile.open(QIODevice::ReadOnly))
	{
		emit problems("Could not open the database file for saving the image. Try with a restart or check if the database.xml file is in the folder");
		return;
	}

	QXmlStreamReader reader{ &databaseFile };

	while (!reader.atEnd())
	{
		if (reader.isStartElement() && reader.name() == "Image")
		{
			BingImage* image = new BingImage;
			image->load_from_xml(reader);
			connect(image, SIGNAL(problems(QString)), this, SIGNAL(problems(QString)));

			m_allImages->append(image);
		}

		reader.readNext();
	}
}

void ImageLoader::save_images_to_database()
{
	// Open the database file to append the new image info
	QFile databaseFile{ SettingsHandler::DatabaseFile };

	QFile tempFile{ SettingsHandler::AppFolder + "temp.xml" };
	if (!tempFile.open(QIODevice::WriteOnly))
	{
		emit problems("Could not open the temp file for saving the image to the database");
		return;
	}

	QXmlStreamWriter writer{ &tempFile };
	writer.setAutoFormatting(true);
	writer.setAutoFormattingIndent(4);

	// Start writing
	writer.writeStartDocument();
	writer.writeStartElement("Images");

	// Appended here the image data
	for (BingImage* image : *m_allImages)
		image->save_to_xml(writer);

	writer.writeEndElement();				// Images
	writer.writeEndDocument();				// Document

	// Switch the temporary with the old file
	tempFile.close();

	if (!databaseFile.rename(SettingsHandler::AppFolder + "oldDatabase.xml"))
	{
		tempFile.remove();
		emit problems("Could not rename the old database file for deletion; couldn't save the info about images to the database");
		return;
	}

	if (!tempFile.rename(SettingsHandler::DatabaseFile))
	{
		databaseFile.rename(SettingsHandler::DatabaseFile);
		emit problems("Could not rename the temp file for deletion; couldn't save the info about images to the database");
		return;
	}
	else
		databaseFile.remove();
}

bool ImageLoader::clear_temp_folder()
{
	// Assume that file remotion always goes well
	for (auto file : QDir{ SettingsHandler::TempImagesFolder }.entryList())
	{
		if (file.length() < 5)
			continue;

		if (!QFile::remove(SettingsHandler::TempImagesFolder + file))
		{
			emit problems("Could not clear the temp folder; try to clean it manually");
			return false;
		}
	}

	return true;
}

void ImageLoader::set_downloading(bool b)
{
	if (m_downloading != b)
	{
		m_downloading = b;
		emit downloadingChanged();
	}
}

void ImageLoader::set_downloadProgress(int n)
{
	if (m_downloadProgress != n)
	{
		m_downloadProgress = n;
		emit downloadProgressChanged();

		if (m_downloadProgress == SettingsHandler::Markets.size())
		{
			set_downloading(false);
			emit downloadFinished();
		}
	}
}

SettingsHandler* ImageLoader::settings() const
{
	return m_settings;
}

BingImagesModel* ImageLoader::todaysImages() const
{
	return m_todaysImages;
}

BingImagesModel* ImageLoader::allImages() const
{
	return m_allImages;
}

bool ImageLoader::downloading() const
{
	return m_downloading;
}

double ImageLoader::downloadProgress() const
{
	return static_cast<double>(m_downloadProgress) / SettingsHandler::Markets.size();
}

// Methods
bool ImageLoader::must_download() const
{
	QDateTime lastDownload = m_settings->lastDownload();
	QDateTime now = QDateTime::currentDateTime();

	if (now < lastDownload)
		throw std::out_of_range("The config.ini file says that the last download was...in the future!!");

	// If the app was last opened more than 1 day ago, download new images
	if (lastDownload.addDays(1) <= now)
		return true;

	int ld = lastDownload.date().day();
	int lh = lastDownload.time().hour();
	int nd = now.date().day();
	int nh = now.time().hour();

	// If the app was last opened before 3 am and now it's opened after 8 am, download new images
	// New images are fed overnight (I think so!)
	if (ld == nd)
	{
		if (lh <= 4 && nh >= 8)
			return true;
	}
	// Simple case: today is not the first day of the month (which is more complicated because the previous day can be any number from 28 to 31 inclusive)
	else
	{
		if (nd != 1)
		{
			if (!(lh >= 8 && nh <= 2))
				return true;
		}
		// To make it simple, since it's late, always force download on day 1 of every month XD
		else
			return true;
	}

	return false;
}

void ImageLoader::load_images(bool forceDownload)
{
	if (forceDownload || must_download())
		download_images();
}

// ======================================================================================================= //
// ======================================= DOWNLOAD FROM NETWORK ========================================= //
// ======================================================================================================= //
void ImageLoader::download_images()
{
	set_downloadProgress(0);
	set_downloading(true);

	// Clear the list of today's images
	m_todaysImages->clear(true);

	// Request the download link for all markets chosen by the user
	for (const auto& mkt : SettingsHandler::Markets)
	{
		BingImage* image = new BingImage{ mkt };
		connect(image, SIGNAL(problems(QString)), this, SIGNAL(problems(QString)));
		m_todaysImages->append(image);

		QUrl url{ m_requestUrl + mkt };
		QNetworkRequest request{ url };
		m_xml_manager.get(request);
	}

	m_settings->set_lastDownload(QDateTime::currentDateTime());
}

// When the site returns the xml, extract the link and request the image download
void ImageLoader::xml_ready(QNetworkReply* xmlReply)
{
	if (xmlReply->error() != QNetworkReply::NoError)
	{
		emit problems("Connection to bing server has failed!! Reason: " + xmlReply->errorString());
		return;
	}

	// Get the market specified in this request
	QString mkt = xmlReply->url().url().right(5);

	// Parse the xml and get the download link for the image
	QXmlStreamReader reader(xmlReply->readAll());
	QUrl imageUrl;

	while (!reader.atEnd())
	{
		reader.readNext();

		int thingsToRead = 0;
		if (reader.name() == "urlBase")
		{
			// Embed the market as a query parameter, so the image_ready handler can easily see the market for the image
			// This query parameter should have no effect on the get request
			imageUrl = QUrl{ "http://www.bing.com" + reader.readElementText() + "_" + m_settings->resolution() + ".jpg" + "?market=" + mkt };

			if (++thingsToRead == 2)
				break;
		}
		else if (reader.name() == "copyright")
		{
			// Save the copyright quote into the matching BingImage
			auto bingImage = std::find_if(m_todaysImages->cbegin(), m_todaysImages->cend(), [mkt](const BingImage* img)
			{
				return img->market() == mkt;
			});

			if (bingImage !=  m_todaysImages->cend())
			{
				(*bingImage)->set_copyright(reader.readElementText());

				if (++thingsToRead == 2)
					break;
			}
		}
	}

	// Download the image if the link is valid
	if (imageUrl.url() != "")
	{
		QNetworkRequest request{ imageUrl };
		m_image_manager.get(request);
	}

	// Schedule the network reply for deletion
	xmlReply->deleteLater();
}

void ImageLoader::image_ready(QNetworkReply* imageReply)
{
	if (imageReply->error() != QNetworkReply::NoError)
	{
		emit problems("Could not download the image of the day!! Reason: " + imageReply->errorString());
		return;
	}

	// Get the market for the current image
	QString mkt = imageReply->url().url().right(5);

	// Build the file name for this image
	QString fileName = SettingsHandler::TempImagesFolder
					   + mkt + "_"
					   + QDateTime::currentDateTime().date().toString("yyyy-MM-dd")
					   + ".jpg";

	// Get the image and save it to disk
	QByteArray bits = imageReply->readAll();
	QImage image = QImage::fromData(bits);

	if (m_settings->embedLogo())
	{
		// Get the bing logo scaled to fit the downloaded image's size properly
		QImage scaledLogo = QImage{ m_bingLogo }.scaledToHeight(image.height() / 8.5, Qt::SmoothTransformation);

		// Embed the logo into the downloaded image
		QPainter painter{ &image };
		painter.drawImage(image.width()*0.7, image.height()*0.75, scaledLogo);
	}

	// Save the image
	if (!image.save(fileName))
		emit problems("Could not save the image from market " + mkt);
	else
	{
		// Save the local path into the matching BingImage
		auto bingImage = std::find_if(m_todaysImages->cbegin(), m_todaysImages->cend(), [mkt](const BingImage* img)
		{
			return img->market() == mkt;
		});

		if (bingImage != m_todaysImages->cend())
		{
			(*bingImage)->set_filePath(fileName);

			// Calculate the md5 sum and check for duplicates
			QString md5sum = QString{ QCryptographicHash::hash(bits, QCryptographicHash::Md5).toHex() };
			(*bingImage)->set_hash(md5sum);
			(*bingImage)->set_duplicate(std::find(std::begin(m_hashCodes), std::end(m_hashCodes), md5sum) != std::end(m_hashCodes));
//			qDebug() << "md5sum for market " << mkt << ": " << md5sum << "; is duplicate: " << (*bingImage)->duplicate() << endl;
		}
	}

	// Schedule the network reply for deletion
	imageReply->deleteLater();

	set_downloadProgress(m_downloadProgress + 1);
}

// ======================================================================================================= //
// ============================================ UI COMMANDS ============================================== //
// ======================================================================================================= //
void ImageLoader::keep_image(int i)
{
	BingImage* image = m_todaysImages->get(i);

	QString path = image->filePath();
	QStringRef fileNameNoPath(&path, path.length() - 20, 16);
	QStringRef currentFolder(&path, 0, path.length() - 20);

	if (currentFolder == SettingsHandler::TempImagesFolder)
	{
		QString destinationPath = SettingsHandler::ImagesFolder + fileNameNoPath.toString() + ".jpg";

		int n = 0;
		while (QFile::exists(destinationPath))
			destinationPath = SettingsHandler::ImagesFolder + fileNameNoPath.toString() + "_" + QString::number(n++) + ".jpg";

		if (!QFile::rename(path, destinationPath))
		{
			emit problems("Could not move the image to the main folder! It will be removed upon quitting the program if you don't manually move it");
			return;
		}

		image->set_filePath(destinationPath);
		m_todaysImages->remove(i);
		m_allImages->prepend(image);
	}
}
