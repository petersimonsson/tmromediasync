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
#ifndef RUNDOWNCREATORCONNECTION_H
#define RUNDOWNCREATORCONNECTION_H

#include <QObject>
#include <QHttpPart>

class QNetworkAccessManager;
class QNetworkReply;

class RundownCreatorConnection : public QObject
{
    Q_OBJECT
public:
    explicit RundownCreatorConnection(QObject *parent = nullptr);

    bool isValid() const { return !m_apiUrl.isEmpty() && !m_apiKey.isEmpty() && !m_deviceName.isEmpty(); }

    void setDeviceName(const QString &name) { m_deviceName = name; }
    QString deviceName() const { return m_deviceName; }

    void setApiUrl(const QString &url) { m_apiUrl = url; }
    QString apiUrl() const { return m_apiUrl; }
    void setApiKey(const QString &key) { m_apiKey = key; }
    QString apiKey() const { return m_apiKey; }

    void registerDevice();

    void setVideoFiles(const QStringList &files);
    void setImageFiles(const QStringList &files);

private slots:
    void handleFinished(QNetworkReply *reply);

private:
    QHttpPart createFormPart(const QString &key, const QByteArray &value) const;
    QUrl requestUrl() const;

    QNetworkAccessManager *m_netManager;
    QString m_apiUrl;
    QString m_apiKey;

    QString m_deviceName;
    quint16 m_deviceId;

signals:
    void logMessage(const QString &message);
};

#endif // RUNDOWNCREATORCONNECTION_H
