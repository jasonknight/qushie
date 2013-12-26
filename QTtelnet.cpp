#include "QTelnet.h"
#include <QDebug>
QTelnet::QTelnet(QObject *parent) :
    QObject(parent),
    socket(0),
    notifier(0),
    connected(false)
{
    this->mccp = false;
    this->setSocket(new QTcpSocket(this));
}
QTelnet::~QTelnet() {
    if ( socket )
        delete socket;
    if ( notifier )
        delete notifier;
}
void QTelnet::setSocket(QTcpSocket *s) {
    if (socket) {
        socket->flush();
    }
    delete socket;
    socket = s;
    connected = false;
    if (socket) {
        connect(
            socket,
            SIGNAL(connected()),
            this,
            SLOT(socketConnected())
        );
        connect(
            socket,
            SIGNAL(disconnected()),
            this,
            SLOT(socketConnectionClosed())
        );
        connect(socket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));

        connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(socketError(QAbstractSocket::SocketError)));
    }
}
void QTelnet::socketConnected() {
    connected = true;
    if ( notifier ) {
        delete notifier;
        notifier = NULL;
    }
    qDebug() << "Socket is connected";
    this->connected = true;
    notifier = new QSocketNotifier(socket->socketDescriptor(),
                                   QSocketNotifier::Exception, this);
    connect(notifier, SIGNAL(activated(int)),
            this, SLOT(socketException(int)));
    emit SignalSocketConnected();
    emit SignalConnected(this->host,this->port);

    //sendOptions();
}
void QTelnet::socketException(int i) {
    qDebug() << "Handle errors here!";
    emit SignalSocketException(i);
}
void QTelnet::socketConnectionClosed() {
    if ( notifier ) {
        delete notifier;
        notifier = NULL;
    }
    connected = false;
    emit SignalSocketConnectionClosed();
}
void QTelnet::socketReadyRead() {
    this->buffer_list.append( this->socket->readAll() );
    this->connected = true;
    emit SignalSocketReadyRead();
}
void QTelnet::socketError(QAbstractSocket::SocketError error) {
    qDebug() << "Error: " << error;
    emit SignalSocketError(error);
}
void QTelnet::connectToHost(const QString &host, quint16 port) {
    if ( this->connected ) {
        return;
    }
    this->socket->connectToHost(host,port);
    emit SignalConnectingToHost(host,port);
}
void QTelnet::close() {
    if ( ! this->connected ) {
        return;
    }
    if ( this->notifier ) {
        delete this->notifier;
        this->notifier = NULL;
    }
    this->connected = false;
    this->socket->close();
    emit SignalSocketConnectionClosed();

}
QString QTelnet::getHost() {
    return this->host;
}
quint16 QTelnet::getPort() {
    return this->port;
}
void QTelnet::write(QString data) {
    QByteArray ba = data.toLocal8Bit();
    if ( this->mccp ) {
        qDebug() << "I am compressing the data to write";
        ba = this->compress(ba);
    }
    this->socket->write(ba);
}
QByteArray QTelnet::compress(QByteArray str) {
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
    return QByteArray( outstring.c_str() );
}
QByteArray QTelnet::decompress(QByteArray str) {
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
    return QByteArray( outstring.c_str() );
}
