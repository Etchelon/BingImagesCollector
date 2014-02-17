#include "BingImage.hpp"
#include "SettingsHandler.hpp"

#include <stdexcept>

BingImage::BingImage(QObject* parent)
	: QObject(parent)
{
}

BingImage::BingImage(QString m)
	: m_date{ QDateTime::currentDateTime() }
{
	if (!SettingsHandler::Markets.contains(m))
		throw std::runtime_error{ "Unknown market: " + m.toStdString() };

	m_market = m;
}

BingImage::BingImage(QString m, QDateTime d, QString p, QString c, bool f, QString h)
	: m_market{ m }, m_date{ d }, m_filePath{ p }, m_copyright{ c }, m_favorite{ f }, m_hash{ h }
{
}

bool BingImage::ready() const
{
	return m_filePath != "";
}

QString BingImage::market() const
{
	return m_market;
}

QString BingImage::date() const
{
	return m_date.date().toString(Qt::SystemLocaleShortDate);
}

QString BingImage::filePath() const
{
	return m_filePath;
}

QUrl BingImage::path() const
{
	return QUrl{ m_filePath };
}

QString BingImage::copyright() const
{
	return m_copyright;
}

bool BingImage::favorite() const
{
	return m_favorite;
}

QString BingImage::hash() const
{
	return m_hash;
}

bool BingImage::duplicate() const
{
	return m_duplicate;
}

void BingImage::set_date(QDateTime d)
{
	if (m_date != d)
	{
		m_date = d;
		emit dateChanged();
	}
}

void BingImage::set_filePath(QString p)
{
	if (m_filePath != p)
	{
		m_filePath = p;
		emit pathChanged();
	}
}

void BingImage::set_copyright(QString c)
{
	if (m_copyright != c)
	{
		m_copyright = c;
		emit copyrightChanged();
	}
}

void BingImage::set_hash(QString h)
{
	if (m_hash != h)
	{
		m_hash = h;
	}
}

void BingImage::set_duplicate(bool d)
{
	if (m_duplicate != d)
	{
		m_duplicate = d;
	}
}

void BingImage::set_favorite(bool isFavorite)
{
	if (m_favorite != isFavorite)
	{
		QString fileNameNoPath = m_filePath.right(20);

		// Image has become favorite: move to the fav folder
		if (isFavorite)
		{
			QString favPath = SettingsHandler::FavoriteFolder + fileNameNoPath;
			if (!QFile::rename(m_filePath, favPath))
			{
				emit problems("Couldn't move the image to the favorite folder!");
				return;
			}

			set_filePath(favPath);
		}
		// Image was removed from favorites: move to the images folder
		else
		{
			QString normalPath = SettingsHandler::ImagesFolder + fileNameNoPath;
			if (!QFile::rename(m_filePath, normalPath))
			{
				emit problems("Couldn't move the image to the main folder! Image is still in the favorite folder");
				return;
			}

			set_filePath(normalPath);
		}

		m_favorite = isFavorite;
		emit favoriteChanged();
	}
}

#ifdef _WIN32
#include "windows.h"
#endif

void BingImage::set_as_wallpaper()
{
#ifdef _WIN32
	LPWSTR p = new WCHAR[m_filePath.length() + 1];
	for (int i = 0; i < m_filePath.length(); ++i)
		*(p + i) = m_filePath.toStdWString()[i];
	*(p + m_filePath.length()) = '\0';

	int result = SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, p, SPIF_UPDATEINIFILE);

	delete p;

	if (!result)
		emit problems("Could not set the chosen image as your background - The Windows API returned " + QString::number(result));
#endif
}

void BingImage::load_from_xml(QXmlStreamReader& reader)
{
	if (!ready())
	{
		reader.readNextStartElement();

		// Assume the image loader calls this function after encountering the opening <Image> tag
		while (!reader.atEnd())
		{
			if (reader.isStartElement() && reader.name() == "Market")
				m_market = reader.readElementText();

			else if (reader.isStartElement() && reader.name() == "Date")
				m_date = QDateTime::fromString(reader.readElementText());

			else if (reader.isStartElement() && reader.name() == "LocalPath")
				m_filePath = reader.readElementText();

			else if (reader.isStartElement() && reader.name() == "Copyright")
				m_copyright = reader.readElementText();

			else if (reader.isStartElement() && reader.name() == "Favorite")
				m_favorite = static_cast<bool>(reader.readElementText().toInt());

			else if (reader.isStartElement() && reader.name() == "Duplicate")
				m_duplicate = static_cast<bool>(reader.readElementText().toInt());

			else if (reader.isStartElement() && reader.name() == "Md5Sum")
				m_hash = reader.readElementText();

			else if (reader.isEndElement() && reader.name() == "Image")
				return;

			else
			{
				emit problems("Unknown tag while reading image from xml; tag: " + reader.readElementText());
				return;
			}

			reader.readNextStartElement();
		}
	}
}

void BingImage::save_to_xml(QXmlStreamWriter& writer) const
{
	// Avoid duplication
	if (ready())
	{
		// Appended here the image data, on top of the element
		writer.writeStartElement("Image");

		writer.writeStartElement("Market");
		writer.writeCharacters(m_market);
		writer.writeEndElement();				// Market

		writer.writeStartElement("Date");
		writer.writeCharacters(m_date.toString());
		writer.writeEndElement();				// Date

		writer.writeStartElement("LocalPath");
		writer.writeCharacters(m_filePath);
		writer.writeEndElement();				// LocalPath

		writer.writeStartElement("Copyright");
		writer.writeCharacters(m_copyright);
		writer.writeEndElement();				// Copyright

		writer.writeStartElement("Favorite");
		writer.writeCharacters(QString::number(static_cast<int>(m_favorite)));
		writer.writeEndElement();				// Favorite

		writer.writeStartElement("Duplicate");
		writer.writeCharacters(QString::number(static_cast<int>(m_duplicate)));
		writer.writeEndElement();				// Duplicate

		writer.writeStartElement("Md5Sum");
		writer.writeCharacters(m_hash);
		writer.writeEndElement();				// Hash

		writer.writeEndElement();				// Image
	}
}

bool BingImage::delete_from_filesystem()
{
	if (!QFile::remove(m_filePath))
	{
		emit problems("Could not delete the image from the file system");
		return false;
	}

	m_filePath = "";
	return true;
}
