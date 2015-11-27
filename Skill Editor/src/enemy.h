#ifndef ENEMY_H
#define ENEMY_H

#include "skill.h"
#include "gameobject.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QList>
#include <QString>
#include <QMap>

class Enemy
{
public:
    Enemy();
    Enemy(const QJsonObject&, const QMap<QString,Skill*>);
    ~Enemy();
    void read(const QJsonObject&);
    void readMeta(const QJsonObject&,const QMap<QString,Skill*>);
    void write(QJsonObject&) const;
    void writeMeta(QJsonObject&) const;
    int health, sp, mp, attack, defence, agility, intelligence, hit, exp, tint;
    QString name, key, description;
    QList<Skill*> active;
    QList<Skill*> passive;
private:
    void writeActive();
    void writePassive();
};

#endif // ENEMY_H
