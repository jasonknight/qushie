#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtWebKit/QWebView>
#include <QEvent>
#include <QKeyEvent>
#include <QTelnet.h>
#include <QushieFile.h>
#include <zlib.h>
#include <QushieDatabase.h>
#include "helpers.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void Init();
protected:
    void resizeEvent(QResizeEvent *ev){
        qDebug() << "Resize even fired!";
        _set("main_window_geometry", QVariant( saveGeometry() ));
        _set("main_window_state", QVariant( saveState()));
    }
// Private member functions
private:
    bool eventFilter(QObject *obj, QEvent *event) {
        if (event->type() == QEvent::ShortcutOverride) {
            QKeyEvent *ke = static_cast<QKeyEvent *>(event);
            if (ke->modifiers() & Qt::ControlModifier) {
                switch (ke->key()) {
                case Qt::Key_C:
                case Qt::Key_D:
                case Qt::Key_Z:
                    return true;
                default:
                    break;
                }
            }
        }
        return QObject::eventFilter(obj, event);
    }
// Member Variables
public:
    QString to_url;
public slots:
    QObject* createTelnetObject();
    QObject* getTelnetObjectById( int id );
    QObject* createFileObject();
    QObject* createDatabaseObject();
    QString compressString(QByteArray str);
    QString decompressString( QByteArray s);
private:
    QWebView * web_view;
    QList<QTelnet*> connections;

private slots:
    void attach();
};

#endif // MAINWINDOW_H
