#ifndef HELPERS_H
#define HELPERS_H
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <fcntl.h>
#include <signal.h>
#include <QSettings>
#include <QFile>
#include <QDateTime>
#include <QDebug>
#include <QDesktopServices>
#ifdef WIN32
    #include <windows.h>
    #define WINDOWS
#endif

extern const QString PathSettings;
extern QSettings *settings;

QVariant _get(QString key);

void _set(QString key, QString &value);
void _set(QString key, int value);
void _set(QString key, QVariant value);
void _set(QString key, qreal value);
#endif // HELPERS_H
