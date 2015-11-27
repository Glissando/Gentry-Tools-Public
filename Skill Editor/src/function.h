#ifndef FUNCTION_H
#define FUNCTION_H

#include <QString>
#include <QDataStream>
#include <QJsonObject>
#include <QJsonArray>

class Function
{
public:
    Function();
    Function(QString);
    Function(const QJsonObject&);
    ~Function();
    void read(const QJsonObject&);
    void write(QJsonObject&) const;
    QString function;
    QString chance;
    QString parameter;

};

#endif // FUNCTION_H
