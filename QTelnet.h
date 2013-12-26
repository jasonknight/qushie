#ifndef QTELNET_H
#define QTELNET_H
#define HAVE_ZLIB
#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QVariantMap>
#include <QtCore/QPair>
#include <QtCore/QVariant>
#include <QtCore/QSocketNotifier>
#include <QtCore/QBuffer>
#include <QtCore/QVarLengthArray>
#ifdef Q_WS_WIN
#  include <winsock2.h>
#endif
#if defined (Q_OS_UNIX)
#  include <sys/types.h>
#  include <sys/socket.h>
#  include <netinet/in.h>
#endif
#include <zlib.h>
#include <string>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <QDebug>
namespace TelnetOption {
    const uchar BINARY = 0;
    const uchar ECHO = 1;
    const uchar RCP = 2;
    const uchar SGA = 3;
    const uchar NAMS = 4;
    const uchar STATUS = 5;
    const uchar TM = 6;
    const uchar RCTE = 7;
    const uchar NAOL = 8;
    const uchar NAOP = 9;
    const uchar NAOCRD = 10;
    const uchar NAOHTS = 11;
    const uchar NAOHTD = 12;
    const uchar NAOFFD = 13;
    const uchar NAOVTS = 14;
    const uchar NAOVTD = 15;
    const uchar NAOLFD = 16;
    const uchar XASCII = 17;
    const uchar LOGOUT = 18;
    const uchar BM = 19;
    const uchar DET = 20;
    const uchar SUPDUP = 21;
    const uchar SUPDUPOUTPUT = 22;
    const uchar SNDLOC = 23;
    const uchar TTYPE = 24;
    const uchar EOR = 25;
    const uchar TUID = 26;
    const uchar OUTMRK = 27;
    const uchar TTYLOC = 28;
    const uchar REGIME3270 = 29;
    const uchar X3PAD = 30;
    const uchar NAWS = 31;
    const uchar TSPEED = 32;
    const uchar LFLOW = 33;
    const uchar LINEMODE = 34;
    const uchar XDISPLOC = 35;
    const uchar ENVIRON = 36;
    const uchar AUTHENTICATION = 37;
    const uchar ENCRYPT = 38;
    const uchar NEW_ENVIRON = 39;
    const uchar MSSP = 70;
    const uchar COMPRESS2 = 86;
    const uchar ZMP = 93;
    const uchar EXOPL = 255;
    const uchar MCCP2 = 86;
}

namespace TelnetCommands {
    const uchar IAC = 255;
    const uchar DONT = 254;
    const uchar DO = 253;
    const uchar WONT = 252;
    const uchar WILL = 251;
    const uchar SB = 250;
    const uchar GA = 249;
    const uchar EL = 248;
    const uchar EC = 247;
    const uchar AYT = 246;
    const uchar AO = 245;
    const uchar IP = 244;
    const uchar BREAK = 243;
    const uchar DM = 242;
    const uchar NOP = 241;
    const uchar SE = 240;
    const uchar EOR = 239;
    const uchar ABORT = 238;
    const uchar SUSP = 237;
    const uchar XEOF = 236;
}
class QTelnetBufferList {
    public:
        QTelnetBufferList() : bytes_available(0) {}
        void append( const QByteArray &data) { buffers.append(data); }
        void push_back( const QByteArray &data ) { buffers.prepend(data); }
        long size() const { return bytes_available; }
        QByteArray readAll() {
            QByteArray ba;
            while( !buffers.isEmpty() ) {
                ba.append( buffers.takeFirst() );
            }
            return ba;
        }
    private:
        QList<QByteArray> buffers;
        long bytes_available;
};
class QTelnet : public QObject {
    Q_OBJECT
public:
    QTelnet(QObject *parent = 0);
    ~QTelnet();
    void setSocket(QTcpSocket *s);
    QTcpSocket* getSocket() { return this->socket; }
    Q_PROPERTY(QString host READ getHost SCRIPTABLE true)
    Q_PROPERTY(quint16 port READ getPort SCRIPTABLE true)
    int id;
    bool mccp;
    bool mccp_sent;
signals:
    void SignalConnected(QString,quint16);
    void SignalSocketConnected();
    void SignalSocketConnectionClosed();
    void SignalSocketReadyRead();
    void SignalSocketError(QAbstractSocket::SocketError);
    void SignalSocketException(int);
    void SignalConnectingToHost(QString,quint16);
    void SignalClosed();
public Q_SLOTS:
    void socketConnected();
    void socketConnectionClosed();
    void socketReadyRead();
    void socketError(QAbstractSocket::SocketError error);
    void socketException(int i);
    void connectToHost( const QString& host, quint16 port);
    void close();
    void write(QString data);
    QByteArray compress(QByteArray str);
    QByteArray decompress(QByteArray str);
    QString readAll() {
        QByteArray txt = this->buffer_list.readAll();
//        if ( this->mccp ) {
//            txt = this->decompress(txt);
//        } else {
//            qDebug() << "Looking for MCCP Related";
//            for ( int i = 0; i < txt.length(); i++ ) {
//                qDebug() << "Char is: " << txt.at(i);
//                if ( this->mccp_sent == false && (uchar)txt.at(i) == TelnetCommands::IAC &&
//                     (uchar)txt.at(i+1) == TelnetCommands::WILL &&
//                     (uchar)txt.at(i + 2) == TelnetOption::MCCP2
//                   ) {
//                    char buf[3] = {
//                        TelnetCommands::IAC,
//                        TelnetCommands::DO,
//                        TelnetOption::MCCP2
//                    };
//                    //this->socket->write(buf,3);
//                    //this->socket->flush();

//                    qDebug() << "Sent DO MCCP2";
//                    //this->mccp_sent = true;
//                    break;
//                } else if ((uchar)txt.at(i) == TelnetCommands::IAC &&
//                           (uchar)txt.at(i) == TelnetCommands::SB &&
//                           (uchar)txt.at(i + 2) == TelnetOption::MCCP2 &&
//                           (uchar)txt.at(i+1) == TelnetCommands::SE ) {
//                    qDebug() << "Got WILL MCCP";
//                    //this->mccp = true;

//                }
//            }
//        }
        return QString(txt);
    }
    QString getHost();
    quint16 getPort();
    bool isConnected() {
        return this->connected;
    }
    int getId() { return this->id; }
private:
    QTcpSocket* socket;
    QSocketNotifier* notifier;
    bool connected;
    QTelnetBufferList buffer_list;
    QString host;
    quint16 port;
};

#endif // QTELNET_H
