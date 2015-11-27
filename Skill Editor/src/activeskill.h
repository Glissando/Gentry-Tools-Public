#ifndef ACTIVESKILL_H
#define ACTIVESKILL_H

#include "condition.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QList>
#include <QString>

class ActiveSkill
{
public:
    enum skillRange{
        manhattan = 1,
        vonneuman = 2,
        conway = 4
    };
    enum skillType{
        attack = 1,
        heal = 2,
        buff = 4,
        debuff = 8,
        summon = 16,
        passive = 32,
        eventPassive = 64,
        Active = skillType.attack | skillType.heal | skillType.buff | skillType.summon,
        Passive = skillType.passive | skillType.eventPassive
    };
    QString name;
    int spcost, power, iattackRange, oattackRange;
    skillType type;
    QList<QString> reliantStats;
    ActiveSkill();
    ~ActiveSkill();
};

#endif // ACTIVESKILL_H
