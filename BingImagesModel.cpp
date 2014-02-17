#include "BingImagesModel.hpp"

QHash<int, QByteArray> BingImagesModel::m_roles;

BingImagesModel::BingImagesModel(QObject* parent)
	: QAbstractListModel(parent)
{
	if (m_roles.size() == 0)
	{
		// Associate to each role the name (a QString) that will be used in QML
		m_roles = QAbstractListModel::roleNames();
		m_roles.insert(MarketRole, "market");
		m_roles.insert(DateRole, "date");
		m_roles.insert(PathRole, "path");
		m_roles.insert(CopyrightRole, "copyright");
		m_roles.insert(FavoriteRole, "favorite");
		m_roles.insert(DuplicateRole, "duplicate");
	}
}

BingImagesModel::BingImagesModel(const QList<BingImage*> images, QObject* parent)
	: BingImagesModel(parent)
{
	m_images = images;
	for (BingImage* image : m_images)
		image->setParent(this);
}

QHash<int, QByteArray> BingImagesModel::roleNames() const
{
	return m_roles;
}

int BingImagesModel::rowCount(const QModelIndex& parent) const
{
	return m_images.size();
}

QVariant BingImagesModel::data(const QModelIndex& index, int role) const
{
	int i = index.row();

	if (0 <= i && i < m_images.size())
	{
		switch (static_cast<Role>(role))
		{
		case MarketRole:
			return m_images[i]->market();

		case DateRole:
			return m_images[i]->date();

		case PathRole:
			return m_images[i]->path();

		case CopyrightRole:
			return m_images[i]->copyright();

		case FavoriteRole:
			return m_images[i]->favorite();

		case DuplicateRole:
			return m_images[i]->duplicate();
		}
	}

	return QVariant{ };
}

bool BingImagesModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	int i = index.row();

	if (0 <= i && i < m_images.size())
	{
		switch (static_cast<Role>(role))
		{
		/* These roles are not editable
		case MarketRole:
			m_images[i]->set_market(value.toString());
			break;

		case DateRole:
			m_images[i]->set_date(value.toDateTime());
			break;

		case PathRole:
			m_images[i]->set_filePath(value.toString());
			break;

		case CopyrightRole:
			m_images[i]->set_copyright(value.toString());
			break;
		*/

		case FavoriteRole:
			m_images[i]->set_favorite(value.toBool());
			emit dataChanged(index, index, { FavoriteRole });
			break;

		default:
			return false;
		}
	}

	return true;
}

int BingImagesModel::count() const
{
	return m_images.size();
}

bool BingImagesModel::insert(int i, BingImage* newImage)
{
	if (newImage)
		newImage->setParent(this);
	else if (newImage == nullptr || m_images.contains(newImage))
		return false;

	if (i < 0)
		i = 0;
	if (i > m_images.size())
		i = m_images.size();

	beginInsertRows(QModelIndex(), i, i);
	m_images.insert(i, newImage);
	endInsertRows();
	emit countChanged();

	return true;
}

bool BingImagesModel::remove(int i, bool alsoDelete)
{
	if (i < 0 || i >= m_images.size())
		return false;

	BingImage* image = m_images.at(i);

	// Delete the image from the filesystem if required
	if (alsoDelete)
		image->delete_from_filesystem();

	// Remove the image from the list
	beginRemoveRows(QModelIndex{ }, i, i);
	m_images.removeAt(i);
	endRemoveRows();
	emit countChanged();

	return true;
}

BingImage* BingImagesModel::get(int i)
{
	if (0 <= i && i < m_images.size())
		return m_images.at(i);

	return nullptr;
}

bool BingImagesModel::prepend(BingImage* newImage)
{
	return insert(0, newImage);
}

bool BingImagesModel::append(BingImage* newImage)
{
	return insert(rowCount(), newImage);
}

void BingImagesModel::clear(bool alsoDelete)
{
	if (count() == 0)
		return;

	beginRemoveRows(QModelIndex{ }, 0, count() - 1);

	for (BingImage* image : m_images)
	{
		if (alsoDelete)
			image->delete_from_filesystem();
		delete image;
	}
	m_images.clear();

	endRemoveRows();
	emit countChanged();
}

// Friend functions
QList<BingImage*>::iterator BingImagesModel::begin()
{
	return m_images.begin();
}

QList<BingImage*>::iterator BingImagesModel::end()
{
	return m_images.end();
}

QList<BingImage*>::const_iterator BingImagesModel::cbegin() const
{
	return m_images.cbegin();
}

QList<BingImage*>::const_iterator BingImagesModel::cend() const
{
	return m_images.cend();
}

