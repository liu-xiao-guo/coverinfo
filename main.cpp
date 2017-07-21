//#include <QGuiApplication>
#include<QApplication>
#include <QQmlApplicationEngine>
#include<QQmlContext>
#include "c++/LocalSong.h"
#include "c++/myimageprovider.h"

int main(int argc, char *argv[])
{
    // QGuiApplication app(argc, argv);
    QApplication app(argc,argv);

    QQmlApplicationEngine engine;
    MyImageProvider *imageProvider = new MyImageProvider(QQmlImageProviderBase::Image);
    engine.addImageProvider("myprovider", imageProvider );
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    LocalSong lSong;//先实例化
    engine.rootContext()->setContextProperty("localSong",&lSong);//放到QML

    return app.exec();
}
