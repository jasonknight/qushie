#ifndef QUSHIEDATABASE_H
#define QUSHIEDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariantMap>
class QushieDatabase : public QObject
{
    Q_OBJECT
public:
    explicit QushieDatabase(QObject *parent = 0);
    
signals:
    
public Q_SLOTS:
    bool open(QString dbname);
    QString lastError();
    bool create(QString q);
    bool bind(QVariantMap values);
    bool run(); // for selects;
    bool select() { return this->run(); }
    bool update() { return this->run(); }
    bool exec(QString q);
    bool isActive();
    QVariantMap next();
    bool finish();
private:
    QSqlDatabase db;
    QSqlQuery* query;
    
};

#endif // QUSHIEDATABASE_H
