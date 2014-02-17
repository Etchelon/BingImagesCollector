#ifndef BINGIMAGESMODEL_HPP
#define BINGIMAGESMODEL_HPP

#include <QAbstractListModel>
#include <QVariant>
#include "BingImage.hpp"

class BingImagesModel : public QAbstractListModel
{
	Q_OBJECT
	Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
	enum Role {
		MarketRole = Qt::UserRole,
		DateRole,
		PathRole,
		CopyrightRole,
		FavoriteRole,
		DuplicateRole
	};

	QHash<int, QByteArray> roleNames() const;

public:
	explicit BingImagesModel(QObject* parent = nullptr);
	BingImagesModel(const QList<BingImage*> images, QObject* parent = nullptr);

	// Reimplemented APIs from QAbstractListModel
	int rowCount(const QModelIndex& parent = QModelIndex{ }) const;
	QVariant data(const QModelIndex& index, int role) const;
	bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

	// My own APIs
Q_SIGNALS:
	void countChanged();

public Q_SLOTS:
	int count() const;
	bool insert(int i, BingImage* newImage);
	bool remove(int i, bool alsoDelete = false);
	BingImage* get(int i);
	bool prepend(BingImage* newImage);
	bool append(BingImage* newImage);
	void clear(bool alsoDelete = false);

private:
	QList<BingImage*> m_images;
	static QHash<int, QByteArray> m_roles;

	// Friend of ImageLoader
	friend class ImageLoader;

	// To treat the model as a STL container
	QList<BingImage*>::iterator begin();
	QList<BingImage*>::iterator end();
	QList<BingImage*>::const_iterator cbegin() const;
	QList<BingImage*>::const_iterator cend() const;
};

#endif // BINGIMAGESMODEL_HPP
