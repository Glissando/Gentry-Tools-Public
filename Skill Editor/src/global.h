#ifndef GLOBAL_H
#define GLOBAL_H

#include "actor.h"
#include "enemy.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QList>
#include <QString>
#include <QMap>
#include <QStringList>

class Global
{
public:
    Global();
    ~Global();
    void read(const QJsonObject&);
    void readMeta(const QJsonObject&);
    void write(QJsonObject&,QList<Actor>,QList<Enemy>) const;
    void writeMeta(QJsonObject&) const;

    int variance, startingMoney, maxXp, maxLvl, maxClassXp;
    QMap<QString,QString> classDescriptions;
};

#endif // GLOBAL_H
