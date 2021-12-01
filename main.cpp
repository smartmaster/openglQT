#include "mymainwindow.h"

#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{


    /////////////////////////////////////////////////////////////////
    QSurfaceFormat format;
    format.setVersion(4, 5);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setOption(QSurfaceFormat::DebugContext);
    QSurfaceFormat::setDefaultFormat(format);

    /////////////////////////////////////////////////////////////////
    QApplication a(argc, argv);
    MyMainWindow w;
    w.show();
    return a.exec();
}
