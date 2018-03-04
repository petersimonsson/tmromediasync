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
#ifndef ATTRIBUTESDIALOG_H
#define ATTRIBUTESDIALOG_H

#include "attribute.h"

#include <QDialog>

namespace Ui {
class AttributesDialog;
}

class AttributesModel;

class AttributesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AttributesDialog(QWidget *parent = 0);
    ~AttributesDialog();

    void setImageAttributes(const QList<Attribute> &list);
    QList<Attribute> imageAttributes() const;
    void setVideoAttributes(const QList<Attribute> &list);
    QList<Attribute> videoAttributes() const;

private slots:
    void addVideoAttribute();
    void removeVideoAttribute();

    void addImageAttribute();
    void removeImageAttribute();

private:
    Ui::AttributesDialog *ui;

    AttributesModel *m_imageAttributesModel;
    AttributesModel *m_videoAttributesModel;
};

#endif // ATTRIBUTESDIALOG_H
