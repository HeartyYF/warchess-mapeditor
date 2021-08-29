#include "tile.h"


Tile::Tile(const QString& tilename, const QJsonObject& tilejson, const QDir& dir)
{
    name = tilename;
    if(tilejson.find("bimage") != tilejson.end())
    {
        bimagepath = tilejson["bimage"].toString();
        bimage = new QPixmap(dir.filePath(bimagepath));
    }
    else
    {
        bimage = Tile::nullImage();
    }
    if(tilejson.find("fimage") != tilejson.end())
    {
        fimagepath = tilejson["fimage"].toString();
        fimage = new QPixmap(dir.filePath(fimagepath));
    }
    else
    {
        fimage = Tile::nullImage();
    }
    pass = tilejson["pass"].toBool();
    if(pass)
    {
        cost = tilejson["cost"].toInt();
        damage = tilejson["damage"].toInt();
    }
}

Tile::~Tile()
{
    delete bimage;
    delete fimage;
}

const QPixmap* Tile::getbImage() const
{
    return bimage;
}

const QPixmap* Tile::getfImage() const
{
    return fimage;
}

const QString Tile::getName() const
{
    return name;
}

const QString Tile::getbPath() const
{
    return bimagepath;
}

const QString Tile::getfPath() const
{
    return fimagepath;
}

const bool Tile::canPass() const
{
    return pass;
}

const int Tile::getCost() const
{
    return cost;
}

const int Tile::getDamage() const
{
    return damage;
}

QPixmap* Tile::nullImage()
{
    QPixmap* pixmap = new QPixmap(64, 64);
    pixmap->fill(Qt::transparent);
    return pixmap;
}
