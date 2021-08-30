#ifndef CHARACTER_H
#define CHARACTER_H

#include <QJsonObject>
#include <QPixmap>
#include <QDir>

class Character
{
    QString name;
    QString imagepath;
    QPixmap* image;
    bool ally;
    int hp;
    int atk;
    int def;
    int mov;

public:
    Character(const QString& name, const QJsonObject& charjson, const bool& ally, const QDir& dir);
    ~Character();
    const QPixmap* getImage() const;
    const QString getName() const;
    const QString getPath() const;
    const bool isAlly() const;
    const int gethp() const;
    const int getatk() const;
    const int getdef() const;
    const int getmov() const;

};

#endif // CHARACTER_H
