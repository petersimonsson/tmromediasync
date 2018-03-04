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
#ifndef ATTRIBUTESSTORE_H
#define ATTRIBUTESSTORE_H

#include "attribute.h"

#include <QObject>

class QXmlStreamReader;
class QXmlStreamWriter;

class AttributesStore : public QObject
{
    Q_OBJECT
public:
    enum Type {
        NoType,
        VideoType,
        ImageType,
    };

    explicit AttributesStore(QObject *parent = nullptr);
    ~AttributesStore();

    void readAtttributes();
    void writeAttributes();

    void setVideoAttributes(const QList<Attribute> &list) { m_videoAttributes = list; }
    QList<Attribute> videoAttributes() const { return m_videoAttributes; }

    void setImageAttributes(const QList<Attribute> &list) { m_imageAttributes = list; }
    QList<Attribute> imageAttributes() const { return m_imageAttributes; }

    QJsonArray fileAttributes(Type type, const QString &file) const;

private:
    void readFolder(QXmlStreamReader &reader);
    void readVideoFolder(QXmlStreamReader &reader);
    void readImageFolder(QXmlStreamReader &reader);

    void readAttribute(QXmlStreamReader &reader, QList<Attribute> *attrList);

    void writeAttribute(QXmlStreamWriter &writer, const Attribute &attr);

private:
    QList<Attribute> m_videoAttributes;
    QList<Attribute> m_imageAttributes;

signals:
    void logMessage(const QString &message);
};

#endif // ATTRIBUTESSTORE_H
