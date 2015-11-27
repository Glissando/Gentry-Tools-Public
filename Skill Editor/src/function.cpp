#include "function.h"

Function::Function(){}

Function::Function(QString name)
{
    function = name;
}

Function::Function(QJsonObject const &json)
{
    read(json);
}

Function::~Function()
{}

void Function::read(const QJsonObject &json)
{
    function = json["func"].toString();
    chance = json["chance"].toString();
    parameter = json["parameter"].toString();
}

void Function::write(QJsonObject &json) const
{
    json["func"] = function;
    json["chance"] = chance;
    json["parameter"] = parameter;
}
