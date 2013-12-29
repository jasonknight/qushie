#include "MainWindow.h"

#include <QtWebKit/QWebSettings>
#include <QtWebKit/QWebFrame>
#include <QThread>
#include <QWebInspector>
#include <string>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <sstream>
#define CHUNK 16384
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

}


MainWindow::~MainWindow()
{
    
}
void MainWindow::Init() {
        QWebSettings::globalSettings()->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, true);
    QWebSettings::globalSettings()->setOfflineStoragePath(QDesktopServices::storageLocation(QDesktopServices::DataLocation));
    QWebSettings::globalSettings()->setAttribute(QWebSettings::PrintElementBackgrounds, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::LocalStorageEnabled, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::LocalStorageDatabaseEnabled, true);
    QVariant size = _get("main_window_geometry");
    restoreState( _get("main_window_state").toByteArray() );
    restoreGeometry( _get("main_window_geometry").toByteArray() );
    _set("main_window_geometry", QVariant( saveGeometry() ));
    _set("main_window_state", QVariant( saveState()));
    web_view = new QWebView(this);
    setCentralWidget(web_view);
    web_view->show();
    web_view->load(to_url);
    connect(web_view->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()),
            this, SLOT(attach()));
//    QWebInspector *inspector;
//    inspector = new QWebInspector();
//    inspector->setPage(web_view->page());
//    inspector->setGeometry(QRect(500, 10, 700, 500));
//    inspector->show();
}
void MainWindow::attach() {
    web_view->page()->mainFrame()->addToJavaScriptWindowObject("Qushie",this);
}
QObject* MainWindow::createTelnetObject() {
    QTelnet* t = new QTelnet(this);
    this->connections.append(t);
    t->id = this->connections.length() - 1;
    return (QObject *)t;
}
QObject* MainWindow::getTelnetObjectById(int id) {
    for ( int i = 0; i < this->connections.length(); i++ ) {
        QTelnet* t = this->connections.at(i);
        if ( t->id == id ) {
            return (QObject*)t;
        }
    }
    qDebug() << "Couldn't find it, so creating a new one";
    return this->createTelnetObject();
}

QObject* MainWindow::createFileObject() {
    QushieFile* f = new QushieFile(this);
    return (QObject *)f;
}
QObject* MainWindow::createDatabaseObject() {
    QushieDatabase* d = new QushieDatabase(this);
    return (QObject*)d;
}

QString MainWindow::compressString(QByteArray str) {
        z_stream zs;                        // z_stream is zlib's control structure
        memset(&zs, 0, sizeof(zs));

        if (deflateInit(&zs, Z_DEFAULT_COMPRESSION) != Z_OK)
            throw(std::runtime_error("deflateInit failed while compressing."));

        zs.next_in = (Bytef*)str.data();
        zs.avail_in = str.size();           // set the z_stream's input

        int ret;
        char outbuffer[32768];
        std::string outstring;

        // retrieve the compressed bytes blockwise
        do {
            zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
            zs.avail_out = sizeof(outbuffer);

            ret = deflate(&zs, Z_FINISH);

            if (outstring.size() < zs.total_out) {
                // append the block to the output string
                outstring.append(outbuffer,
                                 zs.total_out - outstring.size());
            }
        } while (ret == Z_OK);

        deflateEnd(&zs);

        if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
            std::ostringstream oss;
            oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
            throw(std::runtime_error(oss.str()));
        }
        QString final = outstring.c_str();
        qDebug() << "Compressed: " << final;
        this->decompressString( QByteArray( outstring.c_str() ) );
        return final;

}
QString MainWindow::decompressString(QByteArray str) {
        z_stream zs;                        // z_stream is zlib's control structure
        memset(&zs, 0, sizeof(zs));

        if (inflateInit(&zs) != Z_OK)
            throw(std::runtime_error("inflateInit failed while decompressing."));

        zs.next_in = (Bytef*)str.data();
        zs.avail_in = str.size();

        int ret;
        char outbuffer[32768];
        std::string outstring;

        // get the decompressed bytes blockwise using repeated calls to inflate
        do {
            zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
            zs.avail_out = sizeof(outbuffer);

            ret = inflate(&zs, 0);

            if (outstring.size() < zs.total_out) {
                outstring.append(outbuffer,
                                 zs.total_out - outstring.size());
            }

        } while (ret == Z_OK);

        inflateEnd(&zs);

        if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
            std::ostringstream oss;
            oss << "Exception during zlib decompression: (" << ret << ") "
                << zs.msg;
            //throw(std::runtime_error(oss.str()));
        }
        QString final = outstring.c_str();
        qDebug() << "Decompressed: " << final;
        return final;
}
