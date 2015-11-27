#ifndef CONDITION_H
#define CONDITION_H

#include "gameobject.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QList>
#include <QString>

class Condition
{
public:
    Condition();
    Condition(QJsonObject&);
    Condition(QString,Condition);
    ~Condition();
    QString name;
    bool repeat, reset, isPercent;
    int maxStacks, v, turns;
    QList<QString> affectedStats;
    void setName(const QString&);
    virtual QString getName();
    void read(const QJsonObject&);
    void write(QJsonObject&) const;
};

#endif // CONDITION_H
