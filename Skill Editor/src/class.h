#ifndef CLASS_H
#define CLASS_H

#include "skill.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QList>
#include <QString>
#include <QMap>

class Class
{
public:
    enum classType{
        PE = 1,
        Health,
        Cooking,
        Math,
        Biology,
        Chemistry,
        //Physics = 64,
        Anatomy,
        Music,
        Linguistics,
        Cartography,
        Art,
        Psychology,
        ComputerScience,
        Engineering,
        MeeleeDefensive = PE | Health | Cooking
    };

    Class();
    Class(QJsonObject&,QMap<QString,Skill*>);
    ~Class();
    void read(const QJsonObject&);
    void readMeta(const QJsonObject&, const QMap<QString, Skill *>);
    void write(QJsonObject&) const;
    void writeMeta(QJsonObject&) const;
    QString name;
    QString description;
    QString type;
    int startingLvl; //defined in actor tab.
    QList<Skill*> skills;
};

#endif // CLASS_H
