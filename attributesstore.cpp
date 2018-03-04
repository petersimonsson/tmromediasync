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
#include "attributesstore.h"

#include <QFile>
#include <QStandardPaths>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QJsonArray>
#include <QJsonObject>
#include <QRegularExpression>
#include <QDebug>
#include <QDir>

AttributesStore::AttributesStore(QObject *parent) : QObject(parent)
{
    readAtttributes();
}

AttributesStore::~AttributesStore()
{
    writeAttributes();
}

void AttributesStore::readAtttributes()
{
    QFile file(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/attributes.xml");

    if(!file.exists())
        return;

    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        emit logMessage(tr("Could not open %1 for reading with following error: %2").arg(file.fileName(), file.errorString()));
        return;
    }

    QXmlStreamReader reader(&file);

    if(reader.readNextStartElement())
    {
        if(reader.name() == "tmromediasync" || reader.attributes().value("version") == "1.0")
        {
            while(reader.readNextStartElement())
            {
                if(reader.name() == "folder")
                    readFolder(reader);
                else
                    reader.skipCurrentElement();
            }
        }
        else
        {
            emit logMessage(tr("%1 isn't a valid attributes file.").arg(file.fileName()));
            return;
        }
    }
}

void AttributesStore::readFolder(QXmlStreamReader &reader)
{
    QString type = reader.attributes().value("name").toString();
    QList<Attribute> *attrList = nullptr;

    if(type == "video")
    {
        attrList = &m_videoAttributes;
    }
    else if(type == "image")
    {
        attrList = &m_imageAttributes;
    }

    while(reader.readNextStartElement())
    {
        if(reader.name() == "attribute")
            readAttribute(reader, attrList);
        else
            reader.skipCurrentElement();
    }
}

void AttributesStore::readAttribute(QXmlStreamReader &reader, QList<Attribute> *attrList)
{
    Attribute attr;
    attr.tag = reader.attributes().value("tag").toString();
    attr.value = reader.attributes().value("value").toString();
    attr.regexp = reader.attributes().value("regexp").toString();
    reader.readNext();

    attrList->append(attr);
}

void AttributesStore::writeAttributes()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    QDir dir(path);
    if(!dir.exists())
        dir.mkpath(path);

    QFile file(path + "/attributes.xml");

    if(!file.open(QFile::WriteOnly))
    {
        emit logMessage(tr("Failed to open %1 for writing with following error: %2").arg(file.fileName(), file.errorString()));
        return;
    }

    QXmlStreamWriter writer(&file);

    writer.writeStartDocument();
    writer.writeStartElement("tmromediasync");
    writer.writeAttribute("version", "1.0");

    writer.writeStartElement("folder");
    writer.writeAttribute("name", "video");

    foreach(const Attribute &attr, m_videoAttributes)
    {
        writeAttribute(writer, attr);
    }

    writer.writeEndElement();

    writer.writeStartElement("folder");
    writer.writeAttribute("name", "image");

    foreach(const Attribute &attr, m_imageAttributes)
    {
        writeAttribute(writer, attr);
    }

    writer.writeEndElement();

    writer.writeEndElement();
    writer.writeEndDocument();
}

void AttributesStore::writeAttribute(QXmlStreamWriter &writer, const Attribute &attr)
{
    writer.writeStartElement("attribute");
    writer.writeAttribute("tag", attr.tag);
    writer.writeAttribute("value", attr.value);
    writer.writeAttribute("regexp", attr.regexp);
    writer.writeEndElement();
}

QJsonArray AttributesStore::fileAttributes(Type type, const QString &file) const
{
    QList<Attribute> attrList;
    QJsonArray array;

    switch(type)
    {
    case VideoType:
        attrList = m_videoAttributes;
        break;
    case ImageType:
        attrList = m_imageAttributes;
        break;
    }

    foreach(const Attribute &attr, attrList)
    {
        if(file.contains(QRegularExpression(attr.regexp)))
        {
            QJsonObject attrObject;
            attrObject.insert("Type", "text");
            attrObject.insert("Tag", attr.tag);
            attrObject.insert("Value", attr.value);
            array.append(attrObject);
            break;
        }
    }

    return array;
}
