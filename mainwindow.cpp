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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "casparcgconnection.h"
#include "rundowncreatorconnection.h"
#include "settingsdialog.h"
#include "attributesdialog.h"
#include "attributesstore.h"

#include <QDebug>
#include <QSettings>
#include <QScopedPointer>
#include <QTimer>
#include <QTextCursor>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionQuit, &QAction::triggered,
            qApp, &QApplication::quit);
    connect(ui->actionSettings, &QAction::triggered,
            this, &MainWindow::showSettingsDialog);
    connect(ui->actionConfigureAttributes, &QAction::triggered,
            this, &MainWindow::showAttributesDialog);

    QSettings settings;

    m_updateTimer = new QTimer(this);
    m_updateTimer->setInterval(settings.value("CasparCG/UpdateInterval", 60).toUInt() * 1000);

    m_caspar = new CasparCGConnection(this);
    m_caspar->connectToHost(settings.value("CasparCG/Server").toString(),
                            settings.value("CasparCG/Port", 5250).toUInt());

    connect(m_caspar, &CasparCGConnection::receivedContentList,
            this, &MainWindow::showContentsList);
    connect(m_caspar, &CasparCGConnection::connected,
            this, &MainWindow::startUpdateTimer);
    connect(m_updateTimer, &QTimer::timeout,
            m_caspar, &CasparCGConnection::getContentList);
    connect(m_caspar, &CasparCGConnection::logMessage,
            this, &MainWindow::addLogMessage);

    m_rundownCreator = new RundownCreatorConnection(this);
    m_rundownCreator->setApiUrl(settings.value("RundownCreator/Url").toString());
    m_rundownCreator->setApiKey(settings.value("RundownCreator/Key").toString());
    m_rundownCreator->setDeviceName(settings.value("RundownCreator/DeviceName").toString());

    connect(m_rundownCreator, &RundownCreatorConnection::logMessage,
            this, &MainWindow::addLogMessage);

    if(m_rundownCreator->isValid())
    {
        m_rundownCreator->registerDevice();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startUpdateTimer()
{
    m_caspar->getContentList();
    m_updateTimer->start();
}

void MainWindow::showContentsList()
{
    m_rundownCreator->setVideoFiles(m_caspar->movieList());
    m_rundownCreator->setImageFiles(m_caspar->stillList());
}

void MainWindow::showSettingsDialog()
{
    QScopedPointer<SettingsDialog> dialog(new SettingsDialog);
    QSettings settings;

    dialog->setRundownCreatorUrl(settings.value("RundownCreator/Url").toString());
    dialog->setRundownCreatorKey(settings.value("RundownCreator/Key").toString());
    dialog->setRundownCreatorDeviceName(settings.value("RundownCreator/DeviceName").toString());
    QString casparCGServer = settings.value("CasparCG/Server").toString();
    dialog->setCasparCGServer(casparCGServer);
    quint16 casparCGPort = settings.value("CasparCG/Port", 5250).toUInt();
    dialog->setCasparCGPort(casparCGPort);
    quint16 updateInterval = settings.value("CasparCG/UpdateInterval", 60).toUInt();
    dialog->setCasparCGUpdateInterval(updateInterval);

    if(dialog->exec() == QDialog::Accepted)
    {

        if(m_rundownCreator->apiUrl() != dialog->rundownCreatorUrl() ||
                m_rundownCreator->apiKey() != dialog->rundownCreatorKey() ||
                m_rundownCreator->deviceName() != dialog->rundownCreatorDeviceName())
        {
            settings.setValue("RundownCreator/Url", dialog->rundownCreatorUrl());
            settings.setValue("RundownCreator/Key", dialog->rundownCreatorKey());
            settings.setValue("RundownCreator/DeviceName", dialog->rundownCreatorDeviceName());
            m_rundownCreator->setApiUrl(dialog->rundownCreatorUrl());
            m_rundownCreator->setApiKey(dialog->rundownCreatorKey());
            m_rundownCreator->setDeviceName(dialog->rundownCreatorDeviceName());
            m_rundownCreator->registerDevice();
        }

        if(casparCGServer != dialog->casparCGServer() ||
                casparCGPort != dialog->casparCGPort())
        {
            settings.setValue("CasparCG/Server", dialog->casparCGServer());
            settings.setValue("CasparCG/Port", dialog->casparCGPort());
            m_caspar->connectToHost(dialog->casparCGServer(), dialog->casparCGPort());
        }

        if(updateInterval != dialog->casparCGUpdateInterval())
        {
            settings.setValue("CasparCG/UpdateInterval", dialog->casparCGUpdateInterval());
            m_updateTimer->setInterval(dialog->casparCGUpdateInterval() * 1000);
        }
    }
}

void MainWindow::addLogMessage(const QString &message)
{
    QTextCursor cursor = ui->logBrowser->textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.insertText(message + "\n");
}

void MainWindow::showAttributesDialog()
{
    QScopedPointer<AttributesDialog> dialog(new AttributesDialog(this));

    dialog->setImageAttributes(m_rundownCreator->attributes()->imageAttributes());
    dialog->setVideoAttributes(m_rundownCreator->attributes()->videoAttributes());

    if(dialog->exec() == QDialog::Accepted)
    {
        m_rundownCreator->attributes()->setImageAttributes(dialog->imageAttributes());
        m_rundownCreator->attributes()->setVideoAttributes(dialog->videoAttributes());
    }
}
