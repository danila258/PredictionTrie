#include "TextEditor.h"

#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    bool enableHighDpiScaling = false;

    {
        QApplication startApp(argc, argv);

        if (startApp.screens().at(0)->geometry().width() > 1920 &&
            startApp.screens().at(0)->geometry().height() > 1080)
        {
            enableHighDpiScaling = true;
        }
    }

    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling, enableHighDpiScaling);

    QApplication app(argc, argv);

    TextEditor editor;
    editor.show();

    return QApplication::exec();
}
