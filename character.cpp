#include "character.h"

Character::Character(const QString& charname, const QJsonObject& charjson, const bool& _ally, const QDir& dir)
{
    name = charname;
    imagepath = charjson["image"].toString();
    image = new QPixmap(dir.filePath(imagepath));
    ally = _ally;
    if(!ally)
    {
        hp = charjson["hp"].toInt();
        atk = charjson["atk"].toInt();
        def = charjson["def"].toInt();
        mov = charjson["mov"].toInt();
        range = charjson["range"].toInt();
    }
}

Character::~Character()
{
    delete image;
}

const QPixmap* Character::getImage() const
{
    return image;
}

const QString Character::getName() const
{
    return name;
}

const QString Character::getPath() const
{
    return imagepath;
}

const bool Character::isAlly() const
{
    return ally;
}

const int Character::gethp() const
{
    return hp;
}

const int Character::getatk() const
{
    return atk;
}

const int Character::getdef() const
{
    return def;
}

const int Character::getmov() const
{
    return mov;
}

const int Character::getrange() const
{
    return range;
}
