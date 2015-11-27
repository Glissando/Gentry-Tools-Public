#include "enemy.h"

Enemy::Enemy()
{
    name = "enemy";
    key = "enemy";
    tint = 0xffffff;
    description = "";
    exp = 25;
    health = 60;
    sp = 60;
    mp = 4;
    attack = 10;
    defence = 10;
    intelligence = 10;
    agility = 10;
    hit = 10;

}

Enemy::Enemy(const QJsonObject &json, const QMap<QString,Skill*>map)
{
    readMeta(json,map);
}

Enemy::~Enemy()
{

}

void Enemy::read(const QJsonObject &json)
{
    name = json["name"].toString();
    key = json["key"].toString();
    description = json["description"].toString();

    tint = json["tint"].toInt();
    health = json["health"].toInt();
    sp = json["sp"].toInt();
    mp = json["mp"].toInt();
    attack = json["attack"].toInt();
    defence = json["defence"].toInt();
    agility = json["agility"].toInt();
    intelligence = json["intelligence"].toInt();
    hit = json["hit"].toInt();
    exp = json["exp"].toInt();
}

void Enemy::readMeta(const QJsonObject &json,const QMap<QString,Skill*>map)
{
    read(json);

    QJsonArray array = json["active"].toArray();

    for(int i=0;i<array.size();i++)
    {
        active.append(map.value(array[i].toString()));
    }

    array = json["passive"].toArray();

    for(int i=0;i<array.size();i++)
    {
        passive.append(map.value(array[i].toString()));
    }
}

void Enemy::write(QJsonObject &json) const
{
    json["name"] = name;
    json["key"] = key;
    json["description"] = description;
    json["tint"] = tint;
    json["health"] = health;
    json["sp"] = sp;
    json["mp"] = mp;
    json["attack"] = attack;
    json["defence"] = defence;
    json["agility"] = agility;
    json["intelligence"] = intelligence;
    json["hit"] = hit;
    json["exp"] = exp;
    QJsonArray array;
    foreach(const Skill* skill,active)
    {
        QJsonObject obj;
        skill->write(obj);
        array.append(obj);
    }
    json["active"] = array;
    QJsonArray array2;
    foreach(const Skill* skill,passive)
    {
        QJsonObject obj;
        skill->write(obj);
        array2.append(obj);
    }
    json["passive"] = array2;
}

void Enemy::writeMeta(QJsonObject &json) const
{
    json["name"] = name;
    json["key"] = key;
    json["description"] = description;
    json["tint"] = tint;
    json["health"] = health;
    json["sp"] = sp;
    json["mp"] = mp;
    json["attack"] = attack;
    json["defence"] = defence;
    json["agility"] = agility;
    json["intelligence"] = intelligence;
    json["hit"] = hit;
    json["exp"] = exp;
    QJsonArray array;
    foreach(const Skill *skill,active)
    {
        array.append(skill->name);
    }
    json["active"] = array;
    QJsonArray array2;

    foreach(const Skill *skill,passive)
    {
        array2.append(skill->name);
    }
    json["passive"] = array2;
}
