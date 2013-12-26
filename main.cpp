#include <QtGui/QApplication>
#include "MainWindow.h"
#include "helpers.h"
char * __path;
size_t __size;
#ifdef LINUX
    const QString PathWorking = QDir::homePath() + "/.salor-bin";
#endif

#ifdef WIN32
    const QString PathWorking = _getcwd(__path,__size);
#endif

#ifdef MAC
    const QString PathWorking = getcwd(__path,__size);
#endif
#ifdef LINUX
    const QString PathCookies = "/var/lib/qushie/cookiejar";
    const QString PathCache = "/var/cache/qushie";
    const QString PathSettings = "/etc/qushie/qushie.ini";
    const QString PathLog = "/var/log/qushie/qushie.log";
#endif
#ifdef WIN32
    const QString PathCookies = PathWorking + "/cookiejar";
    const QString PathCache = PathWorking + "/cache";
    const QString PathSettings = PathWorking + "/qushie.ini";
    const QString PathLog = PathWorking + "/qushie.log";
#endif
QSettings *settings = new QSettings(PathSettings, QSettings::IniFormat);
void myMessageOutput(QtMsgType type, const char *msg)
 {
    QFile f(PathLog);
    f.open(QIODevice::Append);
    QDataStream out(&f);

    QDateTime datetime = QDateTime::currentDateTime();
    QString timestamp = datetime.toString("yyyyMMddHHmmss");

    out << timestamp << msg << "\n";

    //in this function, you can write the message to any stream!
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s\n", msg);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s\n", msg);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s\n", msg);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s\n", msg);
        abort();
    }

    f.close();
 }

void purgeLogfile() {
    qDebug() << "Truncating Logfile";
    QFile f(PathLog);
    f.open(QFile::WriteOnly|QFile::Truncate);
    f.close();
}
int main(int argc, char *argv[])
{
    qInstallMsgHandler(myMessageOutput);
    QApplication a(argc, argv);
    MainWindow w;
#ifdef WINDOWS
    purgeLogfile();
#endif
    QVariant url;
    url = _get("url");
    if (url.isNull() != true && url.toString() != "") {
        w.to_url = url.toString();
    } else {
        w.to_url = "file:///C:/Users/ayon/Documents/GitHub/qushie-app/index.html";
    }
    w.Init();
    w.show();
    
    return a.exec();
}
