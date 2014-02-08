#ifndef SETTINGSHANDLER_HPP
#define SETTINGSHANDLER_HPP

#include <QObject>
#include <QDateTime>
#include <QDir>
#include <QString>
#include <QStringList>

class SettingsHandler : public QObject
{
	Q_OBJECT

	Q_PROPERTY(bool embedLogo READ embedLogo WRITE set_embedLogo NOTIFY embedLogoChanged)
	Q_PROPERTY(QString resolution READ resolution WRITE set_resolution NOTIFY resolutionChanged)
	Q_PROPERTY(int gridCellsPerPage READ gridCellsPerPage WRITE set_gridCellsPerPage NOTIFY gridCellsPerPageChanged)

private:
	// Members
	int m_imagesWidth = 1920;
	int m_imagesHeight = 1080;
	bool m_embedLogo = false;
	QDateTime m_lastDownload;
	int m_gridCellsPerPage = 3;

public:
	// Global paths for the application
	static const QString AppFolder;
	static const QString TempImagesFolder;
	static const QString FavoriteFolder;
	static const QString ImagesFolder;
	static const QString SettingsFile;
	static const QString DatabaseFile;

	// List of supported markets
	static const QStringList Markets;

public:
	// Constructor/destructor
	explicit SettingsHandler(QObject* parent = nullptr);
	~SettingsHandler();

	// Getters
	bool embedLogo() const;
	QString resolution() const;
	QDateTime lastDownload() const;
	int gridCellsPerPage() const;

	// Setters
	void set_embedLogo(bool b);
	void set_resolution(QString res);
	void set_lastDownload(QDateTime ld);
	void set_gridCellsPerPage(int c);

Q_SIGNALS:
	void embedLogoChanged();
	void resolutionChanged();
	void gridCellsPerPageChanged();

public Q_SLOTS:
	void save_settings() const;

private slots:

};

#endif // SETTINGSHANDLER_HPP
