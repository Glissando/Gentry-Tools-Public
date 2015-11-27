#include "skill.h"

Skill::Skill()
{
    name = "skill";
    type = "Attack";
    range = "Cross";
    targetRange = "Cross";
    layer = "All";
    pow = 15;
    spcost = 0;
    chance = 50;
    iattackRange = 0;
    oattackRange = 1;
    itargetRange = 0;
    otargetRange = 0;
    boolean = true;
    condition = nullptr;
}

Skill::Skill(const QJsonObject &json, const QMap<QString,Condition*> map)
{
    readMeta(json,map);
}

Skill::~Skill()
{

}

void Skill::read(const QJsonObject &json)
{
    //Deserialize values shared between skill types
    type = json["type"].toString();

    name = json["name"].toString();
    description = json["description"].toString();
    functions.clear();
    if(type=="Attack")
    {
            pow = json["pow"].toInt();
            range = json["range"].toString();
            targetRange = json["targetrange"].toString();
            layer = json["layer"].toString();
            spcost = json["spcost"].toInt();
            iattackRange = json["iattackrange"].toInt();
            oattackRange = json["oattackrange"].toInt();
            itargetRange = json["itargetrange"].toInt();
            otargetRange = json["otargetrange"].toInt();
            chance = json["chance"].toInt();
            readReliantStats(json);
            readFunctions(json);
            readDynamics(json);
     }
     else if(type=="Heal")
     {
            pow = json["pow"].toInt();
            range = json["range"].toString();
            targetRange = json["targetrange"].toString();
            layer = json["layer"].toString();
            spcost = json["spcost"].toInt();
            iattackRange = json["iattackrange"].toInt();
            oattackRange = json["oattackrange"].toInt();
            itargetRange = json["itargetrange"].toInt();
            otargetRange = json["otargetrange"].toInt();
            chance = json["chance"].toInt();
            readReliantStats(json);
            readFunctions(json);
            readDynamics(json);
      }
        else if(type=="Buff")
        {
            range = json["range"].toString();
            targetRange = json["targetrange"].toString();
            layer = json["layer"].toString();
            spcost = json["spcost"].toInt();
            iattackRange = json["iattackrange"].toInt();
            oattackRange = json["oattackrange"].toInt();
            itargetRange = json["itargetrange"].toInt();
            otargetRange = json["otargetrange"].toInt();
            readFunctions(json);
        }
        else if(type=="Debuff")
        {
            range = json["range"].toString();
            targetRange = json["targetrange"].toString();
            layer = json["layer"].toString();
            spcost = json["spcost"].toInt();
            iattackRange = json["iattackrange"].toInt();
            oattackRange = json["oattackrange"].toInt();
            itargetRange = json["itargetrange"].toInt();
            otargetRange = json["otargetrange"].toInt();
            readFunctions(json);
        }
        else if(type=="Passive")
        {
            pow = json["v"].toInt();
            readAffectedStats(json);
            boolean = json["isPercent"].toBool();
        }
        else if(type=="Event Passive")
        {
            boolean = json["isSubect"].toBool();
            event = json["event"].toString();
            readFunctions(json);
        }

}

void Skill::readMeta(const QJsonObject &json, const QMap<QString,Condition*>conditions)
{
    read(json);
    if(json.contains("condition"))
    {
        QString con = json["condition"].toString();
        condition = conditions.value(con);
    }
    else
        condition = nullptr;
}

void Skill::write(QJsonObject &json) const
{
    //Deserialize values shared between skill types
    json["type"] = type;

    json["name"] = name;
    json["description"] = description;
    if(condition!=nullptr&&type!="Passive")
    {
        QJsonObject obj;
        condition->write(obj);
        json["condition"] = obj;
    }
    if(type=="Attack")
    {
            json["pow"] = pow;
            json["range"] = range;
            json["targetrange"] = targetRange;
            json["layer"] = layer;
            json["spcost"] = spcost;
            json["iattackrange"] = iattackRange;
            json["oattackrange"] = oattackRange;
            json["itargetrange"] = itargetRange;
            json["otargetrange"] = otargetRange;
            json["chance"] = chance;
            writeReliantStats(json);
            writeFunctions(json);
            writeDynamics(json);
    }
    else if(type=="Heal")
    {
            json["pow"] = pow;
            json["range"] = range;
            json["targetrange"] = targetRange;
            json["layer"] = layer;
            json["spcost"] = spcost;
            json["iattackrange"] = iattackRange;
            json["oattackrange"] = oattackRange;
            json["itargetrange"] = itargetRange;
            json["otargetrange"] = otargetRange;
            json["chance"] = chance;
            writeReliantStats(json);
            writeFunctions(json);
            writeDynamics(json);
    }
    else if(type=="Buff")
    {
            json["range"] = range;
            json["targetrange"] = targetRange;
            json["layer"] = layer;
            json["spcost"] = spcost;
            json["iattackrange"] = iattackRange;
            json["oattackrange"] = oattackRange;
            json["itargetrange"] = itargetRange;
            json["otargetrange"] = otargetRange;
            writeFunctions(json);
    }
    else if(type=="Debuff")
    {
            json["range"] = range;
            json["targetrange"] = targetRange;
            json["layer"] = layer;
            json["spcost"] = spcost;
            json["iattackrange"] = iattackRange;
            json["oattackrange"] = oattackRange;
            json["itargetrange"] = itargetRange;
            json["otargetrange"] = otargetRange;
            writeFunctions(json);
    }
    else if(type=="Passive")
    {
            json["v"] = pow;
            json["isPercent"] = boolean;
            writeAffectedStats(json);
    }
    else if(type=="Event Passive")
    {
            json["isSubect"] = boolean;
            json["event"] = event;
            writeFunctions(json);
    }

}

