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
#include "attributesdialog.h"
#include "ui_attributesdialog.h"
#include "attributesmodel.h"

#include <QDebug>

AttributesDialog::AttributesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AttributesDialog)
{
    ui->setupUi(this);

    ui->imageAttributesView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->videoAttributesView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    m_imageAttributesModel = new AttributesModel(this);
    ui->imageAttributesView->setModel(m_imageAttributesModel);

    m_videoAttributesModel = new AttributesModel(this);
    ui->videoAttributesView->setModel(m_videoAttributesModel);

    connect(ui->addVideoAttrButton, &QToolButton::clicked,
            this, &AttributesDialog::addVideoAttribute);
    connect(ui->removeVideoAttrButton, &QToolButton::clicked,
            this, &AttributesDialog::removeVideoAttribute);
    connect(ui->addImageAttrButton, &QToolButton::clicked,
            this, &AttributesDialog::addImageAttribute);
    connect(ui->removeImageAttrButton, &QToolButton::clicked,
            this, &AttributesDialog::removeImageAttribute);
}

AttributesDialog::~AttributesDialog()
{
    delete ui;
}

void AttributesDialog::setImageAttributes(const QList<Attribute> &list)
{
    m_imageAttributesModel->setAttributes(list);
}

QList<Attribute> AttributesDialog::imageAttributes() const
{
    return m_imageAttributesModel->attributes();
}

void AttributesDialog::setVideoAttributes(const QList<Attribute> &list)
{
    m_videoAttributesModel->setAttributes(list);
}

QList<Attribute> AttributesDialog::videoAttributes() const
{
    return m_videoAttributesModel->attributes();
}

void AttributesDialog::addVideoAttribute()
{
    QModelIndex index = m_videoAttributesModel->appendAttribute();
    ui->videoAttributesView->edit(index);
}

void AttributesDialog::removeVideoAttribute()
{
    QItemSelectionModel *selectionModel = ui->videoAttributesView->selectionModel();

    QModelIndex index = selectionModel->selectedRows().first();
    m_videoAttributesModel->removeRows(index.row(), 1, QModelIndex());
}

void AttributesDialog::addImageAttribute()
{
    QModelIndex index = m_imageAttributesModel->appendAttribute();
    ui->imageAttributesView->edit(index);
}

void AttributesDialog::removeImageAttribute()
{
    QItemSelectionModel *selectionModel = ui->imageAttributesView->selectionModel();

    QModelIndex index = selectionModel->selectedRows().first();
    m_imageAttributesModel->removeRows(index.row(), 1, QModelIndex());
}
