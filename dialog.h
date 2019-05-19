/*
 * Copyright (C) 2019  Ivan Romanov <drizt72@zoho.eu>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

#include <QDialog>

#include <QImage>
#include <QList>
#include <QMap>
#include <QString>

namespace Ui { class Dialog; }

struct Cursor
{
    QImage image;
    quint32 size;
    QPoint hotSpot;
};

struct CursorFile
{
    QString name;
    QString license;
    QString copyright;
    QString other;
    QMap<QString, Cursor> cursorMap;
};

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

    void openFolder();
    void showCursor(const QString &fileName);

private:
    Ui::Dialog *ui;
    QList<CursorFile> _cursorFileList;
};

