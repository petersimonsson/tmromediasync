/**************************************************************************
  TMROMediaSync
  Copyright (C) 2018  Peter Simonsson <peter.simonsson@gmail.com>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
**************************************************************************/
#include "attributesmodel.h"

#include <QDebug>

AttributesModel::AttributesModel(QObject *parent) :
    QAbstractItemModel(parent)
{
}

void AttributesModel::setAttributes(const QList<Attribute> &list)
{
    beginResetModel();
    m_attributes = list;
    endResetModel();
}

QModelIndex AttributesModel::appendAttribute()
{
    beginInsertRows(QModelIndex(), m_attributes.count(), m_attributes.count());
    m_attributes.append(Attribute());
    endInsertRows();

    return index(m_attributes.count() - 1, 0, QModelIndex());
}

QModelIndex AttributesModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row, column, parent))
        return QModelIndex();

    return createIndex(row, column, row);
}

QModelIndex AttributesModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)

    return QModelIndex();
}

int AttributesModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;

    return m_attributes.count();
}

int AttributesModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 3;
}

QVariant AttributesModel::data(const QModelIndex &index, int role) const
{
    if(!hasIndex(index.row(), index.column(), index.parent()))
        return QVariant();

    const Attribute &attribute = m_attributes[index.row()];

    switch(role)
    {
    case Qt::DisplayRole:
        switch(index.column())
        {
        case 0:
            return attribute.regexp;
        case 1:
            return attribute.tag;
        case 2:
            return attribute.value;
        }
        break;
    }

    return QVariant();
}

QVariant AttributesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation != Qt::Horizontal)
        return QVariant();

    switch(role)
    {
    case Qt::DisplayRole:
        switch(section)
        {
        case 0:
            return tr("Filename RegExp");
        case 1:
            return tr("Attribute Name");
        case 2:
            return tr("Value");
        }
    }

    return QVariant();
}

bool AttributesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!hasIndex(index.row(), index.column(), index.parent()))
        return false;

    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        Attribute &attribute = m_attributes[index.row()];

        switch(index.column())
        {
        case 0:
            attribute.regexp = value.toString();
            break;
        case 1:
            attribute.tag = value.toString();
            break;
        case 2:
            attribute.value = value.toString();
            break;
        }

        emit dataChanged(index, index);
        return true;
    }

    return false;
}

Qt::ItemFlags AttributesModel::flags(const QModelIndex &index) const
{
    if(!hasIndex(index.row(), index.column(), index.parent()))
        return 0;

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

bool AttributesModel::removeRows(int row, int count, const QModelIndex &parent)
{
    int endRow = row + count - 1;

    if(parent.isValid() || row >= m_attributes.count() || endRow >= m_attributes.count())
        return false;

    beginRemoveRows(parent, row, endRow);
    for(int i = row; i <= endRow; ++i)
        m_attributes.removeAt(i);
    endRemoveRows();

    return true;
}
