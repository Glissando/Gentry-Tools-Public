#include "class.h"

Class::Class()
{
    name = "class";
    type = "PE";
    description = "";
    startingLvl = 1;
}

Class::Class(QJsonObject &json,QMap<QString,Skill*> map)
{
    readMeta(json,map);
}

Class::~Class()
{

}

void Class::read(const QJsonObject &json)
{
    type = json["type"].toString();
    name = json["name"].toString();
    description = json["description"].toString();
    if(json.contains("lvl"))
    {
        startingLvl = json["lvl"].toInt();
    }
}

void Class::readMeta(const QJsonObject &json, const QMap<QString,Skill*> map)
{
    read(json);
    QJsonArray cskills = json["skills"].toArray();
    for(int i=0;i<cskills.size();++i)
    {
        skills.append(map.value(cskills[i].toString()));
    }
}
//Used for writing to Export file
void Class::write(QJsonObject &json) const
{
    json["type"] = type;
    //json["name"] = name;
    json["description"] = description;
    if(startingLvl!=0)
    {
        json["lvl"] = startingLvl;
    }
    QJsonArray array;
    foreach(const Skill *skill,skills)
    {
        QJsonObject obj;
        skill->write(obj);
        obj["classtype"] = type;
        array.append(obj);
    }
    json["skills"] = array;
}
//Used for writing MetaFile
void Class::writeMeta(QJsonObject &json) const
{
    json["type"] = type;
    json["name"] = name;
    json["description"] = description;
    if(startingLvl!=0)
    {
        json["lvl"] = startingLvl;
    }
    QJsonArray array;
    foreach(const Skill *skill,skills)
    {
        array.append(skill->name);
    }
    json["skills"] = array;

}
