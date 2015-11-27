#include "actor.h"

Actor::Actor()
{
    name = "actor";
    key = "actor";
    lvl = 1;
    description = "";
    weapon = "---";
    uniform = "---";
    id = "---";
    maxHealth = 300;
    maxSp = 300;
    maxMp = 4;
    maxAttack = 40;
    maxDefence = 40;
    maxIntelligence = 40;
    maxAgility = 40;
    maxHit = 40;
    minHealth = 50;
    minSp = 50;
    minMp = 4;
    minAttack = 15;
    minDefence = 15;
    minIntelligence = 15;
    minAgility = 15;
    minHit = 15;
}

Actor::Actor(QJsonObject &json,QMap<QString,Class*> map)
{
    readMeta(json,map);
}

Actor::~Actor()
{

}

void Actor::read(const QJsonObject &json)
{
    name = json["name"].toString();
    key = json["key"].toString();
    description = json["description"].toString();
    weapon = json["weapon"].toString();
    uniform = json["uniform"].toString();
    id = json["id"].toString();
    maxHealth = json["maxhealth"].toInt();
    maxSp = json["maxsp"].toInt();
    maxMp = json["maxmp"].toInt();
    maxAttack = json["maxattack"].toInt();
    maxDefence = json["maxdefence"].toInt();
    maxIntelligence = json["maxintelligence"].toInt();
    maxAgility = json["maxagility"].toInt();
    maxHit = json["maxhit"].toInt();
    minHealth = json["minhealth"].toInt();
    minSp = json["minsp"].toInt();
    minMp = json["minmp"].toInt();
    minAttack = json["minattack"].toInt();
    minDefence = json["mindefence"].toInt();
    minIntelligence = json["minintelligence"].toInt();
    minAgility = json["minagility"].toInt();
    minHit = json["minhit"].toInt();
    lvl = json["lvl"].toInt();
}

void Actor::readMeta(const QJsonObject &json, QMap<QString,Class*> map)
{
    read(json);
    QJsonArray array = json["classes"].toArray();

    for(int i=0;i<array.size();i++)
    {
        Class *c = map.value(array[i].toString());
        classes.insert(c->type,c);
    }
    array = json["startingclasses"].toArray();
    for(int i=0;i<array.size();i++)
    {
        startingClasses.append(map.value(array[i].toString()));
    }
}

void Actor::write(QJsonObject &json) const
{
    json["name"] = name;
    json["key"] = key;
    json["description"] = description;
    if(weapon!="---")
        json["weapon"] = weapon;
    if(uniform!="---")
        json["uniform"] = uniform;
    if(id!="---")
        json["id"] = id;
    json["maxhealth"] = maxHealth;
    json["maxsp"] = maxSp;
    json["maxmp"] = maxMp;
    json["maxattack"] = maxAttack;
    json["maxdefence"] = maxDefence;
    json["maxintelligence"] = maxIntelligence;
    json["maxagility"] = maxAgility;
    json["maxhit"] = maxHit;
    json["minhealth"] = minHealth;
    json["minsp"] = minSp;
    json["minmp"] = minMp;
    json["minattack"] = minAttack;
    json["minintelligence"] = minIntelligence;
    json["minagility"] = minAgility;
    json["minhit"] = minHit;
    json["lvl"] = lvl;
    //QJsonArray array;
    QJsonObject cjson;
    foreach(const QString k,classes.keys())
    {
        QJsonObject obj;
        classes[k]->write(obj);
        cjson[k] = obj;
        //array.append(obj);
    }
    json["classes"] = cjson;
    QJsonArray array2;
    foreach(const Class* c,startingClasses)
    {
        array2.append(c->type);
    }
    json["startingclasses"] = array2;
}

void Actor::writeMeta(QJsonObject &json) const
{
    json["name"] = name;
    json["key"] = key;
    json["description"] = description;
    json["weapon"] = weapon;
    json["uniform"] = uniform;
    json["id"] = id;
    json["maxhealth"] = maxHealth;
    json["maxsp"] = maxSp;
    json["maxmp"] = maxMp;
    json["maxattack"] = maxAttack;
    json["maxdefence"] = maxDefence;
    json["maxintelligence"] = maxIntelligence;
    json["maxagility"] = maxAgility;
    json["maxhit"] = maxHit;
    json["minhealth"] = minHealth;
    json["minsp"] = minSp;
    json["minmp"] = minMp;
    json["minattack"] = minAttack;
    json["minintelligence"] = minIntelligence;
    json["minagility"] = minAgility;
    json["minhit"] = minHit;
    json["lvl"] = lvl;

    QJsonArray array;
    foreach(const QString k,classes.keys())
    {
        array.append(classes[k]->name);
    }
    json["classes"] = array;
    QJsonArray array2;
    foreach(const Class* c, startingClasses)
    {
        array2.append(c->name);
    }
    json["startingclasses"] = array2;
}
