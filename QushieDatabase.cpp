#include "QushieDatabase.h"

QushieDatabase::QushieDatabase(QObject *parent) :
    QObject(parent)
{
    this->query = NULL;
}
bool QushieDatabase::open(QString dbname) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbname);

    return db.open();
}
QString QushieDatabase::lastError() {
    return db.lastError().text();
}
bool QushieDatabase::create(QString q) {
    if ( this->query == NULL) { this->query = new QSqlQuery; }
    return query->prepare(q);
}
bool QushieDatabase::bind(QVariantMap values) {
    QMapIterator<QString,QVariant> it(values);
    while( it.hasNext() ) {
        it.next();
        query->bindValue(QString(":") + it.key(), it.value());
    }
    return true;
}
bool QushieDatabase::exec(QString q) {
    return query->exec(q);
}
bool QushieDatabase::run() {
    return query->exec();
}

QVariantMap QushieDatabase::next() {
    QVariantMap result;
    if ( query->isActive() ) {
        if ( query->next() ) {
            int len = query->record().count();
            int i;
            for ( i = 0; i < len; i++ ) {
                QString name = query->record().fieldName(i);
                QVariant value = query->value(i);
                result.insert(name,value);
            }
        }
    }
    return result;
}
bool QushieDatabase::isActive() {
    return query->isActive();
}
bool QushieDatabase::finish() {
    if ( query->isActive() ) {
        query->finish();
    }
    return true;
}
