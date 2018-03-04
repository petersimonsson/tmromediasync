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
#ifndef PRESETSMODEL_H
#define PRESETSMODEL_H

#include "attribute.h"

#include <QAbstractItemModel>

class AttributesModel : public QAbstractItemModel
{
public:
    AttributesModel(QObject *parent = nullptr);

    void setAttributes(const QList<Attribute> &list);
    QModelIndex appendAttribute();
    QList<Attribute> attributes() const { return m_attributes; }

    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    virtual QModelIndex parent(const QModelIndex &child) const override;
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual bool removeRows(int row, int count, const QModelIndex &parent) override;

private:
    QList<Attribute> m_attributes;
};

#endif // PRESETSMODEL_H
