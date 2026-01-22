// SPDX-FileCopyrightText: 2019, 2026 Ivan Romanov <drizt72@zoho.eu>
// SPDX-FileContributor: 2020 Ruslan Kabatsayev <b7.10110111@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "dialog.h"

#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addPositionalArgument("path", "Path to cursor file or to directory with cursors", "[path]");
    parser.process(app);
    const auto args = parser.positionalArguments();

    Dialog dlg(args.isEmpty() ? QString{} : args.front());
    dlg.show();
    app.exec();

    return 0;
}
