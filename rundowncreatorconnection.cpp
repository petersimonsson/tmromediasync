/**************************************************************************
  TMROMediaSync
  Copyright (C) 2017  Peter Simonsson <peter.simonsson@gmail.com>

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
#include "rundowncreatorconnection.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

RundownCreatorConnection::RundownCreatorConnection(QObject *parent) :
    QObject(parent), m_deviceId(0)
{
    m_netManager = new QNetworkAccessManager(this);

    connect(m_netManager, &QNetworkAccessManager::finished, this, &RundownCreatorConnection::handleFinished);
}

void RundownCreatorConnection::registerDevice()
{
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    multiPart->append(createFormPart("Key", m_apiKey.toUtf8()));
    multiPart->append(createFormPart("Action", "addDevice"));
    multiPart->append(createFormPart("Name", m_deviceName.toUtf8()));
    multiPart->append(createFormPart("Type", "CasparCG"));
    multiPart->append(createFormPart("RelayServer_IPAddress", "192.168.0.255"));
    multiPart->append(createFormPart("RelayServer_Port", "8080"));
    multiPart->append(createFormPart("Channels", "{\"Video\":[\"ddr\",\"ddr2\"],\"Audio\":[\"sound\"],\"Graphics\":[\"stills\","
                                                 "\"bfr1\",\"bfr2\",\"bfr3\",\"bfr4\",\"bfr5\",\"bfr6\",\"bfr7\",\"bfr8\",\"bfr9\","
                                                 "\"bfr10\",\"bfr11\",\"bfr12\",\"bfr13\",\"bfr14\",\"bfr15\"]}"));
    multiPart->append(createFormPart("Supports", "[\"video_load_preview\",\"video_load_program\",\"video_play\",\"video_stop\","
                                                 "\"audio_load_preview\",\"audio_load_program\",\"audio_play\",\"audio_stop\","
                                                 "\"graphics_load_preview\",\"graphics_load_program\",\"graphics_play\","
                                                 "\"graphics_stop\",\"filecheck\"]"));

    QNetworkRequest request(requestUrl());
    QNetworkReply *reply = m_netManager->post(request, multiPart);
    multiPart->setParent(reply);
    reply->setObjectName("addDevice");

    emit logMessage(tr("Registering device with RundownCreator..."));
}

void RundownCreatorConnection::setVideoFiles(const QStringList &files)
{
    sendFiles("Video", files);
}

void RundownCreatorConnection::setImageFiles(const QStringList &files)
{
    sendFiles("Graphics", files);
}

void RundownCreatorConnection::sendFiles(const QByteArray &type, const QStringList &files)
{
    if(!isValid() || m_deviceId == 0)
    {
        emit logMessage(tr("Device not registered."));
        return;
    }

    QJsonObject object;
    foreach (const QString &file, files)
    {
        object.insert(file, QJsonArray());
    }

    QJsonDocument doc;
    doc.setObject(object);

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    multiPart->append(createFormPart("Key", m_apiKey.toUtf8()));
    multiPart->append(createFormPart("Action", "refreshFiles"));
    multiPart->append(createFormPart("DeviceID", QByteArray::number(m_deviceId)));
    multiPart->append(createFormPart("Type", type));
    multiPart->append(createFormPart("Files", doc.toJson(QJsonDocument::Compact)));

    QNetworkRequest request(requestUrl());
    QNetworkReply *reply = m_netManager->post(request, multiPart);
    multiPart->setParent(reply);
    reply->setObjectName("refreshFiles");
}

void RundownCreatorConnection::handleFinished(QNetworkReply *reply)
{
    QByteArray data = reply->readAll();

    if(reply->objectName() == "addDevice")
    {
        if(!data.startsWith("{\"DeviceID\":"))
        {
            emit logMessage(tr("Device registration error: %1").arg(QString::fromUtf8(data)));
        }
        else
        {
            QJsonDocument doc = QJsonDocument::fromJson(data);
            QJsonObject object = doc.object();
            m_deviceId = object.value("DeviceID").toInt();
            emit logMessage(tr("Device registration successful."));
        }
    }

    reply->deleteLater();
}

QHttpPart RundownCreatorConnection::createFormPart(const QString &key, const QByteArray &value) const
{
    QHttpPart part;
    part.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"" + key + "\""));
    part.setBody(value);

    return part;
}

QUrl RundownCreatorConnection::requestUrl() const
{
    return QUrl(m_apiUrl + "/PlayoutControllerAPI.php");
}
