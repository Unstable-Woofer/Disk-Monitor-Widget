#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "volumes.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    qmlRegisterType<Volumes>("dev.unstablewoofer", 1, 0, "Volumes");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/DiskMonitor/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
