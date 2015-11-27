#ifndef SKILL_H
#define SKILL_H

#include "condition.h"
#include "function.h"
#include "dynamic.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QList>
#include <QString>
#include <QMap>

enum skillType
{
    attack = 1,
    heal = 2,
    buff = 4,
    debuff = 8,
    summon = 16, //Deprecated
    passive = 32,
    eventPassive = 64,
    Active = attack | heal | buff | summon,
    Passive = passive | eventPassive
};
class Skill
{
public:
    enum skillRange
    {
        manhattan = 1,
        vonneuman = 2,
        conway = 4
    };

    enum layerMask
    {
        none = 0,
        semi = 1,
        collide = 2,
        All = semi | collide
    };

    QString name;
    QString description;
    int spcost, pow, iattackRange, oattackRange, itargetRange, otargetRange, chance; //pow used for pow and v
    QString range, targetRange;
    QString type;
    QString layer;
    bool boolean; //used for isPercent, isSubject and is isParty
    QString event;
    QList<QString> stats; //used for affectedStats and reliantStats
    QList<Dynamic> dynamics;
    QList<Function> functions;
    Condition *condition;
    //Skill *skill; Skill chaining deprecated

    Skill();
    Skill(const QJsonObject&,const QMap<QString,Condition*>);
    ~Skill();
    void read(const QJsonObject&);
    void readMeta(const QJsonObject&, const QMap<QString,Condition*>);
    void write(QJsonObject&) const;
    void writeMeta(QJsonObject&) const;
private:
    void readFunctions(const QJsonObject&);
    void readDynamics(const QJsonObject&);
    void readReliantStats(const QJsonObject&);
    void readAffectedStats(const QJsonObject&);
    void writeFunctions(QJsonObject&) const;
    void writeDynamics(QJsonObject&) const;
    void writeReliantStats(QJsonObject&) const;
    void writeAffectedStats(QJsonObject&) const;
};

#endif // SKILL_H
