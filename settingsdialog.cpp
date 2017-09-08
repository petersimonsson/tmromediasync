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
#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::setRundownCreatorUrl(const QString &url)
{
    ui->rundownCreatorUrlEdit->setText(url);
}

QString SettingsDialog::rundownCreatorUrl() const
{
    return ui->rundownCreatorUrlEdit->text();
}

void SettingsDialog::setRundownCreatorKey(const QString &key)
{
    ui->rundownCreatorKeyEdit->setText(key);
}

QString SettingsDialog::rundownCreatorKey() const
{
    return ui->rundownCreatorKeyEdit->text();
}

void SettingsDialog::setRundownCreatorDeviceName(const QString &name)
{
    ui->rundownCreatorDeviceNameEdit->setText(name);
}

QString SettingsDialog::rundownCreatorDeviceName() const
{
    return ui->rundownCreatorDeviceNameEdit->text();
}

void SettingsDialog::setCasparCGServer(const QString &server)
{
    ui->casparCGServerEdit->setText(server);
}

QString SettingsDialog::casparCGServer() const
{
    return ui->casparCGServerEdit->text();
}

void SettingsDialog::setCasparCGPort(quint16 port)
{
    ui->casparCGPortSpin->setValue(port);
}

quint16 SettingsDialog::casparCGPort() const
{
    return ui->casparCGPortSpin->value();
}

void SettingsDialog::setCasparCGUpdateInterval(quint16 interval)
{
    ui->casparCGUpdateIntervalSpin->setValue(interval);
}

quint16 SettingsDialog::casparCGUpdateInterval() const
{
    return ui->casparCGUpdateIntervalSpin->value();
}
