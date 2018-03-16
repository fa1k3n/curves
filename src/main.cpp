#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <line.h>
#include <chaikins.h>
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<Line>("fa1k3n.curves", 1, 0, "Line");
    qmlRegisterType<Chaikins>("fa1k3n.curves", 1, 0, "ChaikinsCurve");
    qmlRegisterType<ChaikinsModel>("fa1k3n.curves", 1, 0, "ChaikinsModel");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
