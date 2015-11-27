#include "global.h"


Global::Global()
{
    variance = 10;
    startingMoney = 0;
    maxLvl = 30;
    maxClassXp = 500;
    maxXp = 5000;
}

Global::~Global()
{

}

void Global::read(const QJsonObject &json)
{
    variance = json["variance"].toInt();
    startingMoney = json["startingMoney"].toInt();
    maxLvl = json["maxLvl"].toInt();
    maxXp = json["maxXp"].toInt();
    maxClassXp = json["maxClassXp"].toInt();

    QJsonObject descriptions = json["classDescriptions"].toObject();

    foreach(const QString k,descriptions.keys())
    {
        classDescriptions.insert(k,descriptions[k].toString());
    }
}
//Writes everything to game file
void Global::write(QJsonObject &json,QList<Actor> actors,QList<Enemy> enemies) const
{
    json["variance"] = variance;
    json["startingMoney"] = startingMoney;
    json["maxLvl"] = maxLvl;
    json["maxXp"] = maxXp;
    json["maxClassXp"] = maxClassXp;

    QJsonArray array;
    foreach(const Actor actor,actors)
    {
        QJsonObject obj;
        actor.write(obj);
        array.append(obj);
    }
    json["actors"] = array;
    QJsonArray array2;
    foreach(const Enemy enemy,enemies)
    {
        QJsonObject obj;
        enemy.write(obj);
        array2.append(obj);
    }
    json["enemies"] = array2;

    QJsonObject descriptions;
    foreach(const QString k,classDescriptions.keys())
    {
        descriptions[k] = classDescriptions.value(k);
    }
    json["classDescriptions"] = descriptions;
}

//Writes only this objects data for metasave.
void Global::writeMeta(QJsonObject &json) const
{
    json["variance"] = variance;
    json["startingMoney"] = startingMoney;
    json["maxLvl"] = maxLvl;
    json["maxXp"] = maxXp;
    json["maxClassXp"] = maxClassXp;

    QJsonObject descriptions;
    foreach(const QString k,classDescriptions.keys())
    {
        descriptions[k] = classDescriptions.value(k);
    }
    json["classDescriptions"] = descriptions;
}
