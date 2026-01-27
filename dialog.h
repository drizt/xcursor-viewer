// SPDX-FileCopyrightText: 2019, 2026 Ivan Romanov <drizt72@zoho.eu>
// SPDX-FileContributor: 2020 Ruslan Kabatsayev <b7.10110111@gmail.com>
// SPDX-FileContributor: 2021 zjeffer <vanhouttetuur@gmail.com>
// SPDX-FileContributor: 2023 Tasos Sahanidis <tasos@tasossah.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QDialog>

#include <QImage>
#include <QList>
#include <QMultiMap>
#include <QString>
#include <QTreeWidgetItem>

namespace Ui
{
class Dialog;
}

struct Cursor {
    QImage image;
    quint32 size;
    QPoint hotSpot;
};

struct CursorFile {
    QString name;
    QString realName;
    QString license;
    QString copyright;
    QString other;
    QMultiMap<QString, Cursor> cursorMap;
    QString cachedCursors;
};

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(const QString &path, QWidget *parent = nullptr);
    ~Dialog();

    void openFolder();
    void openFolderPath(QString path);
    void showCursor(QTreeWidgetItem *current, QTreeWidgetItem *previous);

private:
    void exportCursors();

private:
    Ui::Dialog *ui;
    QMap<QString, CursorFile> _cursorFileMap;
};
