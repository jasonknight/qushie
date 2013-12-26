#ifndef QUSHIEFILE_H
#define QUSHIEFILE_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QThread>
#include <QList>
class QushieFile : public QObject
{
    Q_OBJECT
public:
    QushieFile(QObject *p = 0) : QObject(p) {
        this->file = NULL;
    }

    ~QushieFile() {
        if ( this->file && this->file->isOpen() ) {
            this->file->close();
        }
    }

signals:
    void SignalOpened();
    void SignalBytesRead(QString);
    void SignalBytesWritten(int);
    void SignalError(QString);
    void SignalEndOfFile();
    void SignalFileDoesNotExist();
public Q_SLOTS:
    void read();
    void write(QString text);
    void open(QString fname,QString mode);
    void close();
    QString getContents(QString fname);
    void    putContents(QString fname, QString contents);
    void    append(QString fname,QString contents);
private Q_SLOTS:
    void _read();
private:
    QFile* file;
    QTextStream* stream;
    int bytes_to_read;
    QThread* thread;
};

#endif // QUSHIEFILE_H
