#ifndef DYNAMIC_H
#define DYNAMIC_H

#include <QString>
#include <QDataStream>
#include <QJsonObject>
#include <QJsonArray>

class Dynamic
{
public:
    Dynamic();
    Dynamic(QString);
    Dynamic(const QJsonObject&);
    ~Dynamic();
    void read(const QJsonObject&);
    void write(QJsonObject&) const;
    QString function;
    QString parameter;
};

#endif // DYNAMIC_H
