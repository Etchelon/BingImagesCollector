#ifndef BINGIMAGE_HPP
#define BINGIMAGE_HPP

#include <QDateTime>
#include <QFile>
#include <QObject>
#include <QString>
#include <QUrl>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

class BingImage : public QObject
{
	// QObject related metadata
	Q_OBJECT
	Q_PROPERTY(bool ready READ ready NOTIFY readyChanged)
	Q_PROPERTY(QString market READ market NOTIFY marketChanged)
	Q_PROPERTY(QString date READ date NOTIFY dateChanged)
	Q_PROPERTY(QUrl path READ path NOTIFY pathChanged)
	Q_PROPERTY(QString copyright READ copyright NOTIFY copyrightChanged)
	Q_PROPERTY(bool favorite READ favorite WRITE set_favorite NOTIFY favoriteChanged)

private:
	// Flag to know if the image is usable
	QString m_market;
	QDateTime m_date;
	QString m_filePath;
	QString m_copyright;
	bool m_favorite = false;

public:
	explicit BingImage(QObject* parent = nullptr);
	explicit BingImage(QString m);
	BingImage(QString m, QDateTime d, QString p, QString c);

public:
	// Getters
	bool ready() const;
	QString market() const;
	QString date() const;
	QString filePath() const;
	QUrl path() const;
	QString copyright() const;
	bool favorite() const;

	// Setters
	void set_filePath(QString p);
	void set_copyright(QString c);

public Q_SLOTS:
	void set_favorite(bool b);
	void set_as_wallpaper();

private:
	void set_date(QDateTime d);

Q_SIGNALS:
	void readyChanged();
	void marketChanged();
	void dateChanged();
	void pathChanged();
	void copyrightChanged();
	void favoriteChanged();

	void problems(QString message);

public:
	void load_from_xml(QXmlStreamReader& reader);
	void save_to_xml(QXmlStreamWriter& writer) const;
	bool delete_from_filesystem();
};

#endif // BINGIMAGE_HPP
