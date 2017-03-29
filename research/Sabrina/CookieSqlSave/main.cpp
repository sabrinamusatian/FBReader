#include <QApplication>
#include <QWebEngineView>
#include <QWebEngineProfile>
#include <QWebEngineCookieStore>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    QWebEngineView view;
    view.page()->profile()->setPersistentCookiesPolicy(QWebEngineProfile::ForcePersistentCookies);
    view.page()->profile()->setCachePath("store");
    view.page()->profile()->setPersistentStoragePath("store");
    view.setUrl(QUrl(QStringLiteral("https://books.fbreader.org/opds")));
    view.resize(1024, 750);
    view.show();

    return app.exec();
}