void Skill::writeMeta(QJsonObject &json) const
{
    //Deserialize values shared between skill types
    json["type"] = type;

    json["name"] = name;
    json["description"] = description;

    if(condition!=nullptr&&type!="Passive")
    {
        json["condition"] = condition->name;
        json["chance"] = chance;
    }

    if(type=="Attack")
    {
            json["pow"] = pow;
            json["range"] = range;
            json["targetrange"] = targetRange;
            json["layer"] = layer;
            json["spcost"] = spcost;
            json["iattackrange"] = iattackRange;
            json["oattackrange"] = oattackRange;
            json["itargetrange"] = itargetRange;
            json["otargetrange"] = otargetRange;
            json["chance"] = chance;
            writeReliantStats(json);
            writeFunctions(json);
            writeDynamics(json);
    }
    else if(type=="Heal")
    {
            json["pow"] = pow;
            json["range"] = range;
            json["targetrange"] = targetRange;
            json["layer"] = layer;
            json["spcost"] = spcost;
            json["iattackrange"] = iattackRange;
            json["oattackrange"] = oattackRange;
            json["itargetrange"] = itargetRange;
            json["otargetrange"] = otargetRange;
            json["chance"] = chance;
            writeReliantStats(json);
            writeFunctions(json);
            writeDynamics(json);
    }
    else if(type=="Buff")
    {
            json["range"] = range;
            json["targetrange"] = targetRange;
            json["layer"] = layer;
            json["spcost"] = spcost;
            json["iattackrange"] = iattackRange;
            json["oattackrange"] = oattackRange;
            json["itargetrange"] = itargetRange;
            json["otargetrange"] = otargetRange;
            writeFunctions(json);
    }
    else if(type=="Debuff")
    {
            json["range"] = range;
            json["targetrange"] = targetRange;
            json["layer"] = layer;
            json["spcost"] = spcost;
            json["iattackrange"] = iattackRange;
            json["oattackrange"] = oattackRange;
            json["itargetrange"] = itargetRange;
            json["otargetrange"] = otargetRange;
            writeFunctions(json);
    }
    else if(type=="Passive")
    {
            json["v"] = pow;
            json["isPercent"] = boolean;
            writeAffectedStats(json);
    }
    else if(type=="Event Passive")
    {
            json["isSubect"] = boolean;
            json["event"] = event;
            writeFunctions(json);
    }
}

void Skill::readFunctions(const QJsonObject &json)
{
    if(json.contains("functions"))
    {
        //Setup functions
        QJsonArray funcs = json["functions"].toArray();

        for(int i=0;i<funcs.size();++i)
        {
           functions.append(Function(funcs[i].toObject()));
        }
    }
}

void Skill::readDynamics(const QJsonObject &json)
{
    if(json.contains("functions"))
    {
        //Setup dynamics
        QJsonArray funcs = json["dynamics"].toArray();

        for(int i=0;i<funcs.size();++i)
        {
           dynamics.append(Dynamic(funcs[i].toObject()));
        }
    }
}

void Skill::readReliantStats(const QJsonObject &json)
{
    QJsonArray attributes = json["reliantStats"].toArray();

    for(int i=0;i<attributes.size();++i)
    {
        stats.append(attributes[i].toString());
    }
}

void Skill::readAffectedStats(const QJsonObject &json)
{
    QJsonArray attributes = json["affectedStats"].toArray();

    for(int i=0;i<attributes.size();++i)
    {
        stats.append(attributes[i].toString());
    }
}

void Skill::writeFunctions(QJsonObject &json) const
{
    QJsonArray funcs;
    foreach(const Function func,functions)
    {
        QJsonObject obj;
        func.write(obj);
        funcs.append(obj);
    }
    json["functions"] = funcs;
}

void Skill::writeDynamics(QJsonObject &json) const
{
    QJsonArray funcs;
    foreach(const Dynamic func,dynamics)
    {
        QJsonObject obj;
        func.write(obj);
        funcs.append(obj);
    }
    if(funcs.size()>=0)
    {
        json["dynamics"] = funcs;
        json["change"] = QJsonObject();
    }
}

void Skill::writeReliantStats(QJsonObject &json) const
{
    QJsonArray attributes;
    foreach(const QString attribute,stats)
    {
        attributes.append(attribute);
    }
    json["reliantStats"] = attributes;
}

void Skill::writeAffectedStats(QJsonObject &json) const
{
    QJsonArray attributes;
    foreach(const QString attribute,stats)
    {
        attributes.append(attribute);
    }
    json["affectedStats"] = attributes;
}
