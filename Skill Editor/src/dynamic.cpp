#include "dynamic.h"

Dynamic::Dynamic()
{

}

Dynamic::Dynamic(QString name)
{
    function = name;
}

Dynamic::Dynamic(QJsonObject const &json)
{
    read(json);
}

Dynamic::~Dynamic()
{

}

void Dynamic::read(const QJsonObject &json)
{
    function = json["func"].toString();
    parameter = json["parameter"].toString();
}

void Dynamic::write(QJsonObject &json) const
{
    json["func"] = function;
    json["parameter"] = parameter;
}
