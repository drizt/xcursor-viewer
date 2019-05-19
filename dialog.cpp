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

#include "dialog.h"
#include <ui_dialog.h>

#include <QBuffer>
#include <QDataStream>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileDialog>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    connect(ui->lwCursors, &QListWidget::currentTextChanged, this, &Dialog::showCursor);
    connect(ui->pbOpenFolder, &QPushButton::clicked, this, &Dialog::openFolder);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::openFolder()
{
    QString dirPath = QFileDialog::getExistingDirectory(this);

    if (dirPath.isEmpty()) {
        return;
    }

    QDir dir(dirPath);

    QStringList fileList = dir.entryList(QDir::Filter::Files);

    _cursorFileList.clear();

    for (const QString &fileName: fileList) {
        QFile file(dir.absoluteFilePath(fileName));
        if (!file.open(QIODevice::OpenModeFlag::ReadOnly)) {
            continue;
        }

        QDataStream stream(&file);
        stream.setByteOrder(QDataStream::ByteOrder::LittleEndian);

        quint32 magic;
        quint32 header;
        quint32 version;
        quint32 ntoc;

        stream >> magic >> header >> version >> ntoc;

        if (magic != 0x72756358 /* Xcur */ && header != 16) {
            continue;
        }

        CursorFile cursorFile;

        cursorFile.name = fileName;

        for (quint32 i = 0; i < ntoc; ++i) {
            quint32 type;
            quint32 subtype;
            quint32 position;

            stream >> type >> subtype >> position;
            qint64 tocPos = file.pos(); // position in table of contents entries

            if (type == 0xfffd0002) {
                file.seek(position);

                quint32 imgHeader;
                quint32 imgType;
                quint32 imgSubtype;
                quint32 imgVersion;
                quint32 imgWidth;
                quint32 imgHeight;
                quint32 imgYhot;
                quint32 imgXhot;
                quint32 imgDelay;

                stream  >> imgHeader
                        >> imgType
                        >> imgSubtype
                        >> imgVersion
                        >> imgWidth
                        >> imgHeight
                        >> imgYhot
                        >> imgXhot
                        >> imgDelay;

                if (imgHeader != 36 || imgType != type || imgSubtype != subtype || imgVersion != 1) {
                    continue;
                }

                QByteArray imgData = file.read((imgWidth * imgHeight) * 4);

                Cursor cursor;
                cursor.image = QImage(reinterpret_cast<uchar*>(imgData.data()), static_cast<int>(imgWidth), static_cast<int>(imgHeight), QImage::Format::Format_ARGB32).copy();
                cursor.hotSpot = QPoint(static_cast<int>(imgXhot), static_cast<int>(imgYhot));

                QString key = QStringLiteral("%1").arg(static_cast<int>(subtype), 3, 10, QLatin1Char('0'));
                cursorFile.cursorMap.insertMulti(key, cursor);

                file.seek(tocPos);
            }
        }

        _cursorFileList << cursorFile;
    }

    ui->lwCursors->clear();

    QStringList nameList;

    for (const CursorFile &cursorFile: _cursorFileList) {
        nameList << cursorFile.name;
    }

    ui->lwCursors->addItems(nameList);
}

void Dialog::showCursor(const QString &fileName)
{
    CursorFile foundCursorFile;

    for (const CursorFile &cursorFile: _cursorFileList) {
        if (cursorFile.name == fileName) {
            foundCursorFile = cursorFile;
            break;
        }
    }

    if(foundCursorFile.name.isEmpty()) {
        ui->teCursorInfo->clear();
        return;
    }

    QString msg = "<html><body>";

    QStringList keys = foundCursorFile.cursorMap.keys();
    keys.removeDuplicates();
    keys.sort();

    for (const QString &key: keys) {
        QList<Cursor> cursorList = foundCursorFile.cursorMap.values(key);
        msg += "<p>";
        for (const Cursor &cursor: cursorList) {
            QByteArray imgBa;
            QBuffer buffer(&imgBa);
            cursor.image.save(&buffer, "PNG");

            imgBa = imgBa.toBase64();

            msg += "<img src=\"data:image/png;base64, " + QString::fromLatin1(imgBa) + "\"/>";
        }
        msg += "</p>";
    }

    msg += "</body></html>";

    ui->teCursorInfo->setHtml(msg);
}
