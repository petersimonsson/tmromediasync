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
#include "casparcgconnection.h"

#include <QTcpSocket>
#include <QDebug>
#include <QStringList>
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>

CasparCGConnection::CasparCGConnection(QObject *parent) :
    QObject(parent), m_socket(nullptr), m_currentCode(0)
{
    m_socket = new QTcpSocket(this);

    connect(m_socket, &QTcpSocket::connected, this, &CasparCGConnection::handleConnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &CasparCGConnection::readFromSocket);
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(handleSocketError()));
}

void CasparCGConnection::connectToHost(const QString &address, quint16 port)
{
    if(m_socket->isOpen())
        m_socket->close();

    m_address = address;
    m_port = port;
    m_socket->connectToHost(address, port);
}

void CasparCGConnection::handleConnected()
{
    m_currentCode = 0;

    emit connected();
    emit logMessage(tr("Connected to CasparCG."));
}

bool CasparCGConnection::isConnected() const
{
    return m_socket->isOpen();
}

void CasparCGConnection::getContentList()
{
    if (!isConnected())
    {
        emit logMessage(tr("Not connected to CasparCG."));

        if (!m_address.isEmpty())
        {
            emit logMessage(tr("Trying to reconnect to CasparCG."));
            connectToHost(m_address, m_port);
        }
    }
    else
    {
        m_socket->write("CLS\r\n");
    }
}

void CasparCGConnection::readFromSocket()
{
    while(m_socket->canReadLine())
    {
        QByteArray data = m_socket->readLine();
        data.chop(2); // Remove \r\n
        QString line = QString::fromUtf8(data);

        QStringList parameters = splitLine(line);

        switch(m_currentCode)
        {
        case 0:
        {
            quint16 code = parameters[0].toUInt();
            m_currentAction = parameters[1];

            if(code >= 400)
            {
                m_currentCode = 0;
                emit logMessage(tr("CasparCG error: %1").arg(line));
            }
            else
            {
                if(code == 200 || code == 201)
                    m_currentCode = code;

                parseAction(m_currentAction, parameters);
            }
            break;
        }
        case 200:
            parseData(m_currentAction, parameters);

            if(parameters.isEmpty())
                m_currentCode = 0;
            break;
        case 201:
            parseData(m_currentAction, parameters);
            m_currentCode = 0;
            break;
        }
    }
}

QStringList CasparCGConnection::splitLine(const QString &line) const
{
    QRegularExpression splitRegExp("(\"[^\"]*\"|\\S*)\\s*");
    QStringList parameters;

    if(!line.isEmpty())
    {
        QRegularExpressionMatchIterator it = splitRegExp.globalMatch(line);

        while(it.hasNext())
        {
            QRegularExpressionMatch match = it.next();

            if(match.lastCapturedIndex() > 0)
            {
                QString paramString = match.captured(1);
                parameters.append(paramString.remove('\"'));
            }
        }
    }

    return parameters;
}

void CasparCGConnection::parseAction(const QString &action, const QStringList &parameters)
{
    Q_UNUSED(parameters)

    if(action == "CLS")
    {
        m_movieList.clear();
        m_stillList.clear();
    }
}

void CasparCGConnection::parseData(const QString &action, const QStringList &parameters)
{
    if(action == "CLS")
    {
        if(parameters.isEmpty())
        {
            emit receivedContentList();
            return;
        }

        QString filename = parameters[0];
        filename = filename.replace(QChar('\\'), QChar('/'));

        if(parameters[1] == "MOVIE")
            m_movieList.append(filename);
        else if(parameters[1] == "STILL")
            m_stillList.append(filename);
    }
}

void CasparCGConnection::handleSocketError()
{
    emit logMessage(tr("CasparCG socket error: %1").arg(m_socket->errorString()));
    m_socket->close();
}
