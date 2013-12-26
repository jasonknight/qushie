#include "helpers.h"
QVariant _get(QString key) {
    QVariant val = settings->value(key);
    return val;
}

void _set(QString key, QString &value) {
    settings->setValue(key,value);
}
void _set(QString key, int value) {
    settings->setValue(key,value);
}
void _set(QString key, QVariant value) {
    settings->setValue(key,value);
}
 void _set(QString key, qreal value) {
    settings->setValue(key,value);
}
