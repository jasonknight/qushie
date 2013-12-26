#include "QushieFile.h"
#include <QDebug>
void QushieFile::open(QString fname, QString mode) {
    this->file = new QFile(fname);

    if ( ! this->file->exists() && mode != "w" ) {
        emit SignalFileDoesNotExist();
        emit SignalError( QString("The specified file ") + fname + QString(" does not exist."));
    }
    if ( mode == "r" ) {
        this->file->open(QIODevice::ReadOnly);
    } else if ( mode == "rw" ) {
        this->file->open(QIODevice::ReadWrite);
    } else if ( mode == "w" ) {
        this->file->open(QIODevice::WriteOnly);
    } else if ( mode == "a" ) {
        this->file->open(QIODevice::Append);
    } else {
        emit SignalError( QString("I don't understand ") + mode + QString(" mode.") );
    }
    if ( file->isOpen() ) {
        this->stream = new QTextStream(this->file);
        emit SignalOpened();
    } else {
        emit SignalError( QString("Could not open file.") );
    }

}
void QushieFile::read() {
    this->thread = new QThread(this);
    connect(this->thread,SIGNAL(started()),this,SLOT(_read()));
    connect(this->thread,SIGNAL(finished()),this->thread,SLOT(deleteLater()));
    connect(this->thread,SIGNAL(terminated()),this->thread,SLOT(deleteLater()));
    this->thread->start();
}
void QushieFile::_read() {
    QString s;
    printf("I am in _read\n");
    if ( this->stream->atEnd() ) {
        printf("I am emitting end of file");
        emit SignalEndOfFile();
    } else {
        printf("I am reading");
        s = this->stream->readAll();
        emit SignalBytesRead(s);
    }
    printf("I am at the end of the function\n");
    fflush(stdout);
}
void QushieFile::close() {
    this->file->close();
    connect(this,SIGNAL(SignalEndOfFile()),this,SLOT(deleteLater()));
    emit SignalEndOfFile();
}
void QushieFile::write(QString text) {
    qDebug() << "Trying to write";
    *this->stream << text;
    emit SignalBytesWritten(text.length());
}
QString QushieFile::getContents(QString fname) {
    this->open(fname,"r");
    if ( this->file->isOpen() ) {
        QString contents = this->stream->readAll();
        this->close();
        return contents;
    } else {
        return "";
    }

}
void QushieFile::putContents(QString fname, QString contents) {
    this->open(fname,"w");
    if ( this->file->isOpen() ) {
        *this->stream << contents;
        this->close();
    }
}
void QushieFile::append(QString fname, QString contents) {
    this->open(fname,"a");
    if ( this->file->isOpen() ) {
        *this->stream << contents;
        this->close();
    }
}
