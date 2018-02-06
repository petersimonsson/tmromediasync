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
#ifndef CASPARCGCONNECTION_H
#define CASPARCGCONNECTION_H

#include <QObject>

class QTcpSocket;

class CasparCGConnection : public QObject
{
    Q_OBJECT
public:
    explicit CasparCGConnection(QObject *parent = nullptr);

    void connectToHost(const QString &address, quint16 port);

    QStringList movieList() const { return m_movieList; }
    QStringList stillList() const { return m_stillList; }

public slots:
    void getContentList();

protected:
    void parseAction(const QString &action, const QStringList &parameters);
    void parseData(const QString &action, const QStringList &parameters);

private slots:
    void handleConnected();
    void readFromSocket();

private:
    QStringList splitLine(const QString &line) const;

    QTcpSocket *m_socket;

    quint16 m_currentCode;
    QString m_currentAction;

    QStringList m_movieList;
    QStringList m_stillList;

signals:
    void connected();
    void receivedContentList();
    void logMessage(const QString &message);
};

#endif // CASPARCGCONNECTION_H
