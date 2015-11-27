#include "condition.h"

Condition::Condition()
{
    name = "condition";
    repeat = false;
    reset = false;
    isPercent = true;
    v = 0;
    turns = 1;
    maxStacks = 1;
}

Condition::Condition(QJsonObject &json)
{
    read(json);
}

Condition::Condition(QString id,Condition con)
{
    name = id;
    repeat = false;
    reset = false;
    isPercent = true;
    v = 0;
    turns = 1;
    maxStacks = 1;
}

Condition::~Condition()
{

}

void Condition::setName(const QString &text)
{
    name = text;
}

QString Condition::getName()
{
    return name;
}

void Condition::read(const QJsonObject &json)
{
    name = json["name"].toString();
    repeat = json["repeat"].toBool();
    reset = json["reset"].toBool();
    isPercent = json["isPercent"].toBool();
    turns = json["turns"].toInt();
    v = json["v"].toInt();
    if(json.contains("maxStacks"))
    {
        maxStacks = json["maxStacks"].toInt();
    }
    else
    {
        maxStacks = 1;
    }
    affectedStats.clear();
    QJsonArray stats = json["affectedStats"].toArray();

    for(int i=0;i<stats.size();++i)
    {
       affectedStats.append(stats[i].toString());
    }
}

void Condition::write(QJsonObject &json) const
{
    json["name"] = name;
    json["repeat"] = repeat;
    json["reset"] = reset;
    json["isPercent"] = isPercent;
    json["turns"] = turns;
    json["v"] = v;
    if(maxStacks>1)
    {
        json["maxStacks"] = maxStacks;
    }

    QJsonArray stats;
    foreach(const QString stat,affectedStats)
    {
        stats.append(stat);
    }
    json["affectedStats"] = stats;
}
