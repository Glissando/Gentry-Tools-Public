#ifndef ACTOR_H
#define ACTOR_H

#include "class.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QList>
#include <QString>
#include <QMap>

class Actor
{
public:
    Actor();
    Actor(QJsonObject&,QMap<QString,Class*>);
    ~Actor();
    void read(const QJsonObject&);
    void readMeta(const QJsonObject&,QMap<QString,Class*>);
    void write(QJsonObject&) const;
    void writeMeta(QJsonObject&) const;
    int  maxHealth, maxSp, maxMp, maxAttack, maxDefence, maxAgility, maxIntelligence, maxHit,
    minHealth, minSp, minMp, minAttack, minDefence, minAgility, minIntelligence, minHit, lvl;
    QMap<QString,Class*> classes;
    QList<Class*> startingClasses;
    QString weapon;
    QString uniform;
    QString id;
    QString name;
    QString key;
    QString description;
};


#endif // ACTOR_H
